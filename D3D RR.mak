# Microsoft Developer Studio Generated NMAKE File, Based on D3D RR.dsp
!IF "$(CFG)" == ""
CFG=D3D RR - Win32 Release
!MESSAGE No configuration specified. Defaulting to D3D RR - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "D3D RR - Win32 Release" && "$(CFG)" != "D3D RR - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "D3D RR.mak" CFG="D3D RR - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "D3D RR - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "D3D RR - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "D3D RR - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\.\Release
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\D3D RR.exe"

!ELSE 

ALL : "$(OUTDIR)\D3D RR.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\bitmap.obj"
	-@erase "$(INTDIR)\D3D Road Rage.obj"
	-@erase "$(INTDIR)\d3dapp.obj"
	-@erase "$(INTDIR)\d3dcalls.obj"
	-@erase "$(INTDIR)\d3dmain.obj"
	-@erase "$(INTDIR)\d3dmain.res"
	-@erase "$(INTDIR)\ddcalls.obj"
	-@erase "$(INTDIR)\ddutil.obj"
	-@erase "$(INTDIR)\dsutil.obj"
	-@erase "$(INTDIR)\editor.obj"
	-@erase "$(INTDIR)\input.obj"
	-@erase "$(INTDIR)\lclib.obj"
	-@erase "$(INTDIR)\matrix.obj"
	-@erase "$(INTDIR)\misc.obj"
	-@erase "$(INTDIR)\sound.obj"
	-@erase "$(INTDIR)\stats.obj"
	-@erase "$(INTDIR)\texture.obj"
	-@erase "$(INTDIR)\world.obj"
	-@erase "$(OUTDIR)\D3D RR.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)\D3D RR.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x809 /fo"$(INTDIR)\d3dmain.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\D3D RR.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=dxguid.lib dinput.lib ddraw.lib dsound.lib winmm.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo\
 /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\D3D RR.pdb" /machine:I386\
 /out:"$(OUTDIR)\D3D RR.exe" 
LINK32_OBJS= \
	"$(INTDIR)\bitmap.obj" \
	"$(INTDIR)\D3D Road Rage.obj" \
	"$(INTDIR)\d3dapp.obj" \
	"$(INTDIR)\d3dcalls.obj" \
	"$(INTDIR)\d3dmain.obj" \
	"$(INTDIR)\d3dmain.res" \
	"$(INTDIR)\ddcalls.obj" \
	"$(INTDIR)\ddutil.obj" \
	"$(INTDIR)\dsutil.obj" \
	"$(INTDIR)\editor.obj" \
	"$(INTDIR)\input.obj" \
	"$(INTDIR)\lclib.obj" \
	"$(INTDIR)\matrix.obj" \
	"$(INTDIR)\misc.obj" \
	"$(INTDIR)\sound.obj" \
	"$(INTDIR)\stats.obj" \
	"$(INTDIR)\texture.obj" \
	"$(INTDIR)\world.obj"

"$(OUTDIR)\D3D RR.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "D3D RR - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\.\Debug
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\D3D RR.exe" "$(OUTDIR)\D3D RR.bsc"

!ELSE 

ALL : "$(OUTDIR)\D3D RR.exe" "$(OUTDIR)\D3D RR.bsc"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\bitmap.obj"
	-@erase "$(INTDIR)\bitmap.sbr"
	-@erase "$(INTDIR)\D3D Road Rage.obj"
	-@erase "$(INTDIR)\D3D Road Rage.sbr"
	-@erase "$(INTDIR)\d3dapp.obj"
	-@erase "$(INTDIR)\d3dapp.sbr"
	-@erase "$(INTDIR)\d3dcalls.obj"
	-@erase "$(INTDIR)\d3dcalls.sbr"
	-@erase "$(INTDIR)\d3dmain.obj"
	-@erase "$(INTDIR)\d3dmain.res"
	-@erase "$(INTDIR)\d3dmain.sbr"
	-@erase "$(INTDIR)\ddcalls.obj"
	-@erase "$(INTDIR)\ddcalls.sbr"
	-@erase "$(INTDIR)\ddutil.obj"
	-@erase "$(INTDIR)\ddutil.sbr"
	-@erase "$(INTDIR)\dsutil.obj"
	-@erase "$(INTDIR)\dsutil.sbr"
	-@erase "$(INTDIR)\editor.obj"
	-@erase "$(INTDIR)\editor.sbr"
	-@erase "$(INTDIR)\input.obj"
	-@erase "$(INTDIR)\input.sbr"
	-@erase "$(INTDIR)\lclib.obj"
	-@erase "$(INTDIR)\lclib.sbr"
	-@erase "$(INTDIR)\matrix.obj"
	-@erase "$(INTDIR)\matrix.sbr"
	-@erase "$(INTDIR)\misc.obj"
	-@erase "$(INTDIR)\misc.sbr"
	-@erase "$(INTDIR)\sound.obj"
	-@erase "$(INTDIR)\sound.sbr"
	-@erase "$(INTDIR)\stats.obj"
	-@erase "$(INTDIR)\stats.sbr"
	-@erase "$(INTDIR)\texture.obj"
	-@erase "$(INTDIR)\texture.sbr"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\world.obj"
	-@erase "$(INTDIR)\world.sbr"
	-@erase "$(OUTDIR)\D3D RR.bsc"
	-@erase "$(OUTDIR)\D3D RR.exe"
	-@erase "$(OUTDIR)\D3D RR.ilk"
	-@erase "$(OUTDIR)\D3D RR.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /FR"$(INTDIR)\\" /Fp"$(INTDIR)\D3D RR.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\Debug/
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x809 /fo"$(INTDIR)\d3dmain.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\D3D RR.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\bitmap.sbr" \
	"$(INTDIR)\D3D Road Rage.sbr" \
	"$(INTDIR)\d3dapp.sbr" \
	"$(INTDIR)\d3dcalls.sbr" \
	"$(INTDIR)\d3dmain.sbr" \
	"$(INTDIR)\ddcalls.sbr" \
	"$(INTDIR)\ddutil.sbr" \
	"$(INTDIR)\dsutil.sbr" \
	"$(INTDIR)\editor.sbr" \
	"$(INTDIR)\input.sbr" \
	"$(INTDIR)\lclib.sbr" \
	"$(INTDIR)\matrix.sbr" \
	"$(INTDIR)\misc.sbr" \
	"$(INTDIR)\sound.sbr" \
	"$(INTDIR)\stats.sbr" \
	"$(INTDIR)\texture.sbr" \
	"$(INTDIR)\world.sbr"

"$(OUTDIR)\D3D RR.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=dxguid.lib dinput.lib ddraw.lib dsound.lib winmm.lib kernel32.lib\
 user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib\
 ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo\
 /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\D3D RR.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)\D3D RR.exe" 
LINK32_OBJS= \
	"$(INTDIR)\bitmap.obj" \
	"$(INTDIR)\D3D Road Rage.obj" \
	"$(INTDIR)\d3dapp.obj" \
	"$(INTDIR)\d3dcalls.obj" \
	"$(INTDIR)\d3dmain.obj" \
	"$(INTDIR)\d3dmain.res" \
	"$(INTDIR)\ddcalls.obj" \
	"$(INTDIR)\ddutil.obj" \
	"$(INTDIR)\dsutil.obj" \
	"$(INTDIR)\editor.obj" \
	"$(INTDIR)\input.obj" \
	"$(INTDIR)\lclib.obj" \
	"$(INTDIR)\matrix.obj" \
	"$(INTDIR)\misc.obj" \
	"$(INTDIR)\sound.obj" \
	"$(INTDIR)\stats.obj" \
	"$(INTDIR)\texture.obj" \
	"$(INTDIR)\world.obj"

"$(OUTDIR)\D3D RR.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(CFG)" == "D3D RR - Win32 Release" || "$(CFG)" == "D3D RR - Win32 Debug"
SOURCE=.\bitmap.cpp

!IF  "$(CFG)" == "D3D RR - Win32 Release"

DEP_CPP_BITMA=\
	".\3Dengine.h"\
	".\bitmap.h"\
	".\d3dapp.h"\
	".\d3ddemo.h"\
	".\d3dmacs.h"\
	".\d3dmain.h"\
	".\d3dmath.h"\
	".\d3dres.h"\
	".\d3dsphr.h"\
	".\world.h"\
	

"$(INTDIR)\bitmap.obj" : $(SOURCE) $(DEP_CPP_BITMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "D3D RR - Win32 Debug"

DEP_CPP_BITMA=\
	".\3Dengine.h"\
	".\bitmap.h"\
	".\d3dapp.h"\
	".\d3ddemo.h"\
	".\d3dmacs.h"\
	".\d3dmain.h"\
	".\d3dmath.h"\
	".\d3dres.h"\
	".\d3dsphr.h"\
	".\world.h"\
	

"$(INTDIR)\bitmap.obj"	"$(INTDIR)\bitmap.sbr" : $(SOURCE) $(DEP_CPP_BITMA)\
 "$(INTDIR)"


!ENDIF 

SOURCE=".\D3D Road Rage.cpp"

!IF  "$(CFG)" == "D3D RR - Win32 Release"

DEP_CPP_D3D_R=\
	".\3Dengine.h"\
	".\d3dapp.h"\
	".\d3ddemo.h"\
	".\d3dmacs.h"\
	".\d3dmain.h"\
	".\d3dmath.h"\
	".\d3dres.h"\
	".\d3dsphr.h"\
	".\matrix.h"\
	".\world.h"\
	

"$(INTDIR)\D3D Road Rage.obj" : $(SOURCE) $(DEP_CPP_D3D_R) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "D3D RR - Win32 Debug"

DEP_CPP_D3D_R=\
	".\3Dengine.h"\
	".\d3dapp.h"\
	".\d3ddemo.h"\
	".\d3dmacs.h"\
	".\d3dmain.h"\
	".\d3dmath.h"\
	".\d3dres.h"\
	".\d3dsphr.h"\
	".\matrix.h"\
	".\world.h"\
	

"$(INTDIR)\D3D Road Rage.obj"	"$(INTDIR)\D3D Road Rage.sbr" : $(SOURCE)\
 $(DEP_CPP_D3D_R) "$(INTDIR)"


!ENDIF 

SOURCE=.\d3dapp.c

!IF  "$(CFG)" == "D3D RR - Win32 Release"

DEP_CPP_D3DAP=\
	".\d3dapp.h"\
	".\d3dappi.h"\
	".\d3dmacs.h"\
	".\lclib.h"\
	

"$(INTDIR)\d3dapp.obj" : $(SOURCE) $(DEP_CPP_D3DAP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "D3D RR - Win32 Debug"

DEP_CPP_D3DAP=\
	".\d3dapp.h"\
	".\d3dappi.h"\
	".\d3dmacs.h"\
	".\lclib.h"\
	

"$(INTDIR)\d3dapp.obj"	"$(INTDIR)\d3dapp.sbr" : $(SOURCE) $(DEP_CPP_D3DAP)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\d3dcalls.c

!IF  "$(CFG)" == "D3D RR - Win32 Release"

DEP_CPP_D3DCA=\
	".\d3dapp.h"\
	".\d3dappi.h"\
	".\d3dmacs.h"\
	".\lclib.h"\
	

"$(INTDIR)\d3dcalls.obj" : $(SOURCE) $(DEP_CPP_D3DCA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "D3D RR - Win32 Debug"

DEP_CPP_D3DCA=\
	".\d3dapp.h"\
	".\d3dappi.h"\
	".\d3dmacs.h"\
	".\lclib.h"\
	

"$(INTDIR)\d3dcalls.obj"	"$(INTDIR)\d3dcalls.sbr" : $(SOURCE) $(DEP_CPP_D3DCA)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\d3dmain.cpp

!IF  "$(CFG)" == "D3D RR - Win32 Release"

DEP_CPP_D3DMA=\
	".\3Dengine.h"\
	".\bitmap.h"\
	".\d3dapp.h"\
	".\d3dappi.h"\
	".\d3ddemo.h"\
	".\d3dmacs.h"\
	".\d3dmain.h"\
	".\d3dmath.h"\
	".\d3dres.h"\
	".\d3dsphr.h"\
	".\editor.h"\
	".\input.h"\
	".\lclib.h"\
	".\sound.h"\
	".\Toolbar.h"\
	".\world.h"\
	{$(INCLUDE)}"dinput.h"\
	

"$(INTDIR)\d3dmain.obj" : $(SOURCE) $(DEP_CPP_D3DMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "D3D RR - Win32 Debug"

DEP_CPP_D3DMA=\
	".\3Dengine.h"\
	".\bitmap.h"\
	".\d3dapp.h"\
	".\d3dappi.h"\
	".\d3ddemo.h"\
	".\d3dmacs.h"\
	".\d3dmain.h"\
	".\d3dmath.h"\
	".\d3dres.h"\
	".\d3dsphr.h"\
	".\editor.h"\
	".\input.h"\
	".\lclib.h"\
	".\sound.h"\
	".\Toolbar.h"\
	".\world.h"\
	{$(INCLUDE)}"dinput.h"\
	

"$(INTDIR)\d3dmain.obj"	"$(INTDIR)\d3dmain.sbr" : $(SOURCE) $(DEP_CPP_D3DMA)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\d3dmain.rc

!IF  "$(CFG)" == "D3D RR - Win32 Release"

DEP_RSC_D3DMAI=\
	".\bitmap1.bmp"\
	".\d3d.ico"\
	".\d3dres.h"\
	
NODEP_RSC_D3DMAI=\
	".\Release\0x0080"\
	

"$(INTDIR)\d3dmain.res" : $(SOURCE) $(DEP_RSC_D3DMAI) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "D3D RR - Win32 Debug"

DEP_RSC_D3DMAI=\
	".\bitmap1.bmp"\
	".\d3d.ico"\
	".\d3dres.h"\
	
NODEP_RSC_D3DMAI=\
	".\Debug\0x0080"\
	

"$(INTDIR)\d3dmain.res" : $(SOURCE) $(DEP_RSC_D3DMAI) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


!ENDIF 

SOURCE=.\ddcalls.c

!IF  "$(CFG)" == "D3D RR - Win32 Release"

DEP_CPP_DDCAL=\
	".\d3dapp.h"\
	".\d3dappi.h"\
	".\d3dmacs.h"\
	".\lclib.h"\
	

"$(INTDIR)\ddcalls.obj" : $(SOURCE) $(DEP_CPP_DDCAL) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "D3D RR - Win32 Debug"

DEP_CPP_DDCAL=\
	".\d3dapp.h"\
	".\d3dappi.h"\
	".\d3dmacs.h"\
	".\lclib.h"\
	

"$(INTDIR)\ddcalls.obj"	"$(INTDIR)\ddcalls.sbr" : $(SOURCE) $(DEP_CPP_DDCAL)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\ddutil.cpp
DEP_CPP_DDUTI=\
	".\ddutil.h"\
	

!IF  "$(CFG)" == "D3D RR - Win32 Release"


"$(INTDIR)\ddutil.obj" : $(SOURCE) $(DEP_CPP_DDUTI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "D3D RR - Win32 Debug"


"$(INTDIR)\ddutil.obj"	"$(INTDIR)\ddutil.sbr" : $(SOURCE) $(DEP_CPP_DDUTI)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\dsutil.c
DEP_CPP_DSUTI=\
	".\dsutil.h"\
	

!IF  "$(CFG)" == "D3D RR - Win32 Release"


"$(INTDIR)\dsutil.obj" : $(SOURCE) $(DEP_CPP_DSUTI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "D3D RR - Win32 Debug"


"$(INTDIR)\dsutil.obj"	"$(INTDIR)\dsutil.sbr" : $(SOURCE) $(DEP_CPP_DSUTI)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\editor.c

!IF  "$(CFG)" == "D3D RR - Win32 Release"

DEP_CPP_EDITO=\
	".\3Dengine.h"\
	".\d3dapp.h"\
	".\d3ddemo.h"\
	".\d3dmacs.h"\
	".\d3dmath.h"\
	".\d3dsphr.h"\
	".\editor.h"\
	".\Toolbar.h"\
	".\world.h"\
	

"$(INTDIR)\editor.obj" : $(SOURCE) $(DEP_CPP_EDITO) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "D3D RR - Win32 Debug"

DEP_CPP_EDITO=\
	".\3Dengine.h"\
	".\d3dapp.h"\
	".\d3ddemo.h"\
	".\d3dmacs.h"\
	".\d3dmath.h"\
	".\d3dsphr.h"\
	".\editor.h"\
	".\Toolbar.h"\
	".\world.h"\
	

"$(INTDIR)\editor.obj"	"$(INTDIR)\editor.sbr" : $(SOURCE) $(DEP_CPP_EDITO)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\input.c
DEP_CPP_INPUT=\
	".\input.h"\
	{$(INCLUDE)}"dinput.h"\
	

!IF  "$(CFG)" == "D3D RR - Win32 Release"


"$(INTDIR)\input.obj" : $(SOURCE) $(DEP_CPP_INPUT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "D3D RR - Win32 Debug"


"$(INTDIR)\input.obj"	"$(INTDIR)\input.sbr" : $(SOURCE) $(DEP_CPP_INPUT)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\lclib.c
DEP_CPP_LCLIB=\
	".\lclib.h"\
	

!IF  "$(CFG)" == "D3D RR - Win32 Release"


"$(INTDIR)\lclib.obj" : $(SOURCE) $(DEP_CPP_LCLIB) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "D3D RR - Win32 Debug"


"$(INTDIR)\lclib.obj"	"$(INTDIR)\lclib.sbr" : $(SOURCE) $(DEP_CPP_LCLIB)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\matrix.cpp

!IF  "$(CFG)" == "D3D RR - Win32 Release"

DEP_CPP_MATRI=\
	"..\dx6sdk\include\d3dvec.inl"\
	".\3Dengine.h"\
	".\d3dapp.h"\
	".\d3ddemo.h"\
	".\d3dmacs.h"\
	".\d3dmath.h"\
	".\d3dsphr.h"\
	".\matrix.h"\
	".\world.h"\
	

"$(INTDIR)\matrix.obj" : $(SOURCE) $(DEP_CPP_MATRI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "D3D RR - Win32 Debug"

DEP_CPP_MATRI=\
	".\3Dengine.h"\
	".\d3dapp.h"\
	".\d3ddemo.h"\
	".\d3dmacs.h"\
	".\d3dmath.h"\
	".\d3dsphr.h"\
	".\matrix.h"\
	".\world.h"\
	

"$(INTDIR)\matrix.obj"	"$(INTDIR)\matrix.sbr" : $(SOURCE) $(DEP_CPP_MATRI)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\misc.c

!IF  "$(CFG)" == "D3D RR - Win32 Release"

DEP_CPP_MISC_=\
	".\d3dapp.h"\
	".\d3dappi.h"\
	".\d3dmacs.h"\
	".\lclib.h"\
	

"$(INTDIR)\misc.obj" : $(SOURCE) $(DEP_CPP_MISC_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "D3D RR - Win32 Debug"

DEP_CPP_MISC_=\
	".\d3dapp.h"\
	".\d3dappi.h"\
	".\d3dmacs.h"\
	".\lclib.h"\
	

"$(INTDIR)\misc.obj"	"$(INTDIR)\misc.sbr" : $(SOURCE) $(DEP_CPP_MISC_)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\sound.c

!IF  "$(CFG)" == "D3D RR - Win32 Release"

DEP_CPP_SOUND=\
	".\d3dapp.h"\
	".\dsutil.h"\
	".\sound.h"\
	".\world.h"\
	

"$(INTDIR)\sound.obj" : $(SOURCE) $(DEP_CPP_SOUND) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "D3D RR - Win32 Debug"

DEP_CPP_SOUND=\
	".\d3dapp.h"\
	".\dsutil.h"\
	".\sound.h"\
	".\world.h"\
	

"$(INTDIR)\sound.obj"	"$(INTDIR)\sound.sbr" : $(SOURCE) $(DEP_CPP_SOUND)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\stats.cpp

!IF  "$(CFG)" == "D3D RR - Win32 Release"

DEP_CPP_STATS=\
	".\3Dengine.h"\
	".\d3dapp.h"\
	".\d3ddemo.h"\
	".\d3dmacs.h"\
	".\d3dmain.h"\
	".\d3dmath.h"\
	".\d3dres.h"\
	".\d3dsphr.h"\
	".\world.h"\
	

"$(INTDIR)\stats.obj" : $(SOURCE) $(DEP_CPP_STATS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "D3D RR - Win32 Debug"

DEP_CPP_STATS=\
	".\3Dengine.h"\
	".\d3dapp.h"\
	".\d3ddemo.h"\
	".\d3dmacs.h"\
	".\d3dmain.h"\
	".\d3dmath.h"\
	".\d3dres.h"\
	".\d3dsphr.h"\
	".\world.h"\
	

"$(INTDIR)\stats.obj"	"$(INTDIR)\stats.sbr" : $(SOURCE) $(DEP_CPP_STATS)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\texture.c

!IF  "$(CFG)" == "D3D RR - Win32 Release"

DEP_CPP_TEXTU=\
	".\d3dapp.h"\
	".\d3dappi.h"\
	".\d3dmacs.h"\
	".\lclib.h"\
	

"$(INTDIR)\texture.obj" : $(SOURCE) $(DEP_CPP_TEXTU) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "D3D RR - Win32 Debug"

DEP_CPP_TEXTU=\
	".\d3dapp.h"\
	".\d3dappi.h"\
	".\d3dmacs.h"\
	".\lclib.h"\
	

"$(INTDIR)\texture.obj"	"$(INTDIR)\texture.sbr" : $(SOURCE) $(DEP_CPP_TEXTU)\
 "$(INTDIR)"


!ENDIF 

SOURCE=.\world.cpp

!IF  "$(CFG)" == "D3D RR - Win32 Release"

DEP_CPP_WORLD=\
	".\3Dengine.h"\
	".\d3dapp.h"\
	".\d3ddemo.h"\
	".\d3dmacs.h"\
	".\d3dmain.h"\
	".\d3dmath.h"\
	".\d3dres.h"\
	".\d3dsphr.h"\
	".\world.h"\
	

"$(INTDIR)\world.obj" : $(SOURCE) $(DEP_CPP_WORLD) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "D3D RR - Win32 Debug"

DEP_CPP_WORLD=\
	".\3Dengine.h"\
	".\d3dapp.h"\
	".\d3ddemo.h"\
	".\d3dmacs.h"\
	".\d3dmain.h"\
	".\d3dmath.h"\
	".\d3dres.h"\
	".\d3dsphr.h"\
	".\world.h"\
	

"$(INTDIR)\world.obj"	"$(INTDIR)\world.sbr" : $(SOURCE) $(DEP_CPP_WORLD)\
 "$(INTDIR)"


!ENDIF 


!ENDIF 

