#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 9999
#define SERVER_IP "127.0.0.1"

using namespace std;

int main()
{
    WSADATA ws;
    int err = WSAStartup(MAKEWORD(2, 2), &ws);
    if (err != 0)
    {
        cout << "Failed to initialize Winsock" << endl;
        return EXIT_FAILURE;
    }

    // Creating a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket < 0)
    {
        cout << "Socket creation failed" << endl;
        WSACleanup();
        return EXIT_FAILURE;
    }

    // Server address configuration
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connecting to the server
    err = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (err < 0)
    {
        cout << "Connection to server failed" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return EXIT_FAILURE;
    }
    cout << "Connected to server" << endl;

    // Sending message to the server
    const char* message = "Hello, Server!";
    send(clientSocket, message, strlen(message), 0);

    // Receiving response from the server
    char recvBuffer[1024] = { 0 };
    int recvBytes = recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);
    if (recvBytes > 0)
    {
        cout << "Server response: " << recvBuffer << endl;
    }
    else
    {
        cout << "Failed to receive server response" << endl;
    }

    // Closing the socket
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
