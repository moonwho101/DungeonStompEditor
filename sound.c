

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <dsound.h>
#include "dsutil.h"
#include "sound.h"
#include "world.h"

#define UNSET 0
#define SET 1

HDC hdc;
HRESULT hr;
LPDIRECTSOUND lpDS;
LPDIRECTSOUNDBUFFER lpDSBStreamBuffer = NULL;
LPWAVEFORMATEX lpwfxFormat;
IDirectSoundBuffer *pDSB;

HSNDOBJ hsoEngine = NULL;
HSNDOBJ hsoSkid = NULL;

BOOL sound_intialised_flag = UNSET;

void SetFrequency(HWND hwnd, int frequency) {
	return;
	if (sound_intialised_flag == UNSET)
		return;

	SndObjSetFrequency(hsoEngine, frequency);
}

void InitialiseSound(HWND hwnd) {

	return;
	if (sound_intialised_flag == SET)
		return;
	else
		sound_intialised_flag = SET;

	hdc = GetDC(hwnd);

	if (!SUCCEEDED(DirectSoundCreate(NULL, &lpDS, NULL))) {
		TextOut(hdc, 10, 10, "Create Failed", 13);
		return;
	}

	if (!SUCCEEDED(lpDS->lpVtbl->SetCooperativeLevel(lpDS, hwnd, DSSCL_NORMAL))) {
		TextOut(hdc, 10, 30, "Set Co-op Failed", 18);
		return;
	}

	hsoEngine = SndObjCreate(lpDS, "ENGINE", 1);

	if (hsoEngine == NULL) {
		TextOut(hdc, 10, 50, "Obj Failed", 10);
		return;
	}

	hsoSkid = SndObjCreate(lpDS, "SKID", 1);

	if (!SUCCEEDED(SndObjPlay(hsoEngine, DSBPLAY_LOOPING))) {
		TextOut(hdc, 10, 70, "Play Failed", 11);
		return;
	}

	PrintMessage(hwnd, "DirectSound Initialised");
	ReleaseDC(hwnd, hdc);
}

void PlayDSound() {
	return;
	if (sound_intialised_flag == UNSET)
		return;
	SndObjPlay(hsoSkid, 0);
}
