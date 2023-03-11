#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <winuser.h>			
#include "world.h"
#include "d3dmain.h"
#include "resource.h"
//#include  <mapiwin.h>
#include <math.h>
#define MAX_NUM_OBJECTS_PER_CELL 650
SETUPINFO setupinfo;

HDC hdc;
HWND hwndObjects;
int cell_length[9000];
int cell_xlist[9000][90];
int cell_zlist[9000][90];
int cell_list_debug[9000];


struct TEXNAMES{
char texname[100][1];
} ;

extern "C" TEXNAMES tname[1000];

extern "C" int texturenumber[1000];
extern "C" int texturenumbercount;



struct MODELNAMES{
	char name[100];
	int num;
} ;

extern "C"  MODELNAMES modelname[1000];

extern "C" int modelcount;


extern "C" WORLD edworld;


struct OBJECTNAMES{
char objectname[100][100];
} oname[200];

int objectnumber[1000];
int objectnumbercount;


extern "C" int last_link;
extern "C" int montexid[2000];
extern "C" int LoadMapTextures(char *filename);

int PrintMsgX = 10;
int PrintMsgY = 10;
BOOL start_flag = 0;

void debug_me(char *s,float a,float b,float c) ;
void SavePreCompiledHeader(HWND hwnd, world_ptr wptr) ;
BOOL LoadRRTextures(HWND hwnd, char *filename);
BOOL LoadModels(HWND hwnd, char *filename);    




int LoadObjectData(HWND hwnd,char *filename,world_ptr wptr)
{
	FILE  *fp;    
	char  s[100];
	char  p[100];
	char junk[100];

	float scaleit=0.15f;

	int y_count=30;
	int done=0;
	int object_count=0;
	int vert_count=0;
	int pv_count=0;
	int poly_count=0;
	int texture=0;
	int i;
	int conn_cnt=0;
	int mbshop=0;


// LoadRRTextures(hwnd, "c:\\roadrage2\\roadrage\\RRengine v2_00\\textures.dat");
// LoadModels(hwnd, "c:\\roadrage2\\roadrage\\RRengine v2_00\\modellist.dat");    
LoadRRTextures(hwnd, "textures.dat");
 LoadModels(hwnd, "modellist.dat");    

	HWND hwndList;
	hwndList=GetDlgItem(hwnd,IDC_COMBO1);

						SendMessage(hwndList, CB_RESETCONTENT, 0, 0);
//						SendMessage(hwndList,CB_ADDSTRING,0 , (LPARAM)"LPT1");    
//						SendMessage(hwndList,CB_ADDSTRING,0 , (LPARAM)"LPT2");
//						SendMessage(hwndList,CB_ADDSTRING,0 , (LPARAM)"LPT3");



	BOOL start_flag = TRUE;

    fp = fopen(filename,"r");
	remove ("c:\\r.log");

    if(fp==NULL)
	{     
		MessageBox(hwnd,"Error can't load file",NULL,MB_OK);
		return 1;
    }

	if(start_flag == 0)
	{
		PrintMessage(hwnd, "Loading Object Data");
		start_flag = 1;
	}
	

	while(done==0)
	{
		fscanf( fp, "%s", &s );

		if(strcmp(s,"OBJECT")==0)
		{
			fscanf( fp, "%s", &p );			
//			object_count=atoi(p);

			wptr->num_vert_per_object[object_count]=vert_count;
			wptr->num_polys_per_object[object_count]=poly_count;
			vert_count=0;
			poly_count=0;
			conn_cnt=0;
//new line below

			fscanf( fp, "%s", &junk );
			
			SendMessage(hwndList,CB_ADDSTRING,0 , (LPARAM)&junk);
			
			objectnumber[objectnumbercount] = atoi(p);
			strcpy(oname[objectnumbercount].objectname[0] , junk);
			objectnumbercount++;



			

			object_count = CheckObjectId( (char *)&junk );

			if (object_count=-1)
							object_count=atoi(p);
		}

		if(strcmp(s,"TEXTURE")==0)
		{
			fscanf( fp, "%s", &p );
/*
			if(strcmp(p,"road")==0)
				texture=1;

			if(strcmp(p,"lamp")==0)
				texture=2;
			if(strcmp(p,"lamp_post")==0)
				texture=3;
			if(strcmp(p,"pavement")==0)
				texture=4;
			if(strcmp(p,"curb")==0)
				texture=5;
			if(strcmp(p,"zebra")==0)
				texture=6;

			if(strcmp(p,"house")==0)
				texture=7;
			if(strcmp(p,"houseside")==0)
				texture=8;

			if(strcmp(p,"pigcar_side")==0)
				texture=9;
			if(strcmp(p,"pigcar_side2")==0)
				texture=10;
			if(strcmp(p,"pigcar_front")==0)
				texture=11;
			if(strcmp(p,"pigcar_back")==0)
				texture=12;
			if(strcmp(p,"pigcar_bonnet")==0)
				texture=13;
			if(strcmp(p,"pigcar_window")==0)
				texture=14;
			if(strcmp(p,"wheel")==0)
				texture=15;
			if(strcmp(p,"road_sign")==0)
				texture=16;
			if(strcmp(p,"island_side")==0)
				texture=17;
			if(strcmp(p,"island_front")==0)
				texture=18;
			if(strcmp(p,"petrol")==0)
				texture=19;
			if(strcmp(p,"petrol2")==0)
				texture=20;
			if(strcmp(p,"traffic_light")==0)
				texture=21;
			if(strcmp(p,"grass")==0)
				texture=22;
			if(strcmp(p,"mbshop1")==0) {
				texture=22;
			}
			if(strcmp(p,"floor1")==0) {
				texture=22;
			}
//		texture=22;
*/
			if(strcmp(p,"wall1")==0) {
				texture=22;
			}
			if(texture == 0)
			{
//				MessageBox(hwnd,"Error Bad Texture ID in: LoadObjectList",NULL,MB_OK);
//				return 1;
				texture=22;
			}


				for (int qq=0;qq<texturenumbercount;qq++){
					if (strcmp(tname[qq].texname[0],p)==0) {
						texture=qq+1;
					}
				}


		}

		if(strcmp(s,"TYPE")==0)
		{
			fscanf( fp, "%s", &p );
			//world.oblist[object_count].type=object_count;
		}

		if(strcmp(s,"QUAD")==0)
		{
			for(i=0;i<4;i++)
			{

				//if(mbshop) {



				//scaleit=0.15f;
				//}
				//else {
				//scaleit=1.0f;

				//}

				scaleit = 1.0f;

				fscanf( fp, "%s", &p );
				wptr->obdata[object_count].v[vert_count].x = (float)atof(p)*scaleit;
		
				fscanf( fp, "%s", &p );
				wptr->obdata[object_count].v[vert_count].y = (float)atof(p);
		
				fscanf( fp, "%s", &p );
				wptr->obdata[object_count].v[vert_count].z = (float)atof(p)*scaleit;
		


				vert_count++;
			}
			
			wptr->obdata[object_count].tex[poly_count]=texture;
			wptr->obdata[object_count].num_vert[poly_count]=4;
			poly_count++;


		
		}
		

		scaleit = 1.0f;

		
		if(strcmp(s,"QUADTEX")==0)
		{
			for(i=0;i<4;i++)
			{

				fscanf( fp, "%s", &p );
				wptr->obdata[object_count].v[vert_count].x = (float)atof(p)*scaleit;
		
				fscanf( fp, "%s", &p );
				wptr->obdata[object_count].v[vert_count].y = (float)atof(p);
		
				fscanf( fp, "%s", &p );
				wptr->obdata[object_count].v[vert_count].z = (float)atof(p)*scaleit;
				fscanf( fp, "%s", &p );
				fscanf( fp, "%s", &p );


				vert_count++;
			}
			
			wptr->obdata[object_count].tex[poly_count]=texture;
			wptr->obdata[object_count].num_vert[poly_count]=4;
			poly_count++;


		
		}
		
		if(strcmp(s,"TRITEX")==0)
		{


			for(i=0;i<3;i++)
			{
				fscanf( fp, "%s", &p );
				wptr->obdata[object_count].v[vert_count].x = (float)atof(p);
		
				fscanf( fp, "%s", &p );
				wptr->obdata[object_count].v[vert_count].y = (float)atof(p);
		
				fscanf( fp, "%s", &p );
				wptr->obdata[object_count].v[vert_count].z = (float)atof(p);

				fscanf( fp, "%s", &p );
				fscanf( fp, "%s", &p );
		
				vert_count++;
			}
			wptr->obdata[object_count].tex[poly_count]=texture;
			wptr->obdata[object_count].num_vert[poly_count]=3;
			poly_count++;



		
		}
		
		
		if(strcmp(s,"TRI")==0)
		{
			for(i=0;i<3;i++)
			{
				fscanf( fp, "%s", &p );
				wptr->obdata[object_count].v[vert_count].x = (float)atof(p);
		
				fscanf( fp, "%s", &p );
				wptr->obdata[object_count].v[vert_count].y = (float)atof(p);
		
				fscanf( fp, "%s", &p );
				wptr->obdata[object_count].v[vert_count].z = (float)atof(p);
		
				vert_count++;
			}
			
			wptr->obdata[object_count].tex[poly_count]=texture;
			wptr->obdata[object_count].num_vert[poly_count]=3;
			poly_count++;



		
		}

		if(strcmp(s,"CONNECTION")==0)
		{
			if(conn_cnt<4)
			{
			fscanf( fp, "%s", &p );
			wptr->obdata[object_count].connection[conn_cnt].x = (float)atof(p);
			fscanf( fp, "%s", &p );
			wptr->obdata[object_count].connection[conn_cnt].y = (float)atof(p);
			fscanf( fp, "%s", &p );
			wptr->obdata[object_count].connection[conn_cnt].z = (float)atof(p);
			conn_cnt++;
			}
			else
			{
				fscanf( fp, "%s", &p );
				fscanf( fp, "%s", &p );
				fscanf( fp, "%s", &p );
			}
		}

		if(strcmp(s,"END_FILE")==0)
		{		
			wptr->num_vert_per_object[object_count]=vert_count;
			wptr->num_polys_per_object[object_count]=poly_count;
			wptr->obdata_length = object_count+1;
			done=1;
		}
	}
	fclose(fp);		
	return 0;
}


int LoadWorld(HWND hwnd,char *filename,world_ptr wptr)
{
	FILE  *fp;    
	char  s[81];
	char  p[100];	
	int y_count=30;
	int done=0;
	int object_count=0;
	int vert_count=0;
	int pv_count=0;
	int poly_count=0;
	int object_id;
	BOOL start_flag=TRUE;

	int cell_x;
	int cell_z;
	


	hwndObjects=GetDlgItem(hwnd,IDC_COMBOOBJECT);
	SendMessage(hwndObjects, CB_RESETCONTENT, 0, 0);

	for(cell_z = 0; cell_z < 200; cell_z++)
	{
		for(cell_x = 0; cell_x < 200; cell_x++)
		{

//				wptr->cell[cx][cz] = NULL;			
				wptr->cell[cell_x][cell_z] = NULL;

		}
	}

//last_link=0;




    fp = fopen(filename,"r");
    if(fp==NULL)
    {     
        MessageBox(hwnd,"Error can't load file",NULL,MB_OK);
		return 1;
    }

	PrintMessage(hwnd, "Loading map");
	

	while(done==0)
	{
		fscanf( fp, "%s", &s );

		if(strcmp(s,"OBJECT")==0)
		{
			fscanf( fp, "%s", &p );

			object_id = CheckObjectId( (char *)&p );
			SendMessage(hwndObjects,CB_ADDSTRING,0 , (LPARAM)&p);

			if (object_id==50){

				int wwwaaa=1;
			}
			if(object_id == -1)
			{
//				object_id=atoi(p);			
				MessageBox(hwnd,"Error Bad Object ID in: LoadWorld",NULL,MB_OK);
//				return 1;
			}
			if(start_flag==FALSE)
				object_count++;
			wptr->oblist[object_count].type = object_id;
			strcpy(wptr->oblist[object_count].name ,p);
			strcpy(edworld.oblist[object_count].name ,p);

			
			if (object_id==1){
				int zzz=0;
				zzz=1;

			}
			
			wptr->oblist[object_count].light = -1;
			start_flag=FALSE;
		}

		if(strcmp(s,"CO_ORDINATES")==0)
		{
			fscanf( fp, "%s", &p );
			wptr->oblist[object_count].x = (float)atof(p);
			
			fscanf( fp, "%s", &p );
			wptr->oblist[object_count].y = (float)atof(p);
			
			fscanf( fp, "%s", &p );
			wptr->oblist[object_count].z = (float)atof(p);
		}

		if(strcmp(s,"ROT_ANGLE")==0)
		{
			fscanf( fp, "%s", &p );
			wptr->oblist[object_count].rot_angle = (float)atof(p);
		}

		if(strcmp(s,"LIGHT_ON_VERT")==0)
		{	
			fscanf( fp, "%s", &p );
			wptr->oblist[object_count].light = atoi(p);
		}
		if(strcmp(s,"LIGHT_SOURCE")==0)
		{	
			
//			wptr->oblist[object_count].light = atoi(p);

//				fprintf( fp, "LIGHT_SOURCE Spotlight POS %f %f %f DIR %f %f %f COLOUR %f %f %f\n",

			fscanf( fp, "%s", &p );
			if (strcmp(p,"Spotlight")==0) {
				wptr->oblist[object_count].ltype=0;
			}
			if (strcmp(p,"point")==0) {
				wptr->oblist[object_count].ltype=1;
			}
			if (strcmp(p,"directional")==0) {
				wptr->oblist[object_count].ltype=2;
			}

			fscanf( fp, "%s", &p );
			fscanf( fp, "%s", &p );			
			wptr->oblist[object_count].x=atof(p);

			fscanf( fp, "%s", &p );			
			wptr->oblist[object_count].y=atof(p);

			fscanf( fp, "%s", &p );			
			wptr->oblist[object_count].z=atof(p);

			fscanf( fp, "%s", &p );
			fscanf( fp, "%s", &p );			
			wptr->oblist[object_count].dirx=atof(p);

			fscanf( fp, "%s", &p );			
			wptr->oblist[object_count].diry=atof(p);

			fscanf( fp, "%s", &p );			
			wptr->oblist[object_count].dirz=atof(p);

			fscanf( fp, "%s", &p );
			fscanf( fp, "%s", &p );			
			wptr->oblist[object_count].rcolour=atof(p);

			fscanf( fp, "%s", &p );			
			wptr->oblist[object_count].gcolour=atof(p);
			fscanf( fp, "%s", &p );			
			wptr->oblist[object_count].bcolour=atof(p);



		}

		if(strcmp(s,"END_FILE")==0)
		{
			fscanf( fp, "%s", &p );	
			wptr->oblist_length = object_count+1;
			done=1;
		}
	}
	fclose(fp);		
	return 0;
}

int CheckObjectId(char *p) 
{
/*
	if(strcmp(p,"straight_road")==0)
		return OBJECT_ID_STRAIGHT_ROAD;
		
	if(strcmp(p,"small_straight_road")==0)
		return OBJECT_ID_SMALL_STRAIGHT_ROAD;
		
	if(strcmp(p,"left_curve_road")==0)
		return OBJECT_ID_LEFTCURVE_ROAD;
				
	if(strcmp(p,"right_curve_road")==0)
		return OBJECT_ID_RIGHTCURVE_ROAD;
				
	if(strcmp(p,"t_junction")==0)
		return OBJECT_ID_TJUNCTION;		
				
	if(strcmp(p,"crossroads")==0)
		return OBJECT_ID_CROSSROADS;	
						
	if(strcmp(p,"lamp_post")==0)
		return OBJECT_ID_LAMP_POST;	
			
	if(strcmp(p,"road_sign")==0)
		return OBJECT_ID_ROAD_SIGN;	
			
	if(strcmp(p,"house_semileft")==0)
		return OBJECT_ID_HOUSE;		
	
	if(strcmp(p,"house_semiright")==0)
		return OBJECT_ID_HOUSE_END;		

//	if(strcmp(p,"police_car")==0)
//		return OBJECT_ID_POLICE_CAR;	
					
	if(strcmp(p,"left_corner")==0)
		return OBJECT_ID_LEFTCORNER_ROAD;
					
	if(strcmp(p,"right_corner")==0)
		return OBJECT_ID_RIGHTCORNER_ROAD;
						
	if(strcmp(p,"left_t_junction")==0)
		return OBJECT_ID_LEFT_TJUNCTION;
				
	if(strcmp(p,"right_t_junction")==0)
		return OBJECT_ID_RIGHT_TJUNCTION;			

	if(strcmp(p,"zebra")==0)
		return OBJECT_ID_ZEBRA;

	if(strcmp(p,"island")==0)
		return OBJECT_ID_ISLAND;

	if(strcmp(p,"petrol_station")==0)
		return OBJECT_ID_PETROL_STATION;

	if(strcmp(p,"traffic_light")==0)
		return OBJECT_ID_TRAFFIC_LIGHT;

	if(strcmp(p,"slope")==0)
		return OBJECT_ID_SLOPE;

	if(strcmp(p,"slope_corner")==0)
		return OBJECT_ID_SLOPE_CORNER;
	
//	if(strcmp(p,"central_reservation")==0)
//		return OBJECT_ID_CENTRAL_RESERVATION;
*/

/*	
	if(strcmp(p,"mbshop1")==0)
		return OBJECT_ID_MBSHOP;
	
	if(strcmp(p,"wall1")==0)
		return OBJECT_ID_WALL1;
	if(strcmp(p,"floor1")==0)
		return OBJECT_ID_FLOOR1;

	if(strcmp(p,"monster1")==0)
		return OBJECT_ID_MONSTER1;

	if(strcmp(p,"structure1")==0)
		return OBJECT_ID_STRUCTURE1;

	if(strcmp(p,"structure2")==0)
		return OBJECT_ID_STRUCTURE2;

	if(strcmp(p,"structure3")==0)
		return OBJECT_ID_STRUCTURE3;

	if(strcmp(p,"structure4")==0)
		return OBJECT_ID_STRUCTURE4;

	if(strcmp(p,"structure5")==0)
		return OBJECT_ID_STRUCTURE5;

	if(strcmp(p,"structure6")==0)
		return OBJECT_ID_STRUCTURE6;

	if(strcmp(p,"structure7")==0)
		return OBJECT_ID_STRUCTURE7;

	if(strcmp(p,"structure8")==0)
		return OBJECT_ID_STRUCTURE8;

	if(strcmp(p,"structure9")==0)
		return OBJECT_ID_STRUCTURE9;
*/
char buff[255];
int i=0;
strcpy(buff,p);

				for (i=0;i<objectnumbercount;i++){
					if (strcmp(buff,oname[i].objectname[0])==0) {

//							strcpy(currentobjectname,buff);
							return objectnumber[i];
//							ed.current_object_id=currentobject;
//							sprintf(junk,"%d",ed.current_object_id);
//							result = SetDlgItemText(hwnd,IDC_OBJECT,  junk);

					}
				}



	return -1;
				
}

setupinfo_ptr LoadSetupInfo(HWND hwnd)
{
	FILE  *fp;    
	char  s[100];
	char  p[100];
	char  q[100];
	int done=0;
	BOOL mode_error		= TRUE;
	BOOL screen_error	= TRUE;
	BOOL control_error	= TRUE;
	BOOL sound_error	= TRUE;

	setupinfo.vmode.bpp = 16;
	setupinfo.screen = FULLSCREEN_ONLY;

    fp = fopen("rrsetup.ini","r");
    
	if(fp==NULL)    
	{
		Msg("error in rrsetup.ini");
		return NULL;
	}


	//PrintMessage(hwnd, "Loading rrsetup.ini");

	while(done==0)
	{
		fscanf( fp, "%s", &s );

		if(strcmp(s,"SCREEN")==0)
		{
			fscanf( fp, "%s", &p );
		
			if(strcmp(p,"FULLSCREEN_ONLY")==0)	
			{
				setupinfo.screen = FULLSCREEN_ONLY;
				screen_error = FALSE;
			}
			
			if(strcmp(p,"MAXIMISED_WINDOW")==0)
			{
				setupinfo.screen = MAXIMISED_WINDOW;
				screen_error = FALSE;
			}
			if(screen_error == TRUE)
				return NULL;
			
		}

		if(strcmp(s,"MODE")==0)
		{
			fscanf( fp, "%s", &p );
			fscanf( fp, "%s", &q );	

			if( (strcmp(p,"640")==0) && (strcmp(q,"480")==0) )
			{
				setupinfo.vmode.w = 640;
				setupinfo.vmode.h = 480;
				mode_error=FALSE;	
			}

			if( (strcmp(p,"800")==0) && (strcmp(q,"600")) )
			{
				setupinfo.vmode.w = 800;
				setupinfo.vmode.h = 600;
				mode_error=FALSE;
			}

			if( (strcmp(p,"320")==0) && (strcmp(q,"200")) )
			{
				setupinfo.vmode.w = 320;
				setupinfo.vmode.h = 200;
				mode_error=FALSE;
			}

			if(mode_error == TRUE)
			{
				Msg("error in rrsetup.ini :  Mode");
				return NULL;
			}
		}

		if(strcmp(s,"CONTROL")==0)
		{
			fscanf( fp, "%s", &p );

			if(strcmp(p,"JOYSTICK")==0)
			{
				setupinfo.control = JOYSTICK;
				control_error = FALSE;
			}
			

			if(strcmp(p,"KEYBOARD")==0)
			{
				setupinfo.control = KEYBOARD; 
				control_error = FALSE;
			}
			if(control_error == TRUE)
				return NULL;
		}
		
		if(strcmp(s,"SOUND")==0)
		{
			fscanf( fp, "%s", &p );

			if(strcmp(p,"ON")==0)
			{
				setupinfo.sound = TRUE; // sound On
				sound_error = FALSE;
			}

			if(strcmp(p,"OFF")==0)
			{
				setupinfo.sound = FALSE; // Sound Off
				sound_error = FALSE;
			}
			if(sound_error == TRUE)
				return NULL;
		}

		if(strcmp(s,"END_FILE")==0)
		{
			fscanf( fp, "%s", &p );				
			done=1;
		}
	}
	fclose(fp);	
	return &setupinfo;
}

void InitWorldMap(HWND hwnd, world_ptr wptr)
{
	int i,j,w;
	int cnt=0;
	int angle;
	int ob_type;
	int vert_cnt,poly_cnt;
	int count;
	int poly;
	int length;
	int cx,cz;
	int lit_v;
	int cell_count;
	int cell_x,cell_z;
	int temp[520];
	int mem_count=0;
	int num_vert;
int j2=0;
	int allzero=0;	
	BOOL map[200][200];
	FILE  *fp;  
	FILE  *fp2;  


	FILE  *fpobject;  
	char hit=0;
	char hit2=0;
	float x,y,z;
	float wx,wy,wz;
	float tx,ty,tz,ty2;
	float sine,cosine;
	float last_rot_angle=(float)0;
	float k=(float)0.017453292;
	float sin_table[361];
	float cos_table[361];
	float fangle;
	int countinactive=0;
	int tempslot=0;
	int startobjectdump=0;
	LoadMapTextures("level1.map");
	int qq=0;
	int oldx=-1;
	fp = fopen("debug1.txt","w");

	fp2 = fopen("level1.cmp","w");
    fpobject = fopen("c:\\object.txt","w");
	remove("r.log");

	if(fp==NULL)    
	{
		Msg("Can't debug1.txt file");
		return; 
	}

	for(cell_z = 0; cell_z < 200; cell_z++)
	{
		for(cell_x = 0; cell_x < 200; cell_x++)
		{

//				wptr->cell[cx][cz] = NULL;			
			if (wptr->cell[cell_x][cell_z] != NULL) {
				delete wptr->cell[cell_x][cell_z];
				wptr->cell[cell_x][cell_z] = NULL;

			}else {
				wptr->cell[cell_x][cell_z] = NULL;

			}

		}
	}



	PrintMessage(hwnd, "Processing Map Info");

	fprintf( fp, "%s\n", "object  poly  vert  cell_x  cell_z  ob_type  cell_cnt" );


	for(i=0;i<360;i++)
	{
		fangle = (float)i * k;
		sin_table[i] = sin(fangle);
		cos_table[i] = cos(fangle);
	}
	
	poly_cnt=0;
	j=0;
	j2=0;

	
	for(i=0;i<wptr->oblist_length;i++)
//	for(i=0;i<wptr->oblist_length;i++)
	{	


		fprintf( fp, "\n");	


		wx = wptr->oblist[i].x;
		wy = wptr->oblist[i].y;
		wz = wptr->oblist[i].z;

		
		if (strstr(wptr->oblist[i].name,"!")!=NULL){
//			fprintf( fpobject, "TEXTURE %s\n\n",	tname[montexid[i]].texname   );


				for (qq=0;qq<texturenumbercount;qq++){
					if (texturenumber[qq]==montexid[i]) {
						//fprintf( fpobject, "TEXTURE %s\n\n",	tname[qq].texname[0]   );
					}
				}

		}


		angle = (int)wptr->oblist[i].rot_angle;

		cosine = cos_table[angle];
		sine   = sin_table[angle];
		
		ob_type = wptr->oblist[i].type;

//		wptr->oblist[i].monstertexture
//smacker		
		lit_v = wptr->oblist[i].light;
		
		for(cz=0;cz<200;cz++)
		{
			for(cx=0;cx<200;cx++)
				map[cx][cz] = FALSE;
		}

		j=0;
		j2=0;
		cell_count=0;
		cell_length[i] = cell_count;
		poly=wptr->num_polys_per_object[ob_type];
//		poly=12;
			hit2=0;
			hit=0;
//	debug_me("NEW POLYGON  TYPE",ob_type,0,0);
		for(w=0; w<poly; w++)

		{



			num_vert = wptr->obdata[ob_type].num_vert[w];
//			num_vert=4;

			if (strstr(wptr->oblist[i].name,"!")==NULL){
				for (qq=0;qq<texturenumbercount;qq++){
					if (texturenumber[qq]==wptr->obdata[ob_type].tex[w]) {
						//fprintf( fpobject, "TEXTURE %s\n\n",	tname[qq].texname[0]   );
					}
				}
			}
			allzero=0;
			hit=0;
			for(vert_cnt=0; vert_cnt < num_vert; vert_cnt++)
			{

				y = wptr->obdata[ob_type].v[j2].y;
				ty2 = wy + y;
				j2++;


//				if (ty2 > 0) {
//					allzero=1;
//				}else {
//					hit++;
//				}


			
			
			
//			y = wptr->obdata[ob_type].
			
			
				if (ty2 ==0 ) {
				hit++;

				}
					
 
				if (ty2 ==1 ) {
				hit++;

				}

				if (ty2 ==2 ) {
				hit++;

				}

				

			
			
			
			}

			startobjectdump=0;
			for(vert_cnt=0; vert_cnt < num_vert; vert_cnt++)
			{
				x = wptr->obdata[ob_type].v[j].x; // CRASHED HERE
				y = wptr->obdata[ob_type].v[j].y;
				z = wptr->obdata[ob_type].v[j].z;
				




			
				
				j++;

				tx = wx + (x*cosine - z*sine); 
				ty = wy + y;
				tz = wz + (x*sine + z*cosine);





				if (num_vert==4) {


					if (startobjectdump==0) {
///						fprintf( fpobject, "QUAD ");
//						fprintf( fpobject, "TEXTURE %d\n",wptr->obdata[ob_type].tex[j]);
						startobjectdump=1;
					}
					//fprintf( fpobject, "%f %f %f\n",tx-780.0f,ty,tz-780.0f);


				}
				if (num_vert==3) {


					if (startobjectdump==0) {
//						fprintf( fpobject, "TRI ");
//						fprintf( fpobject, "TEXTURE %d ",wptr->obdata[ob_type].tex[j]);
						startobjectdump=1;
					}
//					fprintf( fpobject, "%f %f %f\n",tx-780.0f,ty,tz-780.0f);


				}
//				if (oldx==tx)
//					allzero=1;

//				oldx = tx;


//				if (ty==0 && allzero==0 && hit2<4) {
				if (hit==4  && i<wptr->oblist_length) {

					
// righthere
					
//				debug_me("TX",tx* (float) 5,0,0);
//				debug_me("TY",ty,0,0);
//				debug_me("TZ",tz* (float) 5,0,0);
				hit2++;
				}

//				if (i>wptr->oblist_length) {
//					int init=0;
//					init=1;
//
//				}

				cell_x = (int)(tx/(float)256);
				cell_z = (int)(tz/(float)256);

				

				if(map[cell_x][cell_z] == FALSE)
				{
					if(cell_count<MAX_NUM_OBJECTS_PER_CELL)
					{
						fprintf( fp, "%d", i );							// object
						fprintf( fp, "%s%d",  "        ", w );			// poly
						fprintf( fp, "%s%d",  "      ", vert_cnt );		// vert
						fprintf( fp, "%s%d",  "      ", cell_x );		// cell_x
						fprintf( fp, "%s%d",  "        ", cell_z );		// cell_y
						fprintf( fp, "%s%d",  "        ", ob_type );	// ob_type
						fprintf( fp, "%s%d",  "         ", cell_count );	// cell_count

						fprintf( fp, "\n");	

						cell_xlist[i][cell_count] = cell_x;
						cell_zlist[i][cell_count] = cell_z;

//						debug_me(cell_x,0,cell_z);				
						cell_count++;
						cell_length[i] = cell_count;
						map[cell_x][cell_z] = TRUE;
					}
				}
				
//			if (hit==4)
//				hit2=1;

		
			poly_cnt++;

			

			}
//			debug_me("----------------",0,0,0);


//			fprintf( fpobject, "\n");

		} // end for w
   

	} // end for i
	
	//////////////////////////////////////////////////////
	
	PrintMessage(hwnd, "Map size : 200 x 200 cells");
	PrintMessage(hwnd, "Loading map data into cells");
	fprintf( fp, "\n\n");	

//	for(cz=0;cz<200;cz++)
//	{
//		for(cx=0;cx<200;cx++)


	for(cz=0;cz<80;cz++)
	{
		for(cx=0;cx<80;cx++)
		{
			count=0;
			countinactive=0;

			for(i=0;i<=wptr->oblist_length;i++)
			{


				length = cell_length[i];
				if (wptr->oblist[i].inactive==1)
					countinactive++;
				
				for(j=0;j<length;j++)
				{
					if(cell_xlist[i][j] == cx)
					{
						if(cell_zlist[i][j] == cz)
						{

							if(count<MAX_NUM_OBJECTS_PER_CELL && wptr->oblist[i].inactive==0 )
							{
								temp[count]=i-countinactive;
								if((cx==3) && (cz==3))
									cell_list_debug[count]=i;
								count++;
							}
						}
					}
				}
			}

			if( (cx==0) && (cz==0) )
				fprintf( fp, "%s%d\n",  "(cx=0, cz=0) count= ", count );	

			if( (cx==0) && (cz==1) )
				fprintf( fp, "%s%d\n",  "(cx=0, cz=1) count = ", count );	

			if( (cx==0) && (cz==2) )
				fprintf( fp, "%s%d\n",  "(cx=0, cz=2) count = ", count );	

			if( (cx==0) && (cz==3) )
				fprintf( fp, "%s%d\n",  "(cx=0, cz=3) count = ", count );	


			if(count>0)
			{
				wptr->cell[cx][cz] = new int[count];
				mem_count = mem_count + (count* sizeof(int));
		fprintf( fp2, "%d %d %d ",cx,cz,count);				
				for(w=0;w<count-1;w++)
				{
					wptr->cell[cx][cz][w] = temp[w];

				
					fprintf( fp2, "%d ",temp[w]);				
				}
				wptr->cell_length[cx][cz] = count;
			fprintf( fp2, "%d\n",temp[w]);
			}
			else {

				if (wptr->oblist[i].inactive == 0)
					wptr->cell[cx][cz] = NULL;

			}
		}


	}

//	for(i=0; i<16; i++)
//		fprintf( fp, "%s%d%s%d\n", "cell[0,0] = ", i, "    ", wptr->cell[0][0][i] );
	
	fprintf( fp, "\n\n");
	fprintf( fp, "%d\n\n",mem_count);
	fprintf( fp2, "END_FILE\n");				
	
	fclose(fp2);
//	fclose(fpobject);



}

void PrintMessage(HWND hwnd,char *message)
{

	return;

	hdc=GetDC(hwnd);
	TextOut(hdc,PrintMsgX,PrintMsgY, message,strlen(message));
	PrintMsgY +=20;
	ReleaseDC(hwnd,hdc); 
}





void debug_me(char *s,float a,float b,float c) {
	
	FILE *fp;
//	CTime t;
//	t = CTime::GetCurrentTime();
	if ((fp=fopen("r.log","a")) == NULL) {
	}

	fprintf(fp,"%-4f\n",a);
	fclose(fp);
}


void SavePreCompiledHeader(HWND hwnd, world_ptr wptr) {

	FILE  *fp2;  
	int cx=0,cz=0;
	int count=0,i=0,j=0;
	int length=0;
	int temp[520];
	int mem_count=0,w=0;

	fp2 = fopen("level1.cmp","w");

	for(cz=0;cz<200;cz++)
	{
		for(cx=0;cx<200;cx++)
		{
			count=0;

			for(i=0;i<=wptr->oblist_length;i++)
			{
				length = cell_length[i];

				for(j=0;j<length;j++)
				{
					if(cell_xlist[i][j] == cx)
					{
						if(cell_zlist[i][j] == cz)
						{
							if(count<MAX_NUM_OBJECTS_PER_CELL)
							{
							temp[count]=i;
							if((cx==3) && (cz==3))
								cell_list_debug[count]=i;
							count++;
							}
						}
					}
				}
			}


			if(count>0)
			{
				wptr->cell[cx][cz] = new int[count];
				mem_count = mem_count + (count* sizeof(int));
		fprintf( fp2, "%d %d %d ",cx,cz,count);				
				for(w=0;w<count-1;w++)
				{
					wptr->cell[cx][cz][w] = temp[w];

				
					fprintf( fp2, "%d ",temp[w]);				
				}
				wptr->cell_length[cx][cz] = count;
			fprintf( fp2, "%d\n",temp[w]);
			}
			else
				wptr->cell[cx][cz] = NULL;
		}
	}

//	for(i=0; i<16; i++)
//		fprintf( fp, "%s%d%s%d\n", "cell[0,0] = ", i, "    ", wptr->cell[0][0][i] );
	
//	fprintf( fp, "\n\n");
//	fprintf( fp, "%d\n\n",mem_count);
	fprintf( fp2, "END_FILE\n");				
	
	fclose(fp2);


}









BOOL LoadRRTextures(HWND hwnd, char *filename)
{    
	FILE *fp;    
	char s[256];
	char p[256];

	int y_count=30;
	int done=0;
	int object_count=0;
	int vert_count=0;
	int pv_count=0;
	int poly_count=0;
	int tex_alias_counter = 0;
	int tex_counter = 0;
	int i;
	BOOL start_flag=TRUE;
	BOOL found;
	LPDIRECTDRAWSURFACE7 lpDDsurface;
	objectnumbercount=0;


    fp = fopen(filename,"r");
    if(fp==NULL)
    {     
		return FALSE;
    }

	HWND hwndList;
	hwndList=GetDlgItem(hwnd,IDC_COMBO2);


						SendMessage(hwndList, CB_RESETCONTENT, 0, 0);
//						SendMessage(hwndList,CB_ADDSTRING,0 , (LPARAM)"LPT1");    



	while(done==0)
	{
		found = FALSE;
		fscanf( fp, "%s", &s );

		if(strcmp(s,"AddTexture")==0)
		{
			fscanf( fp, "%s", &p );
			
			tex_counter++;
			found = TRUE;		
		}

		if(strcmp(s,"Alias")==0)
		{
			fscanf( fp, "%s", &p );
			texturenumber[texturenumbercount]=atoi(p);


			fscanf( fp, "%s", &p );
			strcpy(tname[texturenumbercount].texname[0] , p);
			texturenumbercount++;
			SendMessage(hwndList,CB_ADDSTRING,0 , (LPARAM)&p);
					   			
			fscanf( fp, "%s", &p );			
			fscanf(fp, "%s", &p);

			if(strcmp(p,"WHOLE")==0)
			{

			}

			if(strcmp(p,"TL_QUAD")==0)
			{
			}

			if(strcmp(p,"TR_QUAD")==0)
			{
			}
			if(strcmp(p,"LL_QUAD")==0)
			{
			}
			if(strcmp(p,"LR_QUAD")==0)
			{
			}
			if(strcmp(p,"TOP_HALF")==0)
			{
			}
			if(strcmp(p,"BOT_HALF")==0)
			{
			}
			if(strcmp(p,"LEFT_HALF")==0)
			{
			}
			if(strcmp(p,"RIGHT_HALF")==0)
			{
			}
			if(strcmp(p,"TL_TRI")==0)
			{
			}
			if(strcmp(p,"BR_TRI")==0)
			{

			}

			//texture type
			fscanf(fp, "%s", &p);

			
			tex_alias_counter++;
			found = TRUE;
		}

		if(strcmp(s,"END_FILE")==0)
		{
			found = TRUE;
			done = 1;
		}

		if(found == FALSE)
		{
			return FALSE;
		}
			
		
	}
	fclose(fp);	
	

	return TRUE;
}


BOOL LoadModels(HWND hwnd, char *filename)
{    
	FILE *fp;    
	FILE *fp2;    
	char p[256];
	int done = 0;
	int i;
	char command[256];
	float f;
	int model_id;
	//char model_name[256];
	char model_filename[256];
	int model_tex_alias;
	char model_texture[256];

	float scale;
	int  Q2M_Anim_Counter = 0;
	BOOL command_recognised;	


	fp = fopen(filename,"r");

    if(fp==NULL)
	{     
		return FALSE;
    }

	HWND hwndList;
	hwndList=GetDlgItem(hwnd,IDC_COMBO3);


	SendMessage(hwndList, CB_RESETCONTENT, 0, 0);


	fscanf( fp, "%s", &command );

	if(strcmp(command,"FILENAME") == 0)
		fscanf( fp, "%s", &p );	// ignore comment
	else
		return FALSE;

	while(done == 0)
	{
		command_recognised = FALSE;
		scale = (float)0.4;

		fscanf( fp, "%s", &command ); // get next command

		
		fscanf( fp, "%s", &p );	// read player number

			
		fscanf( fp, "%s", &p );	// find model file foramt, MD2 or 3DS ?
		
		fscanf( fp, "%s", &p );	//  ignore comment "model_id"  

		fscanf( fp, "%s", &p );	// read model id
		model_id = atoi(p);
			
		fscanf( fp, "%s", &model_filename ); // read filename for object

		fscanf( fp, "%s", &p );	//  ignore comment "tex"  

		fscanf( fp, "%s", &p );	// read texture alias id				

		
				
		
	//		model_tex_alias = atoi(p);		


		if(strcmp(command,"PLAYER") == 0)
		{
			fscanf( fp, "%s", &p ); // ignore comment pos
					
			fscanf( fp, "%s", &p ); // x pos
//			pCMyApp->player_list[type_num].x = (float)atoi(p);
					
			fscanf( fp, "%s", &p ); // y pos
//			pCMyApp->player_list[type_num].y = (float)atoi(p);
					
			fscanf( fp, "%s", &p ); // z pos
//			pCMyApp->player_list[type_num].z = (float)atoi(p);
					
			fscanf( fp, "%s", &p ); // ignore comment angle
			fscanf( fp, "%s", &p ); // angle
//			pCMyApp->player_list[type_num].rot_angle = (float)atoi(p);

//			pCMyApp->player_list[type_num].model_id = model_id;
//			pCMyApp->player_list[type_num].skin_tex_id = model_tex_alias;
//			pCMyApp->num_players++;

			fscanf( fp, "%s", &p ); // ignore comment scale
			fscanf( fp, "%f", &f );
			scale = f;

			fscanf( fp, "%s", &p ); // Don't draw players external weapon
//			if(strcmp(p, "NO_EXTERNAL_WEP") == 0)
//				pCMyApp->player_list[type_num].draw_external_wep = FALSE;
			
//			if(strcmp(p, "USE_EXTERNAL_WEP") == 0)
//				pCMyApp->player_list[type_num].draw_external_wep = TRUE;
			


			


			
			
			fscanf( fp, "%s", &p );


			//ac
			fscanf( fp, "%s", &p );
			fscanf( fp, "%s", &p );

			//hd
			fscanf( fp, "%s", &p );
			fscanf( fp, "%s", &p );

			//damage
			fscanf( fp, "%s", &p );
			fscanf( fp, "%s", &p );

			//thaco
			fscanf( fp, "%s", &p );
			fscanf( fp, "%s", &p );





	
			//name
			fscanf( fp, "%s", &p );	//  
			SendMessage(hwndList,CB_ADDSTRING,0 , (LPARAM)&p);    			
			//speed

			strcpy(modelname[modelcount].name,p);
			modelname[modelcount++].num=model_id;
			fscanf( fp, "%s", &p );	//  



			fscanf( fp, "%s", &p );	//  

			command_recognised = TRUE;

		}




		if(strcmp(command,"YOURGUN") == 0)
		{
			fscanf( fp, "%s", &p ); // ignore comment pos
				
			fscanf( fp, "%s", &p ); // x pos

					
			fscanf( fp, "%s", &p ); // y pos

				
			fscanf( fp, "%s", &p ); // z pos

					
			fscanf( fp, "%s", &p ); // ignore comment angle
			fscanf( fp, "%s", &p ); // angle


			fscanf( fp, "%s", &p ); // ignore comment scale
			fscanf( fp, "%f", &f );



			fscanf( fp, "%s", &p ); // ignore comment scale
			fscanf( fp, "%s", &p ); // ignore comment scale


			fscanf( fp, "%s", &p ); // ignore comment scale
			fscanf( fp, "%s", &p ); // ignore comment scale



			fscanf( fp, "%s", &p ); // ignore comment scale
			fscanf( fp, "%s", &p ); // ignore comment scale


			

			fscanf( fp, "%s", &p ); // ignore comment scale

			SendMessage(hwndList,CB_ADDSTRING,0 , (LPARAM)&p);    			
			strcpy(modelname[modelcount].name,p);
			modelname[modelcount++].num=model_id;
			fscanf( fp, "%s", &p ); // ignore comment scale


			command_recognised = TRUE;
		}

		
		if(strcmp(command,"END_FILE") == 0)
		{
			done = 1;
			command_recognised = TRUE;
		}

		if(command_recognised == TRUE)
		{
		}
		else
		{

			MessageBox(hwnd, command, "command unrecognised",MB_OK);
			return FALSE;
		}
		

	} // end while loop

	fclose(fp);	
	return TRUE;

}


