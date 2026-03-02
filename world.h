
#ifndef __WORLD_H__
#define __WORLD_H__

#include <windows.h>
#include <windowsx.h>
#include "d3dapp.h"

#define OBJECT_ID_STRAIGHT_ROAD 0
#define OBJECT_ID_LEFTCURVE_ROAD 1
#define OBJECT_ID_RIGHTCURVE_ROAD 2
#define OBJECT_ID_TJUNCTION 3
#define OBJECT_ID_CROSSROADS 4
#define OBJECT_ID_ROUNDABOUT 5
#define OBJECT_ID_LAMP_POST 6
#define OBJECT_ID_LAMP 7
#define OBJECT_ID_HOUSE 8
#define OBJECT_ID_POLICE_CAR 9
#define OBJECT_ID_ROAD_SIGN 10
#define OBJECT_ID_SMALL_STRAIGHT_ROAD 11
#define OBJECT_ID_LEFTCORNER_ROAD 12
#define OBJECT_ID_RIGHTCORNER_ROAD 13
#define OBJECT_ID_LEFT_TJUNCTION 14
#define OBJECT_ID_RIGHT_TJUNCTION 15
#define OBJECT_ID_ZEBRA 16
#define OBJECT_ID_ISLAND 17
#define OBJECT_ID_HOUSE_END 18
#define OBJECT_ID_PETROL_STATION 19
#define OBJECT_ID_TRAFFIC_LIGHT 20
#define OBJECT_ID_SLOPE 21
#define OBJECT_ID_SLOPE_CORNER 22
#define OBJECT_ID_CENTRAL_RESERVATION 23
// #define OBJECT_ID_MBSHOP				24
// #define OBJECT_ID_WALL1					33
// #define OBJECT_ID_FLOOR1					32
// #define OBJECT_ID_MONSTER1					27
// #define OBJECT_ID_STRUCTURE1				36
// #define OBJECT_ID_STRUCTURE2				37
// #define OBJECT_ID_STRUCTURE3				38
// #define OBJECT_ID_STRUCTURE4				39
// #define OBJECT_ID_STRUCTURE5				40
// #define OBJECT_ID_STRUCTURE6				41
// #define OBJECT_ID_STRUCTURE7				42
// #define OBJECT_ID_STRUCTURE8				43
// #define OBJECT_ID_STRUCTURE9				44

#define KEYBOARD 1
#define JOYSTICK 2

#define MODE_640_480 1
#define MODE_800_600 2
#define MODE_320_200 3

#define FULLSCREEN_ONLY 1
#define MAXIMISED_WINDOW 2

typedef struct setupinfo_typ {
	D3DAppMode vmode;
	int screen;
	int control;
	BOOL sound;

} SETUPINFO, *setupinfo_ptr;

typedef struct texturemapping_typ {
	float tu[4];
	float tv[4];
	int texture;

} TEXTUREMAPPING, *texturemapping_ptr;

typedef struct vert_typ {
	float x;
	float y;
	float z;

} VERT, *vert_ptr;

typedef struct objectlist_typ {
	float x;
	float y;
	float z;
	float rot_angle;
	int type;
	int light;
	int inactive;
	int monsterid;
	int monstertexture;
	char monstername[255];
	char name[200];
	float rcolour;
	float gcolour;
	float bcolour;

	float dirx;
	float diry;
	float dirz;
	int ltype;
	int ability;
	char ctext[1048];
} OBJECTLIST, *objectlist_ptr;

typedef struct objectdata_typ {
	VERT v[5000];
	VERT connection[4];
	int tex[1000];
	int num_vert[5000];

} OBJECTDATA, *objectdata_ptr;

typedef struct world_typ {
	OBJECTLIST oblist[10000];
	int oblist_length;

	OBJECTDATA obdata[500];
	int num_vert_per_object[900];
	int num_polys_per_object[900];
	int obdata_length;

	int *cell[200][200];
	int cell_length[200][200];
	BOOL draw_flags[200][200];

	TEXTUREMAPPING TexMap[50];

} WORLD, *world_ptr;

#ifdef __cplusplus
extern "C" {
#endif

int LoadObjectData(HWND hwnd, char *filename, world_ptr wptr);
int LoadWorld(HWND hwnd, char *filename, world_ptr wptr);
int CheckObjectId(char *p);
void InitWorldMap(HWND hwnd, world_ptr wptr);
void PrintMessage(HWND hwnd, char *message);
world_ptr GetWorldData();
setupinfo_ptr LoadSetupInfo(HWND hwnd);

#ifdef __cplusplus
};
#endif

#endif // __WORLD_H__
