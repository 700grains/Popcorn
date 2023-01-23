// Popcorn.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Main.h"

#define MAX_LOADSTRING 100

// Global Variables:
AsEngine Engine;
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
//------------------------------------------------------------------------------------------------------------
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_POPCORN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow) )
		return FALSE;

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_POPCORN) );

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0) )
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}
//------------------------------------------------------------------------------------------------------------
//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_POPCORN) );
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = AsConfig::BG_Color.Get_Brush();
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_POPCORN);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL) );

	return RegisterClassExW(&wcex);
}
//------------------------------------------------------------------------------------------------------------
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
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	RECT window_rect;
	window_rect.left = 0;
	window_rect.top = 0;
	window_rect.right = 320 * 3;
	window_rect.bottom = 200 * 3;

	AdjustWindowRect(&window_rect, WS_OVERLAPPEDWINDOW, TRUE);

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, 0, 0, window_rect.right - window_rect.left, window_rect.bottom - window_rect.top, 0, 0, hInstance, 0);

	if (hWnd == 0)
		return FALSE;

	Engine.Init_Engine(hWnd);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}
//------------------------------------------------------------------------------------------------------------
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId;
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
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
		// TODO: Add any drawing code that uses hdc here...
		Engine.Draw_Frame(hdc, ps.rcPaint);
		EndPaint(hWnd, &ps);
	break;


	case WM_DESTROY:
		PostQuitMessage(0);
		break;


	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_LEFT:
			return Engine.On_Key(EKT_Left, true);

		case VK_RIGHT:
			return Engine.On_Key(EKT_Right, true);

		case VK_SPACE:
			return Engine.On_Key(EKT_Space, true);
		}
		break;


	case WM_KEYUP:
		switch (wParam)
		{
		case VK_LEFT:
			return Engine.On_Key(EKT_Left, false);

		case VK_RIGHT:
			return Engine.On_Key(EKT_Right, false);

		case VK_SPACE:
			return Engine.On_Key(EKT_Space, false);
		}
		break;


	case WM_TIMER:
		if (wParam == Timer_ID)
			return Engine.On_Timer();
		break;


	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
//------------------------------------------------------------------------------------------------------------
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
			EndDialog(hDlg, LOWORD(wParam) );
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
//------------------------------------------------------------------------------------------------------------
/*

V Level end on ball loss
V 1. Platform melt animation
V 2. Animation of rolling out a new
V 3. Animation of the expanding platform
V 4. Launching the ball with a space and the state of the ball

V Bricks
V 1. Ordinary bricks (blue or red)
V 1.1. Can be with or without letter
V 1.2. When hitting a brick, it disappears from the level immediately, and if the brick was:
v 1.2.1. Without a letter - an active brick is created
v 1.2.2. With a letter - a falling letter is created

V 2. Indestructible brick
V 2.1. On-hit animation

V 3. Reusable brick
V 3.1. 4 brick states (1-4 hits before breaking)
V 3.2. State Transition
V 3.3. Prize animation when destroyed (brick disappears, animation remains)

V 4. Brick with a parachute
V 4.1. Parachute animation - as a variant of the falling letter

V 5. Brick of teleportation
V 5.1. Animation when capturing and releasing the ball

V 6. Advertising brick
V 6.1. Animation of a fragment of "advertising" hidden behind a brick


V Interaction of special bricks
V 1. Falling letter - only interacts with the platform
V 2. Active brick - interacts only with the ball


Actions of letters
1. Simple:
1.1. O ("Cancel") - cancels the action of the symbols K, W, P, L and M.
V 1.2. I ("Invert")
V 1.3. S ("Speed")
1.4. M ("Monsters")
1.5. F ("Life")


2. Complex:
V 2.1. K ("Glue")
V 2.1.1. New state of the platform
V 2.1.2. Spreading glue animation (forward and reverse)
V 2.1.3. Fixing the ball (+ new state of the ball)
V 2.1.4. Ball launch by space and timeout

2.2. W ("Wider")
2.2.1. New platform state (+ resizing)
2.2.2. Expanding platform animation (forward and reverse)

V 2.3. P ("Floor")
V 2.3.1. Floor rendering
V 2.3.2. floor scale
V 2.3.3. The interaction of the floor with the ball
V 2.3.4. Hitting the ball with the bottom edge of the platform

2.4. L ("Laser")
2.4.1. New state of the platform
2.4.2. Animation of transforming into a cannon (forward and reverse)
2.4.3. Space Shooting:
2.4.3.1. Gap handling in a special way when the platform is in laser mode
2.4.3.2. Laser beam objects - animation, movement and interaction with other objects

V 2.5. T ("Three")
V 2.5.1. Add support for multiple balls
V 2.5.2. Output of many balls
V 2.5.3. Interaction of many balls with game elements

2.6. + (Go to the next level)
2.6.1. Animation of early transition to the next level
2.6.2. Separate transition level with special walls (frame)

ball movement
V 1. Bounce of the ball from the frame
V 2. Hitting the edges of the platform
V 3. Sweeping the ball with a platform
4. Hitting the monster

Hitting a brick
1. Hitting a brick - the state of a brick
V 2. Knocking out a falling letter
V 3. Interception of a falling letter
4. Each letter has its own actions

Gates
1. Output gates
2. Animation of opening/closing gates
3. Gate states (closed, opening, closing)


Monsters
1. List of monsters (eye, "turtle", "saturn", head, "tambourine", aquarium, lighter)
2. For everyone - make a storyboard
3. Move, select direction + states (live/pause/disappear)
4. Interaction with the ball and the platform
5. Exit the gate
6. For everyone - animation


Information panel
1. Logo
2. Indicators
2.1. Player name
2.2 Checking account
3. Scale of monsters and gender
4. Change scales
5. Extra lives window
6. Accounting for game actions and display on indicators


Game and levels
1. List of the first 10 levels
2. Game states (splash screen, animation of the beginning of the level, playing the level, loss of life, transition to the next level (normal and early), end of the game)
*/