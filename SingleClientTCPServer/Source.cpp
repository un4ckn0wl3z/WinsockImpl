#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 9999
struct sockaddr_in srv;

using namespace std;

int nListenerSocket;

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
	nListenerSocket = socket(AF_INET, SOCK_STREAM,
		IPPROTO_TCP);
	if (nListenerSocket < 0)
	{
		cout << endl << "The socket failed to open..";
	}
	else
	{
		cout << endl << "Socket opened successfully..";
	}

	//Set the socket options
	int optval = 1;
	err = setsockopt(nListenerSocket, SOL_SOCKET,
		SO_REUSEADDR, (const char*)&optval, sizeof(optval));
	if (err < 0)
	{
		cout << endl << "Not able to set the socket options.";
	}
	else
	{
		cout << endl << "Succefully set the socket options.";
	}


	//Control the mode of socket (I/O)
	//Blocking : recv, send
	//Non blocking socket: recv, send
	u_long nMode = 0;
	err = ioctlsocket(nListenerSocket, FIONBIO,
		&nMode);
	if (err < 0)
	{
		cout << endl << "The ioctlsocket failed..";
	}
	else
	{
		cout << endl << "Socket Mode set to blocking..";
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
	}
	else
	{
		cout << endl << "Bind to local port successfully..";
	}

	//Listen and accept the new connection from client
	err = listen(nListenerSocket, 5);
	if (err < 0)
	{
		cout << endl << "Not able to listen..";
	}
	else
	{
		cout << endl << "Started Listening to the port..";
	}

	//Accept the new incoming connection
	struct sockaddr_in client;
	int nLen = sizeof(client);

	//nClientSocket will become the communication socket
	int nClientSocket = accept(nListenerSocket,
		(struct sockaddr*)&client, &nLen);

	if (nClientSocket < 0)
	{
		cout << endl << "Failed to accept the new connction";

	}
	else
	{
		cout << endl << "Connected with a new client";
		cout << endl << "IP Address:" << inet_ntoa(client.sin_addr);
	}

	char sBuff[1024] = { 0, };
	err = recv(nClientSocket, sBuff, 1024, 0);
	if (err > 0)
	{
		//Show the message from client
		cout << endl << sBuff;

		//Respond to client
		err = send(nClientSocket, "ACKNOWLEDGED..",
			13, 0);
		if (err < 0)
		{
			closesocket(nClientSocket);
			nClientSocket = 0;
		}
	}
	else
	{
		closesocket(nClientSocket);
		nClientSocket = 0;
	}

	return 0;
}

