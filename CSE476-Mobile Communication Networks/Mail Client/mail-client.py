#############################
#   Abdurrahman Bulut       #
#   1901042258              #
#   CSE 476 Term Project    #
#   Part-3 | Mail client    #
#############################

from socket import *
import base64
import ssl

# sender email + sender password -> auth + security control -> mail sent
senderMail = "abdrrhmnbulut@gmail.com"
password= "Give Password"

receiverMail = "abdurrahmanbulut15@gmail.com"


msg = "\r\n I love computer networks!"
endmsg = "\r\n.\r\n"
port = 587


# Choose a mail server (I choose Google mail server) and call mailserver
mailserver = ("smtp.gmail.com", port) 

# Create socket called clientSocket and establish a TCP connection with mailserver
#Fill in start  
clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect(mailserver)
#Fill in end

recv = clientSocket.recv(1024)
print (recv) 
if recv[:3] != '220':
    print ('220 reply not received from server.')

# Send HELO command and print server response.
heloCommand = 'HELO Alice\r\n'
clientSocket.send(heloCommand)
recv1 = clientSocket.recv(1024)
print (recv1)
if recv1[:3] != '250':
    print ('250 reply not received from server.' )


#TLS for Google mail server
starttls = 'starttls\r\n'
clientSocket.send((starttls).encode())
recv2=clientSocket.recv(1024)
print (recv2)
if recv2[:3] != '220':
    print ('220 reply not received from server.')


clientSocket = ssl.wrap_socket(clientSocket)

# AUTH with base64 encoded senderMail and password
base64_str = ("\x00"+senderMail+"\x00"+password).encode()
base64_str = base64.b64encode(base64_str)
authMsg = "AUTH PLAIN ".encode()+base64_str+"\r\n".encode()
clientSocket.send(authMsg)
recv_auth = clientSocket.recv(1024)
print(recv_auth.decode())
if recv1[:3] != '250':
    print('250 reply not received from server.')


# Send MAIL FROM command and print server response.
# Fill in start
mailFrom = "MAIL FROM: <"+senderMail+"> \r\n"
clientSocket.send(mailFrom.encode())
recv6 =clientSocket.recv(1024)
print (recv6)
if recv6[:3] != '250':
    print ('250 reply not received from server.')
# Fill in end

# Fill in start
# Send RCPT TO command and print server response.
rcptTo = "RCPT TO: <"+receiverMail+"> \r\n"
clientSocket.send(rcptTo.encode())
recv7 = clientSocket.recv(1024)
print (recv7)
if recv7[:3] != '250':
    print ('250 reply not received from server.')
# Fill in end

# Fill in start
# Send DATA command and print server response.
data = "DATA\r\n"
clientSocket.send(data.encode())
recv8 = clientSocket.recv(1024).decode()
print (recv8)
if recv8[:3] != '354':
    print ('354 reply not received from server.') 
# Fill in end


# Fill in start
# Send message data.
message = 'SUBJECT: SMTP Mail Client Testing \nContent \r\n'
clientSocket.send(message.encode())
clientSocket.send(msg.encode())
# Fill in end

# Fill in start
#message ends with single period.
clientSocket.send(endmsg.encode())
recv9 = clientSocket.recv(1024)
print (recv9.decode())
if recv9[:3] != '250':
    print ('250 reply not received from server.')
# Fill in end


# Fill in start
# Send QUIT command and get server response.
clientSocket.send("QUIT\r\n".encode())
recv10=clientSocket.recv(1024)
print ('Server response after QUIT command: ', recv10)
if recv10[:3] != '221':
    print ('221 reply not received from server.')

print("Mail Sent")
clientSocket.close() #close socket.
# Fill in end

