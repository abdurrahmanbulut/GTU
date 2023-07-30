################################
#   Abdurrahman Bulut          #
#   1901042258                 #
#   CSE 476 Term Project       #
#   Part-2 | UDP Pinger Client #
################################

from socket import *
from time import *

clientSocket = socket(AF_INET,SOCK_DGRAM) #create the socket.
clientSocket.settimeout(1) #set the timeout at 1 second.
sequence_number = 1 #ping counter.

while sequence_number<=10:

    startTime = time() # keep current time.

    try:
        message = "PING " + str(sequence_number) + " " + str(strftime("%H:%M:%S"))
        clientSocket.sendto(message.encode(), ('Localhost', 12000)) #send ping message via localhost.
        print("Sent Message: " + message)

        data, address = clientSocket.recvfrom(1024) #recieving message from server.
        responseTime = time()   #finish time.
        rtt=responseTime-startTime # the round trip time (RTT)

        print("Receiving Pong Message:",data)
        print("RTT: ", str(rtt) + " seconds\n")

    except timeout:
        print("Request #" + str(sequence_number) + " Time Out\n")

    sequence_number += 1

print("Closing")
clientSocket.close()
