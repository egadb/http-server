#include <stdio.h>
#include <winsock2.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>

#define PORT 8080
int main(int argc, char const *argv[])
{
    //Initializes winsock2
    static WSADATA wsaData;
    int wsaerr = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaerr) {
        printf("WSAStartup failed: %d\n", wsaerr);
        exit(1);
    }
    
    //Initializes variables
    int server_fd, new_socket; long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);


    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    //Binds to address:port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }


    std::string outputText;
    std::string html = "";
    std::string fileName;
    std::cout << "Enter file name :";
    std::cin >> fileName;

    std::ifstream htmlReader(fileName);

    //Reads html file
    while (getline (htmlReader, outputText)) {
        html += outputText;
    }
    htmlReader.close();

    //Creates response header
    std::string messageString = "HTTP/1.1 200 OK";
    messageString += "\n";
    messageString += "Content-Type: text/html";
    messageString += "\n";
    messageString += "Content-Length: " ;
    messageString += std::to_string(html.length());
    messageString += "\n\n" + html;
    const char* messageCharArray = messageString.c_str();

    //Running
    while(1)
    {
        printf("\n---------------- Waiting for new connection ----------------\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        send(new_socket , messageCharArray , strlen(messageCharArray), MSG_DONTROUTE);
        printf("-------------------- Hello message sent --------------------");
        close(new_socket);
    }
    return 0;
}