#define _WINSOCK_DEPRECATED_NO_WARNINGS


#include <iostream>
#include <stdio.h>
#include <WinSock2.h>

#define PORT 9999
struct sockaddr_in srv;

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int nListenerSocket;
int nClientSocket[5];
fd_set fr;
int nMaxFd;

FILE* fp = NULL;

void AcceptNewConnection()
{
	int nIndex;
	int nAvailIndex = -1;
	for (nIndex = 0; nIndex < 5; nIndex++)
	{
		if (nClientSocket[nIndex] == 0)
		{
			nAvailIndex = nIndex;
			break;
		}
	}
	if (nIndex == 4)
	{
		cout << endl << "Server Busy..";
		return;
	}

	int nCliSocket = accept(nListenerSocket, NULL, NULL);
	if (nCliSocket < 0)
	{
		cout << endl << "Failed to accept a new client..";
		return;
	}
	nClientSocket[nAvailIndex] = nCliSocket;

}

void RecvSendClientMsg(int nCliSocket)
{
	char sBuff[255] = { 0, };
	int err = recv(nCliSocket, sBuff, 255, 0);
	if (err < 0)
	{
		cout << endl << "Error at client socket:" << nCliSocket;
		closesocket(nCliSocket);
		for (int nIndex = 0; nIndex < 5; nIndex++)
		{
			if (nClientSocket[nIndex] == nCliSocket)
			{
				nClientSocket[nIndex] = 0;
			}
		}
	}
	else
	{
		//cout << endl << "The msg from client:[" << nCliSocket << "]-->" << sBuff;
		//Send the response to client
		char sMsg[255] = { 0, };
		sprintf_s(sMsg, "\nThe Msg from Client[%d]: %s",
			nCliSocket, sBuff);
		fprintf(fp, sMsg);

		err = send(nCliSocket, "ACK FROM SERVER", 16, 0);
		if (err < 0)
		{
			cout << endl << "Error at client socket:" << nCliSocket;
			closesocket(nCliSocket);
			for (int nIndex = 0; nIndex < 5; nIndex++)
			{
				if (nClientSocket[nIndex] == nCliSocket)
				{
					nClientSocket[nIndex] = 0;
				}
			}
		}
		else
		{
			cout << endl << "Sent the response to client..";
		}
	}
}

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


	//Code for handling Multiple clients requests..
	nMaxFd = nListenerSocket;
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;

	//Open the file.
	int errFile = fopen_s(&fp, "MSGCLIENTS.txt", "a");
	if (errFile < 0)
	{
		cout << endl << "The file not opened..";
		return EXIT_FAILURE;
	}

	while (1)
	{
		FD_ZERO(&fr);
		FD_SET(nListenerSocket, &fr);
		for (int nIndex = 0; nIndex < 5; nIndex++)
		{
			if (nClientSocket[nIndex] != 0)
			{
				FD_SET(nClientSocket[nIndex], &fr);
			}
		}

		//select API
		err = select(nMaxFd + 1, &fr, NULL, NULL, &tv);
		if (err > 0)
		{
			//We will handle new connection/new message
			//from existing client
			if (FD_ISSET(nListenerSocket, &fr))
			{
				//There is a new connection
				//Accept it
				AcceptNewConnection();
			}
			for (int nIndex = 0; nIndex < 5; nIndex++)
			{
				if (nClientSocket[nIndex] != 0)
				{
					if (FD_ISSET(nClientSocket[nIndex], &fr))
					{
						RecvSendClientMsg(nClientSocket[nIndex]);
					}
				}
			}

		}
		else if (err == 0)
		{
			//No one conneting / sending message
			cout << endl << "No new connection/message..";
		}
		else
		{
			cout << endl << "Failed select!! closing the server";
			return (EXIT_FAILURE);
		}
	}

	return 0;
}

