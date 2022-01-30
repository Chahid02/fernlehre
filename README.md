# Fernlehre UEVESY - Group Communication

## How to use:

1. Check if the config.txt file is ready for the program to read

    The file comes prefilled with five peers which can be used if the program gets started on the same system (all peers have the localhost as IP and different ports)

2. Start program

    Use the following command (if you are in the same folder) in the terminal

        ./fernlehre


    

 

Sequence Nr: 1 byte -> uint8

Sender ID: 1 byte -> uint8

ACK/No ACK: 1 byte -> uint8

Peer Nr: 1 byte -> uint8

Payload length: 1 byte -> uint8

Payload: 32 byte -> char[32]

Checksum: 2 byte -> uint16

Terminating Character: 1 byte -> char
