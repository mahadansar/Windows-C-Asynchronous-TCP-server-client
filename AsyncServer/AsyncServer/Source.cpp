#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <conio.h>
#include <signal.h>
#include <thread>
#define PORT 8080 
#pragma comment(lib, "ws2_32.lib")

void recieve(SOCKET m_socket){
	while(1)
	{
		char recvbuf[1000] = "";
		int bytesRecv = recv(m_socket, recvbuf, 1000, 0); 
		if(bytesRecv > 0)
		{
			printf("\n\t\t\t\t\xDB Mahad: %s", recvbuf);
			printf("\n\xDB Hammad: ");
		}
	}
}
void sent(SOCKET m_socket){
	while(1)
	{
		printf("\n\xDB Hammad: ");
		char sendbuf[1000];
		gets_s(sendbuf);
		send(m_socket, sendbuf, strlen(sendbuf), 0);
	}
}

int main(int argc, char const *argv[])
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, FOREGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
		printf("Error \n");

	SOCKET m_socket;
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
	}

	int s = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in sock_info;
	sock_info.sin_family = AF_INET;
	sock_info.sin_addr.s_addr = INADDR_ANY;
	sock_info.sin_port = htons(PORT);
	if (bind(m_socket, (SOCKADDR*)&sock_info, sizeof(sock_info)) == SOCKET_ERROR)
	{
		printf("bind() failed.\n");
		closesocket(m_socket);
	}

	if (listen(m_socket, 1) == SOCKET_ERROR)
		printf("Error listening on socket.\n");

	SOCKET AcceptSocket;
	while (1)
	{ 
		AcceptSocket = SOCKET_ERROR; 
		while (AcceptSocket == SOCKET_ERROR)
		{
			AcceptSocket = accept(m_socket, NULL, NULL);
		}
		m_socket = AcceptSocket;
		break; 
	}

	std::thread th1 (&recieve, m_socket);
	std::thread th2 (&sent, m_socket);

	th1.join(); 
	th2.join(); 
	
	closesocket(m_socket);

	_getch();
	return 0;
}

//Bind    => id address size
//connect => socket, structure(ipv4, address, port), structure size)
//listen  => socket 1(no. of tcp connections that can be queued on server)
//send    => socket buffer length flags
// recv   => socket , buffer , size , flags