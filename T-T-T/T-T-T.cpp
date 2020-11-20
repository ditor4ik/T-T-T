#include<Windows.h>
#include<tchar.h>
#include"Header.h"
#include"AI_Header.h"
TCHAR ChildClassName[MAX_LOADSTRING] = _T("WinChild");
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK ChildWndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR WinName[] = _T("MainFrame");
HINSTANCE hinst;
HMENU hmenu, hMenuPopup;
//==================================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	HWND hMainWnd;
	MSG msg;
	WNDCLASS wc;
	hinst = hInstance;
	wc.hInstance = hInstance;
	wc.lpszClassName = WinName;
	wc.lpfnWndProc = WndProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszMenuName = _T("MAINMENU");
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	if (!RegisterClass(&wc)) return 0;
	//Создаем основное окно приложения
	hMainWnd = CreateWindow(WinName, 
		_T("Крестики-нолики"), 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		NULL, 
		NULL,
		hInstance, 
		NULL);
	ShowWindow(hMainWnd, nCmdShow);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return 0;

}
ATOM MyRegisterChildClass() {
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.lpfnWndProc = ChildWndProc;
	wcex.hInstance = hinst;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszClassName = ChildClassName;
	return RegisterClassEx(&wcex);
}
static HFONT newFont;
static HWND hChild[9];
unsigned char k[9] = { 0 };
char text[] = { ' ','x','0' };

static bool EndGame = false;
static int ScorePlayer = 0, ScoreComputer = 0;

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	int i;
	static int sx, sy;
	TCHAR str[256];
	TCHAR NewGame[] = _T("Новая игра");
	TCHAR Info[] = _T("Информация об игре");
	TCHAR Exit[] = _T("Выход");
	TCHAR Menu[] = _T("Меню");
	switch (uMsg) {
	case WM_CREATE:
		MyRegisterChildClass();
		for (i = 0; i < 9; i++)
			hChild[i] = CreateWindow(ChildClassName, NULL, WS_CHILD | 
				WS_DLGFRAME | WS_VISIBLE, 0, 0, 0, 0, hwnd, NULL, hinst, NULL);
		hmenu = CreateMenu();
		hMenuPopup = CreatePopupMenu();
		AppendMenu(hMenuPopup, MF_STRING, ID_NEW, NewGame);
		AppendMenu(hMenuPopup, MF_STRING, GAME_INFO, Info);
		AppendMenu(hMenuPopup, MF_STRING, IDM_EXIT, Exit);
		AppendMenu(hmenu, MF_POPUP, (UINT)hMenuPopup, Menu);
		SetMenu(hwnd, hmenu);
		break;
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED) break;
		sx = LOWORD(lParam) / 3;
		sy = HIWORD(lParam) / 3;
		if (newFont) DeleteObject(newFont);
		newFont = CreateFont(min(sx, sy), 0, 0, 0, FW_NORMAL, 0, 0, 0, 
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("Arial"));
		for (i = 0; i < 9; i++) 
		{
			MoveWindow(hChild[i], (i % 3) * sx, (i / 3) * sy, sx, sy, TRUE);
			UpdateWindow(hChild[i]);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_NEW:
			for (i = 0; i < 9; i++)
			{
				k[i] = 0;
				InvalidateRect(hChild[i], NULL, 1);
				UpdateWindow(hChild[i]);
			}
			EndGame = false;
			break;
		case IDM_EXIT: DestroyWindow(hwnd); break;
		case GAME_INFO:
			_stprintf_s(str, _T("Количество очков:\nИгрок - %d\nКомпьютер - %d"), ScorePlayer, ScoreComputer);
			MessageBox(hwnd, str, _T("Информация об игре"), NULL);
			break;
		default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		break;
	case WM_DESTROY: PostQuitMessage(0); break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;

}
LRESULT CALLBACK ChildWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rt;
	int i, s;
	char* ch;
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		for (i = 0; hwnd != hChild[i]; i++);
		if (k[i] || EndGame) break;
		else k[i] = 1;
		InvalidateRect(hwnd, NULL, 1);
		UpdateWindow(hwnd);
		srand(lParam);
		if (Check(k, 1)) {
			EndGame = true;
			ScorePlayer++;
			MessageBox(hwnd, _T("Вы выиграли!"), _T("Конец игры"), MB_OK | MB_ICONQUESTION);
			break;
		}
		for (i = s = 0; i < 9; i++)
			if (k[i]) s++;
		if (s == 9)
			MessageBox(hwnd, _T("Ничья! Для новой игры выбирайте \"Новая игра\""), _T("Конец игры"), MB_OK | MB_ICONQUESTION);
		else {
			while (true) {
				s = rand() % 9;
				//s = rand() * 9 / (RAND_MAX + 1);
				if (k[s]) continue;
				int action = Analys(k, 1);
				if (action > 10) 
					s = ((action / 10) - 1)*3 + (action % 10) - 1;
				if (!k[4]) s = 4;
				k[s] = 2;
				InvalidateRect(hChild[s], NULL, 1);
				UpdateWindow(hChild[s]);
				break;
			}
			if (Check(k, 2)) {
				EndGame = true;
				ScoreComputer++;
				MessageBox(hwnd, _T("Вы проиграли!"), _T("Конец игры"), MB_OK | MB_ICONQUESTION);
				break;
			}
			
		}
		break;

	case WM_PAINT:

		for (i = 0; hwnd != hChild[i]; i++);
		if (k[i]) {
			ch = text + k[i];
			hdc = BeginPaint(hwnd, &ps);
			GetClientRect(hwnd, &rt);
			SelectObject(hdc, newFont);
			DrawTextA(hdc, ch, 1, &rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			EndPaint(hwnd, &ps);
		}
		else DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;

}