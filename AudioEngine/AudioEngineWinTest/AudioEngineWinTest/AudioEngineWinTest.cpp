// AudioEngineWinTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "AudioEngineWinTest.h"

#include "AudioEngineAPI.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HWND m_hWnd;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_AUDIOENGINEWINTEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_AUDIOENGINEWINTEST));

	::SetWindowText(m_hWnd, L"ENGINE INITIALIZED");

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_AUDIOENGINEWINTEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_AUDIOENGINEWINTEST);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

int pcKId = -1;

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//

#define NUM_NOTES 256

bool activeNotes[NUM_NOTES];

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   m_hWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   //Init AudioEngine
   wchar_t* folderName = L"E:\\Mine\\Audinos\\HG-SynAssembler\\AudioEngine\\AudioEngineWinTest\\Debug";
   int result = ::initEngine(folderName, (int)hWnd, 44100, 16, 1);
   if (result != 0)
	   return FALSE;
   if (::getFactoryNumber() == 0)
   {
	   //printf("No factory available. Exiting...\n");
	   ::releaseEngine();

	   return FALSE;
   }


#define BASIC_EU_FACTORY	0
#define INOUT_EU_FACTORY	1

   int algoId, dsId, seqId;
   algoId = ::createAlgorithm();
   //Create oscillator
   int oscId = ::createElaborationUnit(BASIC_EU_FACTORY, VIRTUAL_EU_CATEGORY, 0, -1);
   //Create DirectSound
   dsId = ::createElaborationUnit(INOUT_EU_FACTORY, PHYSICAL_EU_CATEGORY, 0, 0);
   //Create Polytrack keyboard
   //seqId = ::createElaborationUnit(BASIC_EU_FACTORY,VIRTUAL_EU_CATEGORY,1, -1);
   //Create PC keyboard
   pcKId = ::createElaborationUnit(INOUT_EU_FACTORY, PHYSICAL_EU_CATEGORY, 3, 0);
   //Add to algorithm
   ::addElaborationUnitToAlgorithm(algoId, dsId);
   ::addElaborationUnitToAlgorithm(algoId, oscId);
   //::addElaborationUnitToAlgorithm(algoId,seqId);
   ::addElaborationUnitToAlgorithm(algoId, pcKId);
   //Connect EU
   ::connectElaboratioUnits(algoId, oscId, 0, dsId, 0);
   //::connectElaboratioUnits(algoId,seqId,0,oscId,2);
   ::connectElaboratioUnits(algoId, pcKId, 0, oscId, 2);

   ::playAlgorithm(algoId);

   for (int i = 0; i < NUM_NOTES; i++)
	   activeNotes[i] = false;

   return TRUE;
}

unsigned char getNoteByChar(char key)
{
	switch (key)
	{
	case 'q':
	case 'Q':
		return 0x40;
	case 'w':
	case 'W':
		return 0x42;
	case 'e':
	case 'E':
		return 0x44;
	case 'r':
	case 'R':
		return 0x45;
	case 't':
	case 'T':
		return 0x47;
	case 'y':
	case 'Y':
		return 0x49;
	case 'u':
	case 'U':
		return 0x51;
	case 'i':
	case 'I':
		return 0x52;
	default:
		return 0;
		break;
	}
}



//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
	case WM_QUIT:
		::stopAlgorithm(0);
		::releaseEngine();
		break;
	case WM_KEYDOWN:
	{
		unsigned char buffer[3];
		unsigned char note = getNoteByChar(wParam);
		if (!activeNotes[note])
		{
			activeNotes[note] = true;
			if (note != 0)
			{
				buffer[0] = 0x90;
				buffer[1] = note;
				buffer[2] = 0x7F;
				::sendMIDIMessage(pcKId, buffer);
				
			}
		}
	}
	break;
	case WM_KEYUP:
	{
		unsigned char buffer[3];
		unsigned char note = getNoteByChar(wParam);
		if (activeNotes[note])
		{
			activeNotes[note] = false;
			if (note != 0)
			{
				buffer[0] = 0x80;
				buffer[1] = note;
				buffer[2] = 0x7F;
				::sendMIDIMessage(pcKId, buffer);
			}
			
		}
	}
	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
