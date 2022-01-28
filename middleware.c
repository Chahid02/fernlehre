#include "middleware.h"

int middleware( void )
{

//----------------------------------------------------------------------------------
// Get Information from UI (about myself) -> ID-Number

    int myID = getID(); //TODO: Get ID from UI (global variable would be suitable)

//----------------------------------------------------------------------------------
// Get Informations from UI (about Group) -> IP, Port, ID-Number

    int groupsize = 2; //TODO: Get groupsize
    groupmember mygroup[groupsize];
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


    char *message[MESSAGESIZE];
    message[0]='\0';

    char *received[MESSAGESIZE];
    received[0]='\0';


// DO FOREVER:

    while (true)
    {

        //TODO: Get User Input from buffer

//----------------------------------------------------------------------------------
// Send Message to group

        sendgroup(&mygroup,groupsize,myID,&mysocket,message);

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

    for (int i = 0; i < groupsize; i++)     //TODO: get memberinfos from ui
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

    //return (mygroup);
}

int setupMW(groupmember (*mygroup)[],int myID, int *mysocket){


    // Create socket file descriptor
    *mysocket = socket((*mygroup)[myID].addr.sin_family, (enum __socket_type) SOCK_DGRAM, 0);

    if( *mysocket == -1)
    {
        printf("ERROR: Could not create socket!");
        return(-1);
    }

    if (bind(*mysocket, (const struct sockaddr *)&(*mygroup)[myID].addr, sizeof((*mygroup)[myID].addr)) < 0 )
    {
        printf("ERROR: Bind failed");
        return(-1);
    }

    return(0);

}

int sendgroup(groupmember (*mygroup)[], int groupsize, int myID, int *mysocket, char *payload){
    
    printf("Messege to send: %s \n", payload);

    //TODO: Build message -> MES_NR + ACK + PEER + payload + checksum

    for (int i = 0; i < groupsize; i++)
    {

        if (i == myID)
        {
            //TODO: "send" to UI
            
        }
        else
        {
            
            for (int y = 0; y < 3; y++)
            {
                if (sendto(*mysocket, (char *)payload, strlen((char *)payload),MSG_CONFIRM,(const struct sockaddr *)&(*mygroup)[i].addr, sizeof((*mygroup)[i].addr)) == (ssize_t)-1) //TODO: Send to all!
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
    
    return (0);
    
}

int ACK(groupmember (*mygroup)[], int *mysocket, int peerid){

    fd_set lese_sock;
    FD_ZERO(&lese_sock);
    FD_SET(*mysocket, &lese_sock);

    struct timeval mytimeout = {0,100}; //Wait for acknoledgement 0s 100ms

    char *received[MESSAGESIZE];
    received[0]='\0';
    
    //TODO: waiting for message only a limited time doesnt work (select function)

    if(select(*mysocket+1,&lese_sock,NULL,NULL,&mytimeout) == -1)
    {
        printf("ERROR: Wait for acknoledgement failed!");
    }
    
    if (recvfrom(*mysocket, received, MESSAGESIZE, 0, (const struct sockaddr *)&(*mygroup)[peerid].addr, sizeof((*mygroup)[peerid].addr)) == (ssize_t)-1)
    {
        printf("ERROR: Could not receive!");
    }

    //TODO: PARSE Message and check for ACK

    received[0]='\0'; //Clear received buffer

    return(0);
}

int recvgroup(int *mysocket, char *message_recv){

    ssize_t bytes_recv;
    message_recv[0] = '\0';

    bytes_recv = recv(*mysocket,message_recv,(size_t)MESSAGESIZE-1,0);

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
        if(checksum > 0xFF)
        {
            checksum -= 0xFF;
        }
    }
    *checksumBuffer = ~checksum;

    return errCode;
}
