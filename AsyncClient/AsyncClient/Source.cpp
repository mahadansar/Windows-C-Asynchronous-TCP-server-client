#include <iostream>
#include <winsock2.h>
#include <thread>
#include <conio.h>
#include <stdio.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996) 

HANDLE hStdin; 
DWORD fdwSaveOldMode;

//Mouse Events
void mouseEventHandler();
void ErrorExit(LPSTR);
void MouseEventProc(MOUSE_EVENT_RECORD); 
void GetMousePosWin(MOUSE_EVENT_RECORD, bool&);
void gotoXY(int x, int y);
void ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);
//-------------------------------------------------------

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

void recieve(SOCKET m_socket){
	while(1)
	{
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			char recvbuf[1000] = "";
			int bytesRecv = recv(m_socket, recvbuf, 1000, 0);
			SetConsoleTextAttribute(hConsole, 3);
			if(bytesRecv > 0)
			{
				printf("\n\t\t\t\t\xDB Hammad: %s", recvbuf);
				SetConsoleTextAttribute(hConsole, 1);
				printf("\n\xDB Mahad: ");
			}
	}
}
void sent(SOCKET m_socket){
	while(1)
	{
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 1);
			printf("\n\xDB Mahad: ");

			char sendbuf[1000];
			gets_s(sendbuf);
			send(m_socket, sendbuf, strlen(sendbuf), 0);
	}
}

int main()
{
	//Random mouse functions
	mouseEventHandler();

	//Socket Code Starts Here

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2),
		&wsaData);
	if (iResult != NO_ERROR)
		printf("Error at WSAStartup()\n");

	SOCKET m_socket;
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET)
	{
		printf("Error at socket(): %ld\n", WSAGetLastError());
		WSACleanup();
	}
	else
	{
		int s = socket(PF_INET, SOCK_STREAM, 0);
		struct sockaddr_in clientService;
		clientService.sin_family = AF_INET;
		clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
		clientService.sin_port = htons(8080);
		if (connect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
			printf("Failed to connect.\n");
			WSACleanup();
		}
		
		std::thread th1 (&recieve, m_socket);
		std::thread th2 (&sent, m_socket);

		th1.join(); 
		th2.join(); 
		
		closesocket(m_socket);
	}

	 // Restore input mode on exit.
    SetConsoleMode(hStdin, fdwSaveOldMode);
	_getch();
	return 0;
}

//Bind    => id address size
//connect => socket, structure(ipv4, address, port), structure size)
//listen  => socket 1(no. of tcp connections that can be queued on server)
//send    => socket buffer length flags
// recv   => socket , buffer , size , flags
//accept => socketid, null(datastructure), null(size))
//socket => (domain (af_inet, ipv4) , type(sock_stream/dgram), protocol(tcp/udp))

//sockaddr_in datastructure has => sin_family/port/addr(ip)


VOID ErrorExit (LPSTR lpszMessage) 
{ 
    fprintf(stderr, "%s\n", lpszMessage); 

    // Restore input mode on exit.

    SetConsoleMode(hStdin, fdwSaveOldMode);

    ExitProcess(0); 
}

VOID MouseEventProc(MOUSE_EVENT_RECORD mer)
{
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
}

// get Window pos through windows api
void GetMousePosWin(MOUSE_EVENT_RECORD mer, bool &chatOn)
{
    int x,y;
    // creates the handle i need to use
    //HANDLE OutputH;
    INPUT_RECORD Inrec;
    DWORD evRead;
    HANDLE hStdIn;
    DWORD dwMode;
    bool Captured=false;

    hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    dwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;

    if( SetConsoleMode( hStdIn, dwMode | ENABLE_MOUSE_INPUT) == TRUE)

        GetConsoleMode(hStdIn, &dwMode);
        SetConsoleMode(hStdIn, (dwMode & (ENABLE_MOUSE_INPUT)));

        // grab the handle to the console so i can use it
        //OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
        //printf("Strated"); //Debug line.

        do
        {
            PeekConsoleInput(hStdIn, &Inrec, 1, &evRead);

            if( evRead ) 
            { 
                ReadConsoleInput(hStdIn, &Inrec, 1, &evRead);
                x= Inrec.Event.MouseEvent.dwMousePosition.X ;
                y= Inrec.Event.MouseEvent.dwMousePosition.Y ;
                switch (Inrec.EventType )
                {

                case MOUSE_EVENT:
                    {
                        Captured = true;
                      /*gotoXY(33,8);
                     std::cout << "x->  " << x << " ";
                       gotoXY(43,8);
                       std::cout << "y->  " << y << " ";*/
                        break;  
                    }
                }
            }

        }while(!Captured);
		
    if((x>48 && x<56 && y==12) && mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
    {
        gotoXY(40,15);
        std::cout << "Connecting to HaxorL0RD Server";
		for(int i=0;i<10;i++)
		{
			Sleep(200);
			std::cout<<".";
		}
		gotoXY(83,15);
        std::cout << "\xFB";
        Sleep(500);
		gotoXY(40,16);
		std::cout << "Initializing Chat";
		for(int i=0;i<3;i++)
		{
			Sleep(200);
			std::cout<<".";
		}
		gotoXY(83,16);
        std::cout << "\xFB";
        Sleep(1200);
		system("cls");
		gotoXY(101,28);
		std::cout << "\xDB\xDB\xDB\xDB\xDB\xDB\xDB Exit Chat";
		gotoXY(0,0);
		chatOn =  true;
    }
	if((x>100 && x<109 && y==28) && mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
    {
        exit(0);
    }
}

void gotoXY(int x, int y) 
{ 
    CursorPosition.X = x; 
    CursorPosition.Y = y; 
    SetConsoleCursorPosition(console,CursorPosition); 
}
VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
    
}

void mouseEventHandler(){
	DWORD cNumRead, fdwMode, i; 
    INPUT_RECORD irInBuf[128]; 
    int counter=0;
	bool chatOn = false;

    // Get the standard input handle. 

    hStdin = GetStdHandle(STD_INPUT_HANDLE); 
    if (hStdin == INVALID_HANDLE_VALUE) 
        ErrorExit("GetStdHandle"); 

    // Save the current input mode, to be restored on exit. 

    if (! GetConsoleMode(hStdin, &fdwSaveOldMode) ) 
        ErrorExit("GetConsoleMode"); 

    // Enable the window and mouse input events. 

    fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT; 
    if (! SetConsoleMode(hStdin, fdwMode) ) 
        ErrorExit("SetConsoleMode"); 
	gotoXY(49,12);
    std::cout << "\xDB\xDB\xDB\xDB\xDB\xDB\xDB Start Chat";
	gotoXY(101,28);
    std::cout << "\xDB\xDB\xDB\xDB\xDB\xDB\xDB Exit Chat";
	while (!chatOn)//++ <= 200) 
    { 
        // Wait for the events. 

        if (! ReadConsoleInput( 
            hStdin,      // input buffer handle 
            irInBuf,     // buffer to read into 
            128,         // size of read buffer 
            &cNumRead) ) // number of records read 
            ErrorExit("ReadConsoleInput"); 

        // Dispatch the events to the appropriate handler. 

        for (i = 0; i < cNumRead; i++) 
        {
            switch(irInBuf[i].EventType) 
            { 
            case MOUSE_EVENT: // mouse input 
                MouseEventProc(irInBuf[i].Event.MouseEvent);
                GetMousePosWin(irInBuf[i].Event.MouseEvent, chatOn);
                break;

			case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing 
                ResizeEventProc( irInBuf[i].Event.WindowBufferSizeEvent );
                break; 

            case FOCUS_EVENT:  // disregard focus events 

            case MENU_EVENT:   // disregard menu events 
                break; 

            default: 
                ErrorExit("Unknown event type"); 
                break; 
            } 
        }
	}
}