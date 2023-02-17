#ifndef DEFINITIONS_H
#define DEFINITIONS_H





// System definitions:
#define WIN32_LEAN_AND_MEAN


// Main function:
#define main()						APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)

// Main function methods:
#define InitializeMain(sys)			ReferToUnusedParameters(); sys->RegisterWindowClass(hInstance); if(!sys->InitWindow(hInstance, nCmdShow)) { return false; } MSG msg;
#define UpdateMain()				TranslateMessage(&msg); DispatchMessage(&msg);
#define ReleaseMain()				main_return_value

// Main function variables
//#define get_sys_msg() 				GetMessage(&msg, nullptr, 0, 0)
#define get_sys_msg() 				PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)
#define main_return_value			(int)msg.wParam

// Internal components:
#define ReferToUnusedParameters()	(hInstance); (hPrevInstance);


// External components:
#define CenterWindowBeforeInit()	sys->uWindowPosX = uScreenWidth / 2 - sys->uWindowWidth / 2; sys->uWindowPosY = uScreenHeight / 2 - sys->uWindowHeight / 2;

// Input handling:
#define SingleHit(k_key, action)	k_key.update_state();		if (k_key.single_hit) { k_key.single_hit = false; action; }
#define HeldDown(k_key, action)		k_key.update_state();		if (k_key.hold_cycle) { k_key.hold_cycle = false; action; }
#define WheelPull(k_key, action)								if (k_key)			  { k_key = false;			  action; }

#endif
