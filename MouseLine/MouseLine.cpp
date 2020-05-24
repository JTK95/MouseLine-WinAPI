#include <windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
HINSTANCE g_hInst;
LPCWSTR lpszClass = L"Mouse";

//------------------------------------------------------
// WinMain()은 윈도우를 만들고 화면에 출력용으로만 쓰인다.
//------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	// 화면에 출력하고자 하는 윈도우의 핸들
	HWND hWnd;

	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	// 클래스 설정 후
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIconW(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc; // 메시지처리 함수 지정
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;

	// 윈도우 클래스 등록
	RegisterClassW(&WndClass);

	// CreateWindow 호출 (윈도우 생성)
	hWnd = CreateWindowW
	(
		lpszClass, // 클래스 이름 지정
		L"Mouse Line", // 타이틀 이름
		WS_OVERLAPPEDWINDOW, // 윈도우 형태 지정
		CW_USEDEFAULT, // x좌표
		CW_USEDEFAULT, // y좌표
		CW_USEDEFAULT, // Width
		CW_USEDEFAULT, // Height
		NULL, // 부모 윈도우가 없을 경우 NULL
		(HMENU)NULL, // (메뉴 설정) WNDCLASS꺼 사용하려면 NULL 
		hInstance, // 윈도우를 만드는 주체
		NULL
	);

	// 만들어진 윈도우를 화면에 출력
	ShowWindow(hWnd, nCmdShow);

	// 메시지 루프
	// 메시지 큐에서 메시지를 꺼내 메시지 처리 함수로 보내주는 작업
	while (GetMessageW(&Message, NULL, 0, 0))
	{
		// ex) A키를 누른 후 다시 A키를 떼면 A문자가 입력되었다는 메시지를 만듬
		TranslateMessage(&Message);
		// 메시지를 WndProc()로 전달한다
		DispatchMessageW(&Message);
	}
	return Message.wParam;
}

HPEN g_hPen;
//-----------------------------------------------
// 대부분 작업은 WndProc()에서 이루어진다.
//-----------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	HPEN hPen;
	static int x;
	static int y;
	static BOOL bnowDraw = FALSE;

	switch (iMessage)
	{
	case WM_LBUTTONDOWN:
		x = LOWORD(lParam);
		y = HIWORD(lParam);
		bnowDraw = !bnowDraw;
		break;
	case WM_MOUSEMOVE:
		if (bnowDraw)
		{
			hdc = GetDC(hWnd);

			hPen = CreatePen
			(
				PS_SOLID,
				20,
				RGB(255, 255, 0)
			);

			SelectObject(hdc, g_hPen);
			MoveToEx(hdc, x, y, NULL);
			
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			
			LineTo(hdc, x, y);
			
			DeleteObject(hPen);
			ReleaseDC(hWnd, hdc);
		}
		break;
	case WM_LBUTTONUP:
		bnowDraw = !bnowDraw;
		break;
	case WM_RBUTTONDOWN:
		g_hPen = CreatePen
		(
			PS_SOLID,
			rand() % 10,
			RGB(rand() % 255, rand() % 255, rand() % 255)
		);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		DefWindowProcW(hWnd, iMessage, wParam, lParam);
		break;
	}
	return (DefWindowProcW(hWnd, iMessage, wParam, lParam));
}