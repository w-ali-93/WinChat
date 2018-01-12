#ifndef WIN32_LEAN_AND_MEAN		// To ensure that header files are loaded correctly / there is no overlapping
#define WIN32_LEAN_AND_MEAN
#endif

#include <winsock2.h>	// Sockets and networking
#include <windows.h>	// Windows
#include <ws2tcpip.h>	// WinSock extension for TCP / IP applications
#include <string>		// String manipulation
#include <fstream>		// File handling
#include <iostream>		// File handling

// Connection status
#define SERVER 1;	// Node is a server
#define CLIENT 2;	// Node is a client
#define ERROR 0;	// Node is not connected to anything

// Check for redundant newtork function calls
#define REDUNDANT -59		// The call was redundant

int node_type = 0;			// No connection initially 

#pragma comment(lib, "Ws2_32.lib") // Includes all the WinSock2 functions

// Control window identifiers:
#define Button1 100			// Connect to server button
#define Button2 101			// Send message button
#define Button3 102			// Receive messages button
#define Button4 103			// Launch server button
#define Button5 113			// Add contact button
#define Button6 114			// Remove contact button
#define Button7 115			// Disconnect / End conversation button

#define Edit1 104			// Sent message textbox
#define Edit2 105			// Conversation textbox
#define Edit3 106			// Server name textbox
#define Edit4 107			// Your name textbox

#define List1 108			// Contact names list

#define Label1 109			// Status label
#define Label2 110			// Conversation label
#define Label3 111			// Message editor label
#define Label5 116			// Connect to
#define Label6 117			// Own name	

LRESULT CALLBACK WinProc1(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);		// Windows callback function to handle all events

using namespace std;

//Control classes
class Window{	//Main window class
public:
	int ExtraParams1;				// Misc. parameters
	UINT WindowSize;				// Memory size of window object to be created
	int ExtraParams2;				// More misc. parameters
	DWORD WindowDWStyle;			// Style options for window object to be created
	HBRUSH BackgroundColor;			// Background color of window object to be created
	HCURSOR CursorType;				// Cursor style of window object to be created
	HICON Icon;						// Icon of window object to be created
	HICON IconSmall;				// Titlebar icon of window object to be created
	HINSTANCE ApplicationHandle;	// A unique value to identify the application that creates the window, passed by WinMain
	WNDPROC CallbackProc;			// The Callback procedure that handles the evnets that occur in the window
	LPCSTR WindowClassName;			// Class name for window, must be unique, passed by WinMain
	LPCSTR MenuName;				// Menu name if the window incorporates a menu bar
	UINT WindowStyle;				// Specifies how the window will be drawn on screen
	HWND WindowHandle;				// A unique value to identify the created window object

	int PosX, PosY;					// Position on screen in pixels
	int SizX, SizY;					// Size on screen in pixels
	char* WindowTitle;				// Text on titlebar

	Window();						// Set default properties for Window object to be created
	void InitWindow();				// Create the window object
	void DisplayWindow(int);		// Show the created window

};

class EditBox{	//EditBox class
public:
	HWND ControlHandle;				// A unique value to identify the created EditBox object
	DWORD ControlStyleEx;			// Misc. 
	LPCSTR ControlClassName;		// Name of the control class used by the EditBox object
	LPCSTR ControlName;				// Name of the created control, may be ""
	DWORD ControlStyle;				// Determines the appearance of the EditBox object
	int PosX, PosY;					// Position on screen in pixels
	int SizX, SizY;					// Size on screen in pixels
	HWND ControlParentWindowHandle;	// A unique value to identify the window which contains the EditBox object
	int ControlIdentifier;			// A unique number to idenitfy the events related to the EditBox object, passed by WinMain
	HINSTANCE ControlInstance;		// --
	LPVOID LParam;					// --

	EditBox();						// Set default properties for EditBox object to be created
	void InitControl();				// Create the EditBox object
	void SetText(char text[256]);	// Place a string in the EditBox object
	void SetFont(HGDIOBJ font);		// Set the font of the text in the EditBox object
	void GetText(char text[256]);	// Get the string present in the EditBox object
};

class Button{
public:
	HWND ControlHandle;				// A unique value to identify the created Button object
	DWORD ControlStyleEx;			// Misc. 
	LPCSTR ControlClassName;		// Name of the control class used by the Button object
	LPCSTR ControlName;				// Name of the created control, may be ""
	DWORD ControlStyle;				// Determines the appearance of the Button object
	int PosX, PosY;					// Position on screen in pixels
	int SizX, SizY;					// Size on screen in pixels
	HWND ControlParentWindowHandle;	// A unique value to identify the window which contains the Button object
	int ControlIdentifier;			// A unique number to idenitfy the events related to the Button object, passed by WinMain
	HINSTANCE ControlInstance;		// --
	LPVOID LParam;					// --

	Button();						// Set default properties for Button object to be created
	void InitControl();				// Create the Button object
};

class ListBox{
public:
	HWND ControlHandle;							// A unique value to identify the created Button object
	DWORD ControlStyleEx;						// Misc. 
	LPCSTR ControlClassName;					// Name of the control class used by the Button object
	LPCSTR ControlName;							// Name of the created control, may be ""
	DWORD ControlStyle;							// Determines the appearance of the Button object
	int PosX, PosY;								// Position on screen in pixels
	int SizX, SizY;								// Size on screen in pixels
	HWND ControlParentWindowHandle;				// A unique value to identify the window which contains the Button object
	int ControlIdentifier;						// A unique number to idenitfy the events related to the Button object, passed by WinMain
	HINSTANCE ControlInstance;					// --
	LPVOID LParam;								// --

	ListBox();									// Set default properties for ListBox object to be created
	void InitControl();							// Create the ListBox object
	int AddStringItem(TCHAR item[50]);			// Add a string value to the ListBox object
	int DeleteStringItem(int index);			// Delete the string located at index location in the ListBox object
	int GetSelectedItemIndex();					// Returns the selected items index
	int GetString(int index, TCHAR str[50]);	// Returns the string value at the index location
	int GetItemCount();							// Returns the total number of items in ListBox
	int ReadContacts();							// Copy the contents of the contacts file into the ListBox
	int SaveContacts();							// Copy the contents of the ListBox into the contacts file
	int Search(string item);					// Lookup the contact whose name is 'item' in the listbox
};

class Label{
public:
	HWND ControlHandle;							// A unique value to identify the created Button object
	DWORD ControlStyleEx;						// Misc. 
	LPCSTR ControlClassName;					// Name of the control class used by the Button object
	LPCSTR ControlName;							// Name of the created control, may be ""
	DWORD ControlStyle;							// Determines the appearance of the Button object
	int PosX, PosY;								// Position on screen in pixels
	int SizX, SizY;								// Size on screen in pixels
	HWND ControlParentWindowHandle;				// A unique value to identify the window which contains the Button object
	int ControlIdentifier;						// A unique number to idenitfy the events related to the Button object, passed by WinMain
	HINSTANCE ControlInstance;					// --
	LPVOID LParam;								// --

	Label();									// Set default properties for Label object to be created
	void InitControl();							// Create the Label object
	void SetText(char text[256]);				// Place a string in the Label object
};

//Window class functions definitions
Window::Window(){
	ExtraParams1 = NULL;
	WindowSize = sizeof(WNDCLASSEX);
	ExtraParams2 = NULL;
	WindowDWStyle = WS_OVERLAPPEDWINDOW,
		BackgroundColor = (HBRUSH)COLOR_WINDOW;
	CursorType = LoadCursor(NULL, IDC_ARROW);
	Icon = NULL;
	IconSmall = NULL;
	MenuName = NULL;
	WindowStyle = CS_HREDRAW | CS_VREDRAW;

	PosX = PosY = 200;
	SizX = 640;
	SizY = 480;
	WindowTitle = "Default title";
}
void Window::InitWindow()
{
	WNDCLASSEX wClass;

	wClass.cbClsExtra = ExtraParams1;
	wClass.cbSize = WindowSize;
	wClass.cbWndExtra = ExtraParams2;
	wClass.hbrBackground = BackgroundColor;
	wClass.hCursor = CursorType;
	wClass.hIcon = Icon;
	wClass.hIconSm = IconSmall;
	wClass.hInstance = ApplicationHandle;
	wClass.lpfnWndProc = CallbackProc;
	wClass.lpszClassName = WindowClassName;
	wClass.lpszMenuName = MenuName;
	wClass.style = WindowStyle;

	if (!RegisterClassEx(&wClass))
	{
		int nResult = GetLastError();
		char* message = strcat("Window creation failed at: ", WindowTitle);
		MessageBox(NULL, message, "Window Class Failed", MB_ICONERROR);
	}

	WindowHandle = CreateWindowEx(NULL,
		WindowClassName,
		WindowTitle,
		WindowDWStyle,
		PosX,
		PosY,
		SizX,
		SizY,
		NULL,
		NULL,
		ApplicationHandle,
		NULL);

	if (!WindowHandle)
	{
		int nResult = GetLastError();

		MessageBox(NULL,
			"Window creation failed",
			"Window Creation Failed",
			MB_ICONERROR);
	}
}
void Window::DisplayWindow(int nShowCmd)
{
	ShowWindow(WindowHandle, nShowCmd);
}

//EditBox class functions definitions
EditBox::EditBox(){
	ControlStyleEx = NULL;
	ControlClassName = "EDIT";
	ControlName = "";
	ControlStyle = WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL;
	PosX = 200;
	PosY = 200;
	SizX = 100;
	SizY = 100;
	ControlInstance = GetModuleHandle(NULL);
	LParam = NULL;
	//Note: Must specify ControlIdentifier and ControlParentWindowHandle explicitly

	//Set default font
	SendMessage(ControlHandle, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));
}
void EditBox::InitControl()
{
	ControlHandle = CreateWindowEx(ControlStyleEx,
		ControlClassName,
		ControlName,
		ControlStyle,
		PosX,
		PosY,
		SizX,
		SizY,
		ControlParentWindowHandle,
		(HMENU)ControlIdentifier,
		ControlInstance,
		LParam);
}
void EditBox::SetText(char *text)
{
	SendMessage(ControlHandle, WM_SETTEXT, NULL, (LPARAM)text);
}
void EditBox::SetFont(HGDIOBJ font = GetStockObject(DEFAULT_GUI_FONT))
{
	SendMessage(ControlHandle, WM_SETFONT, (WPARAM)font, MAKELPARAM(FALSE, 0));
}
void EditBox::GetText(char *text)
{
	SendMessage(ControlHandle, WM_GETTEXT, strlen(text), reinterpret_cast<LPARAM>(text));
}

//Button class functions definitions
Button::Button()
{
	ControlStyleEx = NULL;
	ControlClassName = "BUTTON";
	ControlName = "BUTTON";
	ControlStyle = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON;
	PosX = CW_USEDEFAULT;
	PosY = CW_USEDEFAULT;
	SizX = 50;
	SizY = 50;
	ControlInstance = GetModuleHandle(NULL);
	LParam = NULL;
	//Note: Must specify ControlIdentifier and ControlParentWindowHandle explicitly

	//Set default font
	SendMessage(ControlHandle, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));
}
void Button::InitControl()
{
	ControlHandle = CreateWindowEx(ControlStyleEx,
		ControlClassName,
		ControlName,
		ControlStyle,
		PosX,
		PosY,
		SizX,
		SizY,
		ControlParentWindowHandle,
		(HMENU)ControlIdentifier,
		ControlInstance,
		LParam);
}

//ListBox class functions definition
ListBox::ListBox()
{
	ControlStyleEx = WS_EX_CLIENTEDGE;
	ControlClassName = "LISTBOX";
	ControlName = "";
	ControlStyle = WS_CHILD | WS_VISIBLE | ES_AUTOVSCROLL | LBS_NOTIFY;
	PosX = CW_USEDEFAULT;
	PosY = CW_USEDEFAULT;
	SizX = 300;
	SizY = 300;
	ControlInstance = GetModuleHandle(NULL);
	LParam = NULL;
	//Note: Must specify ControlIdentifier and ControlParentWindowHandle explicitly

	//Set default font
	SendMessage(ControlHandle, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));

	//ReadContacts();
}
void ListBox::InitControl()
{
	ControlHandle = CreateWindowEx(ControlStyleEx,
		ControlClassName,
		ControlName,
		ControlStyle,
		PosX,
		PosY,
		SizX,
		SizY,
		ControlParentWindowHandle,
		(HMENU)ControlIdentifier,
		ControlInstance,
		LParam);
}
int ListBox::AddStringItem(TCHAR item[50])
{
	return (int)SendMessage(ControlHandle, LB_ADDSTRING, 0, (LPARAM)item);
}
int ListBox::DeleteStringItem(int index)
{
	return (int)SendMessage(ControlHandle, LB_DELETESTRING, index, 0);
}
int ListBox::GetSelectedItemIndex()
{
	return (int)SendMessage(ControlHandle, LB_GETCURSEL, 0, 0);
}
int ListBox::GetString(int index, TCHAR str[50])
{
	return (int)SendMessage(ControlHandle, LB_GETTEXT, (WPARAM)index, (LPARAM)str);
}
int ListBox::GetItemCount()
{
	return (int)SendMessage(ControlHandle, LB_GETCOUNT, 0, 0);
}
int ListBox::ReadContacts()
{
	ifstream myfile("Contacts");	// Open contacts file
	string contact;

	int i = 0;
	while (getline(myfile, contact))	// Loop through the contacts file
	{
		TCHAR* cont = (char*)contact.c_str();
		AddStringItem(cont);	// Enter contact in list
		i++;
	}

	myfile.close();	// Close contacts file

	return i;	// Return number of contacts added
}
int ListBox::SaveContacts()
{
	remove("Contacts"); // Delete existing contacts file

	ofstream contacts;	// Create a new contacts file
	contacts.open("Contacts");

	int i;
	for (i = 0; i <= GetItemCount() - 1; i++)	//Loop through list 
	{
		TCHAR cont[50];
		int nret = GetString(i, cont);
		string contact = cont;
		contacts << contact << "\n";	// Add contact at 'i' to Contacts.txt
	}

	contacts.close();		// Close the newly written contacts file

	return i;	// Return number of contacts saved
}
int ListBox::Search(string item)
{
	int i;
	for (i = 0; i <= GetItemCount() - 1; i++)
	{
		TCHAR curr[50];
		GetString(i, curr);
		if (item == (string)curr) return i;
	}
	return -1;
}

//Label class functions definition
Label::Label()
{
	ControlStyleEx = WS_EX_CLIENTEDGE;
	ControlClassName = "STATIC";
	ControlName = NULL;
	ControlStyle = WS_CHILD | WS_VISIBLE;
	PosX = CW_USEDEFAULT;
	PosY = CW_USEDEFAULT;
	SizX = 30;
	SizY = 70;
	ControlInstance = GetModuleHandle(NULL);
	LParam = NULL;
	//Note: Must specify ControlIdentifier and ControlParentWindowHandle explicitly

	//Set default font
	SendMessage(ControlHandle, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), MAKELPARAM(FALSE, 0));
}
void Label::InitControl()
{
	ControlHandle = CreateWindowEx(ControlStyleEx,
		ControlClassName,
		ControlName,
		ControlStyle,
		PosX,
		PosY,
		SizX,
		SizY,
		ControlParentWindowHandle,
		(HMENU)ControlIdentifier,
		ControlInstance,
		LParam);
}
void Label::SetText(char text[256])
{
	SendMessage(ControlHandle, WM_SETTEXT, NULL, (LPARAM)text);
}

//Window declarations
Window win1;
EditBox edit1, edit2, edit3, edit4;
Button button1, button2, button3, button4, button5, button6, button7;
ListBox list1;
Label label1, label2, label3, label5, label6;


//Misc. functions
void SetWindowPos(HWND hWnd, int *x, int *y, int offset_x, int offset_y)	// Postion controls on the main window
{
	HWND hWndParent = GetParent(hWnd);	// Returns the handle to the control's parent window
	POINT p = { 0 };

	MapWindowPoints(hWnd, hWndParent, &p, 1);	// Convert the coordinate position of control window to that of the parent window, place it on 0,0 of the parent window

	(*x) = p.x + offset_x;	// Offset x relative to x=0 of parent
	(*y) = p.y + offset_y;	// Offset y relative to y=0 of parent
}
void SaveConversation(char* ContactName, EditBox Conv)	// Save conversation with specified contact
{
	remove(ContactName); // Delete existing conversation file for 'ContactName'

	ofstream conversation;	// Create a new contacts file
	conversation.open(ContactName);

	char convo[10000];	// Copy conversation into a buffer 'convo'
	Conv.GetText(convo);
	
	conversation << (string)convo << '\0'; // Copy everything in 'convo' to conversation file

	conversation.close();	// Close conversation file
}
void LoadConversation(char* ContactName, EditBox Conv)	// Load conversation with specific contact
{
	char* buffer;
	ifstream conversation(ContactName);		// Open conversation file for 'ContactName'
	if (conversation.is_open())
	{
		string line, conv;					// 'conv' holds full conversation, 'line' holds single line
		while (conversation.good())			// Load the entire contents of the conversation into the buffer
		{
			getline(conversation, line);
			conv += line + (char)10;
		}

		Conv.SetText((char*)conv.c_str());	// Put the entire contents of the buffer into 'Conv'

		conversation.close();				// Close the conversation file
	}
}


//Networking classes
class Client{
	WSADATA WsaDat;										// WinSock2 object for Client
	struct hostent *host;								// Host structure to hold information regarding host
	SOCKADDR_IN SockAddr;								// Address structure to hold information for socket creation
	SOCKET Socket;										// Socket to create the connection with host

public:
	char HostName[30];									// Name of the host to connect to
	char *YourName;										// Client's own name
	int ConnectionState;								// Present connection state, 0 for ERROR, 2 for CLIENT
	int LocalPort;										// Port from where connection will be established locally
	HWND LhWnd;											// Handle to label showing connection status

	Client();											// Initialize Client object
	int Connect();										// Conenct to host
	void Disconnect(bool rude);							// Disconnect from host
	int SndMessage(char msg[256]);						// Send message to host
	int RcvMessage(char msg[256], bool Loud=1);			// Receive message from host
	int CheckConnStatus(int State, bool Loud=1);		// Check if Client object's ConnectionState matches the one supplied in the arguments
	void SetStatusLabel(char status[50]);				// Show the current ConnectionState in the label with handle 'LhWnd'
	~Client();											// Clean up Client object
};

class Server{
	WSADATA WsaDat;										// WinSock2 object for Client
	struct hostent *host;								// Host structure to hold information regarding host
	SOCKADDR_IN SockAddr;								// Address structure to hold information for socket creation
	SOCKET Socket;										// Socket to create the connection with host

public:
	char ConnectedClientName[30];						// Name of the computer connected to the Server object
	int ConnectionState;								// Present connection state, 0 for ERROR, 1 for SERVER
	int LocalPort;										// Port from where connection will be established locally
	HWND LhWnd;											// Handle to label showing connection status

	Server();											// Initialize Server object
	int StartHosting();									// Begin hosting the server, goes on until connected to by a client
	void StopHosting(bool rude);						// Stop hosting the server
	int SndMessage(char msg[256]);						// Send message to client
	int RcvMessage(char msg[256], bool Loud=1);			// Receive message from client
	int CheckConnStatus(int State, bool Loud=1);		// Check if Server object's ConnectionState matches the one supplied in the arguments
	void SetStatusLabel(char status[50]);				// Show the current ConnectionState in the label with handle 'LhWnd'
	~Server();											// Clean up Client object
};

//Client class functions definitions
Client::Client()
{
	ConnectionState = ERROR;
	SetStatusLabel("Not connected.");
}
int Client::Connect()
{
	// Filter out redundant calls
	if (CheckConnStatus(2) == 1 || node_type == 1) return REDUNDANT;

	// Create WinSock object
	if (WSAStartup(MAKEWORD(2, 2), &WsaDat) != 0)
	{
		MessageBox(NULL, "Winsock error - Winsock initialization failed!", "Error", MB_ICONERROR);
		WSACleanup();
	}

	// Create socket
	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Socket == INVALID_SOCKET)
	{
		MessageBox(NULL, "Winsock error - Socket creation failed!", "Error", MB_ICONERROR);
		WSACleanup();
	}

	// Resolve IP address for hostname
	if ((host = gethostbyname(HostName)) == NULL)
	{
		MessageBox(NULL, "Failed to resolve hostname.", "Error", MB_ICONERROR);
		WSACleanup();
		return 0;
	}

	// Setup socket address structure
	SockAddr.sin_port = htons(LocalPort);	// Specifies that the port will be part of a TCP / IP protocol communication
	SockAddr.sin_family = AF_INET;	// TCP / IP protocol
	SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);	//Retrieves the first address in the host table

	// Attempt to connect to server
	if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0)
	{
		MessageBox(NULL, "Failed to establish connection with server.", "Error", MB_ICONERROR);
		WSACleanup();
		return 0;
	}
	ConnectionState = CLIENT;
	SetStatusLabel("Connected to server.");

	u_long iMode = 1;
	ioctlsocket(Socket, FIONBIO, &iMode);	// Specifies whether the socket will be blocking or nonblocking

	// Handshake with server
	int inDataLength = -1;
	char names[30];
	memset(names, 0, 30);
	send(Socket, YourName, strlen(YourName), 0);

	node_type = CLIENT;

	return 1;
}
void Client::Disconnect(bool rude=0)
{
	// Filter out redundant calls
	if (rude != 0&&CheckConnStatus(0) == 1) exit;
	// Send goodbye message if you're not rude
	if (!rude) send(Socket, "~", strlen("~"), 0);
	// Shutdown our socket
	shutdown(Socket, SD_SEND);
	// Close our socket entirely
	closesocket(Socket);
	ConnectionState = node_type = ERROR;
	WSACleanup();
	node_type = 0;
	SetStatusLabel("Not connected.");
	
}
int Client::SndMessage(char msg[256])
{
	if (CheckConnStatus(0) == 1) return REDUNDANT;							// Filter out redundant calls
	send(Socket, msg, strlen(msg), 0);										// Send message
	int nError = WSAGetLastError();											// Error checking
	if (nError != WSAEWOULDBLOCK&&nError != 0)
	{
		MessageBox(NULL, "Server disconnected!", "Error", MB_ICONERROR);
		Disconnect(1);
	}
	return nError;
}
int Client::RcvMessage(char msg[256], bool Loud)
{
	if (CheckConnStatus(0, Loud) == 1) return REDUNDANT;					// Filter out redundant calls
	memset(msg, 0, 255);													// Receive the message in buffer
	int length = recv(Socket, msg, 256, 0);
	int nError = WSAGetLastError();											// Error checking
	if (msg[0] == '~'||(nError != WSAEWOULDBLOCK&&nError != 0))
	{
		MessageBox(NULL, "Server disconnected!", "Error", MB_ICONERROR);
		Disconnect(1);
	}
	return length;
}
int Client::CheckConnStatus(int state, bool Loud)
{
	if (state == ConnectionState)
	{
		if (state == 0 && Loud==1)
			MessageBox(NULL, "You are not connected to anyone!", "Error", MB_ICONEXCLAMATION);
		else if (state !=0 && Loud == 1)
			MessageBox(NULL, "You are connected to someone!", "Error", MB_ICONEXCLAMATION);
		return 1;
	}
	else return 0;
}
void Client::SetStatusLabel(char status[50])
{
	if (LhWnd != NULL)
		SendMessage(LhWnd, WM_SETTEXT, NULL, (LPARAM)status);
}
Client::~Client()
{
	Disconnect();
}

//Server class functions definitions
Server::Server()
{
	ConnectionState = ERROR;
	SetStatusLabel("Not connected.");
}
int Server::StartHosting()
{
	// Filter out redundant calls
	if (CheckConnStatus(1) == 1 || node_type == 2) return REDUNDANT;

	ConnectionState = SERVER;
	// Create WinSock object
	if (WSAStartup(MAKEWORD(2, 2), &WsaDat) != 0)
	{
		MessageBox(NULL, "Winsock error - Winsock initialization failed!", "Error", MB_ICONERROR);
		WSACleanup();
	}

	// Create socket
	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (Socket == INVALID_SOCKET)
	{
		MessageBox(NULL, "Winsock error - Socket creation Failed!", "Error", MB_ICONERROR);
		WSACleanup();
	}

	// Setup socket address structure
	SOCKADDR_IN serverInf;
	serverInf.sin_family = AF_INET;
	serverInf.sin_addr.s_addr = htonl(INADDR_ANY);
	serverInf.sin_port = htons(LocalPort);

	// Bind socket i.e. create the server
	if (bind(Socket, (SOCKADDR*)(&serverInf), sizeof(serverInf)) == SOCKET_ERROR)
	{
		MessageBox(NULL, "Unable to bind socket!", "Error", MB_ICONERROR);
		WSACleanup();
	}
	
	// Start listening for incoming connections, max. clients = 1
	listen(Socket, 1);

	// Accept the connection request from client
	SOCKET TempSock = SOCKET_ERROR;
	SetStatusLabel("Waiting for incoming connections...");
	TempSock = accept(Socket, NULL, NULL);

	u_long iMode = 1;
	ioctlsocket(Socket, FIONBIO, &iMode);

	Socket = TempSock;

	// Receive client handshake
	int inDataLength = -1;
	char namec[30];
	memset(namec, 0, 30);
	inDataLength = recv(Socket, namec, 30, 0);

	strcpy_s(ConnectedClientName, namec);
	char label[70] = "Connected to ";
	SetStatusLabel(strcat(label, namec));
	node_type = SERVER;
	return inDataLength;
}
void Server::StopHosting(bool rude=0)
{
	// Filter out redundant calls
	if (rude != 0 && CheckConnStatus(0) == 1) exit;
	// Send goodbye message if you're not rude
	if (!rude) send(Socket, "~", strlen("~"), 0);
	// Shutdown our socket
	shutdown(Socket, SD_SEND);
	// Close our socket entirely
	closesocket(Socket);
	ConnectionState = node_type = ERROR;
	SetStatusLabel("Not connected.");
	WSACleanup();
}
int Server::SndMessage(char msg[256])
{
	if (CheckConnStatus(0) == 1) return REDUNDANT;							// Filter out redundant calls
	send(Socket, msg, strlen(msg), 0);										// Send message
	int nError = WSAGetLastError();											// Error checking
	if (nError != WSAEWOULDBLOCK&&nError != 0)
	{
		MessageBox(NULL, "Client disconnected!", "Error", MB_ICONERROR);
		StopHosting(1);
	}
	return nError;
}
int Server::RcvMessage(char msg[256], bool Loud)
{
	if (CheckConnStatus(0, Loud) == 1) return REDUNDANT;					// Filter out redundant calls
	memset(msg, 0, 255);													// Receive the message in buffer
	int nret = recv(Socket, msg, 256, 0);
	int nError = WSAGetLastError();											// Error checking
	if (msg[0] == '~' || (nError != WSAEWOULDBLOCK&&nError != 0))
	{
		MessageBox(NULL, "Client disconnected!", "Error", MB_ICONERROR);
		StopHosting(1);
	}
	return nret;
}
int Server::CheckConnStatus(int state, bool Loud)
{
	if (state == ConnectionState)
	{
		if (state == 0 && Loud == 1)
			MessageBox(NULL, "You are not connected to anyone!", "Error", MB_ICONEXCLAMATION);
		else if (state != 0 && Loud == 1)
			MessageBox(NULL, "You are connected to someone!", "Error", MB_ICONEXCLAMATION);
		return 1;
	}
	else return 0;
}
void Server::SetStatusLabel(char status[50])
{
	if (LhWnd != NULL)
		SendMessage(LhWnd, WM_SETTEXT, NULL, (LPARAM)status);
}
Server::~Server()
{
	StopHosting();
}

//Network declarations
Client client1;
Server server1;


//Callback Windows Procedure function
LRESULT CALLBACK WinProc1(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
	case WM_CREATE:
	{
					  // CONVERSATION SECTION
					  // Conversation edit box
					  edit2.ControlIdentifier = Edit2;
					  edit2.ControlParentWindowHandle = hWnd;
					  edit2.SizX = 300;
					  edit2.SizY = 200;
					  SetWindowPos(edit2.ControlHandle, &edit2.PosX, &edit2.PosY, ((640 - edit2.SizX) / 2) - 10, 95 + 145-155);
					  edit2.InitControl();

					  // Conversation label
					  label2.ControlIdentifier = Label2;
					  label2.ControlParentWindowHandle = hWnd;
					  SetWindowPos(label2.ControlHandle, &label2.PosX, &label2.PosY, ((640 - edit2.SizX) / 2) - 10, 95 + 120-155);
					  label2.SizX = 95;
					  label2.SizY = 20;
					  label2.InitControl();
					  label2.SetText("Conversation:");

					  // MESSAGE EDITOR SECTION
					  // Message editor edit box
					  edit1.ControlIdentifier = Edit1;
					  edit1.ControlParentWindowHandle = hWnd;
					  edit1.SizX = 300;
					  edit1.SizY = 80;
					  SetWindowPos(edit1.ControlHandle, &edit1.PosX, &edit1.PosY, ((640 - edit2.SizX) / 2) - 10, 355 + 120-155);
					  edit1.InitControl();

					  // Message editor label
					  label3.ControlIdentifier = Label3;
					  label3.ControlParentWindowHandle = hWnd;
					  SetWindowPos(label3.ControlHandle, &label3.PosX, &label3.PosY, ((640 - edit2.SizX) / 2) - 10, 355 + 95-155);
					  label3.SizX = 70;
					  label3.SizY = 20;
					  label3.InitControl();
					  label3.SetText("Message:");

					  // Connect button
					  button1.ControlIdentifier = Button1;
					  button1.ControlParentWindowHandle = hWnd;
					  button1.ControlName = "Connect";
					  SetWindowPos(button1.ControlHandle, &button1.PosX, &button1.PosY, 495, 240-155);
					  button1.SizX = 100;
					  button1.SizY = 50;
					  button1.InitControl();

					  // Disconnect button
					  button7.ControlIdentifier = Button7;
					  button7.ControlParentWindowHandle = hWnd;
					  button7.ControlName = "Disconnect";
					  SetWindowPos(button7.ControlHandle, &button7.PosX, &button7.PosY, 495, 290 - 155 + 50);
					  button7.SizX = 100;
					  button7.SizY = 50;
					  button7.InitControl();

					  // Send button
					  button2.ControlIdentifier = Button2;
					  button2.ControlParentWindowHandle = hWnd;
					  button2.ControlName = "Send";
					  SetWindowPos(button2.ControlHandle, &button2.PosX, &button2.PosY, 495, 520-155);
					  button2.SizX = 100;
					  button2.SizY = 30;
					  button2.InitControl();

					  // Receive button
					  button3.ControlIdentifier = Button3;
					  button3.ControlParentWindowHandle = hWnd;
					  button3.ControlName = "Receive";
					  SetWindowPos(button3.ControlHandle, &button3.PosX, &button3.PosY, 495, 480-155);
					  button3.SizX = 100;
					  button3.SizY = 30;
					  button3.InitControl();

					  // Connect to textbox
					  edit3.ControlIdentifier = Edit3;
					  edit3.ControlParentWindowHandle = hWnd;
					  edit3.SizX = 100;
					  edit3.SizY = 20;
					  SetWindowPos(edit3.ControlHandle, &edit3.PosX, &edit3.PosY, 5, 20);
					  edit3.InitControl();
					  edit3.SetText("LocalHost");

					  // Your name textbox
					  edit4.ControlIdentifier = Edit3;
					  edit4.ControlParentWindowHandle = hWnd;
					  edit4.SizX = 100;
					  edit4.SizY = 20;
					  SetWindowPos(edit4.ControlHandle, &edit4.PosX, &edit4.PosY, 110, 20);
					  edit4.InitControl();
					  edit4.SetText(win1.WindowTitle); //Try implementing a function to get current user name here

					  // Host server button
					  button4.ControlIdentifier = Button4;
					  button4.ControlParentWindowHandle = hWnd;
					  button4.ControlName = "Start hosting";
					  SetWindowPos(button4.ControlHandle, &button4.PosX, &button4.PosY, 495, 290-155);
					  button4.SizX = 100;
					  button4.SizY = 50;
					  button4.InitControl();

					  // Contact list
					  list1.ControlIdentifier = List1;
					  list1.ControlParentWindowHandle = hWnd;
					  SetWindowPos(list1.ControlHandle, &list1.PosX, &list1.PosY, 0, 95 + 145-155);
					  list1.SizX = 140;
					  list1.SizY = 323;
					  list1.InitControl();
					  list1.ReadContacts();

					  // Add to contacts button
					  button5.ControlIdentifier = Button5;
					  button5.ControlParentWindowHandle = hWnd;
					  button5.SizX = 30;
					  button5.SizY = 30;
					  button5.ControlName = "+";
					  SetWindowPos(button5.ControlHandle, &button5.PosX, &button5.PosY, 2, 50);
					  button5.InitControl();

					  // Remove from contacts button
					  button6.ControlIdentifier = Button6;
					  button6.ControlParentWindowHandle = hWnd;
					  button6.SizX = 30;
					  button6.SizY = 30;
					  button6.ControlName = "-";
					  SetWindowPos(button6.ControlHandle, &button6.PosX, &button6.PosY, 34, 50);
					  button6.InitControl();

					  // Connect to label
					  label5.ControlIdentifier = Label5;
					  label5.ControlParentWindowHandle = hWnd;
					  label5.SizX = 77;
					  label5.SizY = 20;
					  SetWindowPos(label5.ControlHandle, &label5.PosX, &label5.PosY, 4, 0);
					  label5.InitControl();
					  label5.SetText("Connect to:");

					  // Your name label
					  label6.ControlIdentifier = Label6;
					  label6.ControlParentWindowHandle = hWnd;
					  label6.SizX = 77;
					  label6.SizY = 20;
					  SetWindowPos(label6.ControlHandle, &label6.PosX, &label6.PosY, 110, 0);
					  label6.InitControl();
					  label6.SetText("Own name:");

					  // CONNECTION STATUS SECTION
					  label1.ControlIdentifier = Label1;
					  label1.ControlParentWindowHandle = hWnd;
					  SetWindowPos(label1.ControlHandle, &label1.PosX, &label1.PosY, 440, 0);
					  label1.SizX = 200;
					  label1.SizY = 40;
					  label1.InitControl();
					  label1.SetText("Not connected.");
	}
	break;

	case WM_COMMAND:
	{
					   switch (LOWORD(wParam))
					   {
					   case Button1:
					   {
									   switch (HIWORD(wParam))
									   {
									   case BN_CLICKED:	// Connect to server
									   {
															char HostName[30], YourName[30];
															edit3.GetText(HostName);
															edit4.GetText(YourName);
															strcpy_s(client1.HostName, HostName);
															client1.YourName = YourName;
															client1.LocalPort = 8888;
															client1.Connect();
															if (node_type!=0)	LoadConversation(client1.HostName, edit2);
									   }
										   break;
									   }
					   }
						   break;
					   case Button2:
					   {
									   switch (HIWORD(wParam))
									   {
									   case BN_CLICKED:	// Send message to peer
									   {
															int i; char prev[5000];
															char item[256]; int r;
															edit1.GetText(item);

															if (node_type == 2)
																r = client1.SndMessage(item);
															else if (node_type == 1)
																r = server1.SndMessage(item);

															if (r != REDUNDANT&&node_type != 0)	// If node is connected then add message to conversation
															{
																edit2.GetText(prev);
																edit1.SetText("");
																char message[300] = "You: ";
																edit2.SetText(strcat(strcat(prev, "\r\n\r\n"), strcat(message, item)));
															}
									   }
										   break;
									   }
					   }
						   break;
					   case Button3:
					   {
									   switch (HIWORD(wParam))
									   {
									   case BN_CLICKED:	// Receive message from peer
									   {
															char buffer[256]; int i; char prev[5000];
															memset(buffer, 0, 256);
															char identity[30];

															if (node_type == 2)
															{
																i = client1.RcvMessage(buffer);
																edit3.GetText(identity);	// Get server name
															}
															else
															{
																i = server1.RcvMessage(buffer);
																strcpy_s(identity, server1.ConnectedClientName);	// Get client name
															}

															if (i != REDUNDANT&&buffer[0] != '~'&&buffer[0]!='\0')	// Post message in the conversation box
															{
																edit2.GetText(prev);
																char temp[150];
																char signature[150];
																strcpy(signature, strcat(identity, ": "));
																edit2.SetText(strcat(strcat(prev, "\r\n\r\n"), strcat(signature, buffer)));
															}
									   }
										   break;
									   }
					   }
						   break;
					   case Button4:
					   {
									   switch (HIWORD(wParam))
									   {
									   case BN_CLICKED: // Start hosting the server
									   {
															server1.LocalPort = 8888;
															server1.StartHosting();
															LoadConversation(server1.ConnectedClientName, edit2);
									   }
										   break;
									   }
					   }
						   break;
					   case Button5:
					   {
									   switch (HIWORD(wParam))
									   {
									   case BN_CLICKED: // Add current connected peer to contact list
									   {
															char cont[50];
															edit3.GetText(cont);
															if (list1.Search(cont) == -1)	// Add only if the peer is not already in list
															{
																list1.AddStringItem(cont);
															}
									   }
										   break;
									   }
					   }
						   break;
					   case Button6:
					   {
									   switch (HIWORD(wParam))
									   {
									   case BN_CLICKED: // Remove currently selected contact from list
									   {
															list1.DeleteStringItem(list1.GetSelectedItemIndex());
									   }
										   break;
									   }
					   }
						   break;
					   case Button7:
					   {
									   switch (HIWORD(wParam))
									   {
									   case BN_CLICKED:	// Disconnect from peer
									   {
															if (node_type == 2)
															{
																client1.Disconnect(0);
																SaveConversation(client1.HostName, edit2);

															}
															else if (node_type == 1)
															{
																server1.StopHosting(0);
																SaveConversation(server1.ConnectedClientName, edit2);
															}
															edit2.SetText((char*)0);
									   }
											break;
									   }
					   }
							break;
					   case List1:
					   {	 
									 switch (HIWORD(wParam))
									 {
									 case LBN_SELCHANGE:	// Place currently selected contact into Connect To editbox
									 {
														   if (node_type == 0)
														   {
															   TCHAR str[50];
															   list1.GetString(list1.GetSelectedItemIndex(), str);
															   string cont = str;
															   edit3.SetText((char*)cont.c_str());
														   }
									 }
										 break;
									 }
					   }
						   break;
					   }
	}
	break;
	case WM_DESTROY:	// Clean up detail
	{
		list1.SaveContacts();
		if (node_type == 1) SaveConversation(server1.ConnectedClientName, edit2);
		else if (node_type == 2) SaveConversation(client1.HostName, edit2);
		PostQuitMessage(0);		// Sends a quit message to the WinMain function
		return 0;
	}
	break;
	}

	return DefWindowProc(hWnd,msg,wParam,lParam);	// Call the default windows procedure to handle all trivial messages such as resizing, moving etc.
}

//Main function
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine,int nShowCmd)
{
	// Initialize and display the main window
	win1.ApplicationHandle = hInst;
	win1.WindowClassName = "Win1";
	win1.CallbackProc=(WNDPROC)WinProc1;
	win1.WindowDWStyle = WS_OVERLAPPED | WS_SYSMENU;
	win1.PosX = CW_USEDEFAULT; win1.PosY = CW_USEDEFAULT;
	win1.SizX = 640; win1.SizY = 445;
	win1.WindowTitle = "WinChat";
	win1.InitWindow();
	win1.DisplayWindow(nShowCmd);

	// Specify additional parameters to server and client objects
	server1.LhWnd = label1.ControlHandle;
	client1.LhWnd = label1.ControlHandle;

	// Callback loop
	MSG msg;								// Create a variable to hold the windows message
	ZeroMemory(&msg,sizeof(MSG));			// Initialize it

	while(GetMessage(&msg,NULL,0,0) > 0)	// Get message from window and store it in 'msg'
	{
		TranslateMessage(&msg);				// Translate virtual key message to characters
		DispatchMessage(&msg);				// Send the translated message to the Callback Procedure for the Window
	}

	// Terminate the program
	return 0;
}