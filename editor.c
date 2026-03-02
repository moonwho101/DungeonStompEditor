

#include <windows.h>
#include <windowsx.h>
#include <math.h>
#include <stdio.h>
#include "editor.h"
// #include "3Dengine.h"
// #include "d3ddemo.h"
#include "toolbar.h"
#include "world.h"

#include "resource.h"
#define ROAD_TEX 1
#define CURB_TEX 2
#define PAVE_TEX 3
#define MAX_NUM_LINKS 21000
#define MAX_NUM_OBJECT_TYPES 550

int current_poly = 0;
float last_angle = (float)0;
float last_x;
float last_z;
WORLD edworld;
int montexid[2000];

int LoadMapTextures(char *filename);

char currentobjectname[1000];
void SavePreCompiledHeader(HWND hwnd, world_ptr wptr);

HWND gwin = 0;

char modelname1[255];

int FindTextureID(char *p);
struct TEXNAMES {
	char texname[100][1];
} tname[1000];

int texturenumber[1000];
int texturenumbercount;

void FindTexture(int id);

#define MAX_NUM_QUADS 22000
#define MAX_NUM_TRIANGLES 24000
#define MAX_NUM_VERTICES 22000
#define MAX_NUM_TEXTURES 350

int NUM_QUADS = 0; // 95;
float px[4][MAX_NUM_QUADS], py[4][MAX_NUM_QUADS], pz[4][MAX_NUM_QUADS], pw[4][MAX_NUM_QUADS];
float mx[4][MAX_NUM_QUADS], my[4][MAX_NUM_QUADS], mz[4][MAX_NUM_QUADS], mw[4][MAX_NUM_QUADS];
float cx[4][MAX_NUM_QUADS], cy[4][MAX_NUM_QUADS], cz[4][MAX_NUM_QUADS], cw[4][MAX_NUM_QUADS];
int poly_texture_list[MAX_NUM_QUADS];

EDITOR ed;
int object_rot_angle;
// int displaysizex=686;
// int displaysizez=684;

int displaysizex = 686;
int displaysizez = 684;

int displaysizex1 = 0;
int displaysizez1 = 0;

int wtx;
int wtxy;
int wtz;
int wtzy;

int newangle = 0;
//	Rectangle(hdc,0,32,536,384);
//	hrgn=CreateRectRgn(0+1,32+1,536-1,384-1);
char ybuffer[200];

int slpm; // start left  poly marker
int srpm; // start right poly marker
int elpm; // end   left  poly marker
int erpm; // end   right poly marker

int draw_mode = 0;

int db_button_x = 559;
int db_button_y = 120;

int x_old = 0, y_old = 0;

BOOL start_link_flag;
int FindModelID(char *p);
int jumpnewspot = 0;
void FindModel(int id);
float ylocation = 0;
int monsterid = 0;
int monstertexture = 0;
float rcolour;
float gcolour;
float bcolour;

float dirx;
float diry;
float dirz;
int ltype;
int ability = 0;

float joint_x, joint_z;

char gctext[9048];

typedef struct Link_typ {
	float last_x;
	float last_z;
	float last_angle;
	BOOL active_link_flag;
	int inactive;
	int objectid;

} LINK, *Link_ptr;

typedef struct Controlbar_typ {
	int x1;
	int x2;
	int y1;
	int y2;
	int number_buttons;
	int x_button_offset;
	int y_button_offset;

} CONTROLBAR, *Controlbar_ptr;

typedef struct XRect_typ {
	int x1;
	int x2;
	int y1;
	int y2;

} XRECT, *XRect_ptr;

CONTROLBAR statusbar;
CONTROLBAR dialogbar;
CONTROLBAR toolbar;

struct NewValues {
	float ypos;
	int texture;
	int objectid;
	char buf[2048];
	char angle[10];
	char param[10];
} NVALS;

struct MODELNAMES {
	char name[100];
	int num;
} modelname[1000];

int modelcount = 0;

char modeltrue[255];
char texturetrue[255];

XRECT edmapwindow;
XRECT hscroll;
XRECT vscroll;

LINK LinksList[MAX_NUM_LINKS];
int last_link = 0;
int current_link = 0;
int bitmap_number;
MAP edmap;
VERT last_editor_vertex;

HBRUSH hbr_color, holdbrush_color;
HPEN hpen_color, holdpen_color;
HBITMAP hBitmap1, hBitmap2, hBitmap3, hOldBitmap;
BITMAP Bitmap;
HDC hMemoryDC;

HDC memDC;

int placemode = 0;

// world_ptr wptr;
WORLD world;

/////////////////////////////////////////////////////////////////////////////

void DrawDialogControls(HDC hdc, editor_ptr edptr) {
	toolbar.x1 = 0;
	toolbar.y1 = 0;
	toolbar.x2 = 549;
	toolbar.y2 = 32;

	//	dialogbar.x1 = displaysizex+40;
	//	dialogbar.y1 = 0;
	//	dialogbar.x2 = displaysizex+100;
	//	dialogbar.y2 = 430;

	dialogbar.x1 = 40;
	dialogbar.y1 = 0;
	dialogbar.x2 = 40 + 100;
	dialogbar.y2 = 430;

	statusbar.x1 = 0;
	statusbar.y1 = displaysizez + 10;
	statusbar.x2 = displaysizex;
	statusbar.y2 = displaysizez + 40;

	edmapwindow.x1 = 0;
	edmapwindow.y1 = 32;
	edmapwindow.x2 = displaysizex;
	edmapwindow.y2 = displaysizez;

	hscroll.x1 = 200;
	hscroll.y1 = displaysizez;
	hscroll.x2 = displaysizex;
	hscroll.y2 = displaysizez + 220;

	vscroll.x1 = displaysizex + 10;
	vscroll.y1 = 32 + 1;
	vscroll.x2 = displaysizex + 30;
	vscroll.y2 = displaysizez;

	//	DrawStatusBar(hdc,edptr);
	DrawToolBar(hdc);
	DrawDialogBar(hdc, edptr);
	DrawScrollBars(hdc, edptr);
}

void DrawScrollBars(HDC hdc, editor_ptr edptr) {
	int x, y;
	int x1, y1, x2, y2;
	float dx, dy;
	int x_off, y_off;

	x1 = hscroll.x1 + 13 + 6, y1 = hscroll.y1;
	x2 = hscroll.x2 - 13 - 6, y2 = hscroll.y2;

	dx = (float)51200 / (float)(x2 - x1);
	x_off = -1 * edptr->display_x_offset;
	x = x1 + (int)((float)x_off / dx);

	DrawScrollButton(hdc, x - 6, y1, DRAW_BUTTON_ON);

	x1 = vscroll.x1, y1 = vscroll.y1 + 13 + 6;
	x2 = vscroll.x2, y2 = vscroll.y2 - 13 - 6;

	dy = (float)51200 / (float)(y2 - y1);
	y_off = -1 * edptr->display_y_offset;
	y = y1 + (int)((float)y_off / dy);

	DrawScrollButton(hdc, x1, y - 6, DRAW_BUTTON_ON);

	DrawScrollButton(hdc, hscroll.x1, hscroll.y1, LEFT_BUTTON_DOWN);
	DrawScrollButton(hdc, hscroll.x2 - 12, hscroll.y1, LEFT_BUTTON_DOWN);

	DrawScrollButton(hdc, vscroll.x1, vscroll.y1, LEFT_BUTTON_DOWN);
	DrawScrollButton(hdc, vscroll.x1, vscroll.y2 - 12, LEFT_BUTTON_DOWN);
}

int UpdateScrollBars(HDC hdc, int x, int y, int button_status, editor_ptr edptr) {
	int x1, y1, x2, y2;
	float dx, dy;
	int x_off, y_off;

	x1 = hscroll.x1 + 13 + 6, y1 = hscroll.y1;
	x2 = hscroll.x2 - 13 - 6, y2 = hscroll.y2;

	if ((x >= x1 && x <= x2) && (y >= y1 && y <= y2)) {
		dx = (float)51200 / (float)(x2 - x1);
		x_off = (int)((float)(x - x1) * dx);
		edptr->display_x_offset = -1 * x_off;
		if (x_old > 0)
			DrawScrollButton(hdc, x_old, y1, ERASE_BUTTON);

		DrawScrollButton(hdc, x - 6, y1, DRAW_BUTTON_ON);
		x_old = x - 6;
		DrawEditorMap(hdc, edptr);
		edptr->scroll_bar_flag = TRUE;
	}

	x1 = vscroll.x1, y1 = vscroll.y1 + 13 + 6;
	x2 = vscroll.x2, y2 = vscroll.y2 - 13 - 6;

	if ((x >= x1 && x <= x2) && (y >= y1 && y <= y2)) {
		dy = (float)51200 / (float)(y2 - y1);
		y_off = (int)((float)(y - y1) * dy);
		edptr->display_y_offset = -1 * y_off;
		if (y_old > 0)
			DrawScrollButton(hdc, x1, y_old, ERASE_BUTTON);

		DrawScrollButton(hdc, x1, y - 6, DRAW_BUTTON_ON);
		y_old = y - 6;
		DrawEditorMap(hdc, edptr);
		edptr->scroll_bar_flag = TRUE;
	}

	if (edptr->scroll_bar_flag == TRUE)
		return 0;

	if (UpdateScrollBarButtons(hdc, x, y, hscroll.x1, hscroll.y1,
	                           hscroll.x1 + 12, hscroll.y1 + 12, button_status) == 1)
		return 1;

	if (UpdateScrollBarButtons(hdc, x, y, hscroll.x2 - 12, hscroll.y1,
	                           hscroll.x2, hscroll.y1 + 12, button_status) == 1)
		return 2;

	if (UpdateScrollBarButtons(hdc, x, y, vscroll.x1, vscroll.y1,
	                           vscroll.x1 + 12, vscroll.y1 + 12, button_status) == 1)
		return 3;

	if (UpdateScrollBarButtons(hdc, x, y, vscroll.x1, vscroll.y2 - 12,
	                           vscroll.x1 + 12, vscroll.y2, button_status) == 1)
		return 4;

	return 0;
}

int UpdateScrollBarButtons(HDC hdc, int x, int y, int x1, int y1, int x2, int y2, int button_status) {
	if ((x >= x1 && x <= x2) && (y >= y1 && y <= y2)) {
		if (button_status == LEFT_BUTTON_DOWN)
			DrawScrollButton(hdc, x1, y1, DRAW_BUTTON_OFF);

		if (button_status == LEFT_BUTTON_UP)
			DrawScrollButton(hdc, x1, y1, DRAW_BUTTON_ON);

		return 1;
	} else
		return 0;
}

int UpdateStatusBar(HDC hdc, int x, int y, editor_ptr edptr) {
	float s;
	char buffer[50];
	int scale = 100;
	LPTSTR buffer2;

	SetBkMode(hdc, OPAQUE);
	s = edptr->display_scale;

	itoa((int)edptr->object_rot_angle, buffer, 10);
	buffer2 = lstrcat(buffer, " deg");
	TextOut(hdc, 220 + scale, displaysizez + 20, "              ", 14);
	TextOut(hdc, 220 + scale, displaysizez + 20, buffer2, lstrlen(buffer2));

	itoa((int)(s * 100), buffer, 10);
	buffer2 = lstrcat(buffer, " %");
	TextOut(hdc, 160 + scale, displaysizez + 20, "          ", 10);
	TextOut(hdc, 160 + scale, displaysizez + 20, buffer2, lstrlen(buffer2));

	// if (draw_mode==1)
	//	TextOut(hdc,560+scale,displaysizez+20,"DRAW",10);

	return 0;
}

int UpdateEditorMapWindow(HWND hwnd, HDC hdc, int x, int y, int button_status, editor_ptr edptr) {
	int x1, y1, x2, y2;
	int MouseX, MouseY;
	int x_off, z_off;
	float s;
	char buffer[50];
	int result;
	char junk[255];
	HWND cwin;
	RECT rc;
	x1 = edmapwindow.x1;
	y1 = edmapwindow.y1;
	x2 = edmapwindow.x2;
	y2 = edmapwindow.y2;

	gwin = hwnd;

	GetWindowRect(hwnd, &rc);
	displaysizex = rc.right - 200;
	displaysizez = rc.bottom - 200;

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

	if (displaysizex < 10)
		displaysizex = 10;

	if (displaysizez < 10)
		displaysizez = 10;

	if (!((x >= x1 && x <= x2) && (y >= y1 && y <= y2))) {
		KillTimer(hwnd, 2);
		return 0;
	}

	if (button_status == LEFT_BUTTON_DOWN) {
		s = edptr->display_scale;        // 0.25
		x_off = edptr->display_x_offset; // 320
		z_off = edptr->display_y_offset; // 100

		MouseX = (int)((float)x / s) - x_off;
		MouseY = (int)(((float)y - (float)32) / s) - z_off;
		//	MouseX=MouseX-800;
		if (edptr->editor_mode == 0) {

			float ypos;
			int texture;
			SetLinkWithMouse(MouseX, MouseY);
			DrawEditorMap(hdc, edptr);
			//	result = SetDlgItemText(hwnd,IDC_COMBOOBJECT,  "");
			sprintf(junk, "ID:%d %s", LinksList[current_link].objectid,
			        edptr->wptr->oblist[LinksList[current_link].objectid].name);

			strcpy(modeltrue, "");
			strcat(junk, " ");

			if (strcmp(edptr->wptr->oblist[LinksList[current_link].objectid].name, "!monster1") == 0)
				FindModel(edworld.oblist[LinksList[current_link].objectid].monsterid);
			strcat(junk, modeltrue);
			result = SetDlgItemText(hwnd, IDC_COMBOOBJECT, junk);

			DrawEditorMap(hdc, edptr);
			DrawDialogControls(hdc, &ed);
		} else {
			PlaceObjectWithMouse(edptr, MouseX, MouseY);

			DrawEditorMap(hdc, edptr);
		}
	}

	if (button_status == MOUSE_MOVE) {
		s = edptr->display_scale;        // 0.25
		x_off = edptr->display_x_offset; // 320
		z_off = edptr->display_y_offset; // 100

		MouseX = (int)((float)x / s) - x_off;
		MouseY = (int)(((float)y - (float)32) / s) - z_off;

		// MouseX=MouseX-800;
		//	mosuedisplay
		itoa(MouseX, buffer, 10);
		TextOut(hdc, 10 + 400, displaysizez + 20, "              ", 14);
		TextOut(hdc, 10 + 400, displaysizez + 20, buffer, lstrlen(buffer));
		itoa(MouseY, buffer, 10);
		TextOut(hdc, 80 + 400, displaysizez + 20, "              ", 14);
		TextOut(hdc, 80 + 400, displaysizez + 20, buffer, lstrlen(buffer));

		edptr->MouseX = MouseX;
		edptr->MouseY = MouseY;

		if (edptr->editor_mode == 1) {
			//			SetTimer(hwnd,2,1000,NULL);
			SetTimer(hwnd, 2, 100, NULL);
			if (edptr->Redraw_editor_map_flag == 1) {
				DrawEditorMap(hdc, edptr);
				edptr->Redraw_editor_map_flag = 0;
			}
		}
	}
}

int UpdateDialogBar(HDC hdc, int x, int y, int button_status, editor_ptr edptr) {
	int col, row;
	int x1, x2;
	int y1, y2;
	int button_number;

	x1 = dialogbar.x1 + 9;
	x2 = x1 + 64;

	x1 = displaysizex + 45;
	;
	x2 = displaysizex + 130;
	;

	//	displaysizex+50;
	y1 = dialogbar.y1 + 20;
	y1 = dialogbar.y1 + 120;

	//	y2=y1+256; //8*32
	y2 = y1 + 8 * 32;

	if (x >= x1 && x <= x2) {
		if (y >= y1 && y <= y2) {
			// find the exact button
			col = (x - x1) / 32;
			row = (y - y1) / 32;

			button_number = row * 2 + col + 1;
			bitmap_number = button_number - 1;
			// DrawButton(hdc,displaysizex+45 + x*32,db_button_y + y*32,
			if (button_status == LEFT_BUTTON_DOWN) {
				DrawButton(hdc, x1 + col * 32, y1 + row * 32, DRAW_BUTTON_OFF, edptr);
			}

			if (button_status == LEFT_BUTTON_UP) {
				DrawButton(hdc, x1 + col * 32, y1 + row * 32, DRAW_BUTTON_ON, edptr);
			}
			return button_number;
		}
	}
	return 0;
}

int UpdateToolBar(HDC hdc, int x, int y, int button_status) {
	int col;
	int x1, x2;
	int y1, y2;
	int button_number;

	x1 = toolbar.x1 + 4;
	x2 = x1 + 22 * 12 + 22;
	y1 = toolbar.y1 + 4;
	y2 = y1 + 22 + 22;

	if (x >= x1 && x <= x2) {
		if (y >= y1 && y <= y2) {
			// find the exact button
			col = (x - x1) / 22;

			button_number = col + 1;
			bitmap_number = button_number - 1;

			if (button_status == LEFT_BUTTON_DOWN) {
				DrawSmallButton(hdc, 4 + col * 22 + toolbar.x1, toolbar.y1 + 4, DRAW_BUTTON_OFF);
			}

			if (button_status == LEFT_BUTTON_UP) {
				DrawSmallButton(hdc, 4 + col * 22 + toolbar.x1, toolbar.y1 + 4, DRAW_BUTTON_ON);
			}
			return button_number;
		}
	}
	return 0;
}

void LoadToolbarBitmaps(HINSTANCE hInst) {
	hBitmap1 = LoadBitmap(hInst, "bitmap1");
	GetObject(hBitmap1, 16, (LPSTR)&Bitmap);

	hBitmap2 = LoadBitmap(hInst, "TOOLBAR");
	GetObject(hBitmap2, 16, (LPSTR)&Bitmap);

	hBitmap3 = LoadBitmap(hInst, "bitmap2");
	GetObject(hBitmap1, 16, (LPSTR)&Bitmap);
}

void DrawBar(HDC hdc, int x1, int y1, int x2, int y2) {
	int nIndex;

	nIndex = COLOR_ACTIVEBORDER;
	holdbrush_color = SelectObject(hdc, GetSysColorBrush(nIndex));
	Rectangle(hdc, x1, y1, x2, y2);
	Delete_Brush_Color(hdc);

	Select_Pen_Color(hdc, WHITE);
	MoveToEx(hdc, x1, y2, NULL);
	LineTo(hdc, x1, y1); // left
	LineTo(hdc, x2, y1); // top
	Delete_Pen_Color(hdc);

	Select_Pen_Color(hdc, GRAY);
	MoveToEx(hdc, x1, y2, NULL);
	LineTo(hdc, x2, y2); // bottom
	LineTo(hdc, x2, y1); // right
	Delete_Pen_Color(hdc);
}

void DrawToolBar(HDC hdc) {
	int x;

	DrawBar(hdc, toolbar.x1, toolbar.y1, toolbar.x2, toolbar.y2);

	for (x = 0; x < 16; x++) {
		bitmap_number = x;
		DrawSmallButton(hdc, 4 + x * 22 + toolbar.x1, toolbar.y1 + 4, DRAW_BUTTON_ON);
	}
}

void DrawStatusBar(HDC hdc, editor_ptr edptr) {
	DrawBar(hdc, statusbar.x1, statusbar.y1, statusbar.x2, statusbar.y2);
	UpdateStatusBar(hdc, 0, 0, edptr);
}

void DrawDialogBar(HDC hdc, editor_ptr edptr) {
	int x, y;

	dialogbar.x1 = displaysizex + 45;
	dialogbar.y1 = 0;
	dialogbar.x2 = displaysizex + 120;

	//	dialogbar.x1 = 45;
	//	dialogbar.y1 = 0;
	//	dialogbar.x2 = 120;

	//	DrawBar(hdc,dialogbar.x1, dialogbar.y1, dialogbar.x2, dialogbar.y2);

	for (y = 0; y < 8; y++) {
		for (x = 0; x < 2; x++) {
			bitmap_number = y * 2 + x;
			DrawButton(hdc, displaysizex + 45 + x * 32, db_button_y + y * 32, DRAW_BUTTON_ON, edptr);
		}
	}
}

void Select_Brush_Color(HDC hdc, int color) {
	hbr_color = CreateSolidBrush(RGB(red[color], green[color], blue[color]));
	holdbrush_color = SelectObject(hdc, hbr_color);
}

void Select_Pen_Color(HDC hdc, int color) {
	hpen_color = CreatePen(PS_SOLID, 1, RGB(red[color], green[color], blue[color]));
	holdpen_color = SelectObject(hdc, hpen_color);
}

void Delete_Brush_Color(HDC hdc) {
	SelectObject(hdc, holdbrush_color);
	DeleteObject(hbr_color);
}

void Delete_Pen_Color(HDC hdc) {
	SelectObject(hdc, holdpen_color);
	DeleteObject(hpen_color);
}

void DrawButton(HDC hdc, int x, int y, int status, editor_ptr edptr) {
	int sx1, sx2;
	int sy1, sy2;
	int nIndex;

	SetBkMode(hdc, TRANSPARENT);
	SetBkColor(hdc, RGB(255, 255, 255));
	nIndex = COLOR_ACTIVEBORDER;

	sx1 = x;
	sx2 = x + 30;
	sy1 = y;
	sy2 = y + 30;

	if (status == DRAW_BUTTON_ON) {
		holdbrush_color = SelectObject(hdc, GetSysColorBrush(nIndex));
		Rectangle(hdc, sx1, sy1, sx2 + 1, sy2 + 1);

		Select_Pen_Color(hdc, WHITE);
		MoveToEx(hdc, sx1, sy2, NULL);
		LineTo(hdc, sx1, sy1); // left
		LineTo(hdc, sx2, sy1); // top
		Delete_Pen_Color(hdc);

		Select_Pen_Color(hdc, BLACK);
		MoveToEx(hdc, sx1 + 1, sy2 - 1, NULL);
		LineTo(hdc, sx2 - 1, sy2 - 1); // bottom
		LineTo(hdc, sx2 - 1, sy1 + 1); // right
		Delete_Pen_Color(hdc);

		if (edptr->dialogbar_mode == DIALOGBAR_MODE_ROADS) {
			hMemoryDC = CreateCompatibleDC(hdc);
			hOldBitmap = SelectObject(hMemoryDC, hBitmap1);
		}
		if (edptr->dialogbar_mode == DIALOGBAR_MODE_OBJECTS) {
			hMemoryDC = CreateCompatibleDC(hdc);
			hOldBitmap = SelectObject(hMemoryDC, hBitmap3);
		}

		if (bitmap_number <= 34)
			BitBlt(hdc, sx1 + 4, sy1 + 4, 18, 18, hMemoryDC, (bitmap_number) * 20, 1, SRCAND);

		SelectObject(hMemoryDC, hOldBitmap);
		Delete_Brush_Color(hdc);
		DeleteDC(hMemoryDC);
	}

	if (status == DRAW_BUTTON_OFF) {
		holdbrush_color = SelectObject(hdc, GetSysColorBrush(nIndex));
		Rectangle(hdc, sx1, sy1, sx2 + 1, sy2 + 1);

		Select_Pen_Color(hdc, BLUE);
		MoveToEx(hdc, sx1, sy2, NULL);
		LineTo(hdc, sx1, sy1); // left
		LineTo(hdc, sx2, sy1); // top
		Delete_Pen_Color(hdc);

		MoveToEx(hdc, sx1, sy2, NULL);
		LineTo(hdc, sx2, sy2); // bottom
		LineTo(hdc, sx2, sy1); // right
		Delete_Pen_Color(hdc);

		if (edptr->dialogbar_mode == DIALOGBAR_MODE_ROADS) {
			hMemoryDC = CreateCompatibleDC(hdc);
			hOldBitmap = SelectObject(hMemoryDC, hBitmap1);
		}
		if (edptr->dialogbar_mode == DIALOGBAR_MODE_OBJECTS) {
			hMemoryDC = CreateCompatibleDC(hdc);
			hOldBitmap = SelectObject(hMemoryDC, hBitmap3);
		}

		if (bitmap_number <= 34)
			BitBlt(hdc, sx1 + 4, sy1 + 4, 18, 18, hMemoryDC, (bitmap_number) * 20, 1, SRCAND);

		SelectObject(hMemoryDC, hOldBitmap);
		DeleteDC(hMemoryDC);
		Delete_Brush_Color(hdc);
		Delete_Pen_Color(hdc);
	}
}

void DrawSmallButton(HDC hdc, int x, int y, int status) {
	int sx1, sx2;
	int sy1, sy2;
	int nIndex;

	SetBkMode(hdc, TRANSPARENT);
	SetBkColor(hdc, RGB(255, 255, 255));
	nIndex = COLOR_ACTIVEBORDER;

	sx1 = x;
	sx2 = x + 21;
	sy1 = y;
	sy2 = y + 21;

	if (status == DRAW_BUTTON_ON) {
		holdbrush_color = SelectObject(hdc, GetSysColorBrush(nIndex));
		Rectangle(hdc, sx1, sy1, sx2, sy2);

		Select_Pen_Color(hdc, WHITE);
		MoveToEx(hdc, sx1, sy2, NULL);
		LineTo(hdc, sx1, sy1); // left
		LineTo(hdc, sx2, sy1); // top
		Delete_Pen_Color(hdc);

		Select_Pen_Color(hdc, BLACK);
		MoveToEx(hdc, sx1, sy2, NULL);
		LineTo(hdc, sx2, sy2); // bottom
		LineTo(hdc, sx2, sy1); // right
		Delete_Pen_Color(hdc);

		hMemoryDC = CreateCompatibleDC(hdc);
		hOldBitmap = SelectObject(hMemoryDC, hBitmap2);

		if (bitmap_number < 16)
			BitBlt(hdc, sx1 + 2, sy1 + 2, 16, 16, hMemoryDC, (bitmap_number) * 16, 1, SRCAND);

		SelectObject(hMemoryDC, hOldBitmap);
		Delete_Brush_Color(hdc);
		DeleteDC(hMemoryDC);
	}

	if (status == DRAW_BUTTON_OFF) {
		holdbrush_color = SelectObject(hdc, GetSysColorBrush(nIndex));
		Rectangle(hdc, sx1, sy1, sx2, sy2);

		Select_Pen_Color(hdc, GRAY);
		MoveToEx(hdc, sx1, sy2, NULL);
		LineTo(hdc, sx1, sy1); // left
		LineTo(hdc, sx2, sy1); // top
		Delete_Pen_Color(hdc);

		MoveToEx(hdc, sx1, sy2, NULL);
		LineTo(hdc, sx2, sy2); // bottom
		LineTo(hdc, sx2, sy1); // right
		Delete_Pen_Color(hdc);

		hMemoryDC = CreateCompatibleDC(hdc);
		hOldBitmap = SelectObject(hMemoryDC, hBitmap2);

		if (bitmap_number < 16)
			BitBlt(hdc, sx1 + 2, sy1 + 4, 16, 16, hMemoryDC, (bitmap_number) * 16, 1, SRCAND);

		SelectObject(hMemoryDC, hOldBitmap);
		DeleteDC(hMemoryDC);
		Delete_Brush_Color(hdc);
		Delete_Pen_Color(hdc);
	}
}

void DrawScrollButton(HDC hdc, int x, int y, int status) {
	int sx1, sx2;
	int sy1, sy2;
	int nIndex;
	COLORREF color;

	SetBkMode(hdc, TRANSPARENT);
	SetBkColor(hdc, RGB(255, 255, 255));
	nIndex = COLOR_ACTIVEBORDER;

	sx1 = x;
	sx2 = x + 12;
	sy1 = y;
	sy2 = y + 12;

	if (status == DRAW_BUTTON_ON) {
		holdbrush_color = SelectObject(hdc, GetSysColorBrush(nIndex));
		Rectangle(hdc, sx1, sy1, sx2, sy2);

		Select_Pen_Color(hdc, WHITE);
		MoveToEx(hdc, sx1, sy2, NULL);
		LineTo(hdc, sx1, sy1); // left
		LineTo(hdc, sx2, sy1); // top
		Delete_Pen_Color(hdc);

		Select_Pen_Color(hdc, BLACK);
		MoveToEx(hdc, sx1 + 1, sy2 - 1, NULL);
		LineTo(hdc, sx2 - 1, sy2 - 1); // bottom
		LineTo(hdc, sx2 - 1, sy1 + 1); // right
		Delete_Pen_Color(hdc);

		hMemoryDC = CreateCompatibleDC(hdc);
		hOldBitmap = SelectObject(hMemoryDC, hBitmap2);

		SelectObject(hMemoryDC, hOldBitmap);
		Delete_Brush_Color(hdc);
		DeleteDC(hMemoryDC);
	}

	if (status == DRAW_BUTTON_OFF) {
		holdbrush_color = SelectObject(hdc, GetSysColorBrush(nIndex));
		Rectangle(hdc, sx1, sy1, sx2, sy2);

		Select_Pen_Color(hdc, BLUE);
		MoveToEx(hdc, sx1, sy2, NULL);
		LineTo(hdc, sx1, sy1); // left
		LineTo(hdc, sx2, sy1); // top
		Delete_Pen_Color(hdc);

		MoveToEx(hdc, sx1, sy2, NULL);
		LineTo(hdc, sx2, sy2); // bottom
		LineTo(hdc, sx2, sy1); // right
		Delete_Pen_Color(hdc);

		hMemoryDC = CreateCompatibleDC(hdc);
		hOldBitmap = SelectObject(hMemoryDC, hBitmap2);

		SelectObject(hMemoryDC, hOldBitmap);
		DeleteDC(hMemoryDC);
		Delete_Brush_Color(hdc);
		Delete_Pen_Color(hdc);
	}

	if (status == ERASE_BUTTON) {
		color = (COLORREF)GetSysColor(nIndex);
		hpen_color = CreatePen(PS_SOLID, 1, color);
		holdpen_color = SelectObject(hdc, hpen_color);

		holdbrush_color = SelectObject(hdc, GetSysColorBrush(nIndex));
		Rectangle(hdc, sx1, sy1, sx2, sy2 + 1);
		Delete_Brush_Color(hdc);

		SelectObject(hdc, holdpen_color);
		DeleteObject(hpen_color);
	}
}

/////////////////////////////////////////////////////////////////////////////

void PlaceObjectWithMouse(editor_ptr edptr, int x, int z) {
	int i, dx, dz;
	int result;
	i = edptr->wptr->oblist_length;

	dx = x / 20;
	x = dx * 20;

	dz = z / 20;
	z = dz * 20;

	edptr->wptr->oblist[i].x = (float)x;
	edptr->wptr->oblist[i].y = (float)ylocation;
	edptr->wptr->oblist[i].z = (float)z;

	edptr->wptr->oblist[i].type = edptr->current_object_id;
	edptr->wptr->oblist[i].rot_angle = (float)edptr->object_rot_angle;
	edptr->wptr->oblist[i].light = -1;

	edptr->wptr->oblist[i].inactive = 0;

	edptr->wptr->oblist[i].monsterid = monsterid;
	edptr->wptr->oblist[i].monstertexture = monstertexture;

	strcpy(edptr->wptr->oblist[i].monstername, modelname1);

	//	montexid[i]=monstertexture;

	edptr->wptr->oblist[i].rcolour = rcolour;
	edptr->wptr->oblist[i].gcolour = gcolour;
	edptr->wptr->oblist[i].bcolour = bcolour;

	edptr->wptr->oblist[i].dirx = dirx;
	edptr->wptr->oblist[i].diry = diry;
	edptr->wptr->oblist[i].dirz = dirz;

	edptr->wptr->oblist[i].ltype = ltype;
	edptr->wptr->oblist[i].ability = ability;

	strcpy(edptr->wptr->oblist[i].ctext, gctext);

	strcpy(edptr->wptr->oblist[i].name, currentobjectname);
	strcpy(edworld.oblist[i].name, currentobjectname);

	last_x = x;
	last_z = z;
	last_angle = (float)edptr->object_rot_angle;

	AddLinkToList();

	edptr->wptr->oblist_length++;

	//	edptr->wptr->oblist_length++;

	//	ReplaceLinkInList();
}

void SetLinkWithMouse(int x, int z) {
	int i, count;
	double dx, dz;
	float dist;
	float min_dist = (float)100000;
	char junk[255];
	float ypos = 0;
	int result = 0;
	int texture = 0;
	char buf[2048];
	for (i = 0; i < last_link; i++) {

		if (LinksList[i].inactive == 0) {
			dx = fabs((float)x - LinksList[i].last_x);
			dz = fabs((float)z - LinksList[i].last_z);

			dist = (float)sqrt(dx * dx + dz * dz);
			if (dist < min_dist) {
				min_dist = dist;
				count = i;
			}
		}
	}

	if (GetAsyncKeyState(VK_UP) < 0) {

		// if (placemode==1){
		last_x = (float)x;
		last_z = (float)z;
		last_angle = (float)object_rot_angle;
		//		current_link=last_link;
		jumpnewspot = 1;

	} else {
		current_link = count;
		last_x = LinksList[current_link].last_x;
		last_z = LinksList[current_link].last_z;
		last_angle = LinksList[current_link].last_angle;

		// abba
	}

	ypos = ed.wptr->oblist[LinksList[current_link].objectid].y;

	if (ed.editor_mode == 0) {

		sprintf(junk, "%f", ypos);

		result = SetDlgItemText(gwin, IDC_EDIT1, junk);

		NVALS.ypos = ypos;

		texture = ed.wptr->oblist[LinksList[current_link].objectid].monstertexture;

		sprintf(buf, "%s", ed.wptr->oblist[LinksList[current_link].objectid].ctext);
		result = SetDlgItemText(gwin, IDC_EDIT12, buf);

		sprintf(buf, "%f", ed.wptr->oblist[LinksList[current_link].objectid].rot_angle);
		result = SetDlgItemText(gwin, IDC_OBJECT2, buf);
		strcpy(NVALS.angle, buf);

		sprintf(buf, "%d", ed.wptr->oblist[LinksList[current_link].objectid].ability);
		result = SetDlgItemText(gwin, IDC_EDIT13, buf);
		strcpy(NVALS.param, buf);

		strcpy(NVALS.buf, buf);

		sprintf(junk, "%d", texture);

		result = SetDlgItemText(gwin, IDC_EDIT5, junk);

		NVALS.texture = texture;
		NVALS.objectid = LinksList[current_link].objectid;
	}
}

void InitLinkList(editor_ptr edptr) {
	int i;

	for (i = 0; i < MAX_NUM_LINKS; i++) {
		LinksList[i].last_x = (float)0;
		LinksList[i].last_z = (float)0;
		LinksList[i].last_angle = (float)0;
		LinksList[i].active_link_flag = FALSE;
	}
	current_link = 0;
	last_link = 0;

	//	last_x=(float)760;
	//	last_z=(float)600;
	//	last_angle=(float)180;
	//	AddLinkToList();

	last_x = (float)600;
	last_z = (float)600;
	last_angle = (float)0;
	//	AddLinkToList();

	//	current_link=1;
}

void AddLinkToList() {

	int i = 0;
	int oblist_length;

	oblist_length = ed.wptr->oblist_length;

	LinksList[last_link].last_x = last_x;
	LinksList[last_link].last_z = last_z;
	// h1
	//	if (GetAsyncKeyState(VK_UP) <0 ) {

	//		i=ed.wptr->oblist_length;
	//		ed.wptr->oblist[i].x = last_x;
	//		ed.wptr->oblist[i].y = (float)0;
	//		ed.wptr->oblist[i].z = (float)last_z;
	//		ed.wptr->oblist[i].rot_angle =(float) 90;

	//		LinksList[last_link].last_angle=(float)90;

	//	}
	//	else {

	LinksList[last_link].last_angle = (float)last_angle;

	//	}

	LinksList[last_link].active_link_flag = TRUE;

	LinksList[last_link].inactive = 0;
	LinksList[last_link].objectid = oblist_length;

	if (jumpnewspot == 1) {
	} else {

		current_link = last_link;
		last_link++;
	}
}

void UndoLastLink(editor_ptr edptr) {
	int i, obtype;
	int num_links = 1;
	int oblist_length;

	oblist_length = edptr->wptr->oblist_length;
	obtype = edptr->wptr->oblist[oblist_length].type;

	switch (obtype) {
	case OBJECT_ID_STRAIGHT_ROAD:
		num_links = 1;
		break;

	case OBJECT_ID_SMALL_STRAIGHT_ROAD:
		num_links = 1;
		break;

	case OBJECT_ID_LEFTCURVE_ROAD:
		num_links = 1;
		break;

	case OBJECT_ID_RIGHTCURVE_ROAD:
		num_links = 1;
		break;

	case OBJECT_ID_TJUNCTION:
		num_links = 2;
		break;

	case OBJECT_ID_CROSSROADS:
		num_links = 3;
		break;

	case OBJECT_ID_LEFTCORNER_ROAD:
		num_links = 1;
		break;

	case OBJECT_ID_RIGHTCORNER_ROAD:
		num_links = 1;
		break;

	case OBJECT_ID_LEFT_TJUNCTION:
		num_links = 2;
		break;

	case OBJECT_ID_RIGHT_TJUNCTION:
		num_links = 2;
		break;

	case OBJECT_ID_ZEBRA:
		num_links = 1;
		break;
	}

	for (i = 0; i < num_links; i++) {
		last_link--;
		if (last_link < 2)
			last_link = 2;

		current_link = last_link - 1;

		last_x = LinksList[current_link].last_x;
		last_z = LinksList[current_link].last_z;
		last_angle = LinksList[current_link].last_angle;
	}
}

void ReplaceLinkInList() {
	AddLinkToList();
	LinksList[current_link].active_link_flag = FALSE;
}

void ChangeCurrentLink(int direction) {
	if (direction == 1) {
		current_link++;
		if (current_link >= last_link)
			current_link = 0;
		last_x = LinksList[current_link].last_x;
		last_z = LinksList[current_link].last_z;
		last_angle = LinksList[current_link].last_angle;
	}
}

void DisplayLinksList(HDC hdc) {
	int x, z, a;
	int i;
	char buffer[50];

	return;

	for (i = 0; i < last_link; i++) {
		x = (int)LinksList[i].last_x;
		z = (int)LinksList[i].last_z;
		a = (int)LinksList[i].last_angle;

		itoa(x, buffer, 10);
		TextOut(hdc, 20, 20, "x =", 3);
		TextOut(hdc, 70 + i * 50, 20, buffer, lstrlen(buffer));

		itoa(z, buffer, 10);
		TextOut(hdc, 20, 40, "z =", 3);
		TextOut(hdc, 70 + i * 50, 40, buffer, lstrlen(buffer));

		itoa(a, buffer, 10);
		TextOut(hdc, 20, 60, "a =", 3);
		TextOut(hdc, 70 + i * 50, 60, buffer, lstrlen(buffer));
	}
	itoa(current_link, buffer, 10);
	TextOut(hdc, 20, 100, "CL =", 3);
	TextOut(hdc, 70, 100, buffer, lstrlen(buffer));

	itoa(last_link, buffer, 10);
	TextOut(hdc, 20, 120, "LL =", 3);
	TextOut(hdc, 70, 120, buffer, lstrlen(buffer));

	itoa((int)last_x, buffer, 10);
	TextOut(hdc, 20, 140, "Lx =", 3);
	TextOut(hdc, 70, 140, buffer, lstrlen(buffer));

	itoa((int)last_z, buffer, 10);
	TextOut(hdc, 20, 160, "Lz =", 3);
	TextOut(hdc, 70, 160, buffer, lstrlen(buffer));
}

void Draw_2D_Map(HDC hdc, CAMERA cam) {
}

void DrawEditorMap(HDC hdc, editor_ptr edptr) {
	HRGN hrgn;
	HPEN hpen_color, holdpen_color;
	int i, j, w, z, x;
	int x_offset = 20;
	int vert_cnt, lit_v, ob_vert_count, ob_type;
	int poly, poly_cnt, pcount;
	float lx, lz, k = (float)0.017453292;
	float x_off, z_off, s;
	float cross_x, cross_z;
	float wx, wy, wz;
	float sx, sz;
	float stx, stz;
	float sine, cosine;
	double ay;
	int start_x, start_z;
	int end_x, end_z;
	int nIndex;

	//	Rectangle(hdc,0,32,displaysizex,displaysizez);
	//	hrgn=CreateRectRgn(0+1,32+1,displaysizex,displaysizez);

	//	Rectangle(hdc,200,32,wtz,wtzy);
	//	hrgn=CreateRectRgn(200,32,wtz,wtzy);
	// Select_Pen_Color(hdc,GRAY);

	//	hpen_color = CreatePen(PS_SOLID,1,RGB(0,0,0));
	//	holdpen_color=SelectObject(hdc,hpen_color);

	nIndex = COLOR_ACTIVEBORDER;
	holdbrush_color = SelectObject(hdc, GetSysColorBrush(nIndex));
	Rectangle(hdc, 200, 32, wtz, wtzy);
	Delete_Brush_Color(hdc);

	hrgn = CreateRectRgn(200, 32, displaysizex, displaysizez);

	//	HDC memDC;
	// memDC=CreateCompatible(&hdc);
	// memDC=FillRect(CRect(0,0,wtz,wtzy),memDC);

	//	FillRect(hdc,Rectangle(0,0,wtz,wtzy),hpen_color);

	SelectClipRgn(hdc, hrgn);

	s = edptr->display_scale;
	x_off = (float)edptr->display_x_offset;
	z_off = (float)edptr->display_y_offset;

	hpen_color = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	holdpen_color = SelectObject(hdc, hpen_color);

	start_x = (int)x_off - 260;
	start_z = (int)z_off - 260;

	end_x = (int)((float)displaysizex / s);
	end_z = (int)((float)displaysizez / s);

	if (s * 20 > 2) {
		for (z = start_z; z < end_z; z += 20) {
			sx = s * start_x;
			sz = s * z;

			MoveToEx(hdc, (int)sx, 32 + (int)sz, NULL);

			sx = s * end_x;

			LineTo(hdc, (int)sx, 32 + (int)sz);
		}

		for (x = start_x; x < end_x; x += 20) {
			sz = s * start_z;
			sx = s * x;

			MoveToEx(hdc, (int)sx, 32 + (int)sz, NULL);

			sz = s * end_z;

			LineTo(hdc, (int)sx, 32 + (int)sz);
		}
	}

	SelectObject(hdc, holdpen_color);
	DeleteObject(hpen_color);

	// cell lines

	hpen_color = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	holdpen_color = SelectObject(hdc, hpen_color);

	for (z = start_z; z < end_z; z += 260) {
		sx = s * start_x;
		sz = s * z;

		MoveToEx(hdc, (int)sx, 32 + (int)sz, NULL);

		sx = s * end_x;

		LineTo(hdc, (int)sx, 32 + (int)sz);
	}

	for (x = start_x; x < end_x; x += 260) {
		sz = s * start_z;
		sx = s * x;

		MoveToEx(hdc, (int)sx, 32 + (int)sz, NULL);

		sz = s * end_z;

		LineTo(hdc, (int)sx, 32 + (int)sz);
	}

	SelectObject(hdc, holdpen_color);
	DeleteObject(hpen_color);

	if (edptr->wptr == NULL)
		return;
	if (edptr->wptr->oblist_length != 0) {

		// Draw all polys in map //////////////////////////////////////////////

		for (i = 0; i < edptr->wptr->oblist_length; i++) {
			wx = edptr->wptr->oblist[i].x;
			wy = edptr->wptr->oblist[i].y;
			wz = edptr->wptr->oblist[i].z;

			ay = (double)edptr->wptr->oblist[i].rot_angle * k;
			cosine = (float)cos(ay);
			sine = (float)sin(ay);
			ob_type = edptr->wptr->oblist[i].type;
			lit_v = edptr->wptr->oblist[i].light;

			if (edptr->wptr->oblist[i].inactive == 0) {
				j = 0;
				ob_vert_count = 0;
				// fixthis
				poly = edptr->wptr->num_vert_per_object[ob_type] / 3;

				for (w = 0; w < poly; w++) {

					int numv = edptr->wptr->obdata[ob_type].num_vert[w];

					for (vert_cnt = 0; vert_cnt < numv; vert_cnt++) {
						lx = edptr->wptr->obdata[ob_type].v[j].x;
						lz = edptr->wptr->obdata[ob_type].v[j].z;

						sx = s * (x_off + wx + (lx * cosine - lz * sine));
						sz = s * (z_off + wz + (lx * sine + lz * cosine));

						if (vert_cnt == 0) {
							MoveToEx(hdc, (int)sx, 32 + (int)sz, NULL);
							stx = sx;
							stz = sz;
						} else
							LineTo(hdc, (int)sx, 32 + (int)sz);

						j++;
						ob_vert_count++;
					}
					LineTo(hdc, (int)stx, 32 + (int)stz);
					poly_cnt++;
				}

			} else {
				int man = 0;
				man = 1;
			}

			pcount += 15;
		}
	}
	// draw cross at current link

	if (GetAsyncKeyState(VK_UP) < 0) {
		cross_x = last_x;
		cross_z = last_z;

	} else {

		cross_x = LinksList[current_link].last_x;
		cross_z = LinksList[current_link].last_z;
	}
	hpen_color = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
	holdpen_color = SelectObject(hdc, hpen_color);

	MoveToEx(hdc, (int)((cross_x - 58 + x_off) * s), 32 + (int)((cross_z - 58 + z_off) * s), NULL);
	LineTo(hdc, (int)((cross_x + 58 + x_off) * s), 32 + (int)((cross_z + 58 + z_off) * s));

	MoveToEx(hdc, (int)((cross_x + 58 + x_off) * s), 32 + (int)((cross_z - 58 + z_off) * s), NULL);
	LineTo(hdc, (int)((cross_x - 58 + x_off) * s), 32 + (int)((cross_z + 58 + z_off) * s));

	DrawDialogControls(hdc, &ed);
	DrawScrollBars(hdc, &ed);

	SelectObject(hdc, holdpen_color);
	DeleteObject(hpen_color);
}

void DrawCross(HDC hdc, editor_ptr edptr, float cross_x, float cross_z) {

	HRGN hrgn;
	HPEN hpen_color, holdpen_color;
	int i, j, w, z, x;
	int x_offset = 20;
	int vert_cnt, lit_v, ob_vert_count, ob_type;
	int poly, poly_cnt, pcount;
	float lx, lz, k = (float)0.017453292;
	float x_off, z_off, s;

	float wx, wy, wz;
	float sx, sz;
	float stx, stz;
	float sine, cosine;
	double ay;
	int start_x, start_z;
	int end_x, end_z;
	int nIndex;

	//	Rectangle(hdc,0,32,displaysizex,displaysizez);
	//	hrgn=CreateRectRgn(0+1,32+1,displaysizex,displaysizez);

	//	Rectangle(hdc,200,32,wtz,wtzy);
	//	hrgn=CreateRectRgn(200,32,wtz,wtzy);
	// Select_Pen_Color(hdc,GRAY);

	//	hpen_color = CreatePen(PS_SOLID,1,RGB(0,0,0));
	//	holdpen_color=SelectObject(hdc,hpen_color);

	nIndex = COLOR_ACTIVEBORDER;
	//	holdbrush_color=SelectObject(hdc,GetSysColorBrush(nIndex));
	//	Rectangle(hdc,200,32,wtz,wtzy);
	Delete_Brush_Color(hdc);

	//	hrgn=CreateRectRgn(200,32,displaysizex,displaysizez);

	//	HDC memDC;
	// memDC=CreateCompatible(&hdc);
	// memDC=FillRect(CRect(0,0,wtz,wtzy),memDC);

	//	FillRect(hdc,Rectangle(0,0,wtz,wtzy),hpen_color);

	//	SelectClipRgn(hdc,hrgn);

	s = edptr->display_scale;
	x_off = (float)edptr->display_x_offset;
	z_off = (float)edptr->display_y_offset;

	hpen_color = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	holdpen_color = SelectObject(hdc, hpen_color);

	start_x = (int)x_off - 260;
	start_z = (int)z_off - 260;

	end_x = (int)((float)displaysizex / s);
	end_z = (int)((float)displaysizez / s);

	hpen_color = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	holdpen_color = SelectObject(hdc, hpen_color);

	MoveToEx(hdc, (int)((cross_x - 58 + x_off) * s), 32 + (int)((cross_z - 58 + z_off) * s), NULL);
	LineTo(hdc, (int)((cross_x + 58 + x_off) * s), 32 + (int)((cross_z + 58 + z_off) * s));

	MoveToEx(hdc, (int)((cross_x + 58 + x_off) * s), 32 + (int)((cross_z - 58 + z_off) * s), NULL);
	LineTo(hdc, (int)((cross_x - 58 + x_off) * s), 32 + (int)((cross_z + 58 + z_off) * s));

	DrawDialogControls(hdc, &ed);
	DrawScrollBars(hdc, &ed);

	SelectObject(hdc, holdpen_color);
	DeleteObject(hpen_color);
}

void DrawEditorMapObject(HDC hdc, editor_ptr edptr) {
	HRGN hrgn;
	HPEN hpen_color, holdpen_color;
	int j, w;
	int x_offset = 20;
	int vert_cnt, lit_v, ob_vert_count, ob_type;
	int poly, poly_cnt, pcount;
	float lx, lz, k = (float)0.017453292;
	float x_off, z_off, s;
	float wx, wy, wz;
	float sx, sz;
	float stx, stz;
	float sine, cosine;
	double ay;
	int dx, dz;
	int vv = 0;

	hrgn = CreateRectRgn(0 + 200, 32 + 1, displaysizex - 1, displaysizez - 1);
	SelectClipRgn(hdc, hrgn);

	s = edptr->display_scale;
	x_off = (float)edptr->display_x_offset;
	z_off = (float)edptr->display_y_offset;

	hpen_color = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
	holdpen_color = SelectObject(hdc, hpen_color);

	SelectObject(hdc, holdpen_color);
	DeleteObject(hpen_color);

	if (edptr->wptr == NULL)
		return;

	wx = (float)edptr->MouseX;
	wy = (float)1;
	wz = (float)edptr->MouseY;

	dx = (int)(wx / (float)20);
	wx = (float)(dx * (float)20);

	dz = (int)(wz / (float)20);
	wz = (float)(dz * (float)20);

	ay = (double)edptr->object_rot_angle * k;
	cosine = (float)cos(ay);
	sine = (float)sin(ay);
	ob_type = edptr->current_object_id; // OBJECT_ID_HOUSE;

	lit_v = edptr->wptr->oblist[ob_type].light;

	j = 0;
	ob_vert_count = 0;

	// fixthis
	poly = edptr->wptr->num_vert_per_object[ob_type] / 3;
	//	poly=edptr->wptr->num_vert_per_object[ob_type];

	for (w = 0; w < poly; w++) {
		int numv = edptr->wptr->obdata[ob_type].num_vert[w];
		for (vert_cnt = 0; vert_cnt < numv; vert_cnt++) {
			lx = edptr->wptr->obdata[ob_type].v[j].x;
			lz = edptr->wptr->obdata[ob_type].v[j].z;

			sx = s * (x_off + wx + (lx * cosine - lz * sine));
			sz = s * (z_off + wz + (lx * sine + lz * cosine));

			if (vert_cnt == 0) {
				MoveToEx(hdc, (int)sx, 32 + (int)sz, NULL);
				stx = sx;
				stz = sz;
			} else
				LineTo(hdc, (int)sx, 32 + (int)sz);

			j++;
			ob_vert_count++;
		}
		LineTo(hdc, (int)stx, 32 + (int)stz);
		poly_cnt++;
		vv = 0;
	}
	pcount += 15;
}

void ReloadMainMap() {
	edmap.num_poly = current_poly;
	SetMap(&edmap);
}

void CheckAngle() {
	if (last_angle < 0)
		last_angle = last_angle + 360;

	if (last_angle >= 360)
		last_angle = last_angle - 360;
}

void DrawRoadSection(HDC hdc, editor_ptr edptr, int instruction) {
	switch (instruction) {
	case LEFT_HAND_CURVE:
		AddLeftCurve(hdc, edptr);
		ReplaceLinkInList();
		DrawEditorMap(hdc, edptr);
		break;

	case RIGHT_HAND_CURVE:
		AddRightCurve(hdc, edptr);
		ReplaceLinkInList();
		DrawEditorMap(hdc, edptr);
		break;

	case STRAIGHT:
		AddStraight(hdc, edptr);
		DrawEditorMap(hdc, edptr);
		break;

	case ZEBRA:
		AddZebra(hdc, edptr);
		DrawEditorMap(hdc, edptr);
		break;

	case SMALL_STRAIGHT:
		AddSmallStraight(hdc, edptr);
		DrawEditorMap(hdc, edptr);
		break;

	case T_JUNCTION:
		AddTJunction(hdc, edptr);
		DrawEditorMap(hdc, edptr);
		break;

	case LEFT_T_JUNCTION:
		AddLeftTJunction(hdc, edptr);
		DrawEditorMap(hdc, edptr);
		break;

	case RIGHT_T_JUNCTION:
		AddRightTJunction(hdc, edptr);
		DrawEditorMap(hdc, edptr);
		break;

	case ROUND_ABOUT:
		AddRoundAbout(hdc, edptr);
		DrawEditorMap(hdc, edptr);
		break;

	case CROSSROAD:
		AddCrossRoad(hdc, edptr);
		DrawEditorMap(hdc, edptr);
		break;

	case LEFT_CORNER:
		AddLeftCorner(hdc, edptr);
		DrawEditorMap(hdc, edptr);
		break;

	case RIGHT_CORNER:
		AddRightCorner(hdc, edptr);
		DrawEditorMap(hdc, edptr);
		break;
	}
}

void AddLeftCurve(HDC hdc, editor_ptr edptr) {
	int i;
	float x, z;
	float angle, k;

	k = (float)0.017453292;
	i = edptr->wptr->oblist_length;
	edptr->wptr->oblist[i].x = LinksList[current_link].last_x;
	edptr->wptr->oblist[i].y = (float)ylocation;
	edptr->wptr->oblist[i].z = LinksList[current_link].last_z;
	edptr->wptr->oblist[i].rot_angle = LinksList[current_link].last_angle;
	edptr->wptr->oblist[i].type = OBJECT_ID_LEFTCURVE_ROAD;
	edptr->wptr->oblist[i].light = -1;
	edptr->wptr->oblist[i].inactive = 0;
	angle = k * LinksList[current_link].last_angle;

	strcpy(edptr->wptr->oblist[i].name, currentobjectname);
	strcpy(edworld.oblist[i].name, currentobjectname);

	x = edptr->wptr->obdata[OBJECT_ID_LEFTCURVE_ROAD].connection[0].x;
	z = edptr->wptr->obdata[OBJECT_ID_LEFTCURVE_ROAD].connection[0].z;
	last_x += (x * (float)cos(angle) - z * (float)sin(angle));
	last_z += (x * (float)sin(angle) + z * (float)cos(angle));

	last_angle += 15;
	CheckAngle();
	edptr->wptr->oblist_length++;
}

void AddRightCurve(HDC hdc, editor_ptr edptr) {
	int i;
	float x, z;
	float angle, k;

	k = (float)0.017453292;
	i = edptr->wptr->oblist_length;
	edptr->wptr->oblist[i].x = LinksList[current_link].last_x;
	edptr->wptr->oblist[i].y = (float)ylocation;
	edptr->wptr->oblist[i].z = LinksList[current_link].last_z;
	edptr->wptr->oblist[i].rot_angle = LinksList[current_link].last_angle;
	edptr->wptr->oblist[i].type = OBJECT_ID_RIGHTCURVE_ROAD;
	edptr->wptr->oblist[i].light = -1;
	edptr->wptr->oblist[i].inactive = 0;
	angle = k * LinksList[current_link].last_angle;
	strcpy(edptr->wptr->oblist[i].name, currentobjectname);
	strcpy(edworld.oblist[i].name, currentobjectname);

	x = edptr->wptr->obdata[OBJECT_ID_RIGHTCURVE_ROAD].connection[0].x;
	z = edptr->wptr->obdata[OBJECT_ID_RIGHTCURVE_ROAD].connection[0].z;
	last_x += (x * (float)cos(angle) - z * (float)sin(angle));
	last_z += (x * (float)sin(angle) + z * (float)cos(angle));

	last_angle -= 15;
	CheckAngle();
	edptr->wptr->oblist_length++;
}

void AddLeftCorner(HDC hdc, editor_ptr edptr) {
	int i;
	float x, z;
	float angle, k, deg_angle;

	k = (float)0.017453292;
	i = edptr->wptr->oblist_length;
	edptr->wptr->oblist[i].x = LinksList[current_link].last_x;
	edptr->wptr->oblist[i].y = (float)ylocation;
	edptr->wptr->oblist[i].z = LinksList[current_link].last_z;
	edptr->wptr->oblist[i].rot_angle = LinksList[current_link].last_angle;
	edptr->wptr->oblist[i].type = OBJECT_ID_LEFTCORNER_ROAD;
	edptr->wptr->oblist[i].light = -1;
	edptr->wptr->oblist[i].inactive = 0;
	deg_angle = LinksList[current_link].last_angle;
	last_angle = deg_angle + 90;
	CheckAngle();

	angle = k * LinksList[current_link].last_angle;

	x = edptr->wptr->obdata[OBJECT_ID_LEFTCORNER_ROAD].connection[0].x;
	z = edptr->wptr->obdata[OBJECT_ID_LEFTCORNER_ROAD].connection[0].z;
	last_x += (x * (float)cos(angle) - z * (float)sin(angle));
	last_z += (x * (float)sin(angle) + z * (float)cos(angle));
	strcpy(edptr->wptr->oblist[i].name, currentobjectname);
	strcpy(edworld.oblist[i].name, currentobjectname);

	ReplaceLinkInList();

	edptr->wptr->oblist_length++;
}

void AddRightCorner(HDC hdc, editor_ptr edptr) {
	int i;
	float x, z;
	float angle, k, deg_angle;

	k = (float)0.017453292;
	i = edptr->wptr->oblist_length;
	edptr->wptr->oblist[i].x = LinksList[current_link].last_x;
	edptr->wptr->oblist[i].y = (float)ylocation;
	edptr->wptr->oblist[i].z = LinksList[current_link].last_z;
	edptr->wptr->oblist[i].rot_angle = LinksList[current_link].last_angle;
	edptr->wptr->oblist[i].type = OBJECT_ID_RIGHTCORNER_ROAD;
	edptr->wptr->oblist[i].light = -1;
	edptr->wptr->oblist[i].inactive = 0;
	deg_angle = LinksList[current_link].last_angle;
	last_angle = deg_angle - 90;
	CheckAngle();
	angle = k * LinksList[current_link].last_angle;

	x = edptr->wptr->obdata[OBJECT_ID_RIGHTCORNER_ROAD].connection[0].x;
	z = edptr->wptr->obdata[OBJECT_ID_RIGHTCORNER_ROAD].connection[0].z;
	last_x += (x * (float)cos(angle) - z * (float)sin(angle));
	last_z += (x * (float)sin(angle) + z * (float)cos(angle));
	strcpy(edptr->wptr->oblist[i].name, currentobjectname);
	strcpy(edworld.oblist[i].name, currentobjectname);

	ReplaceLinkInList();

	edptr->wptr->oblist_length++;
}

void AddZebra(HDC hdc, editor_ptr edptr) {
	int i;
	float x, z;
	float angle, k;

	k = (float)0.017453292;
	i = edptr->wptr->oblist_length;
	edptr->wptr->oblist[i].x = LinksList[current_link].last_x;
	edptr->wptr->oblist[i].y = (float)ylocation;
	edptr->wptr->oblist[i].z = LinksList[current_link].last_z;
	edptr->wptr->oblist[i].rot_angle = LinksList[current_link].last_angle;
	edptr->wptr->oblist[i].type = OBJECT_ID_ZEBRA;
	edptr->wptr->oblist[i].light = -1;
	edptr->wptr->oblist[i].inactive = 0;
	angle = k * LinksList[current_link].last_angle;
	strcpy(edptr->wptr->oblist[i].name, currentobjectname);
	strcpy(edworld.oblist[i].name, currentobjectname);

	x = edptr->wptr->obdata[OBJECT_ID_ZEBRA].connection[0].x;
	z = edptr->wptr->obdata[OBJECT_ID_ZEBRA].connection[0].z;
	last_x += (x * (float)cos(angle) - z * (float)sin(angle));
	last_z += (x * (float)sin(angle) + z * (float)cos(angle));

	ReplaceLinkInList();

	edptr->wptr->oblist_length++;
}

void AddStraight(HDC hdc, editor_ptr edptr) {
	int i;
	float x, z;
	float angle, k;
	float test = 0;
	k = (float)0.017453292;
	i = edptr->wptr->oblist_length;

	//	if (GetAsyncKeyState(VK_UP) <0 ) {
	//	edptr->wptr->oblist[i].x = last_x;
	//	edptr->wptr->oblist[i].y = (float)0;
	//	edptr->wptr->oblist[i].z = (float)last_z;
	//	edptr->wptr->oblist[i].rot_angle = (float)object_rot_angle;
	//	angle = k * (float)object_rot_angle;
	//
	//	}
	//	else{

	test = (float)object_rot_angle;
	angle = k * test;

	if (jumpnewspot == 1) {

		edptr->wptr->oblist[i].x = last_x;
		edptr->wptr->oblist[i].y = (float)ylocation;
		edptr->wptr->oblist[i].z = last_z;
		angle = (float)k * (float)object_rot_angle;
		edptr->wptr->oblist[i].rot_angle = (float)object_rot_angle;
		jumpnewspot = 0;
	} else {

		edptr->wptr->oblist[i].x = LinksList[current_link].last_x;
		edptr->wptr->oblist[i].y = (float)ylocation;
		edptr->wptr->oblist[i].z = LinksList[current_link].last_z;

		edptr->wptr->oblist[i].rot_angle = LinksList[current_link].last_angle;
		angle = k * LinksList[current_link].last_angle;
	}

	edptr->wptr->oblist[i].type = OBJECT_ID_STRAIGHT_ROAD;
	edptr->wptr->oblist[i].light = -1;
	edptr->wptr->oblist[i].inactive = 0;

	strcpy(edptr->wptr->oblist[i].name, currentobjectname);
	strcpy(edworld.oblist[i].name, currentobjectname);

	x = edptr->wptr->obdata[OBJECT_ID_STRAIGHT_ROAD].connection[0].x;
	z = edptr->wptr->obdata[OBJECT_ID_STRAIGHT_ROAD].connection[0].z;
	last_x += (x * (float)cos(angle) - z * (float)sin(angle));
	last_z += (x * (float)sin(angle) + z * (float)cos(angle));

	ReplaceLinkInList();

	edptr->wptr->oblist_length++;
}

void AddSmallStraight(HDC hdc, editor_ptr edptr) {
	int i;
	float x, z;
	float angle, k;

	k = (float)0.017453292;
	i = edptr->wptr->oblist_length;
	edptr->wptr->oblist[i].x = LinksList[current_link].last_x;
	edptr->wptr->oblist[i].y = (float)ylocation;
	edptr->wptr->oblist[i].z = LinksList[current_link].last_z;
	edptr->wptr->oblist[i].rot_angle = LinksList[current_link].last_angle;
	edptr->wptr->oblist[i].type = OBJECT_ID_SMALL_STRAIGHT_ROAD;
	edptr->wptr->oblist[i].light = -1;
	edptr->wptr->oblist[i].inactive = 0;
	angle = k * LinksList[current_link].last_angle;

	strcpy(edptr->wptr->oblist[i].name, currentobjectname);
	strcpy(edworld.oblist[i].name, currentobjectname);

	x = edptr->wptr->obdata[OBJECT_ID_SMALL_STRAIGHT_ROAD].connection[0].x;
	z = edptr->wptr->obdata[OBJECT_ID_SMALL_STRAIGHT_ROAD].connection[0].z;
	last_x += (x * (float)cos(angle) - z * (float)sin(angle));
	last_z += (x * (float)sin(angle) + z * (float)cos(angle));

	ReplaceLinkInList();
	edptr->wptr->oblist_length++;
}

void AddTJunction(HDC hdc, editor_ptr edptr) {
	int i;
	float x, z;
	float rad_angle, deg_angle, k;
	float start_x, start_z;

	k = (float)0.017453292;
	i = edptr->wptr->oblist_length;
	edptr->wptr->oblist[i].x = LinksList[current_link].last_x;
	edptr->wptr->oblist[i].y = (float)ylocation;
	edptr->wptr->oblist[i].z = LinksList[current_link].last_z;
	edptr->wptr->oblist[i].rot_angle = LinksList[current_link].last_angle;
	edptr->wptr->oblist[i].type = OBJECT_ID_TJUNCTION;
	edptr->wptr->oblist[i].light = -1;
	edptr->wptr->oblist[i].inactive = 0;
	start_x = last_x;
	start_z = last_z;

	rad_angle = k * LinksList[current_link].last_angle;
	deg_angle = LinksList[current_link].last_angle;

	last_angle = deg_angle + 270;
	CheckAngle();
	x = edptr->wptr->obdata[OBJECT_ID_TJUNCTION].connection[0].x;
	z = edptr->wptr->obdata[OBJECT_ID_TJUNCTION].connection[0].z;
	last_x += (x * (float)cos(rad_angle) - z * (float)sin(rad_angle));
	last_z += (x * (float)sin(rad_angle) + z * (float)cos(rad_angle));
	ReplaceLinkInList();
	current_link = last_link;

	strcpy(edptr->wptr->oblist[i].name, currentobjectname);
	strcpy(edworld.oblist[i].name, currentobjectname);

	last_angle = deg_angle + 90;
	CheckAngle();
	x = edptr->wptr->obdata[OBJECT_ID_TJUNCTION].connection[1].x;
	z = edptr->wptr->obdata[OBJECT_ID_TJUNCTION].connection[1].z;
	last_x = start_x + (x * (float)cos(rad_angle) - z * (float)sin(rad_angle));
	last_z = start_z + (x * (float)sin(rad_angle) + z * (float)cos(rad_angle));
	AddLinkToList();

	last_x = LinksList[current_link].last_x;
	last_z = LinksList[current_link].last_z;
	last_angle = LinksList[current_link].last_angle;

	edptr->wptr->oblist_length++;
}

void AddLeftTJunction(HDC hdc, editor_ptr edptr) {
	int i;
	float x, z;
	float rad_angle, deg_angle, k;
	float start_x, start_z;

	k = (float)0.017453292;
	i = edptr->wptr->oblist_length;
	edptr->wptr->oblist[i].x = LinksList[current_link].last_x;
	edptr->wptr->oblist[i].y = (float)ylocation;
	edptr->wptr->oblist[i].z = LinksList[current_link].last_z;
	edptr->wptr->oblist[i].rot_angle = LinksList[current_link].last_angle;
	edptr->wptr->oblist[i].type = OBJECT_ID_LEFT_TJUNCTION;
	edptr->wptr->oblist[i].light = -1;
	edptr->wptr->oblist[i].inactive = 0;
	start_x = last_x;
	start_z = last_z;

	rad_angle = k * LinksList[current_link].last_angle;
	deg_angle = LinksList[current_link].last_angle;

	last_angle = deg_angle + 0;
	CheckAngle();
	x = edptr->wptr->obdata[OBJECT_ID_LEFT_TJUNCTION].connection[0].x;
	z = edptr->wptr->obdata[OBJECT_ID_LEFT_TJUNCTION].connection[0].z;
	last_x += (x * (float)cos(rad_angle) - z * (float)sin(rad_angle));
	last_z += (x * (float)sin(rad_angle) + z * (float)cos(rad_angle));
	ReplaceLinkInList();
	current_link = last_link;

	strcpy(edptr->wptr->oblist[i].name, currentobjectname);
	strcpy(edworld.oblist[i].name, currentobjectname);

	last_angle = deg_angle + 90;
	CheckAngle();
	x = edptr->wptr->obdata[OBJECT_ID_LEFT_TJUNCTION].connection[1].x;
	z = edptr->wptr->obdata[OBJECT_ID_LEFT_TJUNCTION].connection[1].z;
	last_x = start_x + (x * (float)cos(rad_angle) - z * (float)sin(rad_angle));
	last_z = start_z + (x * (float)sin(rad_angle) + z * (float)cos(rad_angle));
	AddLinkToList();

	last_x = LinksList[current_link].last_x;
	last_z = LinksList[current_link].last_z;
	last_angle = LinksList[current_link].last_angle;

	edptr->wptr->oblist_length++;
}

void AddRightTJunction(HDC hdc, editor_ptr edptr) {
	int i;
	float x, z;
	float rad_angle, deg_angle, k;
	float start_x, start_z;

	k = (float)0.017453292;
	i = edptr->wptr->oblist_length;
	edptr->wptr->oblist[i].x = LinksList[current_link].last_x;
	edptr->wptr->oblist[i].y = (float)ylocation;
	edptr->wptr->oblist[i].z = LinksList[current_link].last_z;
	edptr->wptr->oblist[i].rot_angle = LinksList[current_link].last_angle;
	edptr->wptr->oblist[i].type = OBJECT_ID_RIGHT_TJUNCTION;
	edptr->wptr->oblist[i].light = -1;
	edptr->wptr->oblist[i].inactive = 0;
	start_x = last_x;
	start_z = last_z;

	rad_angle = k * LinksList[current_link].last_angle;
	deg_angle = LinksList[current_link].last_angle;

	strcpy(edptr->wptr->oblist[i].name, currentobjectname);
	strcpy(edworld.oblist[i].name, currentobjectname);

	last_angle = deg_angle + 0;
	CheckAngle();
	x = edptr->wptr->obdata[OBJECT_ID_RIGHT_TJUNCTION].connection[0].x;
	z = edptr->wptr->obdata[OBJECT_ID_RIGHT_TJUNCTION].connection[0].z;
	last_x += (x * (float)cos(rad_angle) - z * (float)sin(rad_angle));
	last_z += (x * (float)sin(rad_angle) + z * (float)cos(rad_angle));
	ReplaceLinkInList();
	current_link = last_link;

	last_angle = deg_angle + 270;
	CheckAngle();
	x = edptr->wptr->obdata[OBJECT_ID_RIGHT_TJUNCTION].connection[1].x;
	z = edptr->wptr->obdata[OBJECT_ID_RIGHT_TJUNCTION].connection[1].z;
	last_x = start_x + (x * (float)cos(rad_angle) - z * (float)sin(rad_angle));
	last_z = start_z + (x * (float)sin(rad_angle) + z * (float)cos(rad_angle));
	AddLinkToList();

	last_x = LinksList[current_link].last_x;
	last_z = LinksList[current_link].last_z;
	last_angle = LinksList[current_link].last_angle;

	edptr->wptr->oblist_length++;
}

void AddCrossRoad(HDC hdc, editor_ptr edptr) {
	int i;
	float x, z;
	float rad_angle, deg_angle, k;
	float start_x, start_z;

	k = (float)0.017453292;
	i = edptr->wptr->oblist_length;
	edptr->wptr->oblist[i].x = LinksList[current_link].last_x;
	edptr->wptr->oblist[i].y = (float)ylocation;
	edptr->wptr->oblist[i].z = LinksList[current_link].last_z;
	edptr->wptr->oblist[i].rot_angle = LinksList[current_link].last_angle;
	edptr->wptr->oblist[i].type = OBJECT_ID_CROSSROADS;
	edptr->wptr->oblist[i].light = -1;
	edptr->wptr->oblist[i].inactive = 0;
	start_x = LinksList[current_link].last_x;
	start_z = LinksList[current_link].last_z;
	strcpy(edptr->wptr->oblist[i].name, currentobjectname);
	strcpy(edworld.oblist[i].name, currentobjectname);

	rad_angle = k * LinksList[current_link].last_angle;
	deg_angle = LinksList[current_link].last_angle;
	last_angle = deg_angle;
	x = edptr->wptr->obdata[OBJECT_ID_CROSSROADS].connection[0].x;
	z = edptr->wptr->obdata[OBJECT_ID_CROSSROADS].connection[0].z;
	last_x = start_x + (x * (float)cos(rad_angle) - z * (float)sin(rad_angle));
	last_z = start_z + (x * (float)sin(rad_angle) + z * (float)cos(rad_angle));
	ReplaceLinkInList();

	last_angle = deg_angle + 270;
	CheckAngle();
	x = edptr->wptr->obdata[OBJECT_ID_CROSSROADS].connection[1].x;
	z = edptr->wptr->obdata[OBJECT_ID_CROSSROADS].connection[1].z;
	last_x = start_x + (x * (float)cos(rad_angle) - z * (float)sin(rad_angle));
	last_z = start_z + (x * (float)sin(rad_angle) + z * (float)cos(rad_angle));
	AddLinkToList();

	last_angle = deg_angle + 90;
	CheckAngle();
	x = edptr->wptr->obdata[OBJECT_ID_CROSSROADS].connection[2].x;
	z = edptr->wptr->obdata[OBJECT_ID_CROSSROADS].connection[2].z;
	last_x = start_x + (x * (float)cos(rad_angle) - z * (float)sin(rad_angle));
	last_z = start_z + (x * (float)sin(rad_angle) + z * (float)cos(rad_angle));
	AddLinkToList();

	last_x = LinksList[current_link].last_x;
	last_z = LinksList[current_link].last_z;
	last_angle = LinksList[current_link].last_angle;
	edptr->wptr->oblist_length++;
}

void AddRoundAbout(HDC hdc, editor_ptr edptr) {
	// TODO : Add Roundabout code here
}

int LoadMapInEditor(HWND hwnd, char *filename, world_ptr wptr) {
	FILE *fp;
	char s[81];
	char p[100];

	char bigbuf[2048];
	char bigbuf2[2048];
	int bufc = 0;
	int bufc2 = 0;
	int loop1 = 0;

	int addanewtext = 0;
	// int addanewstartpos=0;
	int y_count = 30;
	int done = 0;
	int object_count = 0;
	int vert_count = 0;
	int pv_count = 0;
	int poly_count = 0;
	int object_id;
	int addanewplayer = 0;
	int addanewkey = 0;
	int mid = 0;
	int mtex = 0;
	char strmonid[250];
	char strmontex[250];
	char num[255];
	char ability[255];
	char junk[100];
	HWND hwndObjects;

	int tobjectcount = 0;
	BOOL start_flag = TRUE;

	//	last_link = 0;
	current_link = 0;
	hwndObjects = GetDlgItem(hwnd, IDC_COMBOOBJECT);
	SendMessage(hwndObjects, CB_RESETCONTENT, 0, 0);

	fp = fopen(filename, "r");
	if (fp == NULL) {
		MessageBox(hwnd, "Error can't load file", NULL, MB_OK);
		return 1;
	}

	while (done == 0) {
		fscanf(fp, "%s", &s);

		if (strcmp(s, "OBJECT") == 0) {
			fscanf(fp, "%s", &p);

			object_id = CheckObjectId((char *)&p);
			sprintf(junk, "ID:%d %s", tobjectcount, p);

			if (strcmp(p, "!monster1") != 0)
				SendMessage(hwndObjects, CB_ADDSTRING, 0, (LPARAM)&junk);

			// new line here

			//			fscanf( fp, "%s", &p );
			//			if (object_id==27 || object_id==25 ||object_id==26 || object_id==23) {

			//			if (object_id==23 || object_id==35 || object_id==32 || object_id==33 || object_id==57) {

			if (strstr(p, "!") != NULL) {
				addanewplayer = 1;
				// adplayer
			}
			//			if (strstr(p,"startpos")!=NULL) {
			//				addanewstartpos=1;
			//
			//			}

			if (strstr(p, "text") != NULL) {
				addanewtext = 1;
			}

			if (strstr(p, "door") != NULL) {
				addanewkey = 1;
			}

			if (object_id == -1) {
				MessageBox(hwnd, "Error Bad Object ID in: LoadMapInEditor", NULL, MB_OK);
				return 1;
			}

			if (start_flag == FALSE)
				object_count++;
			wptr->oblist[object_count].type = object_id;
			tobjectcount++;

			wptr->oblist[object_count].light = -1;
			start_flag = FALSE;
		}

		if (strcmp(s, "CO_ORDINATES") == 0) {
			fscanf(fp, "%s", &p);
			wptr->oblist[object_count].x = (float)atof(p);

			fscanf(fp, "%s", &p);
			wptr->oblist[object_count].y = (float)atof(p);

			fscanf(fp, "%s", &p);
			wptr->oblist[object_count].z = (float)atof(p);
		}

		if (strcmp(s, "ROT_ANGLE") == 0) {

			if (addanewplayer == 1) {

				//		fprintf( fp, "%d %d %d\n",(int)wptr->oblist[i].rot_angle,(int)wptr->oblist[i].monsterid, (int)wptr->oblist[i].monstertexture );

				strcpy(strmonid, "");
				strcpy(strmontex, "");

				fscanf(fp, "%s %s %s %s %s", &p, &strmonid, &strmontex, &num, &ability);

				//				fscanf( fp, "%s %s %s %s", &p, &strmonid,&strmontex,&num);
				//				fgets( bigbuf,100,fp );

				//				bufc2=0;

				//				for (loop1=1;loop1<strlen(bigbuf);loop1++){

				//						if (bigbuf[loop1]!=13 &&  bigbuf[loop1]!=10)
				//						bigbuf2[bufc2++]=bigbuf[loop1];

				//				}
				//
				//				bigbuf2[bufc2] ='\0';

				if (strlen(strmonid) == 0) {
					strcpy(strmonid, "0");
				}
				if (strlen(strmontex) == 0) {
					strcpy(strmontex, "27");
				}
				//				mid = (int) atof(strmonid);

				//				mtex= (int) atof(strmontex);
				mid = (int)FindModelID(strmonid);

				strcat(junk, " ");
				strcat(junk, strmonid);
				SendMessage(hwndObjects, CB_ADDSTRING, 0, (LPARAM)&junk);

				if (strcmp(strmontex, "0") == 0)
					mtex = 0;
				else if (strcmp(strmontex, "-1") == 0)
					mtex = -1;
				else
					mtex = (int)FindTextureID(strmontex);

				wptr->oblist[object_count].monsterid = (int)mid;
				wptr->oblist[object_count].monstertexture = (int)mtex;
				//				montexid[object_count]=mtex;
				wptr->oblist[object_count].rot_angle = (float)atof(p);

				wptr->oblist[object_count].ability = (float)atof(ability);
				;

				strcpy(wptr->oblist[object_count].ctext, "");

				addanewplayer = 0;

			} else {

				if (addanewtext == 1) {

					fscanf(fp, "%s", &ability); // type

					fgets(bigbuf, 2048, fp);
					bufc2 = 0;
					for (loop1 = 1; loop1 < strlen(bigbuf); loop1++) {

						if (bigbuf[loop1] != 13 && bigbuf[loop1] != 10)
							bigbuf2[bufc2++] = bigbuf[loop1];
					}

					bigbuf2[bufc2] = '\0';
					strcpy(wptr->oblist[object_count].ctext, bigbuf2);

					addanewtext = 0;
				} else {
					//					fscanf( fp, "%s", &p );

					if (addanewkey == 1) {
						fscanf(fp, "%s %s", &p, &ability);
						//						pCMyApp->oblist[object_count].rot_angle = (float)atof(p);
						//						ability=(int)atof(abil);
						addanewkey = 0;

						//						pCMyApp->oblist[object_count].ability = (int)atof(abil);
					} else {

						fscanf(fp, "%s", &p);
					}
				}

				wptr->oblist[object_count].rot_angle = (float)atof(p);
				wptr->oblist[object_count].ability = (float)atof(ability);

				//			montexid[object_count]=mtex;
			}
		}

		if (strcmp(s, "LIGHT_ON_VERT") == 0) {
			fscanf(fp, "%s", &p);
			wptr->oblist[object_count].light = atoi(p);
		}

		if (strcmp(s, "LIGHT_SOURCE") == 0) {
			fscanf(fp, "%s", &p);

			if (strcmp(p, "Spotlight") == 0) {
				wptr->oblist[object_count].ltype = 0.0f;

			} else if (strcmp(p, "point") == 0) {
				wptr->oblist[object_count].ltype = 1.0f;

			} else if (strcmp(p, "directional") == 0) {
				wptr->oblist[object_count].ltype = 2.0f;
			} else if (strcmp(p, "flicker") == 0) {
				wptr->oblist[object_count].ltype = 3.0f;
			}

			fscanf(fp, "%s", &p);

			if (strcmp(p, "POS") == 0) {
				fscanf(fp, "%s", &p);
				wptr->oblist[object_count].x = (float)atof(p);
				fscanf(fp, "%s", &p);
				wptr->oblist[object_count].y = (float)atof(p);
				fscanf(fp, "%s", &p);
				wptr->oblist[object_count].z = (float)atof(p);
			}

			fscanf(fp, "%s", &p);

			if (strcmp(p, "DIR") == 0) {
				fscanf(fp, "%s", &p);
				wptr->oblist[object_count].dirx = (float)atof(p);
				fscanf(fp, "%s", &p);
				wptr->oblist[object_count].diry = (float)atof(p);
				fscanf(fp, "%s", &p);
				wptr->oblist[object_count].dirz = (float)atof(p);
			}

			fscanf(fp, "%s", &p);

			if (strcmp(p, "COLOUR") == 0) {
				fscanf(fp, "%s", &p);
				wptr->oblist[object_count].rcolour = (float)atof(p);
				fscanf(fp, "%s", &p);
				wptr->oblist[object_count].gcolour = (float)atof(p);
				fscanf(fp, "%s", &p);
				wptr->oblist[object_count].bcolour = (float)atof(p);
			}
		}

		if (strcmp(s, "LINK") == 0) {
			fscanf(fp, "%s", &p);
			LinksList[last_link].last_x = (float)atoi(p);

			fscanf(fp, "%s", &p);
			LinksList[last_link].last_z = (float)atoi(p);

			fscanf(fp, "%s", &p);
			LinksList[last_link].last_angle = (float)atoi(p);

			fscanf(fp, "%s", &p);
			LinksList[last_link].objectid = (float)atoi(p);

			last_link++;
		}

		if (strcmp(s, "END_FILE") == 0) {
			fscanf(fp, "%s", &p);
			wptr->oblist_length = object_count + 1;
			done = 1;
		}
	}
	fclose(fp);
	return 0;
}

int LoadMapTextures(char *filename) {
	FILE *fp;
	char s[81];
	char p[100];
	int y_count = 30;
	int done = 0;
	int object_count = 0;
	int vert_count = 0;
	int pv_count = 0;
	int poly_count = 0;
	int object_id;
	int addanewplayer = 0;
	int mid = 0;
	int mtex = 0;
	char strmonid[250];
	char strmontex[250];

	BOOL start_flag = TRUE;

	//	last_link = 0;
	current_link = 0;

	fp = fopen(filename, "r");

	while (done == 0) {
		fscanf(fp, "%s", &s);

		if (strcmp(s, "OBJECT") == 0) {
			fscanf(fp, "%s", &p);
			if (strstr(p, "!") != NULL) {
				//			if (strstr(p,"floor")!=NULL) {
				//				if (object_id==33 || object_id==32 ) {
				addanewplayer = 1;
			}

			if (start_flag == FALSE)
				object_count++;

			start_flag = FALSE;
		}

		if (strcmp(s, "CO_ORDINATES") == 0) {
			fscanf(fp, "%s", &p);

			fscanf(fp, "%s", &p);

			fscanf(fp, "%s", &p);
		}

		if (strcmp(s, "ROT_ANGLE") == 0) {

			if (addanewplayer == 1) {
				strcpy(strmonid, "");
				strcpy(strmontex, "");

				fscanf(fp, "%s %s %s", &p, &strmonid, &strmontex);

				if (strlen(strmonid) == 0) {
					strcpy(strmonid, "0");
				}
				if (strlen(strmontex) == 0) {
					strcpy(strmontex, "27");
				}
				mid = (int)atof(strmonid);
				mtex = (int)atof(strmontex);

				montexid[object_count] = FindTextureID(strmontex);

				addanewplayer = 0;

			} else {
				fscanf(fp, "%s", &p);
			}
		}

		if (strcmp(s, "LIGHT_ON_VERT") == 0) {
			fscanf(fp, "%s", &p);
		}

		if (strcmp(s, "LIGHT_SOURCE") == 0) {
			fscanf(fp, "%s", &p);

			fscanf(fp, "%s", &p);

			if (strcmp(p, "POS") == 0) {
				fscanf(fp, "%s", &p);

				fscanf(fp, "%s", &p);

				fscanf(fp, "%s", &p);
			}

			fscanf(fp, "%s", &p);

			if (strcmp(p, "DIR") == 0) {
				fscanf(fp, "%s", &p);

				fscanf(fp, "%s", &p);

				fscanf(fp, "%s", &p);
			}

			fscanf(fp, "%s", &p);

			if (strcmp(p, "COLOUR") == 0) {
				fscanf(fp, "%s", &p);

				fscanf(fp, "%s", &p);

				fscanf(fp, "%s", &p);
			}
		}

		if (strcmp(s, "LINK") == 0) {
			fscanf(fp, "%s", &p);

			fscanf(fp, "%s", &p);

			fscanf(fp, "%s", &p);

			fscanf(fp, "%s", &p);
		}

		if (strcmp(s, "END_FILE") == 0) {
			fscanf(fp, "%s", &p);

			done = 1;
		}
	}
	fclose(fp);
	return 0;
}

int SaveMap(HWND hwnd, char *filename, world_ptr wptr) {
	FILE *fp;
	int ob_list_length;
	int i, a;
	int obtype;
	int objectid = 0;
	float x, z;
	int countinactive = 0;
	int lastobj = 0;
	float ypos = 0;
	int monsterfound = 0;
	char buf[1000];
	char object_string[2000];
	float a1, b1, c1, d1, e1, f1, g1, h1, i1;

	/*
	char *object_string[] ={"straight_road",		// 0
	"left_curve_road",		// 1
	"right_curve_road",		// 2
	"t_junction",			// 3
	"crossroads",			// 4
	"roundabout",			// 5
	"lamp_post",			// 6
	"lamp",					// 7
	"house_semileft",				// 8
	"police_car",			// 9
	"road_sign",			// 10
	"small_straight_road",	// 11
	"left_corner",			// 12
	"right_corner",			// 13
	"left_t_junction",		// 14
	"right_t_junction",		// 15
	"zebra",				// 16
	"island",				// 17
	"house_semiright",			// 18
	"petrol_station",		// 19
	"traffic_light",		// 20
	"slope",				// 21
	"slope_corner",			// 22
	"central_reservation",	// 23
	"mbshop1",				//24
	"wall1",			//25
	"floor1",   //26
	"monster1",  //27
	"abbb1", //28
	"abbb2", //29
	"abbb3", //30
	"abbb4",//31
	"abbb5",//32
	"abbb6",//33
	"abbb7",//34
	"abbb8",//35
	"structure1", //36
	"structure2",//37
	"structure3",//38
	"structure4",//39
	"structure5",//40
	"structure6",//41
	"structure7",//42
	"structure8",//43
	"structure9"//44
	};
	*/

	fp = fopen(filename, "w");
	if (fp == NULL) {
		MessageBox(hwnd, "Error can't save file", NULL, MB_OK);
		return 1;
	}

	ob_list_length = wptr->oblist_length;
	//	wptr=edworld;

	for (i = 0; i < ob_list_length; i++) {

		if (wptr->oblist[i].inactive == 0) {

			obtype = wptr->oblist[i].type;
			strcpy(object_string, wptr->oblist[i].name);

			fprintf(fp, "%s", "OBJECT ");

			if (obtype < MAX_NUM_OBJECT_TYPES)
				fprintf(fp, "%s\n", object_string);
			else {
				MessageBox(hwnd, "Bad object ID Error in: SaveMap", NULL, MB_OK);
				return 1;
			}
			fprintf(fp, "%s", "CO_ORDINATES ");
			fprintf(fp, "%f%s", wptr->oblist[i].x, " ");
			fprintf(fp, "%f%s", wptr->oblist[i].y, " ");
			ypos = wptr->oblist[i].y;
			//		fprintf( fp, "%f%s",(float)0," " );
			fprintf(fp, "%f%s\n", wptr->oblist[i].z, " ");

			fprintf(fp, "%s", "ROT_ANGLE ");

			if (strstr(object_string, "!monster1") != NULL) {

				FindModel(edworld.oblist[i].monsterid);

				if (edworld.oblist[i].monstertexture <= 0) {
					sprintf(texturetrue, "%d", edworld.oblist[i].monstertexture);
					//			strcpy(modeltrue,"0");

				} else {
					FindTexture(edworld.oblist[i].monstertexture);
				}
				//				fprintf( fp, "%d %s %s %d\n",(int)edworld.oblist[i].rot_angle,modeltrue, texturetrue, i );

				fprintf(fp, "%d %s %s %d %d\n", (int)edworld.oblist[i].rot_angle, modeltrue, texturetrue, i, (int)edworld.oblist[i].ability);

			} else if (strstr(object_string, "text") != NULL) {

				//				FindModel(edworld.oblist[i].monsterid);

				//				if (edworld.oblist[i].monstertexture<=0) {
				//					sprintf(texturetrue,"%d",edworld.oblist[i].monstertexture);
				//			strcpy(modeltrue,"0");

				//				}else{
				//					FindTexture(edworld.oblist[i].monstertexture);
				//				}
				//				fprintf( fp, "%d %s %s %d\n",(int)edworld.oblist[i].rot_angle,modeltrue, texturetrue, i );

				//				fprintf( fp, "%d %s %s %d %s\n",(int)0,"text", "text", i,edworld.oblist[i].ctext );

				fprintf(fp, "%d %s\n", (int)edworld.oblist[i].ability, edworld.oblist[i].ctext);

			} else if (strstr(object_string, "!") != NULL) {

				FindTexture(edworld.oblist[i].monstertexture);

				fprintf(fp, "%d %d %s %d %d\n", (int)edworld.oblist[i].rot_angle, (int)edworld.oblist[i].monsterid, texturetrue, i, (int)edworld.oblist[i].ability);
			}

			//			else if (strstr(object_string,"KEY2")!=NULL) {
			//				fprintf( fp, "%d %d\n",(int)wptr->oblist[i].rot_angle,(int)wptr->oblist[i].ability);
			//			}
			else {

				if (strstr(object_string, "door") != NULL) {
					fprintf(fp, "%d %d\n", (int)wptr->oblist[i].rot_angle, (int)wptr->oblist[i].ability);
				} else {
					fprintf(fp, "%d\n", (int)wptr->oblist[i].rot_angle);
				}
			}

			if (strcmp(object_string, "lamp_post") == 0) {

				if (wptr->oblist[i].ltype == 0) {

					a1 = (float)wptr->oblist[i].x;
					b1 = (float)wptr->oblist[i].y;
					c1 = (float)wptr->oblist[i].z;
					d1 = (float)wptr->oblist[i].dirx;
					e1 = (float)wptr->oblist[i].diry;
					f1 = (float)wptr->oblist[i].dirz;
					g1 = (float)wptr->oblist[i].rcolour;
					h1 = (float)wptr->oblist[i].gcolour;
					i1 = (float)wptr->oblist[i].bcolour;

					sprintf(buf, "LIGHT_SOURCE Spotlight POS %f %f %f DIR %f %f %f COLOUR %f %f %f\n",
					        a1, b1, c1, d1, e1, f1, g1, h1, i1);
					fprintf(fp, "LIGHT_SOURCE Spotlight POS %f %f %f DIR %f %f %f COLOUR %f %f %f\n",
					        wptr->oblist[i].x,
					        wptr->oblist[i].y,
					        wptr->oblist[i].z,
					        wptr->oblist[i].dirx,
					        wptr->oblist[i].diry,
					        wptr->oblist[i].dirz,
					        wptr->oblist[i].rcolour,
					        wptr->oblist[i].gcolour,
					        wptr->oblist[i].bcolour);
				}

				if (wptr->oblist[i].ltype == 1) {
					fprintf(fp, "LIGHT_SOURCE point POS %f %f %f DIR %f %f %f COLOUR %f %f %f\n",
					        wptr->oblist[i].x,
					        wptr->oblist[i].y,
					        wptr->oblist[i].z,
					        wptr->oblist[i].dirx,
					        wptr->oblist[i].diry,
					        wptr->oblist[i].dirz,
					        wptr->oblist[i].rcolour,
					        wptr->oblist[i].gcolour,
					        wptr->oblist[i].bcolour);
				}
				if (wptr->oblist[i].ltype == 2) {

					fprintf(fp, "LIGHT_SOURCE directional POS %f %f %f DIR %f %f %f COLOUR %f %f %f\n",
					        wptr->oblist[i].x,
					        wptr->oblist[i].y,
					        wptr->oblist[i].z,
					        wptr->oblist[i].dirx,
					        wptr->oblist[i].diry,
					        wptr->oblist[i].dirz,
					        wptr->oblist[i].rcolour,
					        wptr->oblist[i].gcolour,
					        wptr->oblist[i].bcolour);
				}
				if (wptr->oblist[i].ltype == 3) {

					fprintf(fp, "LIGHT_SOURCE flicker POS %f %f %f DIR %f %f %f COLOUR %f %f %f\n",
					        wptr->oblist[i].x,
					        wptr->oblist[i].y,
					        wptr->oblist[i].z,
					        wptr->oblist[i].dirx,
					        wptr->oblist[i].diry,
					        wptr->oblist[i].dirz,
					        wptr->oblist[i].rcolour,
					        wptr->oblist[i].gcolour,
					        wptr->oblist[i].bcolour);
				}
			}

			// LIGHT_SOURCE Spotlight POS 734 112 600 DIR 0 -112 0
			if (wptr->oblist[i].light >= 0) {
				fprintf(fp, "%s", "LIGHT_ON_VERT ");
				fprintf(fp, "%d\n\n", wptr->oblist[i].light);
			}
		}
	}

	lastobj = -999;
	for (i = 0; i < last_link; i++) {
		x = LinksList[i].last_x;
		z = LinksList[i].last_z;
		objectid = LinksList[i].objectid;
		a = (int)LinksList[i].last_angle;

		if (LinksList[i].inactive == 0) {
			fprintf(fp, "%s", "LINK ");
			fprintf(fp, "%f%s", x, " ");
			fprintf(fp, "%f%s", z, " ");
			fprintf(fp, "%d%s%d\n", a, " ", objectid - countinactive);
		} else {

			if (lastobj != objectid)
				countinactive++;
		}
		lastobj = objectid;
	}

	fprintf(fp, "%s", "END_FILE");
	fclose(fp);

	return 0;
}

void FindModel(int id) {

	int i;
	for (i = 0; i < modelcount; i++) {

		if (id == modelname[i].num) {
			strcpy(modeltrue, modelname[i].name);
		}
	}
}

void FindTexture(int id) {

	int i;
	for (i = 0; i < texturenumbercount; i++) {

		if (id == texturenumber[i]) {
			strcpy(texturetrue, tname[i].texname);
		}
	}
}

int FindTextureID(char *p) {

	int i;
	for (i = 0; i < texturenumbercount; i++) {

		if (strcmp(p, tname[i].texname) == 0) {
			return texturenumber[i];
		}
	}

	return 0;
}

int FindModelID(char *p) {

	int i;
	for (i = 0; i < modelcount; i++) {

		if (strcmp(p, modelname[i].name) == 0) {
			return modelname[i].num;
		}
	}

	return 0;
}

void SetMap(Map_ptr edmap) {
	int i, j;

	NUM_QUADS = edmap->num_poly;

	for (j = 0; j < NUM_QUADS; j++) {
		for (i = 0; i < 4; i++) {
			mx[i][j] = edmap->mx[i][j];
			my[i][j] = edmap->my[i][j];
			mz[i][j] = edmap->mz[i][j];
			mw[i][j] = edmap->mw[i][j];

			//			debug_me(mx[i][j],my[i][j],mz[i][j]);
		}
		poly_texture_list[j] = edmap->texture[j];
	}
}
