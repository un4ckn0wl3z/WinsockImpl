#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
        printf("gethostname failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    printf("Host name: %s\n", hostname);

    // Cleanup Winsock
    WSACleanup();
    return 0;
}