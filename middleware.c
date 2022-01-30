#include "middleware.h"


//Global Variables:
char frameToSend[BYTES_FRAME_TOTAL];     //Connection between MW and UI
uint8_t groupsize = 3;          //Amount of group members (needs to be set by UI)
uint8_t myID = 99;               //ID of Peer (needs to be set by UI)
uint32_t message_cnt = 0;       //message counter represents the latest message id
uint8_t mesID_cnt[MAX_PEERS];
groupmember mygroup[MAX_PEERS];

extern inputData myInputData;

int middleware( void )
{

    //TODO: Wait for ID input

//----------------------------------------------------------------------------------
// Get Informations from UI (about Group) -> IP, Port, ID-Number

    getMembers(&mygroup, groupsize);


    for (int i = 0; i < groupsize; i++)
    {
        printf("ID: %i ,",mygroup[i].id);
        printf("IPv4: %s ,",mygroup[i].ipv4);
        printf("Port: %i\n",mygroup[i].port);
    }
    

//----------------------------------------------------------------------------------
// Start Middleware

    int mysocket;
    setupMW(&mygroup,myID,&mysocket);

//----------------------------------------------------------------------------------
// Get Message (for testing purposes directly in MW)

    char buff[BYTES_PAYLOAD];                    //message buffer
    char *fptr;
    ssize_t bytes_recv;


    char *message[BYTES_FRAME_TOTAL];
    message[0]='\0';

    char *received[BYTES_FRAME_TOTAL];
    received[0]='\0';


// DO FOREVER:

    while (true)
    {

        //TODO: Get User Input from buffer

//----------------------------------------------------------------------------------
// Send Message to group
        if (myInputData.newMsgReceived == true)
        {
            printf("%s",frameToSend);
            sendgroup(&mygroup,groupsize,myID,&mysocket,frameToSend);
            myInputData.newMsgReceived = false;
        }

//----------------------------------------------------------------------------------
// Receive Message from group

        recvgroup(&mysocket,received);

        //TODO: Parse Message (or get it already parsed from function)
        //TODO: Message already received? If yes: ignore, if no: hand massage text over to "sendgroup"
    }

    if(close(mysocket)==-1)
    {
        printf("ERROR: Could not close socket!");
    }

    return 0;

}



int getID(void){

    //TODO: Get my own infos from ui

    return 1;
}

void getMembers(groupmember (*mygroup)[], int groupsize){

     for (int i = 0; i < groupsize; i++)     //TODO: get memberinfos from ui --> DONE!!!
    {
        // (*mygroup)[i].id = i;
        // printf("%i",(*mygroup)[i].id);
        // strcpy((*mygroup)[i].ipv4,"127.0.0.1");
        // (*mygroup)[i].port = 8080+i;
        (*mygroup)[i].addr.sin_family = AF_INET;

        if(inet_aton((*mygroup)[i].ipv4, &(*mygroup)[i].addr.sin_addr)==0)
        {
            printf("ERROR: Conversion failed!");
        }

        (*mygroup)[i].addr.sin_port = htons((*mygroup)[i].port);
    } 

    //return (mygroup);
}

int setupMW(groupmember (*mygroup)[],int myID, int *mysocket){

    mesID_cnt[myID] = 1;
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
    
    printf("Message to send: %s \n", payload);


    for (int i = 0; i < groupsize; i++)
    {

        if (i == myID)
        {
            Frame frame_recv;
            storeFrame(&frame_recv, payload);
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

        usleep(1000); // Requirement: Wait for 1s between transmissions
    }

    mesID_cnt[myID]=(mesID_cnt[myID]+1)%256;
    
    return (0);
    
}

int ACK(groupmember (*mygroup)[], int *mysocket, int peerid){

    // fd_set lese_sock;
    // FD_ZERO(&lese_sock);
    // FD_SET(*mysocket, &lese_sock);

    // struct timeval mytimeout = {0,100}; //Wait for acknoledgement 0s 100ms

    Frame frame_recv;
    char *received[BYTES_FRAME_TOTAL];
    received[0]='\0';
    
    //TODO: waiting for message only a limited time doesnt work (select function)

    // if(select(*mysocket+1,&lese_sock,NULL,NULL,&mytimeout) == -1)
    // {
    //     printf("ERROR: Wait for acknoledgement failed!");
    // }

    usleep(100000); //wait 100ms for acknoledgement

    if (recvfrom(*mysocket, received, BYTES_FRAME_TOTAL, 0, (const struct sockaddr *)&(*mygroup)[peerid].addr, sizeof((*mygroup)[peerid].addr)) == (ssize_t)-1)
    {
        printf("ERROR: Could not receive!");
    }

    storeFrame(&frame_recv, received);

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

    if ((frame_recv.ack == 1)&&(frame_recv.msgId == mesID_cnt[myID])&&(frame_recv.peerNr==peerid))
    {
        return (0);
    }
    

    received[0]='\0'; //Clear received buffer

    return(-1);
}

int recvgroup(int *mysocket, char *message_recv){

    ssize_t bytes_recv;
    Frame frame_recv;
    
    int8_t idtoack;
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
        printf("Checksum: %d\n", frame_recv.checksum);
        
        //printf("TEST0");
        inputData buff = {.userMsg=frame_recv.payload};

        if (frame_recv.ack == 0)
        {
            idtoack = frame_recv.peerNr;
            frame_recv.ack = 1;
            frame_recv.peerNr = myID;

            
            frame_recv.payload[BYTES_PAYLOAD]='\0';
            buff.userMsg[0]='\0';
            
            //memcpy(buff.userMsg, frame_recv.payload, frame_recv.payloadLength);
            buff.msgLength = frame_recv.payloadLength;

            createRawFrame(message_recv,frame_recv.msgId,frame_recv.sndId,frame_recv.ack,frame_recv.peerNr,buff);

            // printf("MsgID: %d\n", frame_recv.msgId);
            // printf("SndID: %d\n", frame_recv.sndId);
            // printf("ACK: %d\n", frame_recv.ack);
            // printf("PeerNr: %d\n", frame_recv.peerNr);
            // printf("PayloadLength: %d\n", frame_recv.payloadLength);
            // printf("Payload: %s\n", frame_recv.payload);
            
            if (sendto(*mysocket, message_recv, (size_t)BYTES_FRAME_TOTAL,MSG_CONFIRM,(const struct sockaddr *)&mygroup[idtoack].addr, sizeof(mygroup[idtoack].addr)) == (ssize_t)-1)
            {
                printf("ERROR: Could not send!");
            }
            printf("Acknoledged!\n");

            if (mesID_cnt[frame_recv.sndId] < frame_recv.msgId)
            {
                printf("RESEND to group!\n");
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
        //printf("Checksum before bitshift: %d \n", checksum);
        if(checksum > 0xFFFF)
        {
            checksum -= 0xFFFF;
        }
        //printf("Checksum after bitshift: %d\n", checksum);
    }
    *checksumBuffer = ~checksum;

    return errCode;
}

uint8_t storeFrame(Frame* storageFrame, char rawFrame [BYTES_FRAME_TOTAL])
{
    uint8_t errCode = 0;
    uint8_t bufferPosition = 0;

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
    memcpy(&(storageFrame->checksum), rawFrame + bufferPosition, BYTES_CHECKSUM);

    return errCode;
}

uint8_t createRawFrame(char rawFrame[BYTES_FRAME_TOTAL], uint8_t msgId, uint8_t sndId, uint8_t ack, uint8_t peerNr, inputData userInputData)
{
 
    uint8_t errCode = 0;
    uint8_t bufferPosition = 0;
    char payloadTemp[BYTES_PAYLOAD + 1];
    uint16_t checksum;

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
    memcpy(rawFrame+bufferPosition, &checksum, BYTES_CHECKSUM);
    bufferPosition += BYTES_CHECKSUM;
    rawFrame[bufferPosition] = '\0';

    return errCode;
}

uint8_t createLog(char* filepath)
{
    uint8_t errCode;
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
    uint8_t errCode;
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