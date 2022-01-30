# Fernlehre UEVESY - Group Communication

## How to use:

1. Check if the config.txt file is ready for the program to read

    The file comes prefilled with five peers which can be used if the program gets started on the same system (all peers have the localhost as IP and different ports)

2. Start program

    Use the following command (if you are in the same folder) in the terminal

        ./fernlehre

3. You are asked to input the path of the configfile

    For example:

        ../config.txt
    
4. You are asked to input the path of the logfile

    For example:

        ../log.txt

5. Now you see the menu, and you can select the following:

    1. Send message: You can input a message (max 32 bytes) and the program sends it to all members (To fulfill the INTEGRITY requirement, the message gets signed with a unique Message ID witch is a combination of sequence number and sender id)
    2. Show received messages: You can see all received messages (including the own sent messages, to fulfill the VALIDITY requirement)
    3. Set error injection bit: You can set the error injection bit, which flips the bit (on the location you wrote) in the next message which you want to send.
    4. Show infos about group: You can show the config table.

The AGREEMENT requirement is fulfilled, because EVERY new message (with a new or not yet received ID) gets delivered.


## Limitations

Due to a lack of timemanagement we had to take some minor limitations into account:

1. Only 255 Messages can be sent from one peer
2. The maximum message size is 32 

## Message Frame

Sequence Nr: 1 byte -> uint8

Sender ID: 1 byte -> uint8

ACK/No ACK: 1 byte -> uint8

Peer Nr: 1 byte -> uint8

Payload length: 1 byte -> uint8

Payload: 32 byte -> char[32]

Checksum: 2 byte -> uint16

Terminating Character: 1 byte -> char
