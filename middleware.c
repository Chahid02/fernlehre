#include "middleware.h"


//Global Variables:
char frameToSend[BYTES_FRAME_TOTAL];     //Connection between MW and UI
uint8_t groupsize = 3;          //Amount of group members (needs to be set by UI)
uint8_t myID = 255;               //ID of Peer (needs to be set by UI)
uint32_t message_cnt = 0;       //message counter represents the latest message id
uint8_t mesID_cnt[MAX_PEERS];
groupmember mygroup[MAX_PEERS];
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexInput = PTHREAD_MUTEX_INITIALIZER;
inputData myInputData = {.newMsgReceived = false};

int middleware( void )
{

//----------------------------------------------------------------------------------
// 1. Wait for the UI to get the ID of this Peer

    getID();

//----------------------------------------------------------------------------------
// 2. Get Informations from UI (about Group) -> IP, Port, ID-Number

    getMembers(&mygroup, groupsize);
    
//----------------------------------------------------------------------------------
// 3. Start Middleware 

    int mysocket;
    setupMW(&mygroup,myID,&mysocket);

//----------------------------------------------------------------------------------
// 4. Check for received message from other peers & check if there is something to send 

    while (true)
    {

//----------------------------------------------------------------------------------
// Check for user input & send message to group
        pthread_mutex_lock(&mutexInput);
        if (myInputData.newMsgReceived == true)
        {
            createRawFrame(frameToSend, mesID_cnt[myID], myID, 0x00, myID, myInputData);
            printf("Checksum send byte 1: %x\n", frameToSend[37]);
            printf("Checksum send byte 2: %x\n", frameToSend[38]);
            printf("Terminating byte: %x\n", frameToSend[39]);
            sendgroup(&mygroup,groupsize,myID,&mysocket,frameToSend);
            myInputData.newMsgReceived = false;
        }
        pthread_mutex_unlock(&mutexInput);
//----------------------------------------------------------------------------------
// Receive Message from group

        recvgroup(&mysocket);

        //TODO: Parse Message (or get it already parsed from function)
        //TODO: Message already received? If yes: ignore, if no: hand massage text over to "sendgroup"
    }

    if(close(mysocket)==-1)
    {
        printf("ERROR: Could not close socket!");
    }

    return 0;

}

void getID(void){

    while (true)
    {
        pthread_mutex_lock(&mymutex);
        if (myID!=255)
        {
            break;
        }
        pthread_mutex_unlock(&mymutex);
    }

    return 1;
}

void getMembers(groupmember (*mygroup)[], int groupsize){

     for (int i = 0; i < groupsize; i++)     //TODO: get memberinfos from ui --> DONE!!!
    {
        (*mygroup)[i].id = i;
        printf("%i",(*mygroup)[i].id);
        strcpy((*mygroup)[i].ipv4,"127.0.0.1");
        (*mygroup)[i].port = 8080+i;
        (*mygroup)[i].addr.sin_family = AF_INET;

        if(inet_aton((*mygroup)[i].ipv4, &(*mygroup)[i].addr.sin_addr)==0)
        {
            printf("ERROR: Conversion failed!");
        }

        (*mygroup)[i].addr.sin_port = htons((*mygroup)[i].port);
    }

    #ifdef DEBUG

    printf("\n-------------------------------------------------------------\n");
    printf("This is the Group:\n");

    for (int i = 0; i < groupsize; i++)
    {
        printf("ID: %i ,",(*mygroup)[i].id);
        printf("IPv4: %s ,",(*mygroup)[i].ipv4);
        printf("Port: %i\n",(*mygroup)[i].port);
        if (i==myID)
        {
            printf("<-- This is the selected Peer.");
        }
    }

    printf("-------------------------------------------------------------\n");

    #endif
}

int setupMW(groupmember (*mygroup)[],int myID, int *mysocket){

    mesID_cnt[myID] = 1;

    createLog(logfilePathTesting);

    // Create socket
    *mysocket = socket((*mygroup)[myID].addr.sin_family, (enum __socket_type) SOCK_DGRAM, 0);

    if( *mysocket == -1)
    {
        printf("ERROR: Could not create socket!");
        return(-1);
    }

    // Bind socket
    if (bind(*mysocket, (const struct sockaddr *)&(*mygroup)[myID].addr, sizeof((*mygroup)[myID].addr)) < 0 )
    {
        printf("ERROR: Bind failed");
        return(-1);
    }

    // Make socket nonblocking
    long save_fd;
    save_fd = fcntl(*mysocket,F_GETFL);
    save_fd |= O_NONBLOCK;
    fcntl(*mysocket, F_SETFL,save_fd);

    return(0);

}

int sendgroup(groupmember (*mygroup)[], int groupsize, int myID, int *mysocket, char *payload){
    
    Frame frame_recv;

    for (int i = 0; i < groupsize; i++)
    {

        if (i == myID)
        {
            
            storeFrame(&frame_recv, payload);
            frame_recv.peerNr = frame_recv.sndId;
            #ifdef DEBUG
            printf("Received Message [%i %i] from Peer %i: %s\n",frame_recv.msgId,frame_recv.sndId,frame_recv.peerNr,frame_recv.payload);
            #endif
            logMessage(frame_recv, logfilePathTesting);
        }
        else
        {
            
            for (int y = 0; y < 3; y++)
            {
                if (sendto(*mysocket, (char *)payload, (size_t)BYTES_FRAME_TOTAL,MSG_CONFIRM,(const struct sockaddr *)&(*mygroup)[i].addr, sizeof((*mygroup)[i].addr)) == (ssize_t)-1) //TODO: Send to all!
                {
                    printf("ERROR: Could not send!");
                    break;
                }
                
                if(ACK(mygroup, mysocket, i) == -1)
                {
                    printf("INFO: Message was not received by member %i!\n",i);
                    printf("INFO: Resend...\n");
                } else
                {
                    break;
                }
            }

        }

        sleep(1); // Requirement: Wait for 1s between transmissions
    }

    if (frame_recv.sndId==myID)
    {
        mesID_cnt[myID]=(mesID_cnt[myID]+1)%256;
    }
    
    return (0);
    
}

int ACK(groupmember (*mygroup)[], int *mysocket, int peerid){


    Frame frame_recv;
    char *received[BYTES_FRAME_TOTAL];
    received[0]='\0';

    usleep(100000); //wait 100ms for acknoledgement

    if (recvfrom(*mysocket, received, BYTES_FRAME_TOTAL, 0, (const struct sockaddr *)&(*mygroup)[peerid].addr, sizeof((*mygroup)[peerid].addr)) == (ssize_t)-1)
    {
        printf("ERROR: Could not receive!");
    }

    storeFrame(&frame_recv, received);

    #ifdef DEBUG
        printf("Received to Acknoledge:\n");
        printf("MsgID: %d\n", frame_recv.msgId);
        printf("SndID: %d\n", frame_recv.sndId);
        printf("ACK: %d\n", frame_recv.ack);
        printf("PeerNr: %d\n", frame_recv.peerNr);
        printf("PayloadLength: %d\n", frame_recv.payloadLength);
        printf("Payload: %s\n", frame_recv.payload);

        printf("%i == 1\n",frame_recv.ack);
        printf("%i == %i\n",frame_recv.msgId,mesID_cnt[myID]);
        printf("%i == %i\n",frame_recv.peerNr,peerid);
    #endif

    if ((frame_recv.ack == 1)&&(frame_recv.msgId == mesID_cnt[myID])&&(frame_recv.peerNr==peerid))
    {
        return (0);
    }
    

    received[0]='\0'; //Clear received buffer

    return(-1);
}

int recvgroup(int *mysocket){

    char message_recv[BYTES_PAYLOAD];
    ssize_t bytes_recv;
    Frame frame_recv;
    uint16_t recvChecksumCalculated;
    bool checksumValid = false;
    
    int8_t prev_sender;
    message_recv[0] = '\0';

    bytes_recv = recv(*mysocket,message_recv,(size_t)BYTES_FRAME_TOTAL,0);

    if (bytes_recv >= 0)
    {
        storeFrame(&frame_recv, message_recv);
        printf("Received:\n");
        printf("MsgID: %d\n", frame_recv.msgId);
        printf("SndID: %d\n", frame_recv.sndId);
        printf("ACK: %d\n", frame_recv.ack);
        printf("PeerNr: %d\n", frame_recv.peerNr);
        printf("PayloadLength: %d\n", frame_recv.payloadLength);
        printf("Payload: %s\n", frame_recv.payload);
        printf("Checksum received: %d\n", frame_recv.checksum);
        
        //printf("TEST0");
        inputData buff = {.userMsg=frame_recv.payload};

        calcChecksum(frame_recv.payload, &recvChecksumCalculated);
        printf("Checksum calced: %d\n", recvChecksumCalculated);
        if(frame_recv.checksum != recvChecksumCalculated)
        {
            checksumValid = false;
            
            printf("Checksum invalid!\n");
        }
        else
        {
            checksumValid = true;
        }


        if ((frame_recv.ack == 0) && checksumValid)
        {
            prev_sender = frame_recv.peerNr;
            frame_recv.ack = 1;
            frame_recv.peerNr = myID;

            buff.msgLength = frame_recv.payloadLength;

            createRawFrame(message_recv,frame_recv.msgId,frame_recv.sndId,frame_recv.ack,frame_recv.peerNr,buff);

            #ifdef DEBUG
            printf("MsgID: %d\n", frame_recv.msgId);
            printf("SndID: %d\n", frame_recv.sndId);
            printf("ACK: %d\n", frame_recv.ack);
            printf("PeerNr: %d\n", frame_recv.peerNr);
            printf("PayloadLength: %d\n", frame_recv.payloadLength);
            printf("Payload: %s\n", frame_recv.payload);
            #endif
            
            if (sendto(*mysocket, message_recv, (size_t)BYTES_FRAME_TOTAL,MSG_CONFIRM,(const struct sockaddr *)&mygroup[prev_sender].addr, sizeof(mygroup[prev_sender].addr)) == (ssize_t)-1)
            {
                printf("ERROR: Could not send!");
            }

            if (mesID_cnt[frame_recv.sndId] < frame_recv.msgId)
            {
                #ifdef DEBUG
                printf("RESEND to group!\n");
                #endif

                mesID_cnt[frame_recv.sndId] = frame_recv.msgId;
                frame_recv.ack = 0;
                createRawFrame(message_recv,frame_recv.msgId,frame_recv.sndId,frame_recv.ack,frame_recv.peerNr,buff);
                sendgroup(&mygroup,groupsize,myID,mysocket,message_recv);
            }
        }
        
    }
    
    return (0);
}

uint8_t calcChecksum(char* data, uint16_t* checksumBuffer)
{
    uint8_t errCode = 0;
    uint32_t checksum = 0x00;

    for (int i = 0; i + 1 < BYTES_PAYLOAD; i+=2)
    {
        uint16_t dataBlock;
        memcpy(&dataBlock, data + i, 2);
        checksum += dataBlock;
        if(checksum > 0xFFFF)
        {
            checksum -= 0xFFFF;
        }
    }
    *checksumBuffer = ~checksum;

    return errCode;
}

uint8_t storeFrame(Frame* storageFrame, char rawFrame [BYTES_FRAME_TOTAL])
{
    uint8_t errCode = 0;
    uint8_t bufferPosition = 0;
    int16_t tempChecksum;

    if(rawFrame == NULL || storageFrame == NULL)
    {
        printf("NULL Pointer! \n");
    }

    memcpy(&(storageFrame->msgId), rawFrame, BYTES_MSG_ID);
    bufferPosition = BYTES_MSG_ID;
    memcpy(&(storageFrame->sndId), rawFrame + bufferPosition, BYTES_SENDER_ID);
    bufferPosition += BYTES_SENDER_ID;
    memcpy(&(storageFrame->ack), rawFrame + bufferPosition, BYTES_ACK);
    bufferPosition += BYTES_ACK;
    memcpy(&(storageFrame->peerNr), rawFrame + bufferPosition, BYTES_PEER_NR);
    bufferPosition += BYTES_PEER_NR;
    memcpy(&(storageFrame->payloadLength), rawFrame + bufferPosition, BYTES_PAYLOAD_LENGTH);
    bufferPosition += BYTES_PAYLOAD_LENGTH;
    memcpy(&(storageFrame->payload), rawFrame + bufferPosition, BYTES_PAYLOAD);
    bufferPosition += BYTES_PAYLOAD;
    storageFrame->payload[BYTES_PAYLOAD] = '\0';
    tempChecksum = (uint16_t) rawFrame[bufferPosition] << 8;
    tempChecksum += (uint8_t) rawFrame[bufferPosition+1];
    storageFrame->checksum = tempChecksum;

    return errCode;
}

uint8_t createRawFrame(char rawFrame[BYTES_FRAME_TOTAL], uint8_t msgId, uint8_t sndId, uint8_t ack, uint8_t peerNr, inputData userInputData)
{
 
    uint8_t errCode = 0;
    uint8_t bufferPosition = 0;
    char payloadTemp[BYTES_PAYLOAD + 1];
    uint16_t checksum;
    char checksumTemp[BYTES_CHECKSUM + 1];
    checksumTemp[BYTES_CHECKSUM] = '\0';

    rawFrame[0] = msgId;
    bufferPosition += BYTES_MSG_ID;
    rawFrame[bufferPosition] = sndId;
    bufferPosition += BYTES_SENDER_ID;
    rawFrame[bufferPosition] = ack;
    bufferPosition += BYTES_ACK;
    rawFrame[bufferPosition] = peerNr;
    bufferPosition += BYTES_PEER_NR;
    rawFrame[bufferPosition] = userInputData.msgLength;
    
    bufferPosition += BYTES_PAYLOAD_LENGTH;
    
    //copy inputData to payloadTemp 
    for (uint8_t i = 0; i < userInputData.msgLength; i++)
    {
        payloadTemp[i] = userInputData.userMsg[i];
        //rawFrame[bufferPosition + i] = inputData[i];
    }
    //fill the unused bytes of the payload with 0x00s
    for (uint8_t i = 0; i < BYTES_PAYLOAD - userInputData.msgLength; i++)
    {
        payloadTemp[userInputData.msgLength + i] = 0x00;
    }
    payloadTemp[BYTES_PAYLOAD] = '\0';

    //copy all data bytes except the terminating char to rawFrame
    memcpy(rawFrame+bufferPosition, payloadTemp, BYTES_PAYLOAD);

    bufferPosition += BYTES_PAYLOAD;
    errCode = calcChecksum(payloadTemp, &checksum);
    checksumTemp[0] = checksum >> 8;
    checksumTemp[1] = (checksum & 0xFF);;
    memcpy(rawFrame+bufferPosition, &checksumTemp, BYTES_CHECKSUM);
    bufferPosition += BYTES_CHECKSUM;
    rawFrame[bufferPosition] = '\0';

    return errCode;
}

uint8_t createLog(char* filepath)
{
    uint8_t errCode = 0;
    FILE* myFile = fopen(filepath, "w");
    if(myFile == NULL)
    {
        printf("Error opening logfile!\n");
    }
    fprintf(myFile, "MessageID PeerNr Payload\n");
    fclose(myFile);

    return errCode;
}

uint8_t logMessage(Frame msgFrame, char* filepath)
{
    uint8_t errCode = 0;
    //printf("Writing to log...\n");
    FILE* myFile = fopen(filepath, "a");
    if(myFile == NULL)
    {
        printf("Error opening logfile!\n");
    }
    //print MsgId Peer Payload
    fprintf(myFile, "%9d %6d %s \n", msgFrame.msgId, msgFrame.peerNr, msgFrame.payload);
    fclose(myFile);

    return errCode;
}

uint8_t injectError(char* rawFrame, uint16_t bitIndex)
{
    uint8_t errCode = 0;
    Frame inputFrame;
    uint8_t byteIndex;
    uint8_t indexWithinByte;

    storeFrame(&inputFrame, rawFrame);

    //calculate which byte of the payload to address
    for(byteIndex = 0; byteIndex * 8 <= bitIndex; byteIndex++);
    byteIndex -= 1;
    indexWithinByte = bitIndex % 8;

    rawFrame[BYTES_MSG_ID + BYTES_SENDER_ID + BYTES_ACK + BYTES_PEER_NR + BYTES_PAYLOAD_LENGTH + byteIndex] ^= (1 << (7-indexWithinByte));


    return errCode;
}