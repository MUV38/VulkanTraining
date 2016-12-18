#include <windows.h>
#include "SimpleVulkan.h"

SimpleVulkan g_simpleVulkan;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	// ウィンドウ クラスを登録する
	const wchar_t CLASS_NAME[] = L"Vulkan";

	WNDCLASS wc = {};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// ウィンドウを作成する

	HWND hwnd = CreateWindowEx(
		0,                              // オプションのウィンドウ スタイル
		CLASS_NAME,                     // ウィンドウ クラス
		L"Vulkan",    // ウィンドウ テキスト
		WS_OVERLAPPEDWINDOW,            // ウィンドウ スタイル

										// サイズと位置
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // 親ウィンドウ    
		NULL,       // メニュー
		hInstance,  // インスタンス ハンドル
		NULL        // 追加のアプリケーション データ
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	// Vulkan初期化
	g_simpleVulkan.Init();

	ShowWindow(hwnd, nCmdShow);

	// メッセージ ループを実行する

	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Vulkan後始末
	g_simpleVulkan.Cleanup();

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

		EndPaint(hwnd, &ps);
	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}