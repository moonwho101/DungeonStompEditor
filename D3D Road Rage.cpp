
// D3D Road Rage version 1.0
// Written by William Chin
// Developed in MSDEV C++ v4.2 with DirectX 5 SDK


//#define D3D_OVERLOADS
//#include <math.h>

#include <d3d.h>
//#include "d3ddemo.h"
#include "d3dmain.h"
#include <ddraw.h>
#include <stdio.h>
#include <math.h>
#include "matrix.h"
//#include "d3ddemo.h"
#include "world.h"

// This program allows for a maximum of 1000 quads
// which equates to 2000 triagles and 6000 vertices

//#define MAX_NUM_QUADS		2000
//#define MAX_NUM_TRIANGLES	4000
//#define MAX_NUM_VERTICES	12000
//#define MAX_NUM_TEXTURES	50

#define MAX_NUM_QUADS		22000
#define MAX_NUM_TRIANGLES	24000
#define MAX_NUM_VERTICES	22000
#define MAX_NUM_TEXTURES	350


 
D3DLVERTEX src_v[MAX_NUM_VERTICES];
LPDIRECT3DMATERIAL2 lpBmat, lpMat1[MAX_NUM_TEXTURES];

DWORD hTex[MAX_NUM_TEXTURES];
DWORD hMat[MAX_NUM_TEXTURES];

void debug_me(float a,float b,float c) ;

static const float pi = 3.141592654f;
static const float piover2 = (pi / 2.0f);

D3DMATRIX trans, roty;
D3DMATRIX WorldMatrix, ViewMatrix, ProjMatrix;

void HandleUserInput(void);
D3DVALUE posx, posy, posz; //Player position
D3DVALUE angx, angy, angz; //Player angles
D3DVECTOR from, at, up; // Used to set view matrix
D3DVALUE Z_NEAR = D3DVAL(6.0);
D3DVALUE Z_FAR = D3DVAL(10000.0);
D3DVALUE FOV = D3DVAL(piover2);

float sin_table[361];
float cos_table[361];

//////////////////////////////////////////////////

// int world_list[64][64][20];
world_ptr wptr;
float car_z = (float)0.0, car2_z = (float)0.0;

//////////////////////////////////////////////////
int cell_flag =0;
int NUM_QUADS=0;//95;
int NUM_TRIANGLES;
int NUM_VERTICES;

int Num_Quads;
int Num_Triangles;
int Num_Vertices;

int sx,sy,sz;
int display_poly_flag;
int clip_display_poly_flag;
int num_polys_after_culling;
int num_clipped_polys;
int poly_clip_flags[MAX_NUM_QUADS];
int poly_clip[MAX_NUM_QUADS];

float gradient;
float c;
float inv_gradient;
float ncp_angle;
float view_point_x,view_point_z;
float *buffer2;

int cnt=0;
int i,j;
//int count;
int color;
int num_poly;
int texture_number;
int poly_texture_list[MAX_NUM_QUADS];
int ctext[MAX_NUM_QUADS]; //clipped polygon texture 
int RampSize;
int cell_debug[1000];

float k;

float px[4][MAX_NUM_QUADS],py[4][MAX_NUM_QUADS],pz[4][MAX_NUM_QUADS],pw[4][MAX_NUM_QUADS];
float mx[4][MAX_NUM_QUADS],my[4][MAX_NUM_QUADS],mz[4][MAX_NUM_QUADS],mw[4][MAX_NUM_QUADS];
float cx[4][MAX_NUM_QUADS],cy[4][MAX_NUM_QUADS],cz[4][MAX_NUM_QUADS],cw[4][MAX_NUM_QUADS];

int mlr[4][MAX_NUM_QUADS],mlg[4][MAX_NUM_QUADS],mlb[4][MAX_NUM_QUADS];

RESULTS Rdebug;
MAP map;

int clength;
int poly_cnt;



Map_ptr GetMap(void)
{
	int i,j;
	
	
	map.num_poly=NUM_QUADS;

	for(j=0;j<NUM_QUADS;j++)
	{	
		for(i=0;i<4;i++)
		{	
			map.mx[i][j]=mx[i][j];
			map.my[i][j]=my[i][j];
			map.mz[i][j]=mz[i][j];


			
		}
		map.poly_clip_flags[j]=poly_clip_flags[j];
	}
	return &map;	
}

void SetMap(Map_ptr edmap)
{
	int i,j;
	
	NUM_QUADS = edmap->num_poly;

	for(j=0;j<NUM_QUADS;j++)
	{	
		for(i=0;i<4;i++)
		{	
			mx[i][j]=edmap->mx[i][j];
			my[i][j]=edmap->my[i][j];
			mz[i][j]=edmap->mz[i][j];
			mw[i][j]=edmap->mw[i][j];	

//			debug_me(mx[i][j],my[i][j],mz[i][j]);
		}
		poly_texture_list[j]=edmap->texture[j];
	}

}

void OverrideDefaults(Defaults* defaults)
{
    lstrcpy(defaults->Name, "D3D Road Rage");
    defaults->rs.bPerspCorrect = TRUE;//FALSE;
    defaults->bResizingDisabled = FALSE;
}

BOOL RenderScene(LPDIRECT3DDEVICE2 lpDev, LPDIRECT3DVIEWPORT2 lpView,
            LPD3DRECT lpExtent,CAMERA cam)
{


	return TRUE;

	int i,j,w;
	int cnt=0;
	int angle;
	int last_texture_number;
	int texture_alias_number;
	int ob_type;
	int vert_cnt; //,poly_cnt;
	int num_vert;
	int vert_index;
	int pcount;
	int poly;
	int lit_v;
	int ob_vert_count;
	int cell_x,cell_z;
	int cx,cz;
	int vi,q,ix,iz,icnt;
	int vol_length;
	int vol_x[MAX_NUM_QUADS];
	int vol_z[MAX_NUM_QUADS];
	int verts_per_poly[MAX_NUM_QUADS];
	float x,y,z;
	float wx,wy,wz;
	float sine,cosine;
	float last_rot_angle = (float)0;
	//double ay;
	double k=0.017453292;
	DWORD color_key=0;
	

    D3DCLIPSTATUS status={D3DCLIPSTATUS_EXTENTS2, 0, 2048.0f, 0.0f, 2048.0f, 0.0f, 0.0f, 0.0f};
	
	
	RampSize=cam.ramp_size;
	poly_cnt=0;

	
	for(cx=0;cx<200;cx++)
	{
		for(cz=0;cz<200;cz++)
			wptr->draw_flags[cx][cz]=FALSE;
	}

//	cell_x = (int)(cam.view_point_x / 256);
//	cell_z = (int)(cam.view_point_z / 256);

	cell_x = (int)(cam.view_point_x /256);
	cell_z = (int)(cam.view_point_z /256);


	icnt=0;
	angy = (D3DVALUE)cam.view_angle;
	
	if(angy>=360)
		angy=angy-360;

	if(angy<0)
		angy+=360;

	if((angy >= 315) || (angy <=45)) // 1
	{
		for(ix=-1;ix<=1;ix++)
		{
		vol_x[icnt]   = cell_x + ix, vol_z[icnt]   = cell_z-1;
		vol_x[icnt+1] = cell_x + ix, vol_z[icnt+1] = cell_z;
		vol_x[icnt+2] = cell_x + ix, vol_z[icnt+2] = cell_z+1;

		icnt+=3;
		}
	}

	if((angy < 315) && (angy > 225)) // 2
	{
		for(iz=-1;iz<=1;iz++)
		{
		vol_x[icnt]   = cell_x ,    vol_z[icnt]   = cell_z + iz;
		vol_x[icnt+1] = cell_x - 1, vol_z[icnt+1] = cell_z + iz;
		vol_x[icnt+2] = cell_x - 2, vol_z[icnt+2] = cell_z + iz;

		icnt+=3;
		}
	}

	if((angy <= 225) && (angy >= 135)) // 3
	{
		for(ix=-1;ix<=1;ix++)
		{
		vol_x[icnt]   = cell_x + ix, vol_z[icnt]=cell_z;
		vol_x[icnt+1] = cell_x + ix, vol_z[icnt+1]=cell_z-1;
		vol_x[icnt+2] = cell_x + ix, vol_z[icnt+2]=cell_z-2;

		icnt+=3;
		}
	}

	if((angy > 45) && (angy < 135)) // 4
	{
		for(iz=-1;iz<=1;iz++)
		{
		vol_x[icnt]   = cell_x ,    vol_z[icnt]   = cell_z + iz;
		vol_x[icnt+1] = cell_x + 1, vol_z[icnt+1] = cell_z + iz;
		vol_x[icnt+2] = cell_x + 2, vol_z[icnt+2] = cell_z + iz;

		icnt+=3;
		}
	}

	

	clength = wptr->cell_length[2][3];
			
	for(q=0;q<clength;q++)
	{
		i=wptr->cell[2][3][q];
		cell_debug[q]= i;
	}



	vol_length=9;

	for(vi=0;vi<vol_length;vi++)
	{		
		cell_x=vol_x[vi];
		cell_z=vol_z[vi];
	
		if(wptr->cell[cell_x][cell_z] != NULL)
		{
			if(!(cell_x<0) || (cell_z<0))
			{
				cell_flag =1;
				clength = wptr->cell_length[cell_x][cell_z];
			
				for(q=0;q<clength;q++)
				{
					i=wptr->cell[cell_x][cell_z][q];
				
					//if(wptr->draw_flags[cell_x][cell_z]==FALSE)
					//{
					wptr->draw_flags[cell_x][cell_z]=TRUE;

					wx = wptr->oblist[i].x;
					wy = wptr->oblist[i].y;
					wz = wptr->oblist[i].z;
		
					angle = (int)wptr->oblist[i].rot_angle;

					cosine = cos_table[angle];
					sine   = sin_table[angle];
		
					ob_type = wptr->oblist[i].type;
					lit_v = wptr->oblist[i].light;

					j=0;
					ob_vert_count=0;
					poly=wptr->num_polys_per_object[ob_type];

					for(w=0; w<poly; w++)
					{
						num_vert = wptr->obdata[ob_type].num_vert[w];

						for(vert_cnt=0; vert_cnt < num_vert; vert_cnt++)
						{
							x = wptr->obdata[ob_type].v[j].x;
							y = wptr->obdata[ob_type].v[j].y;
							z = wptr->obdata[ob_type].v[j].z;

							mx[vert_cnt][poly_cnt] = wx + (x*cosine - z*sine); 
							my[vert_cnt][poly_cnt] = wy + y;
							mz[vert_cnt][poly_cnt] = wz + (x*sine + z*cosine);
							j++;
				
							if(ob_vert_count==lit_v)
							{
								mlr[vert_cnt][poly_cnt]=155;
								mlg[vert_cnt][poly_cnt]=100;
								mlb[vert_cnt][poly_cnt]=0;
							}
							else
							{
								mlr[vert_cnt][poly_cnt]=0;
								mlg[vert_cnt][poly_cnt]=0;
								mlb[vert_cnt][poly_cnt]=0;
							}
							ob_vert_count++;
						}	
						ctext[poly_cnt]=wptr->obdata[ob_type].tex[w];
						verts_per_poly[poly_cnt] = wptr->obdata[ob_type].num_vert[w];
						poly_cnt++;
					} // end for w
					pcount+=15;
				
				} // end for q
			} // end if
		} // end if
	} // end for vi
	
	num_polys_after_culling=poly_cnt;

	if(num_polys_after_culling==0)
		return TRUE;

	/////////////////////////////////////////////////

	for(j=0;j<num_polys_after_culling;j++)
    {   
		if(verts_per_poly[j] == 4)
		{
			// Upper Triangle //////////////

			src_v[cnt].x = D3DVAL(mx[0][j]);
			src_v[cnt].y = D3DVAL(my[0][j]);
			src_v[cnt].z = D3DVAL(mz[0][j]);
			src_v[cnt].color = RGBA_MAKE(255, 255, 255, 255);
			src_v[cnt].specular = RGB_MAKE(mlr[0][j],mlg[0][j],mlb[0][j]);
			src_v[cnt].tu = D3DVAL(wptr->TexMap[ctext[j]].tu[0]); // D3DVAL(0.0);
			src_v[cnt].tv = D3DVAL(wptr->TexMap[ctext[j]].tv[0]); // D3DVAL(1.0);
	
			src_v[cnt+1].x = D3DVAL(mx[1][j]);
			src_v[cnt+1].y = D3DVAL(my[1][j]);
			src_v[cnt+1].z = D3DVAL(mz[1][j]);
			src_v[cnt+1].color = RGBA_MAKE(255, 255, 255, 255);
			src_v[cnt+1].specular = RGB_MAKE(mlr[1][j],mlg[1][j],mlb[1][j]);
			src_v[cnt+1].tu = D3DVAL(wptr->TexMap[ctext[j]].tu[1]); // D3DVAL(0.0);
			src_v[cnt+1].tv = D3DVAL(wptr->TexMap[ctext[j]].tv[1]); // D3DVAL(0.0);
		
			src_v[cnt+2].x = D3DVAL(mx[3][j]);
			src_v[cnt+2].y = D3DVAL(my[3][j]);
			src_v[cnt+2].z = D3DVAL(mz[3][j]);
			src_v[cnt+2].color = RGBA_MAKE(255, 255, 255, 255);
			src_v[cnt+2].specular = RGB_MAKE(mlr[2][j],mlg[2][j],mlb[2][j]);
			src_v[cnt+2].tu = D3DVAL(wptr->TexMap[ctext[j]].tu[2]); //D3DVAL(1.0);
			src_v[cnt+2].tv = D3DVAL(wptr->TexMap[ctext[j]].tv[2]); //D3DVAL(1.0);
		
			// Lower Triangle ////////////
		
			src_v[cnt+3].x = D3DVAL(mx[2][j]);
			src_v[cnt+3].y = D3DVAL(my[2][j]);
			src_v[cnt+3].z = D3DVAL(mz[2][j]);
			src_v[cnt+3].color = RGBA_MAKE(255,255,255,255);
			src_v[cnt+3].specular = RGB_MAKE(mlr[3][j],mlg[3][j],mlb[3][j]);
			src_v[cnt+3].tu = D3DVAL(wptr->TexMap[ctext[j]].tu[3]); //D3DVAL(1.0);
			src_v[cnt+3].tv = D3DVAL(wptr->TexMap[ctext[j]].tv[3]); //D3DVAL(0.0);

			cnt+=4;
		}

		if(verts_per_poly[j] == 3)
		{
			// Triangle //////////////

			src_v[cnt].x = D3DVAL(mx[0][j]);
			src_v[cnt].y = D3DVAL(my[0][j]);
			src_v[cnt].z = D3DVAL(mz[0][j]);
			src_v[cnt].color = RGBA_MAKE(255, 255, 255, 255);
			src_v[cnt].specular = RGB_MAKE(mlr[0][j],mlg[0][j],mlb[0][j]);
			src_v[cnt].tu = D3DVAL(0.0);
			src_v[cnt].tv = D3DVAL(0.0);
	
			src_v[cnt+1].x = D3DVAL(mx[1][j]);
			src_v[cnt+1].y = D3DVAL(my[1][j]);
			src_v[cnt+1].z = D3DVAL(mz[1][j]);
			src_v[cnt+1].color = RGBA_MAKE(255, 255, 255, 255);
			src_v[cnt+1].specular = RGB_MAKE(mlr[1][j],mlg[1][j],mlb[1][j]);
			src_v[cnt+1].tu = D3DVAL(1.0);
			src_v[cnt+1].tv = D3DVAL(0.0);
		
			src_v[cnt+2].x = D3DVAL(mx[2][j]);
			src_v[cnt+2].y = D3DVAL(my[2][j]);
			src_v[cnt+2].z = D3DVAL(mz[2][j]);
			src_v[cnt+2].color = RGBA_MAKE(255, 255, 255, 255);
			src_v[cnt+2].specular = RGB_MAKE(mlr[2][j],mlg[2][j],mlb[2][j]);
			src_v[cnt+2].tu = D3DVAL(0.0);
			src_v[cnt+2].tv = D3DVAL(1.0);
		
			cnt+=3;
		}
		

	} // end for j

    car_speed=(float)0;

	
    if (lpDev->BeginScene() != D3D_OK)
        return FALSE;
	
	if (lpDev->SetClipStatus(&status) != D3D_OK)
        return FALSE; 

	k=(float)0.017453292; // pi/180
	angy=(float)cam.view_angle*(float)k;

	// camera location      
	from.x = -cam.view_point_x;
    from.y = -cam.view_point_y;
    from.z = -cam.view_point_z;
	
    WorldMatrix = IdentityMatrix();    
	WorldMatrix = MatrixMult(Translate(from.x,from.y,from.z),WorldMatrix);
	WorldMatrix = MatrixMult(RotateY(angy), WorldMatrix); 
	    
	if (lpDev->SetTransform(D3DTRANSFORMSTATE_WORLD, &WorldMatrix) != D3D_OK)
        return FALSE; 	
	
    ProjMatrix = SetProjectionMatrix(Z_NEAR, Z_FAR, FOV);
    if (lpDev->SetTransform(D3DTRANSFORMSTATE_PROJECTION, &ProjMatrix) != D3D_OK)
        return FALSE; 
    
	if (lpDev->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_NONE) != D3D_OK) 
		return FALSE;
	

	if (lpDev->SetRenderState(D3DRENDERSTATE_COLORKEYENABLE, TRUE) != D3D_OK) 
		return FALSE;	

	///////////////////////////////////////////////////////////////////////

	last_texture_number=1000;
	vert_index=0;

	for(i=0;i<num_polys_after_culling;i++)
	{
		texture_alias_number=ctext[i];
		texture_number = wptr->TexMap[texture_alias_number].texture; 

		if(texture_number!=last_texture_number)
		{
			if (lpDev->SetLightState(D3DLIGHTSTATE_MATERIAL, 
				hMat[texture_number]) != D3D_OK)
			{
				return FALSE; 
			}
		
			if (lpDev->SetRenderState(D3DRENDERSTATE_TEXTUREHANDLE, 
				hTex[texture_number]) != D3D_OK) 
			{
				return FALSE;
			}
		}

		if(verts_per_poly[i]==4)
		{
			if (lpDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 
				D3DVT_LVERTEX, (LPVOID)&src_v[vert_index], 4, 
				NULL) != D3D_OK) 
			{
				return FALSE;
			}
			last_texture_number=texture_number;
			vert_index += 4;

		}

		if(verts_per_poly[i]==3)
		{
			if (lpDev->DrawPrimitive(D3DPT_TRIANGLELIST, 
				D3DVT_LVERTEX, (LPVOID)&src_v[vert_index], 3, 
				NULL) != D3D_OK)
			{
				return FALSE;
			}
			last_texture_number=texture_number;
			vert_index += 3;
		}

	} // end for i

    if (lpDev->GetClipStatus(&status) != D3D_OK)
        return FALSE; 
	
    if (lpDev->EndScene() != D3D_OK)	
        return FALSE;
	
	lpExtent->x1 = (LONG)floor((double)status.minx);
	lpExtent->x2 = (LONG)ceil((double)status.maxx);
	lpExtent->y1 = (LONG)floor((double)status.miny);
	lpExtent->y2 = (LONG)ceil((double)status.maxy);
	
    return TRUE;
}

void ReleaseScene(void)
{
    return;
}

void ReleaseView(LPDIRECT3DVIEWPORT2 lpView)
{
    lpView;
    //RELEASE(lpMat1[10]);
    RELEASE(lpBmat);
}

BOOL InitScene(void)
{
	int i;
	float angle;
	float k=(float)0.017453292;

	wptr=GetWorldData();

	for(i=0;i<360;i++)
	{
		angle = (float)i * k;
		sin_table[i] = sin(angle);
		cos_table[i] = cos(angle);
	}
	return TRUE;
}

BOOL InitView(LPDIRECTDRAW lpDD, LPDIRECT3D2 lpD3D, LPDIRECT3DDEVICE2 lpDev,
	   LPDIRECT3DVIEWPORT2 lpView, int NumTextures,
	   LPD3DTEXTUREHANDLE TextureHandle)
{
    D3DMATERIAL bmat, mat[MAX_NUM_TEXTURES];
    D3DMATERIALHANDLE hBmat, hMat1[MAX_NUM_TEXTURES];
	int i;
	
    if (lpD3D->CreateMaterial(&lpBmat, NULL) != D3D_OK) 
	{
		return FALSE;
    }
    memset(&bmat, 0, sizeof(D3DMATERIAL));
    bmat.dwSize = sizeof(D3DMATERIAL);
    bmat.diffuse.r = (D3DVALUE)1.0;
    bmat.diffuse.g = (D3DVALUE)1.0;
    bmat.diffuse.b = (D3DVALUE)1.0;
    bmat.ambient.r = (D3DVALUE)1.0;
    bmat.ambient.g = (D3DVALUE)1.0;
    bmat.ambient.b = (D3DVALUE)1.0;
    bmat.hTexture = TextureHandle[0];
    bmat.dwRampSize = 1;
    lpBmat->SetMaterial  (&bmat);
    lpBmat->GetHandle    (lpDev, &hBmat);
    lpView->SetBackground(hBmat);
	
	if(NumTextures>MAX_NUM_TEXTURES)
		return FALSE;
	
	for(i=0;i<NumTextures;i++)
	{

		if (lpD3D->CreateMaterial(&lpMat1[i], NULL) != D3D_OK) 
		{	
			return FALSE;
		}
    
		memset(&mat[i], 0, sizeof(D3DMATERIAL));
		mat[i].dwSize = sizeof(D3DMATERIAL);
		mat[i].diffuse.r = (D3DVALUE)1.0;
		mat[i].diffuse.g = (D3DVALUE)1.0;
		mat[i].diffuse.b = (D3DVALUE)1.0;
		mat[i].ambient.r = (D3DVALUE)1.0;
		mat[i].ambient.g = (D3DVALUE)1.0;
		mat[i].ambient.b = (D3DVALUE)1.0;
	#define SPECULAR
	#ifdef SPECULAR
		mat[i].specular.r = (D3DVALUE)1.0;
		mat[i].specular.g = (D3DVALUE)1.0;
		mat[i].specular.b = (D3DVALUE)1.0;
		mat[i].power = (float)40.0;
	#else
		mat[i].specular.r = (D3DVALUE)0.0;
		mat[i].specular.g = (D3DVALUE)0.0;
		mat[i].specular.b = (D3DVALUE)0.0;
		mat[i].power = (float)0.0;
	#endif    
		mat[i].hTexture = TextureHandle[i];
		mat[i].dwRampSize = RampSize;
		lpMat1[i]->SetMaterial(&mat[i]);
		lpMat1[i]->GetHandle  (lpDev, &hMat1[i]);
		hMat[i] = (DWORD) hMat1[i];
		hTex[i] = TextureHandle[i];
    
	} // end for i

	// backgnd.ppm	#0
	wptr->TexMap[0].tu[0] = (float)0.0;
	wptr->TexMap[0].tv[0] = (float)1.0;
	wptr->TexMap[0].tu[1] = (float)0.0;
	wptr->TexMap[0].tv[1] = (float)0.0;
	wptr->TexMap[0].tu[2] = (float)1.0;
	wptr->TexMap[0].tv[2] = (float)1.0;
	wptr->TexMap[0].tu[3] = (float)1.0;
	wptr->TexMap[0].tv[3] = (float)0.0;
	wptr->TexMap[0].texture = 0;
			
	// road2.ppm	#1	
		
	wptr->TexMap[1].tu[0] = (float)0.0;
	wptr->TexMap[1].tv[0] = (float)1.0;
	wptr->TexMap[1].tu[1] = (float)0.0;
	wptr->TexMap[1].tv[1] = (float)0.0;
	wptr->TexMap[1].tu[2] = (float)1.0;
	wptr->TexMap[1].tv[2] = (float)1.0;
	wptr->TexMap[1].tu[3] = (float)1.0;
	wptr->TexMap[1].tv[3] = (float)0.0;
	wptr->TexMap[1].texture = 1;

	// lamppave	#2	- lamp
		
	wptr->TexMap[2].tu[0] = (float)0.0;
	wptr->TexMap[2].tv[0] = (float)0.5;
	wptr->TexMap[2].tu[1] = (float)0.0;
	wptr->TexMap[2].tv[1] = (float)0.0;
	wptr->TexMap[2].tu[2] = (float)0.5;
	wptr->TexMap[2].tv[2] = (float)0.5;
	wptr->TexMap[2].tu[3] = (float)0.5;
	wptr->TexMap[2].tv[3] = (float)0.0;
	wptr->TexMap[2].texture = 2;
	
	// lamppave	#3 - lamp_post
		
	wptr->TexMap[3].tu[0] = (float)0.5;
	wptr->TexMap[3].tv[0] = (float)0.5;
	wptr->TexMap[3].tu[1] = (float)0.5;
	wptr->TexMap[3].tv[1] = (float)0.0;
	wptr->TexMap[3].tu[2] = (float)1.0;
	wptr->TexMap[3].tv[2] = (float)0.5;
	wptr->TexMap[3].tu[3] = (float)1.0;
	wptr->TexMap[3].tv[3] = (float)0.0;
	wptr->TexMap[3].texture = 2;

	// lamppave	#4	- pave
		
	wptr->TexMap[4].tu[0] = (float)0.0;
	wptr->TexMap[4].tv[0] = (float)0.99;

	wptr->TexMap[4].tu[1] = (float)0.0;
	wptr->TexMap[4].tv[1] = (float)0.50;

	wptr->TexMap[4].tu[2] = (float)0.5;
	wptr->TexMap[4].tv[2] = (float)0.99;

	wptr->TexMap[4].tu[3] = (float)0.5;
	wptr->TexMap[4].tv[3] = (float)0.50;

	wptr->TexMap[4].texture = 2;

	// lamppave	#5	- curb
		
	wptr->TexMap[5].tu[0] = (float)0.5;
	wptr->TexMap[5].tv[0] = (float)1.0;
	wptr->TexMap[5].tu[1] = (float)0.5;
	wptr->TexMap[5].tv[1] = (float)0.5;
	wptr->TexMap[5].tu[2] = (float)1.0;
	wptr->TexMap[5].tv[2] = (float)1.0;
	wptr->TexMap[5].tu[3] = (float)1.0;
	wptr->TexMap[5].tv[3] = (float)0.5;
	wptr->TexMap[5].texture = 2;

	// zebra.ppm	#6			
	
	wptr->TexMap[6].tu[0] = (float)0.0;
	wptr->TexMap[6].tv[0] = (float)0.99;
	wptr->TexMap[6].tu[1] = (float)0.0;
	wptr->TexMap[6].tv[1] = (float)0.0;
	wptr->TexMap[6].tu[2] = (float)0.99;
	wptr->TexMap[6].tv[2] = (float)0.99;
	wptr->TexMap[6].tu[3] = (float)0.99;
	wptr->TexMap[6].tv[3] = (float)0.0;
	wptr->TexMap[6].texture = 3;

	// housefnt.ppm	#7	
	
	wptr->TexMap[7].tu[0] = (float)0.0;
	wptr->TexMap[7].tv[0] = (float)1.0;
	wptr->TexMap[7].tu[1] = (float)0.0;
	wptr->TexMap[7].tv[1] = (float)0.0;
	wptr->TexMap[7].tu[2] = (float)1.0;
	wptr->TexMap[7].tv[2] = (float)1.0;
	wptr->TexMap[7].tu[3] = (float)1.0;
	wptr->TexMap[7].tv[3] = (float)0.0;
	wptr->TexMap[7].texture = 4;
	
	// houseside.ppm	#8		
	
	wptr->TexMap[8].tu[0] = (float)0.0;
	wptr->TexMap[8].tv[0] = (float)1.0;
	wptr->TexMap[8].tu[1] = (float)0.0;
	wptr->TexMap[8].tv[1] = (float)0.0;
	wptr->TexMap[8].tu[2] = (float)1.0;
	wptr->TexMap[8].tv[2] = (float)1.0;
	wptr->TexMap[8].tu[3] = (float)1.0;
	wptr->TexMap[8].tv[3] = (float)0.0;
	wptr->TexMap[8].texture = 5;
	
	// pigcarsides.ppm	#9	side 1
	
	wptr->TexMap[9].tu[0] = (float)0.0;
	wptr->TexMap[9].tv[0] = (float)0.5;
	wptr->TexMap[9].tu[1] = (float)0.0;
	wptr->TexMap[9].tv[1] = (float)0.0;
	wptr->TexMap[9].tu[2] = (float)1.0;
	wptr->TexMap[9].tv[2] = (float)0.5;
	wptr->TexMap[9].tu[3] = (float)1.0;
	wptr->TexMap[9].tv[3] = (float)0.0;
	wptr->TexMap[9].texture = 6;

	// pigcarsides.ppm	#10 side 2
	
	wptr->TexMap[10].tu[0] = (float)0.0;
	wptr->TexMap[10].tv[0] = (float)1.0;
	wptr->TexMap[10].tu[1] = (float)0.0;
	wptr->TexMap[10].tv[1] = (float)0.5;
	wptr->TexMap[10].tu[2] = (float)1.0;
	wptr->TexMap[10].tv[2] = (float)1.0;
	wptr->TexMap[10].tu[3] = (float)1.0;
	wptr->TexMap[10].tv[3] = (float)0.5;
	wptr->TexMap[10].texture = 6;
	
	// pigcarfront.ppm	#11 - front		
	wptr->TexMap[11].tu[0] = (float)0.0;
	wptr->TexMap[11].tv[0] = (float)1.0;
	wptr->TexMap[11].tu[1] = (float)0.0;
	wptr->TexMap[11].tv[1] = (float)0.0;
	wptr->TexMap[11].tu[2] = (float)1.0;
	wptr->TexMap[11].tv[2] = (float)1.0;
	wptr->TexMap[11].tu[3] = (float)1.0;
	wptr->TexMap[11].tv[3] = (float)0.0;
	wptr->TexMap[11].texture = 7;

	// pigcarfront.ppm	#12 - back		
	wptr->TexMap[12].tu[0] = (float)0.0;
	wptr->TexMap[12].tv[0] = (float)1.0;
	wptr->TexMap[12].tu[1] = (float)0.0;
	wptr->TexMap[12].tv[1] = (float)0.0;
	wptr->TexMap[12].tu[2] = (float)1.0;
	wptr->TexMap[12].tv[2] = (float)1.0;
	wptr->TexMap[12].tu[3] = (float)1.0;
	wptr->TexMap[12].tv[3] = (float)0.0;
	wptr->TexMap[12].texture = 7;

	// pigcarbonnet.ppm	#13

	wptr->TexMap[13].tu[0] = (float)0.0;
	wptr->TexMap[13].tv[0] = (float)1.0;
	wptr->TexMap[13].tu[1] = (float)0.0;
	wptr->TexMap[13].tv[1] = (float)0.0;
	wptr->TexMap[13].tu[2] = (float)1.0;
	wptr->TexMap[13].tv[2] = (float)1.0;
	wptr->TexMap[13].tu[3] = (float)1.0;
	wptr->TexMap[13].tv[3] = (float)0.0;
	wptr->TexMap[13].texture = 8;
	
	// pigcarwindow.ppm	#14
		
	wptr->TexMap[14].tu[0] = (float)0.0;
	wptr->TexMap[14].tv[0] = (float)1.0;
	wptr->TexMap[14].tu[1] = (float)0.0;
	wptr->TexMap[14].tv[1] = (float)0.0;
	wptr->TexMap[14].tu[2] = (float)1.0;
	wptr->TexMap[14].tv[2] = (float)1.0;
	wptr->TexMap[14].tu[3] = (float)1.0;
	wptr->TexMap[14].tv[3] = (float)0.0;
	wptr->TexMap[14].texture = 9;
	
	// wheel.ppm	#15			

	wptr->TexMap[15].tu[0] = (float)0.0;
	wptr->TexMap[15].tv[0] = (float)1.0;
	wptr->TexMap[15].tu[1] = (float)0.0;
	wptr->TexMap[15].tv[1] = (float)0.0;
	wptr->TexMap[15].tu[2] = (float)1.0;
	wptr->TexMap[15].tv[2] = (float)1.0;
	wptr->TexMap[15].tu[3] = (float)1.0;
	wptr->TexMap[15].tv[3] = (float)0.0;
	wptr->TexMap[15].texture = 10;

	// roadsign.ppm	#16	
	
	wptr->TexMap[16].tu[0] = (float)0.0;
	wptr->TexMap[16].tv[0] = (float)1.0;
	wptr->TexMap[16].tu[1] = (float)0.0;
	wptr->TexMap[16].tv[1] = (float)0.0;
	wptr->TexMap[16].tu[2] = (float)1.0;
	wptr->TexMap[16].tv[2] = (float)1.0;
	wptr->TexMap[16].tu[3] = (float)1.0;
	wptr->TexMap[16].tv[3] = (float)0.0;
	wptr->TexMap[16].texture = 11;
	
	// island.ppm - island_side	#17
	
	wptr->TexMap[17].tu[0] = (float)0.0;
	wptr->TexMap[17].tv[0] = (float)1.0;
	wptr->TexMap[17].tu[1] = (float)0.0;
	wptr->TexMap[17].tv[1] = (float)0.0;
	wptr->TexMap[17].tu[2] = (float)0.5;
	wptr->TexMap[17].tv[2] = (float)1.0;
	wptr->TexMap[17].tu[3] = (float)0.5;
	wptr->TexMap[17].tv[3] = (float)0.0;
	wptr->TexMap[17].texture = 12;
	
	// island.ppm - island_front	#18	

	wptr->TexMap[18].tu[0] = (float)0.5;
	wptr->TexMap[18].tv[0] = (float)1.0;
	wptr->TexMap[18].tu[1] = (float)0.5;
	wptr->TexMap[18].tv[1] = (float)0.0;
	wptr->TexMap[18].tu[2] = (float)1.0;
	wptr->TexMap[18].tv[2] = (float)1.0;
	wptr->TexMap[18].tu[3] = (float)1.0;
	wptr->TexMap[18].tv[3] = (float)0.0;
	wptr->TexMap[18].texture = 12;

	// petrol.ppm - petrol_front	#19	

	wptr->TexMap[19].tu[0] = (float)0.0;
	wptr->TexMap[19].tv[0] = (float)1.0;
	wptr->TexMap[19].tu[1] = (float)0.0;
	wptr->TexMap[19].tv[1] = (float)0.0;
	wptr->TexMap[19].tu[2] = (float)1.0;
	wptr->TexMap[19].tv[2] = (float)1.0;
	wptr->TexMap[19].tu[3] = (float)1.0;
	wptr->TexMap[19].tv[3] = (float)0.0;
	wptr->TexMap[19].texture = 13;
	
	// petrol2.ppm - petrol_front	#20	
/*
	wptr->TexMap[20].tu[0] = (float)0.0;
	wptr->TexMap[20].tv[0] = (float)1.0;
	wptr->TexMap[20].tu[1] = (float)0.0;
	wptr->TexMap[20].tv[1] = (float)0.5;
	wptr->TexMap[20].tu[2] = (float)1.0;
	wptr->TexMap[20].tv[2] = (float)1.0;
	wptr->TexMap[20].tu[3] = (float)1.0;
	wptr->TexMap[20].tv[3] = (float)0.5;
*/
	wptr->TexMap[20].tu[0] = (float)0.0;
	wptr->TexMap[20].tv[0] = (float)0.5;
	wptr->TexMap[20].tu[1] = (float)0.0;
	wptr->TexMap[20].tv[1] = (float)0.0;
	wptr->TexMap[20].tu[2] = (float)1.0;
	wptr->TexMap[20].tv[2] = (float)0.5;
	wptr->TexMap[20].tu[3] = (float)1.0;
	wptr->TexMap[20].tv[3] = (float)0.0;
	wptr->TexMap[20].texture = 14;

	wptr->TexMap[21].tu[0] = (float)0.0;
	wptr->TexMap[21].tv[0] = (float)1.0;
	wptr->TexMap[21].tu[1] = (float)0.0;
	wptr->TexMap[21].tv[1] = (float)0.0;
	wptr->TexMap[21].tu[2] = (float)0.5;
	wptr->TexMap[21].tv[2] = (float)1.0;
	wptr->TexMap[21].tu[3] = (float)0.5;
	wptr->TexMap[21].tv[3] = (float)0.0;
	wptr->TexMap[21].texture = 15;

	wptr->TexMap[22].tu[0] = (float)0.0;
	wptr->TexMap[22].tv[0] = (float)1.0;
	wptr->TexMap[22].tu[1] = (float)0.0;
	wptr->TexMap[22].tv[1] = (float)0.0;
	wptr->TexMap[22].tu[2] = (float)1.0;
	wptr->TexMap[22].tv[2] = (float)1.0;
	wptr->TexMap[22].tu[3] = (float)1.0;
	wptr->TexMap[22].tv[3] = (float)0.0;
	wptr->TexMap[22].texture = 16;
	
    return TRUE;
}



