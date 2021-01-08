#include "CursorLock.h"
#include "MinHook.h"
#include "GameManager.h"

RECT fullscreen({ INT_MIN, INT_MIN, INT_MAX, INT_MAX });
bool clipped = true;
RECT currClip;
RECT window_rectangle, client_rectangle;
int height, width;
RECT a, b;

inline int get_title_bar_thickness(const HWND window_handle)
{
	
	GetWindowRect(window_handle, &window_rectangle);
	GetClientRect(window_handle, &client_rectangle);
	height = (window_rectangle.bottom - window_rectangle.top) -
		(client_rectangle.bottom - client_rectangle.top);
	width = (window_rectangle.right - window_rectangle.left) -
		(client_rectangle.right - client_rectangle.left);
	return height - (width / 2);
}

bool isFullscreen(HWND window)
{
	GetWindowRect(window, &a);
	GetWindowRect(GetDesktopWindow(), &b);
	return (a.left == b.left &&
		a.top == b.top &&
		a.right == b.right &&
		a.bottom == b.bottom);
}

DWORD WINAPI hook(void* hModule) {
	DWORD oPID = GetCurrentProcessId();
	while (true) {
		HWND hwnd = GetForegroundWindow();
		DWORD pid;
		GetWindowThreadProcessId(hwnd, &pid);
		if (pid == oPID) {
			void* gm = GameManager::getSharedState();
			if (GameManager::getGameVariable(gm, "1003") && !(GetKeyState(VK_LMENU) & 0x8000 || GetKeyState(VK_RMENU) & 0x8000)) {
				RECT rect;
				GetWindowRect(hwnd, &rect);

				if (!isFullscreen(hwnd)) {
					rect.left += 9;
					rect.top += get_title_bar_thickness(hwnd);
					rect.right -= 9;
					rect.bottom -= 9;
				}
				clipped = true;
				ClipCursor(&rect);
			}
			else {
				if (clipped) ClipCursor(&fullscreen);
				clipped = false;
			}
		}
		else {
			if (clipped) ClipCursor(&fullscreen);
			clipped = false;
		}
	}

	return 0;
}

void CursorLock::start(HMODULE hModule) {
	CreateThread(0, 0x1000, hook, hModule, 0, 0);
}
