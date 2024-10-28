#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <WinSock2.h>

#define PORT 9999
struct sockaddr_in srv;
struct sockaddr_in cli;

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int nListenerSocket;
FILE* fp;

int main()
{
	WSADATA ws;
	//Loading the DLL (for socket APIs) in your process
	int err = WSAStartup(MAKEWORD(2, 2), &ws);
	if (err == 0)
	{
		cout << endl << "Successfully Initialized socket LIB";
	}
	else if (err == -1)
	{
		cout << endl << "Not initialized the SOCKET API..";
		return EXIT_FAILURE;
	}

	//IPV4, IPV6
	nListenerSocket = socket(AF_INET, SOCK_DGRAM,
		IPPROTO_UDP);
	if (nListenerSocket < 0)
	{
		cout << endl << "The socket failed to open..";
		return EXIT_FAILURE;
	}
	else
	{
		cout << endl << "Socket opened successfully..";
	}


	//Bind the server code to a port
	//IP Address
	//Port
	srv.sin_family = AF_INET;
	srv.sin_port = htons(PORT);
	srv.sin_addr.s_addr = INADDR_ANY;
	memset(&srv.sin_zero, 0, sizeof(srv.sin_zero));

	err = bind(nListenerSocket, (struct sockaddr*)&srv,
		sizeof(srv));

	if (err < 0)
	{
		cout << endl << "Failed to bind to local port..";
		return EXIT_FAILURE;
	}
	else
	{
		cout << endl << "Bind to local port successfully..";
	}

	fopen_s(&fp, "UDPMSGS.txt", "a");

	while (1)
	{
		char sBuff[255] = { 0 };
		int nLen = sizeof(cli);

		err = recvfrom(nListenerSocket, sBuff, 255,
			0, (struct sockaddr*)&cli, &nLen);
		if (err > 0)
		{
			fprintf(fp, "From Client:%s", sBuff);
			fflush(fp);
		}
		else
		{
			cout << endl << "recvfrom falied..";
		}

		err = sendto(nListenerSocket, "ACK FROM SERVER",
			16, 0, (struct sockaddr*)&cli, nLen);
		if (err < 0)
		{
			cout << endl << "sendto failed..";
		}
	}

	return 0;
}

