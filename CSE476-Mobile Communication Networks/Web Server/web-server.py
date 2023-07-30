#############################
#   Abdurrahman Bulut       #
#   1901042258              #
#   CSE 476 Term Project    #
#   Part-1 | Web Server     #
#############################

# import socket module
from socket import *

serverSocket = socket(AF_INET, SOCK_STREAM)

# Prepare a server socket
port = 6789

# Fill in start
serverSocket.bind(('', port))

# put the socket into listening mode
serverSocket.listen(1)
# Fill in end

while True:
    # Establish the connection
    print("Ready to serve...")
    # Fill in start
    connectionSocket, addr = serverSocket.accept()
    # Fill in end

    try:
        # Fill in start
        message = connectionSocket.recv(1024)
        # Fill in end

        filename = message.split()[1]
        f = open(filename[1:])

        # Fill in start
        outputdata = f.read()
        f.close()  
        # Fill in end

        # Fill in start
        # Send one HTTP header line into socket
        info = "HTTP/1.1 200 OK\r\n\r\n "
        print(info)
        connectionSocket.send(info.encode())
        # Fill in end

        # Send the content of the requested file to the client
        for i in range(0, len(outputdata)):
            connectionSocket.send(outputdata[i].encode())
        connectionSocket.close()

    except IOError:

        # Send response message for file not found
        # Fill in start
        info = "HTTP/1.1 404 Not Found\r\n\n"
        print(info)
        connectionSocket.send(info.encode())

        info = "<html><head><title> 404 </title></head><body><h1>404 Not Found!</h1></body></html>\r\n"
        connectionSocket.send(info.encode())
        # Fill in end

        # Close client socket
        # Fill in start
        connectionSocket.close()
        # Fill in end

serverSocket.close()