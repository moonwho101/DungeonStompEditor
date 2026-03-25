/*
 *  Copyright (C) 1995, 1996 Microsoft Corporation. All Rights Reserved.
 *
 *  File: d3dmain.cpp
 *
 *  Each of the Direct3D samples must be linked with this file.  It contains
 *  the code which allows them to run in the Windows environment.
 *
 *  A window is created using d3dmain.res which allows the user to select the
 *  Direct3D driver to use and change the render options.  The D3DApp
 *  collection of functions is used to initialize DirectDraw, Direct3D and
 *  keep surfaces and D3D devices available for rendering.
 *
 *  Frame rate and a screen mode information buffer is Blt'ed to the screen
 *  by functions in stats.cpp.
 *
 *  Each sample is executed through the functions: InitScene, InitView,
 *  RenderScene, ReleaseView, ReleaseScene and OverrideDefaults, as described
 *  in d3ddemo.h.  Samples can also read mouse and keyboard input via
 *  SetMouseCallback and SetKeyboardCallback.
 */

#include "d3dmain.h"
// #include "sound.h"
#include "editor.h"
#include "toolbar.h"
#include "world.h"
// #include "bitmap.h"
#include <winbase.h>
// #include "d3dappi.h"
// #include <dinput.h>
// #include "input.h"
#include "resource.h"
// extern char texutrename[100][100];
extern "C" int displaysizex;
extern "C" int displaysizez;

extern "C" int displaysizex1;
extern "C" int displaysizez1;

extern "C" int wtx;
extern "C" int wtxy;

extern "C" int wtz;
extern "C" int wtzy;

extern char objectname[100][100];
extern int objectnumber[1000];
extern int objectnumbercount;

extern "C" int ability;

extern "C" int current_link;

extern "C" float last_angle;
extern "C" float last_x;
extern "C" float last_z;

struct TEXNAMES {
	char texname[100][1];
};

extern "C" TEXNAMES tname[1000];

extern "C" int texturenumber[1000];
extern "C" int texturenumbercount;

struct NewValues {
	float ypos;
	int texture;
	int objectid;
	char buf[2048];
	char angle[10];
	char param[10];
};

extern "C" NewValues NVALS;

extern "C" char currentobjectname[1000];

struct OBJECTNAMES {
	char objectname[100][100];
};

extern struct OBJECTNAMES oname[200];
extern "C" int placemode;

int truemode = 0;

extern "C" int FindModelID(char *p);
extern "C" int draw_mode;
extern "C" char gctext[9048];
extern "C" int jumpnewspot;

typedef struct Link_typ {
	float last_x;
	float last_z;
	float last_angle;
	BOOL active_link_flag;
	int inactive;
	int objectid;
} LINK;
// #define MAX_NUM_LINKS 1000

#define MAX_NUM_LINKS 21000

// GLOBAL VARIABLES
extern "C" LINK LinksList[MAX_NUM_LINKS];
extern "C" int current_link;
extern "C" int last_link;
extern "C" float ylocation;
extern "C" int monsterid;
extern "C" int monstertexture;

extern "C" char ybuffer[200];

extern "C" float rcolour;
extern "C" float gcolour;
extern "C" float bcolour;

extern "C" float dirx;
extern "C" float diry;
extern "C" float dirz;
extern "C" int ltype;

void gettrueobjectname(HWND hwnd);
int currentobject = 0;

D3DAppInfo *d3dapp;     /* Pointer to read only collection of DD and D3D
                           objects maintained by D3DApp */
d3dmainglobals myglobs; /* collection of global variables */

BOOL (*MouseHandler)(UINT, WPARAM, LPARAM);    /* sample's function which traps
                                                  mouse input */
BOOL (*KeyboardHandler)(UINT, WPARAM, LPARAM); /* sample's functions which traps



                                                  keyboard input */
extern "C" EDITOR ed;
int drawcross = 0;
CAMERA cam;
int keycode;
int car_gear = 1;
int editor_insert_mode = 0;
int MouseX, MouseY;
int xPos, yPos;
BOOL Joystick_Enabled_Flag;
BOOL Car_Sound_On_Flag = TRUE;
int current_mode;
setupinfo_ptr setupinfo;
int current_screen_status = FULLSCREEN_ONLY; // default, in case rrsetup.ini
                                             // bugs out

extern "C" int object_rot_angle = 0;

BOOL ScrollRangeFlag = FALSE;
HWND hwndScrollBar;
SCROLLINFO lpsi;
DWORD last_error;

extern "C" char modelname1[255];

int old_xPos, old_yPos;

WORLD world;
extern "C" WORLD edworld;
LPDIRECTDRAWSURFACE bk;
float temp_view_angle;

/*
 *  INTERNAL FUNCTION PROTOTYPES
 */
static BOOL AppInit(HINSTANCE hInstance, LPSTR lpCmdLine);
static BOOL CreateD3DApp(LPSTR lpCmdLine);
static BOOL BeforeDeviceDestroyed(LPVOID lpContext);
static BOOL AfterDeviceCreated(int w, int h, LPDIRECT3DVIEWPORT2 *lpViewport,
                               LPVOID lpContext);
void CleanUpAndPostQuit(void);
static void InitGlobals(void);
static BOOL AppPause(BOOL f);
void ReportD3DAppError(void);
static BOOL RenderLoop(void);
static BOOL RestoreSurfaces();
long FAR PASCAL WindowProc(HWND hWnd, UINT message, WPARAM wParam,
                           LPARAM lParam);

void SavePreCompiledHeader(HWND hwnd, world_ptr wptr);

/****************************************************************************/
/*                            WinMain                                       */
/****************************************************************************/

// Initializes the application then enters a message loop which calls sample's
// RenderScene until a quit message is received.

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
	int failcount = 0; /* number of times RenderLoop has failed */
	MSG msg;
	HACCEL hAccelApp;

	hPrevInstance;

	// Create the window and initialize all objects needed to begin rendering

	if (!AppInit(hInstance, lpCmdLine))
		return FALSE;
	hAccelApp = LoadAccelerators(hInstance, "AppAccel");

	while (!myglobs.bQuit) {
		// Monitor the message queue until there are no pressing
		// messages

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				CleanUpAndPostQuit();
				break;
			}

			if (!myglobs.hWndMain || !TranslateAccelerator(myglobs.hWndMain,
			                                               hAccelApp, &msg)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			// If the app is not minimized, not about to quit, not paused, either the
			// active fullscreen app or in a window and D3D has been initialized,
			// we can render

		} else {
			WaitMessage();
		}
	} // end while

	DestroyWindow(myglobs.hWndMain);
	return msg.wParam;
}

/****************************************************************************/
/*             D3DApp Initialization and callback functions                 */
/****************************************************************************/
/*
 * AppInit
 * Creates the window and initializes all objects necessary to begin rendering
 */
static BOOL
AppInit(HINSTANCE hInstance, LPSTR lpCmdLine) {
	WNDCLASS wc;

	/*
	 * Initialize the global variables
	 */
	InitGlobals();
	myglobs.hInstApp = hInstance;
	/*
	 * Register the window class
	 */
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, "AppIcon");
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = "AppMenu";
	wc.lpszClassName = "Example";
	if (!RegisterClass(&wc))
		return FALSE;
	/*
	 * Create a window with some default settings that may change
	 */
	myglobs.hWndMain = CreateWindowEx(
	    WS_EX_APPWINDOW,                           // extended window style
	    "Example",                                 // pointer to registered class name
	    "Dungeon Stomp Editor Ver 1.0",            // pointer to windwos name
	    WS_SYSMENU | WS_MAXIMIZEBOX | WS_MAXIMIZE, // windows style
	    CW_USEDEFAULT,                             // Horizontal position of window
	    CW_USEDEFAULT,                             // Veritcal position of window
	    640, 480,                                  // window width , height
	    NULL,                                      // parent window
	    NULL,                                      // menu handle
	    hInstance,                                 // program handle
	    NULL);                                     // create parms

	if (!myglobs.hWndMain) {

		return FALSE;
	}

	//	if(!InitInput(hInstance, myglobs.hWndMain))
	//		MessageBox(myglobs.hWndMain,"DirectInput initialization Failed!",NULL,MB_OK);

	setupinfo = LoadSetupInfo(myglobs.hWndMain);

	if (setupinfo != NULL) {
		current_screen_status = setupinfo->screen;
		Car_Sound_On_Flag = setupinfo->sound;
	}

	// Display the window
	if (current_screen_status != FULLSCREEN_ONLY) {
		ShowWindow(myglobs.hWndMain, SW_SHOWNORMAL);
		UpdateWindow(myglobs.hWndMain);
	}
	// Have the example initialize it components which remain constant
	// throughout execution

	//    if (!InitScene())
	//      return FALSE;

	// Call D3DApp to initialize all DD and D3D objects necessary to render.
	// D3DApp will call the device creation callback which will initialize the
	// viewport and the sample's execute buffers.

	//    if (!CreateD3DApp(lpCmdLine))
	//	return FALSE;

	return TRUE;
}

// CreateD3DApp
// Create all DirectDraw and Direct3D objects necessary to begin rendering.
// Add the list of D3D drivers to the file menu.

static BOOL CreateD3DApp(LPSTR lpCmdLine) {

	return TRUE;
}

/*
 * AfterDeviceCreated
 * D3DApp will call this function immediately after the D3D device has been
 * created (or re-created).  D3DApp expects the D3D viewport to be created and
 * returned.  The sample's execute buffers are also created (or re-created)
 * here.
 */
static BOOL
AfterDeviceCreated(int w, int h, LPDIRECT3DVIEWPORT2 *lplpViewport, LPVOID lpContext) {
	return TRUE;
}

// BeforeDeviceDestroyed
// D3DApp will call this function before the current D3D device is destroyed
// to give the app the opportunity to destroy objects it has created with the
// DD or D3D objects.

static BOOL BeforeDeviceDestroyed(LPVOID lpContext) {
	return TRUE;
}

/****************************************************************************/
/*                            Rendering loop                                */
/****************************************************************************/

// RenderLoop
// Render the next frame and update the window

static BOOL RenderLoop() {

	return TRUE;
}

/*
 * AppPause
 * Pause and unpause the application
 */
static BOOL AppPause(BOOL f) {

	return TRUE;
}

// RestoreSurfaces
// Restores any lost surfaces.  Returns TRUE if all surfaces are not lost and
// FALSE if one or more surfaces is lost and can not be restored at the
// moment.

static BOOL RestoreSurfaces() {
}

/*************************************************************************
  Windows message handlers
 *************************************************************************/

BOOL FAR PASCAL InsertRoadSection(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;

	hdc = GetDC(hwnd);
	switch (msg) {
	case WM_INITDIALOG:
		editor_insert_mode = 0;
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			if (editor_insert_mode == 1)
				DrawRoadSection(hdc, &ed, STRAIGHT);
			if (editor_insert_mode == 2)
				DrawRoadSection(hdc, &ed, LEFT_HAND_CURVE);
			if (editor_insert_mode == 3)
				DrawRoadSection(hdc, &ed, RIGHT_HAND_CURVE);
			if (editor_insert_mode == 4)
				DrawRoadSection(hdc, &ed, T_JUNCTION);
			if (editor_insert_mode == 5)
				DrawRoadSection(hdc, &ed, CROSSROAD);
			if (editor_insert_mode == 6)
				DrawRoadSection(hdc, &ed, ROUND_ABOUT);
			if (editor_insert_mode == 1)
				DrawRoadSection(hdc, &ed, SMALL_STRAIGHT);
			EndDialog(hwnd, TRUE);
			break;

		case IDCANCEL:
			EndDialog(hwnd, TRUE);
			break;

		case IDC_RADIO1:
			editor_insert_mode = 1;
			break;

		case IDC_RADIO2:
			editor_insert_mode = 2;
			break;

		case IDC_RADIO3:
			editor_insert_mode = 3;
			break;

		case IDC_RADIO4:
			editor_insert_mode = 4;
			break;

		case IDC_RADIO5:
			editor_insert_mode = 5;
			break;

		case IDC_RADIO6:
			editor_insert_mode = 6;
			break;
		}

	case WM_TIMER:
		// KillTimer(hwnd,1);
		break;
	}
	ReleaseDC(hwnd, hdc);
	return FALSE;
}

BOOL FAR PASCAL MapEditor(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	float ypos = 0;
	int texture = 0;

	HDC hdc;
	int button_number;
	int toolbar_button_number;
	int result;
	BOOL bIsActive;
	hdc = GetDC(hwnd);
	int dwIndex;
	char achTemp[2000];
	char buff[2000];
	char junk[200];
	HWND hwndList;
	//	HWND hwndObjects;
	int i;
	int foundit = 0;
	RECT rc;
	GetWindowRect(hwnd, &rc);
	displaysizex = (rc.right - rc.left) - (int)150;
	displaysizez = (rc.bottom - rc.top) - (int)100;

	displaysizex1 = rc.left;
	displaysizez1 = rc.top;

	wtx = rc.left;
	wtxy = rc.top;
	wtz = rc.right;
	wtzy = rc.bottom;

	//	HRGN hrgn=CreateRectRgn(wtx,wtxy,wtz,wtzy);
	//		SelectClipRgn(hdc,hrgn);

	if (displaysizex < (int)10)
		displaysizex = (int)10;

	if (displaysizez < (int)10)
		displaysizez = (int)10;

	switch (msg) {

		//		case WM_GETTEXT:
		//			int result;

		//			result = GetDlgItemText(hwnd,IDC_EDIT1,  &ybuffer[0],50);

		//			result = GetWindowText(hwnd,  &ybuffer[0],50);

		//			result=1;

		//		break;
	case WM_ACTIVATEAPP:
		bIsActive = (BOOL)wParam;

		if (bIsActive) {
			// we are active, need to reacquire the keyboard
			//		ReacquireInput();
			//						DrawDialogControls(hdc, &ed);
			DrawEditorMap(hdc, &ed);
		}

		break;

	case WM_INITDIALOG:
		SetTimer(hwnd, 1, 200, NULL);
		ed.wptr = &edworld;
		ed.scroll_bar_flag = FALSE;
		ed.current_object_id = 5;
		ed.dialogbar_mode = DIALOGBAR_MODE_ROADS;
		ed.current_object_id = OBJECT_ID_HOUSE;
		ed.editor_mode = 0;
		ed.Redraw_editor_map_flag = 0;

		result = SetDlgItemText(hwnd, IDC_EDIT1, "0.000");
		result = SetDlgItemText(hwnd, IDC_EDIT4, "0.000");
		result = SetDlgItemText(hwnd, IDC_EDIT5, "27.000");

		result = SetDlgItemText(hwnd, IDC_EDIT2, "0.200");
		result = SetDlgItemText(hwnd, IDC_EDIT3, "0.200");
		result = SetDlgItemText(hwnd, IDC_EDIT6, "0.200");
		result = SetDlgItemText(hwnd, IDC_EDIT7, "0.000");
		result = SetDlgItemText(hwnd, IDC_EDIT8, "-1.000");
		result = SetDlgItemText(hwnd, IDC_EDIT9, "0.000");
		result = SetDlgItemText(hwnd, IDC_EDIT10, "0.000");
		result = SetDlgItemText(hwnd, IDC_EDIT11, "0.000");

		// ed.display_scale = 0.25;
		break;

	case WM_PAINT:
		DrawEditorMap(hdc, &ed);

		DrawDialogControls(hdc, &ed);
		DrawScrollBars(hdc, &ed);
		UpdateStatusBar(hdc, 0, 0, &ed);
		UpdateDialogBar(hdc, xPos, yPos, LEFT_BUTTON_DOWN, &ed);
		//			DrawDialogControls(hdc, &ed);
		break;
	case WM_SIZE:
		DrawEditorMap(hdc, &ed);

		DrawDialogControls(hdc, &ed);
		DrawScrollBars(hdc, &ed);
		UpdateStatusBar(hdc, 0, 0, &ed);
		break;

	case WM_MOUSEMOVE:
		xPos = LOWORD(lParam);
		yPos = HIWORD(lParam);
		result = GetDlgItemText(hwnd, IDC_EDIT1, &ybuffer[0], 50);
		ylocation = atof(ybuffer);
		result = GetDlgItemText(hwnd, IDC_EDIT4, &ybuffer[0], 50);
		monsterid = atof(ybuffer);
		result = GetDlgItemText(hwnd, IDC_EDIT5, &ybuffer[0], 50);
		monstertexture = atof(ybuffer);

		result = GetDlgItemText(hwnd, IDC_EDIT2, &ybuffer[0], 50);
		rcolour = atof(ybuffer);
		result = GetDlgItemText(hwnd, IDC_EDIT3, &ybuffer[0], 50);
		gcolour = atof(ybuffer);
		result = GetDlgItemText(hwnd, IDC_EDIT6, &ybuffer[0], 50);
		bcolour = atof(ybuffer);

		result = GetDlgItemText(hwnd, IDC_EDIT7, &ybuffer[0], 50);
		dirx = atof(ybuffer);
		result = GetDlgItemText(hwnd, IDC_EDIT8, &ybuffer[0], 50);
		diry = atof(ybuffer);
		result = GetDlgItemText(hwnd, IDC_EDIT9, &ybuffer[0], 50);
		dirz = atof(ybuffer);

		result = GetDlgItemText(hwnd, IDC_EDIT10, &ybuffer[0], 50);
		ltype = atoi(ybuffer);

		result = GetDlgItemText(hwnd, IDC_EDIT11, &ybuffer[0], 50);
		ability = atoi(ybuffer);

		result = GetDlgItemText(hwnd, IDC_EDIT12, &ybuffer[0], 50);
		strcpy(gctext, ybuffer);

		// extern "C"	float rcolour;
		// extern "C"	float gcolour;
		// extern "C"	float bcolour;

		// extern "C"	float dirx;
		// extern "C"	float diry;
		// extern "C"	float dirz;
		// extern "C"	int ltype;
		currentobject = 0;
		result = GetDlgItemText(hwnd, IDC_COMBO1, &buff[0], 50);
		for (i = 0; i < objectnumbercount; i++) {
			if (strstr(buff, oname[i].objectname[0]) != NULL) {

				strcpy(currentobjectname, buff);
				currentobject = objectnumber[i];
				ed.current_object_id = currentobject;
				sprintf(junk, "%d", ed.current_object_id);
				result = SetDlgItemText(hwnd, IDC_OBJECT, junk);
			}
		}

		char buff[200];
		sprintf(buff, "%d", ed.current_object_id);

		if (truemode != 1)
			result = SetDlgItemText(hwnd, IDC_OBJECT, buff);

		result = GetDlgItemText(hwnd, IDC_OBJECT, &ybuffer[0], 50);
		currentobject = atoi(ybuffer);

		ed.current_object_id = currentobject;

		int currenttexture;

		currenttexture = 0;

		int blank;
		result = GetDlgItemText(hwnd, IDC_COMBO2, &buff[0], 50);
		blank = atoi(buff);
		for (i = 0; i < texturenumbercount; i++) {
			if (strstr(buff, tname[i].texname[0]) != NULL) {
				currenttexture = texturenumber[i];
				//							ed.current_object_id=currentobject;
				sprintf(junk, "%d", currenttexture);

				result = SetDlgItemText(hwnd, IDC_EDIT5, junk);
			}
		}

		sprintf(buff, "%d", currenttexture);

		if (currenttexture == 0 || currenttexture == -1) {
		} else {

			if (truemode != 1)
				result = SetDlgItemText(hwnd, IDC_EDIT5, buff);
			result = GetDlgItemText(hwnd, IDC_EDIT5, &ybuffer[0], 50);
			currenttexture = atoi(ybuffer);
		}

		result = GetDlgItemText(hwnd, IDC_COMBO3, &buff[0], 50);

		if (strlen(buff) > 0) {
			sprintf(junk, "%d", FindModelID(buff));
			//				if (truemode != 1)
			result = SetDlgItemText(hwnd, IDC_EDIT4, junk);
		}

		GetDlgItemText(hwnd, IDC_EDIT4, &buff[0], 50);
		placemode = 0;
		if (strstr(buff, "1") != NULL) {
			placemode = 1;
		}
		result = GetDlgItemText(hwnd, IDC_COMBOOBJECT, &buff[0], 50);

		if (strlen(buff) > 0 && draw_mode == 1) {

			junk[0] = buff[3];
			junk[1] = buff[4];
			junk[2] = buff[5];
			junk[3] = buff[6];
			if (junk[1] == ' ')
				junk[1] = '\0';

			if (junk[2] == ' ')
				junk[2] = '\0';

			if (junk[3] == ' ')
				junk[3] = '\0';

			if (junk[4] == ' ')
				junk[4] = '\0';

			current_link = (int)atoi(junk);
			for (i = 0; i < last_link; i++) {
				if (current_link == (int)LinksList[i].objectid) {
					current_link = i;
					break;
				}
			}

			// current_link = (int)atoi(junk);
			last_x = LinksList[current_link].last_x;
			last_z = LinksList[current_link].last_z;
			last_angle = LinksList[current_link].last_angle;
			float cross_x = LinksList[current_link].last_x;
			float cross_z = LinksList[current_link].last_z;
			// DrawCross(	hdc,&ed,cross_x,cross_z);

			if (current_link != drawcross) {
				DrawEditorMap(hdc, &ed);
				// result = SetDlgItemText(hwnd,IDC_EDIT4,  junk);

				ypos = ed.wptr->oblist[LinksList[current_link].objectid].y;

				sprintf(junk, "%f", ypos);

				if (truemode != 1)
					result = SetDlgItemText(hwnd, IDC_EDIT1, junk);
				NVALS.ypos = ypos;

				char buf[2048];
				sprintf(buf, "%s", ed.wptr->oblist[LinksList[current_link].objectid].ctext);

				if (truemode != 1)
					result = SetDlgItemText(hwnd, IDC_EDIT12, buf);
				strcpy(NVALS.buf, buf);

				texture = ed.wptr->oblist[LinksList[current_link].objectid].monstertexture;

				sprintf(junk, "%d", texture);
				if (truemode != 1)
					result = SetDlgItemText(hwnd, IDC_EDIT5, junk);

				NVALS.texture = texture;
				NVALS.objectid = LinksList[current_link].objectid;

				sprintf(buf, "%f", ed.wptr->oblist[LinksList[current_link].objectid].rot_angle);
				strcpy(NVALS.angle, buf);

				if (truemode != 1)
					result = SetDlgItemText(hwnd, IDC_OBJECT2, buf);

				sprintf(buf, "%d", ed.wptr->oblist[LinksList[current_link].objectid].ability);
				if (truemode != 1)
					result = SetDlgItemText(hwnd, IDC_EDIT13, buf);
				strcpy(NVALS.param, buf);
			}
		}
		drawcross = current_link;
		//
		//			DrawDialogControls(hdc, &ed);
		//			DrawScrollBars(hdc,&ed);

		UpdateEditorMapWindow(hwnd, hdc, xPos, yPos, MOUSE_MOVE, &ed);
		//			DrawEditorMap(hdc, &ed);
		//			DrawDialogControls(hdc, &ed);

		DrawScrollBars(hdc, &ed);
		UpdateStatusBar(hdc, 0, 0, &ed);
		DrawDialogControls(hdc, &ed);
		break;

	case WM_LBUTTONDBLCLK:
		//			int rr=1;
		//
		//			rr=1;
		//
		break;

	case WM_VSCROLL:
		//			MessageBeep(0);
		break;

	case WM_MBUTTONDOWN:
		//			MessageBeep(0);
		break;

	case WM_LBUTTONDOWN:
		SetTimer(hwnd, 3, 100, NULL);
		old_xPos = xPos;
		old_yPos = yPos;

		UpdateEditorMapWindow(hwnd, hdc, xPos, yPos, LEFT_BUTTON_DOWN, &ed);
		UpdateToolBar(hdc, xPos, yPos, LEFT_BUTTON_DOWN);

		button_number = UpdateScrollBars(hdc, xPos, yPos, LEFT_BUTTON_DOWN, &ed);
		if (button_number == 1) {
			ed.display_x_offset += 20;
			DrawEditorMap(hdc, &ed);
		}
		if (button_number == 2) {
			ed.display_x_offset -= 20;
			DrawEditorMap(hdc, &ed);
		}
		if (button_number == 3) {
			ed.display_y_offset += 20;
			DrawEditorMap(hdc, &ed);
		}
		if (button_number == 4) {
			ed.display_y_offset -= 20;
			DrawEditorMap(hdc, &ed);
		}

		UpdateDialogBar(hdc, xPos, yPos, LEFT_BUTTON_DOWN, &ed);
		//	DrawDialogControls(hdc, &ed);
		DrawScrollBars(hdc, &ed);
		UpdateStatusBar(hdc, 0, 0, &ed);

		ypos = ed.wptr->oblist[LinksList[current_link].objectid].y;

		sprintf(junk, "%f", ypos);
		if (truemode != 1)
			result = SetDlgItemText(hwnd, IDC_EDIT1, junk);

		NVALS.ypos = ypos;

		char buf[2048];
		sprintf(buf, "%s", ed.wptr->oblist[LinksList[current_link].objectid].ctext);

		if (truemode != 1)
			result = SetDlgItemText(hwnd, IDC_EDIT12, buf);
		strcpy(NVALS.buf, buf);

		texture = ed.wptr->oblist[LinksList[current_link].objectid].monstertexture;

		sprintf(junk, "%d", texture);
		if (truemode != 1)
			result = SetDlgItemText(hwnd, IDC_EDIT5, junk);

		NVALS.texture = texture;
		NVALS.objectid = LinksList[current_link].objectid;

		sprintf(buf, "%f", ed.wptr->oblist[LinksList[current_link].objectid].rot_angle);
		if (truemode != 1)
			result = SetDlgItemText(hwnd, IDC_OBJECT2, buf);
		strcpy(NVALS.angle, buf);

		sprintf(buf, "%d", ed.wptr->oblist[LinksList[current_link].objectid].ability);
		if (truemode != 1)
			result = SetDlgItemText(hwnd, IDC_EDIT13, buf);
		strcpy(NVALS.param, buf);

		break;

	case WM_LBUTTONUP:
		KillTimer(hwnd, 3);
		ed.scroll_bar_flag = FALSE;

		toolbar_button_number = UpdateToolBar(hdc, old_xPos, old_yPos, LEFT_BUTTON_UP);

		if (toolbar_button_number == 6) // undo
		{
			int delobj;
			int dellist;
			delobj = current_link;
			dellist = current_link;

			dellist = LinksList[dellist].objectid;
			for (delobj = 0; delobj < last_link; delobj++) {

				if (LinksList[delobj].objectid == dellist) {
					LinksList[delobj].inactive = 1;
				}
			}

			ed.wptr->oblist[dellist].inactive = 1;
			DrawEditorMap(hdc, &ed);
		}

		if (toolbar_button_number == 7) // undo
		{
			ed.wptr->oblist_length--;

			if (ed.wptr->oblist_length < 0)
				ed.wptr->oblist_length = 0;
			UndoLastLink(&ed);
			DrawEditorMap(hdc, &ed);
			//				MessageBeep(0);
		}
		if (toolbar_button_number == 8) // rotate 90 degrees clockwise
		{
			//				object_rot_angle+=90;
			object_rot_angle += 45;
			if (object_rot_angle > 359)
				object_rot_angle = object_rot_angle - 360;
			ed.object_rot_angle = object_rot_angle;
		}
		if (toolbar_button_number == 9) // zoom out
		{
			if (ed.display_scale != 0)
				ed.display_scale = ed.display_scale / 2;

			UpdateStatusBar(hdc, 0, 0, &ed);
			DrawEditorMap(hdc, &ed);
		}
		if (toolbar_button_number == 10) // zoom in
		{
			ed.display_scale = ed.display_scale * 2;
			if (ed.display_scale > 4)
				ed.display_scale = (float)4;

			UpdateStatusBar(hdc, 0, 0, &ed);
			DrawEditorMap(hdc, &ed);
		}
		if (toolbar_button_number == 11) {

			// edit
			ed.dialogbar_mode = DIALOGBAR_MODE_ROADS;
			ed.editor_mode = 0;
			DrawDialogBar(hdc, &ed);
			// result = SetDlgItemText(hwnd,IDC_EDIT4,  "0.00");
			truemode = 0;
			draw_mode = 1;
		}

		if (toolbar_button_number == 12) {
			truemode = 1;
			// object
			ed.dialogbar_mode = DIALOGBAR_MODE_OBJECTS;
			ed.editor_mode = 1;
			DrawDialogBar(hdc, &ed);
			draw_mode = 0;
			// result = SetDlgItemText(hwnd,IDC_EDIT4,  "0.00");
		}

		if (toolbar_button_number == 13) {

			// 0 =edit
			// 1= drop
			// 2 =draw

			truemode = 2;
			// draw
			ed.dialogbar_mode = DIALOGBAR_MODE_ROADS;
			ed.editor_mode = 0;
			DrawDialogBar(hdc, &ed);
			draw_mode = 0;
			// result = SetDlgItemText(hwnd,IDC_EDIT4,  "1.00");
		}

		UpdateStatusBar(hdc, 0, 0, &ed);

		UpdateScrollBars(hdc, xPos, yPos, LEFT_BUTTON_UP, &ed);

		button_number = UpdateDialogBar(hdc, old_xPos, old_yPos, LEFT_BUTTON_UP, &ed);

		if (ed.dialogbar_mode == DIALOGBAR_MODE_ROADS) {

			int obid = 0;

			if (button_number == 1) {
				strcpy(currentobjectname, "left_curve_road");
				obid = LEFT_HAND_CURVE;
				DrawRoadSection(hdc, &ed, LEFT_HAND_CURVE);
			}
			if (button_number == 2) {
				strcpy(currentobjectname, "right_curve_road");

				obid = RIGHT_HAND_CURVE;
				DrawRoadSection(hdc, &ed, RIGHT_HAND_CURVE);
			}

			if (button_number == 3) {

				obid = STRAIGHT;
				strcpy(currentobjectname, "straight_road");
				DrawRoadSection(hdc, &ed, STRAIGHT);
			}
			if (button_number == 4) {

				obid = SMALL_STRAIGHT;
				strcpy(currentobjectname, "small_straight_road");
				DrawRoadSection(hdc, &ed, SMALL_STRAIGHT);
			}

			if (button_number == 5) {

				obid = LEFT_CORNER;
				strcpy(currentobjectname, "left_corner");
				DrawRoadSection(hdc, &ed, LEFT_CORNER);
			}
			if (button_number == 6) {

				obid = RIGHT_CORNER;
				strcpy(currentobjectname, "right_corner");
				DrawRoadSection(hdc, &ed, RIGHT_CORNER);
			}

			if (button_number == 7) {

				obid = CROSSROAD;
				strcpy(currentobjectname, "mainroad_crossroads");
				DrawRoadSection(hdc, &ed, CROSSROAD);
			}
			if (button_number == 8) {

				obid = T_JUNCTION;
				strcpy(currentobjectname, "t_junction");
				DrawRoadSection(hdc, &ed, T_JUNCTION);
			}

			if (button_number == 9) {

				obid = LEFT_T_JUNCTION;
				strcpy(currentobjectname, "left_t_junction");
				DrawRoadSection(hdc, &ed, LEFT_T_JUNCTION);
			}

			if (button_number == 10) {

				obid = RIGHT_T_JUNCTION;
				strcpy(currentobjectname, "right_t_junction");
				DrawRoadSection(hdc, &ed, RIGHT_T_JUNCTION);
			}

			if (button_number == 11) {

				obid = ROUND_ABOUT;
				DrawRoadSection(hdc, &ed, ROUND_ABOUT);
			}

			if (button_number == 12) {

				obid = ZEBRA;
				DrawRoadSection(hdc, &ed, ZEBRA);
			}

			//				char buff[200];
			//			sprintf(buff,"%d",obid);
			//			result = SetDlgItemText(hwnd,IDC_OBJECT,  buff);
			//			gettrueobjectname(hwnd);

			//				strcpy(currentobjectname,"straight_road");
		}

		if (ed.dialogbar_mode == DIALOGBAR_MODE_OBJECTS) {

			if (button_number == 3) {

				// monster
				ed.current_object_id = 5;
				result = SetDlgItemText(hwnd, IDC_COMBO1, "!flamesnohit");
				result = SetDlgItemText(hwnd, IDC_EDIT5, "0.0");
				result = SetDlgItemText(hwnd, IDC_COMBO2, "");
				result = SetDlgItemText(hwnd, IDC_EDIT10, "0.0");
				// result = SetDlgItemText(hwnd,IDC_EDIT5,  "");

				result = SetDlgItemText(hwnd, IDC_COMBO2, "flames@1");
				// result = SetDlgItemText(hwnd,IDC_COMBO3,  "diamond");
			}

			if (button_number == 4) {

				// monster
				ed.current_object_id = 5;
				result = SetDlgItemText(hwnd, IDC_COMBO1, "torch");
				result = SetDlgItemText(hwnd, IDC_EDIT5, "0.0");
				result = SetDlgItemText(hwnd, IDC_COMBO2, "");
				result = SetDlgItemText(hwnd, IDC_EDIT10, "0.0");
				// result = SetDlgItemText(hwnd,IDC_EDIT5,  "");
				// result = SetDlgItemText(hwnd,IDC_COMBO3,  "diamond");
			}

			if (button_number == 5) {

				// monster
				ed.current_object_id = 5;
				result = SetDlgItemText(hwnd, IDC_COMBO1, "lamp_post");
				result = SetDlgItemText(hwnd, IDC_EDIT5, "0.0");
				result = SetDlgItemText(hwnd, IDC_COMBO2, "");
				result = SetDlgItemText(hwnd, IDC_EDIT10, "0.0");
				// result = SetDlgItemText(hwnd,IDC_EDIT5,  "");
				// result = SetDlgItemText(hwnd,IDC_COMBO3,  "diamond");
			}

			if (button_number == 6) {

				// monster
				ed.current_object_id = 5;
				result = SetDlgItemText(hwnd, IDC_COMBO1, "lamp_post");
				result = SetDlgItemText(hwnd, IDC_EDIT5, "0.0");
				result = SetDlgItemText(hwnd, IDC_COMBO2, "");
				result = SetDlgItemText(hwnd, IDC_EDIT10, "1.0");
				// result = SetDlgItemText(hwnd,IDC_EDIT5,  "");
				// result = SetDlgItemText(hwnd,IDC_COMBO3,  "diamond");
			}

			if (button_number == 7) {

				// monster
				ed.current_object_id = 5;
				result = SetDlgItemText(hwnd, IDC_COMBO1, "lamp_post");
				result = SetDlgItemText(hwnd, IDC_EDIT5, "0.0");
				result = SetDlgItemText(hwnd, IDC_COMBO2, "");
				result = SetDlgItemText(hwnd, IDC_EDIT10, "2.0");
				// result = SetDlgItemText(hwnd,IDC_EDIT5,  "");
				// result = SetDlgItemText(hwnd,IDC_COMBO3,  "diamond");
			}

			if (button_number == 8) {

				// monster
				ed.current_object_id = 5;
				result = SetDlgItemText(hwnd, IDC_COMBO1, "lamp_post");
				result = SetDlgItemText(hwnd, IDC_EDIT5, "0.0");
				result = SetDlgItemText(hwnd, IDC_COMBO2, "");
				result = SetDlgItemText(hwnd, IDC_EDIT10, "3.0");
				// result = SetDlgItemText(hwnd,IDC_EDIT5,  "");
				// result = SetDlgItemText(hwnd,IDC_COMBO3,  "diamond");
			}

			if (button_number == 9) {

				// monster
				ed.current_object_id = 5;
				result = SetDlgItemText(hwnd, IDC_COMBO1, "!monster1");
				result = SetDlgItemText(hwnd, IDC_EDIT5, "-1.0");
				result = SetDlgItemText(hwnd, IDC_COMBO2, "");
				result = SetDlgItemText(hwnd, IDC_EDIT11, "5.0");
				// result = SetDlgItemText(hwnd,IDC_EDIT5,  "");
				result = SetDlgItemText(hwnd, IDC_COMBO3, "diamond");
			}

			if (button_number == 10) {

				// monster
				ed.current_object_id = 5;
				result = SetDlgItemText(hwnd, IDC_COMBO1, "!monster1");
				result = SetDlgItemText(hwnd, IDC_EDIT5, "-1.0");
				result = SetDlgItemText(hwnd, IDC_COMBO2, "");

				// result = SetDlgItemText(hwnd,IDC_EDIT5,  "");
				result = SetDlgItemText(hwnd, IDC_COMBO3, "BASTARDSWORD");
			}

			if (button_number == 11) {

				// monster
				ed.current_object_id = 5;
				result = SetDlgItemText(hwnd, IDC_COMBO1, "!monster1");
				result = SetDlgItemText(hwnd, IDC_EDIT5, "-1.0");
				result = SetDlgItemText(hwnd, IDC_COMBO2, "");

				// result = SetDlgItemText(hwnd,IDC_EDIT5,  "");
				result = SetDlgItemText(hwnd, IDC_COMBO3, "POTION");
			}

			if (button_number == 12) {

				// monster
				ed.current_object_id = 5;
				result = SetDlgItemText(hwnd, IDC_COMBO1, "!monster1");
				result = SetDlgItemText(hwnd, IDC_EDIT11, "0.0");
				result = SetDlgItemText(hwnd, IDC_COMBO2, "goblin");

				// result = SetDlgItemText(hwnd,IDC_EDIT5,  "");
				result = SetDlgItemText(hwnd, IDC_COMBO3, "GOBLIN");
			}

			if (button_number == 13) {

				// monster
				ed.current_object_id = 5;
				result = SetDlgItemText(hwnd, IDC_COMBO1, "!monster1");
				result = SetDlgItemText(hwnd, IDC_EDIT11, "5.0");
				result = SetDlgItemText(hwnd, IDC_COMBO2, "");
				result = SetDlgItemText(hwnd, IDC_EDIT5, "-1.0");
				result = SetDlgItemText(hwnd, IDC_COMBO3, "COIN");
			}

			if (button_number == 14) {

				// monster
				ed.current_object_id = 5;
				result = SetDlgItemText(hwnd, IDC_COMBO1, "!monster1");
				result = SetDlgItemText(hwnd, IDC_EDIT11, "10.0");
				result = SetDlgItemText(hwnd, IDC_COMBO2, "");
				result = SetDlgItemText(hwnd, IDC_EDIT5, "-1.0");
				result = SetDlgItemText(hwnd, IDC_COMBO3, "COIN");
			}

			if (button_number == 15) {

				// monster
				ed.current_object_id = 5;
				result = SetDlgItemText(hwnd, IDC_COMBO1, "!monster1");
				result = SetDlgItemText(hwnd, IDC_EDIT11, "20.0");
				result = SetDlgItemText(hwnd, IDC_COMBO2, "");
				result = SetDlgItemText(hwnd, IDC_EDIT5, "-1.0");
				result = SetDlgItemText(hwnd, IDC_COMBO3, "COIN");
			}

			if (button_number == 16) {

				// monster
				ed.current_object_id = 5;
				result = SetDlgItemText(hwnd, IDC_COMBO1, "!monster1");
				result = SetDlgItemText(hwnd, IDC_EDIT11, "50.0");
				result = SetDlgItemText(hwnd, IDC_COMBO2, "");
				result = SetDlgItemText(hwnd, IDC_EDIT5, "-1.0");
				result = SetDlgItemText(hwnd, IDC_COMBO3, "COIN");
			}

			char buff[200];
			sprintf(buff, "%d", ed.current_object_id);
			result = SetDlgItemText(hwnd, IDC_OBJECT, buff);
			DrawDialogControls(hdc, &ed);
		}
		break;

	case WM_NCLBUTTONUP:
		KillTimer(hwnd, 3);
		ed.scroll_bar_flag = FALSE;
		UpdateToolBar(hdc, old_xPos, old_yPos, LEFT_BUTTON_UP);
		UpdateScrollBars(hdc, xPos, yPos, LEFT_BUTTON_UP, &ed);
		UpdateDialogBar(hdc, old_xPos, old_yPos, LEFT_BUTTON_UP, &ed);
		DrawDialogControls(hdc, &ed);
		DrawScrollBars(hdc, &ed);
		UpdateStatusBar(hdc, 0, 0, &ed);

		break;

	case WM_RBUTTONDOWN:
		// DisplayLinksList(hdc);

		object_rot_angle += 90;
		if (object_rot_angle > 359)
			object_rot_angle = object_rot_angle - 360;
		ed.object_rot_angle = object_rot_angle;
		DrawDialogControls(hdc, &ed);
		DrawScrollBars(hdc, &ed);
		UpdateStatusBar(hdc, 0, 0, &ed);

		break;

	case WM_TIMER:
		if (wParam == 1) {
			KillTimer(hwnd, 1);
			LoadObjectData(hwnd, "objects.dat", &edworld);
			DrawDialogControls(hdc, &ed);
			DrawEditorMap(hdc, &ed);
		}
		if (wParam == 2) {
			KillTimer(hwnd, 2);
			DrawEditorMapObject(hdc, &ed);
			ed.Redraw_editor_map_flag = 1;
			// UpdateEditorMapWindow(hwnd, hdc, xPos, yPos, MOUSE_MOVE, &ed);
			DrawDialogControls(hdc, &ed);
			DrawScrollBars(hdc, &ed);

			//				MessageBeep(0);
		}
		if (wParam == 3) {
			button_number = UpdateScrollBars(hdc, xPos, yPos, LEFT_BUTTON_DOWN, &ed);
			if (button_number == 1) {
				ed.display_x_offset += 20;
				DrawEditorMap(hdc, &ed);
			}
			if (button_number == 2) {
				ed.display_x_offset -= 20;
				DrawEditorMap(hdc, &ed);
			}
			if (button_number == 3) {
				ed.display_y_offset += 20;
				DrawEditorMap(hdc, &ed);
			}
			if (button_number == 4) {
				ed.display_y_offset -= 20;
				DrawEditorMap(hdc, &ed);
			}
		}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			EndDialog(hwnd, TRUE);
			break;

		case IDCANCEL:
			EndDialog(hwnd, TRUE);
			break;

		case IDC_COMBOOBJECT:

			break;

		case IDC_BUTTON1:
			DrawScrollBars(hdc, &ed);
			UpdateStatusBar(hdc, 0, 0, &ed);

			// abba3
			char buf[2048];

			ed.wptr->oblist[NVALS.objectid].monstertexture = NVALS.texture;

			result = GetDlgItemText(hwnd, IDC_EDIT1, &ybuffer[0], 50);

			ed.wptr->oblist[NVALS.objectid].y = atof(ybuffer);
			result = GetDlgItemText(hwnd, IDC_EDIT12, NVALS.buf, 2048);
			strcpy(ed.wptr->oblist[NVALS.objectid].ctext, NVALS.buf);
			break;

		case IDC_SCROLLBAR1:
			//					MessageBeep(0);
			break;
		case IDC_COMBO1:
			//					MessageBeep(0);

			//					result = SetDlgItemText(hwnd,IDC_COMBO1,  "FRED");
			//					hwndList=GetDlgItem(hwnd,IDC_COMBO1);

			break;

		case IDC_SCROLLBAR2:
			//					MessageBeep(0);
			break;

		case ID_MODE_AUTOROAD:
			ed.dialogbar_mode = DIALOGBAR_MODE_ROADS;
			ed.editor_mode = 0;
			DrawDialogBar(hdc, &ed);
			break;

		case ID_MODE_MANUAL:
			ed.dialogbar_mode = DIALOGBAR_MODE_OBJECTS;
			ed.editor_mode = 1;
			DrawDialogBar(hdc, &ed);
			break;

		case ID_MODE_EDIT:
			ed.dialogbar_mode = DIALOGBAR_MODE_OBJECTS;
			ed.editor_mode = 2;
			DrawDialogBar(hdc, &ed);
			break;

		case ID_INSERT_ROADSECTION:
			AppPause(TRUE);
			DialogBox(myglobs.hInstApp, "INSERTROADSECTION", myglobs.hWndMain, (DLGPROC)InsertRoadSection);
			DrawEditorMap(hdc, &ed);
			AppPause(FALSE);
			break;

		case ID_INSERT_ROUNDABOUT:
			break;

		case ID_INSERT_HOUSE:
			break;

		case ID_INSERT_OBJECT:
			break;

		case IDC_SCROLL_UP:
			ed.display_y_offset -= 10;
			DrawEditorMap(hdc, &ed);
			break;

		case IDC_SCROLL_DOWN:
			ed.display_y_offset += 10;
			DrawEditorMap(hdc, &ed);
			break;

		case IDC_SCROLL_LEFT:
			ed.display_x_offset -= 10;
			DrawEditorMap(hdc, &ed);
			break;

		case IDC_SCROLL_RIGHT:
			ed.display_x_offset += 10;
			DrawEditorMap(hdc, &ed);
			break;

		case ID_VIEW_ZOOM_IN:
			ed.display_scale = ed.display_scale * 2;
			if (ed.display_scale > 4)
				ed.display_scale = (float)4;

			DrawEditorMap(hdc, &ed);

			break;

		case ID_VIEW_ZOOM_OUT:
			if (ed.display_scale != 0)
				ed.display_scale = ed.display_scale / 2;

			DrawEditorMap(hdc, &ed);
			break;

		case IDC_LINK_NEXT:
			ChangeCurrentLink(1); // forward
			DrawEditorMap(hdc, &ed);
			DisplayLinksList(hdc);
			break;

		case IDC_LINK_PREV:
			ChangeCurrentLink(-1); // backward
			DrawEditorMap(hdc, &ed);
			DisplayLinksList(hdc);
			break;

		case ID_EDIT_CLEAR:
			InitLinkList(&ed);
			ed.wptr->oblist_length = 0;
			DrawEditorMap(hdc, &ed);
			break;

		case ID_EDIT_UNDO:
			ed.wptr->oblist_length--;

			if (ed.wptr->oblist_length < 0)
				ed.wptr->oblist_length = 0;
			UndoLastLink(&ed);
			DrawEditorMap(hdc, &ed);
			//					MessageBeep(0);
			break;

		case ID_VIEW_UPDATE:
			ReloadMainMap();
			break;

		case ID_FILE_OPEN:
			// LoadObjectList(hwnd,"objects.dat",&edworld);
			InitLinkList(&ed);
			LoadMapInEditor(hwnd, "Level1.map", &edworld);
			DrawEditorMap(hdc, &ed);
			break;

		case ID_FILE_SAVE:
			//					SaveMap(hwnd,"Level1.map",&edworld);

			SaveMap(hwnd, "Level1.map", &edworld);

			//				SaveMap(hwnd,"Level1.map",ed);

			InitWorldMap(hwnd, &edworld);

			// backhere

			CopyFile("level1.map", "level1.map", 0);
			CopyFile("level1.cmp", "level1.cmp", 0);
			CopyFile("level1.mod", "level1.mod", 0);

			//	CopyFile("level1.map","d:\\3dc\\bin\\level1.map",0);
			//	CopyFile("level1.cmp","d:\\3dc\\bin\\level1.cmp",0);

			break;
		case ID_DELETE:
			int delobj;
			int dellist;
			delobj = current_link;
			dellist = current_link;

			dellist = LinksList[dellist].objectid;
			for (delobj = 0; delobj < last_link; delobj++) {

				if (LinksList[delobj].objectid == dellist) {
					LinksList[delobj].inactive = 1;
				}
			}

			ed.wptr->oblist[dellist].inactive = 1;

			break;
		}
		break;
	}
	ReleaseDC(hwnd, hdc);
	return FALSE;
}

BOOL FAR PASCAL DisplayMap(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;

	hdc = GetDC(hwnd);
	switch (msg) {
	case WM_INITDIALOG:
		SetTimer(hwnd, 1, 100, NULL);
		break;

	case WM_COMMAND:
		if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			EndDialog(hwnd, TRUE);
		break;

	case WM_TIMER:
		KillTimer(hwnd, 1);
		Draw_2D_Map(hdc, cam);
		break;
	}
	ReleaseDC(hwnd, hdc);
	return FALSE;
}

// AppAbout
// About box message handler

BOOL FAR PASCAL AppAbout(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	MEMORYSTATUS memStatus;
	int mem;
	char buffer[255];
	LPTSTR buffer2;

	hdc = GetDC(hwnd);

	switch (msg) {
	case WM_INITDIALOG:
		SetTimer(hwnd, 1, 100, NULL);
		break;

	case WM_TIMER:
		KillTimer(hwnd, 1);

		memStatus.dwLength = sizeof(MEMORYSTATUS);
		GlobalMemoryStatus(&memStatus);

		mem = (int)memStatus.dwMemoryLoad;
		itoa(mem, buffer, 10);
		buffer2 = lstrcat(buffer, " %");
		TextOut(hdc, 10, 100, "System RAM in use :", 19);
		TextOut(hdc, 200, 100, buffer2, strlen(buffer2));

		// mem=(int)memStatus.dwAvailPhys;
		// itoa(mem,buffer,10);
		// TextOut(hdc,10,140,"Available mem =",15);
		// TextOut(hdc,200,140,buffer,strlen(buffer));

		mem = (int)memStatus.dwTotalPhys;
		itoa(mem, buffer, 10);
		TextOut(hdc, 10, 120, "Total System RAM :", 18);
		TextOut(hdc, 200, 120, buffer, strlen(buffer));

		break;

	case WM_COMMAND:
		if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			EndDialog(hwnd, TRUE);
		break;

		break;
	}
	ReleaseDC(hwnd, hdc);
	return FALSE;
}

// WindowProc
// Main window message handler.

long
    FAR PASCAL
    WindowProc(HWND hWnd, UINT message, WPARAM wParam,
               LPARAM lParam) {
	HDC hdc;
	int i;
	int idc_device_id;
	// DDCOLORKEY ckey;
	UINT cmd;

	BOOL bStop;
	BOOL bIsActive;
	LRESULT lresult;
	HINSTANCE hInst;

	hInst = myglobs.hInstApp;

	// Give D3DApp an opportunity to process any messages it MUST see in order
	// to perform it's function.

	//    if (!D3DAppWindowProc(&bStop, &lresult, hWnd, message, wParam, lParam))
	//	{
	//		ReportD3DAppError();
	//		CleanUpAndPostQuit();
	//		return 0;
	//}

	// Prevent resizing through this message

	if (message == WM_GETMINMAXINFO && myglobs.bResizingDisabled && !d3dapp->bFullscreen && !d3dapp->bMinimized) {
		RECT rc;
		GetWindowRect(hWnd, &rc);
		((LPMINMAXINFO)lParam)->ptMaxTrackSize.x = START_WIN_SIZE;
		((LPMINMAXINFO)lParam)->ptMaxTrackSize.y = START_WIN_SIZE;
		((LPMINMAXINFO)lParam)->ptMinTrackSize.x = START_WIN_SIZE;
		((LPMINMAXINFO)lParam)->ptMinTrackSize.y = START_WIN_SIZE;
		return 0;
	}

	// If bStop is set by D3DApp, the app should not process the message but
	// return lresult.

	//    if (bStop)	return lresult;

	switch (message) {
	case WM_ACTIVATEAPP:
		bIsActive = (BOOL)wParam;

		if (bIsActive) {
			// we are active, need to reacquire the keyboard
			//		ReacquireInput();
			//						DrawDialogControls(hdc, &ed);
			//	DrawEditorMap(hdc, &ed);
		}

		break;

	case WM_CREATE:

		car_speed = (float)0;

		cam.direction_x = (float)0;
		cam.direction_y = (float)0;
		cam.direction_z = (float)0;

		cam.view_angle = 90;
		cam.view_point_x = (float)720;
		cam.view_point_y = (float)14;
		cam.view_point_z = (float)450;

		cam.ramp_size = 1;

		ed.position.x = (float)200;
		ed.position.y = (float)0;
		ed.position.z = (float)200;

		ed.pave_width = (float)20;
		ed.road_width = (float)120;
		ed.num_sections = 3;
		ed.angle = (float)0;
		ed.delta_angle = (float)5;
		ed.display_scale = (float).25;
		ed.display_x_offset = 0;
		ed.display_y_offset = 0;
		ed.draw_road_section_flags = ALL;
		InitLinkList(&ed);
		Joystick_Enabled_Flag = FALSE;

		LoadToolbarBitmaps(hInst);
		temp_view_angle = (float)cam.view_angle;
		hdc = GetDC(hWnd);

		ReleaseDC(hWnd, hdc);
		SetTimer(hWnd, 1, 1000, NULL);
		break;

	case WM_RBUTTONDOWN:
		//			DDBitmapScreenDump(hWnd, d3dapp);
		break;

	case WM_TIMER:
		KillTimer(hWnd, 1);
		LoadObjectData(hWnd, "objects.dat", &world);
		LoadWorld(hWnd, "level1.map", &world);

		InitWorldMap(hWnd, &world);

		//			if(current_screen_status == FULLSCREEN_ONLY)
		//				D3DAppFullscreen(d3dapp->CurrMode);

		//			if (!D3DAppSetRenderState(&myglobs.rstate))
		//			{
		//				ReportD3DAppError();
		//				break;
		//			}
		//			if(Car_Sound_On_Flag == TRUE)
		//				InitialiseSound(hWnd);

		break;

	case WM_LBUTTONDOWN:
		DrawDialogControls(hdc, &ed);
		break;

	case WM_KEYDOWN:
		break;

		// Pause and unpause the app when entering/leaving the menu

	case WM_ENTERMENULOOP:
		AppPause(TRUE);
		break;
	case WM_EXITMENULOOP:
		AppPause(FALSE);
		break;
	case WM_DESTROY:
		myglobs.hWndMain = NULL;
		CleanUpAndPostQuit();
		break;

	case WM_INITMENUPOPUP:
		break;

	case WM_GETMINMAXINFO:
		// Some samples don't like being resized, such as those which use
		// screen coordinates (TLVERTEXs).

		if (myglobs.bResizingDisabled) {
			((LPMINMAXINFO)lParam)->ptMaxTrackSize.x = START_WIN_SIZE;
			((LPMINMAXINFO)lParam)->ptMaxTrackSize.y = START_WIN_SIZE;
			((LPMINMAXINFO)lParam)->ptMinTrackSize.x = START_WIN_SIZE;
			((LPMINMAXINFO)lParam)->ptMinTrackSize.y = START_WIN_SIZE;
			return 0;
		}
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
			cmd = GET_WM_COMMAND_ID(wParam, lParam);

			if (cmd >= IDC_DEVICES && cmd < IDC_DEVICES + 100) {
				//					PickInputDevice(cmd - IDC_DEVICES);
				//					MessageBeep(0);
			}

		case ID_TOOLS_MAPEDITOR:
			AppPause(TRUE);
			DialogBox(myglobs.hInstApp, "Editor", myglobs.hWndMain, (DLGPROC)MapEditor);
			int result;

			AppPause(FALSE);
			break;

		case ID_FILE_MAP:
			AppPause(TRUE);
			DialogBox(myglobs.hInstApp, "MapDialog", myglobs.hWndMain, (DLGPROC)DisplayMap);
			AppPause(FALSE);
			break;

		case MENU_ABOUT:
			AppPause(TRUE);
			DialogBox(myglobs.hInstApp, "AppAbout", myglobs.hWndMain, (DLGPROC)AppAbout);
			AppPause(FALSE);
			break;

		case MENU_EXIT:
			CleanUpAndPostQuit();
			break;

		} // end switch WM_COMMAND

		return 0L;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

/****************************************************************************/
/*                 Setting up callbacks for examples                        */
/****************************************************************************/
/*
 * SetMouseCallback
 * Called in an example to set a callback function for all WM messages
 * dealing with the mouse.  The handler should return whether or not
 * it handled the message.
 */
BOOL SetMouseCallback(BOOL (*Handler)(UINT, WPARAM, LPARAM)) {
	MouseHandler = Handler;
	return TRUE;
}

/*
 * SetKeyboardCallback
 * Called in an example to set a callback function for all WM messages
 * dealing with the keyboard. The handler should return whether or not it
 * handled the message.
 */
BOOL SetKeyboardCallback(BOOL (*Handler)(UINT, WPARAM, LPARAM)) {

	KeyboardHandler = Handler;
	return TRUE;
}

/****************************************************************************/
/*          Initialization, error reporting and release functions.          */
/****************************************************************************/
/*
 * InitGlobals
 * Called once at program initialization to initialize global variables.
 */
static void
InitGlobals(void) {
	d3dapp = NULL;
	//    memset(&myglobs.rstate, 0, sizeof(myglobs.rstate));
	memset(&myglobs, 0, sizeof(myglobs));
	myglobs.bClearsOn = TRUE;
	myglobs.bShowFrameRate = FALSE; // TRUE;
	myglobs.bShowInfo = FALSE;      // TRUE;
	MouseHandler = NULL;
	KeyboardHandler = NULL;
}

/*
 * CleanUpAndPostQuit
 * Release all D3D objects, post a quit message and set the bQuit flag
 */
void CleanUpAndPostQuit(void) {
	if (myglobs.bQuit)
		return;
	//    if (!D3DAppDestroy())
	//	ReportD3DAppError();
	//    ReleaseScene();
	myglobs.bQuit = TRUE;
	PostQuitMessage(0);
}

/*
 * ReportD3DAppError
 * Reports an error during a d3d app call.
 */
void ReportD3DAppError(void) {
	//    Msg("%s", D3DAppLastErrorString());
}

/* Msg
 * Message output for error notification.
 */
void __cdecl
Msg(LPSTR fmt, ...) {
	char buff[256];
	va_list va;

	va_start(va, fmt);
	wvsprintf(buff, fmt, va);
	va_end(va);
	lstrcat(buff, "\r\n");
	AppPause(TRUE);
	if (d3dapp && d3dapp->bFullscreen)
		SetWindowPos(myglobs.hWndMain, HWND_NOTOPMOST, 0, 0, 0, 0,
		             SWP_NOSIZE | SWP_NOMOVE);
	MessageBox(myglobs.hWndMain, buff, "D3D Example Message", MB_OK);
	if (d3dapp && d3dapp->bFullscreen)
		SetWindowPos(myglobs.hWndMain, HWND_TOPMOST, 0, 0, 0, 0,
		             SWP_NOSIZE | SWP_NOMOVE);
	AppPause(FALSE);
}

world_ptr GetWorldData() {
	return &world;
}

void gettrueobjectname(HWND hwnd) {

	int i;
	char buff[2000];
	char junk[2000];

	int result;

	currentobject = 0;
	result = GetDlgItemText(hwnd, IDC_COMBO1, &buff[0], 50);
	for (i = 0; i < objectnumbercount; i++) {
		if (strstr(buff, oname[i].objectname[0]) != NULL) {

			strcpy(currentobjectname, buff);
			currentobject = objectnumber[i];
			ed.current_object_id = currentobject;
			sprintf(junk, "%d", ed.current_object_id);
			result = SetDlgItemText(hwnd, IDC_OBJECT, junk);
		}
	}
}