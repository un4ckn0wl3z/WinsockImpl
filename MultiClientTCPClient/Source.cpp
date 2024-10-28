#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;
#define PORT 9999

struct sockaddr_in srv;

int main()
{
    WSADATA ws;
    if (WSAStartup(MAKEWORD(2, 2), &ws) < 0)
    {
        cout << endl << "WSAStartup call failed";
        return EXIT_FAILURE;
    }

    int nSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (nSocket < 0)
    {
        cout << endl << "The socket API call failed";
        return EXIT_FAILURE;
    }

    srv.sin_family = AF_INET;
    srv.sin_port = htons(PORT);
    srv.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(&srv.sin_zero, 0, sizeof(srv.sin_zero));

    int err = connect(nSocket, (struct sockaddr*)&srv,
        sizeof(srv));
    if (err < 0)
    {
        cout << endl << "connect failed..";
        return EXIT_FAILURE;
    }

    //send the message to the server
    while (1)
    {
        Sleep(2000);
        err = send(nSocket, "Fixed Test Message", 32, 0);
        if (err < 0)
        {
            cout << endl << "send API failed to deliver the message";
            return EXIT_FAILURE;
        }

        char sBuff[255] = { 0, };
        err = recv(nSocket, sBuff, 255, 0);
        if (err < 0)
        {
            cout << endl << "recv failed..";
            return EXIT_FAILURE;
        }
        cout << endl << "Message from Server:" << sBuff;
    }


    return 0;
}

