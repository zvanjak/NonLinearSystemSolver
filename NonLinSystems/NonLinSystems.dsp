# Microsoft Developer Studio Project File - Name="NonLinSystems" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=NonLinSystems - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NonLinSystems.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NonLinSystems.mak" CFG="NonLinSystems - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NonLinSystems - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "NonLinSystems - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "NonLinSystems - Win32 DebugStatic" (based on "Win32 (x86) Application")
!MESSAGE "NonLinSystems - Win32 ReleaseStatic" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NonLinSystems - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G5 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x41a /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x41a /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Mathlib.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"LIBCMT"

!ELSEIF  "$(CFG)" == "NonLinSystems - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x41a /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x41a /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 MathLibDebug.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "NonLinSystems - Win32 DebugStatic"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "NonLinSystems___Win32_DebugStatic"
# PROP BASE Intermediate_Dir "NonLinSystems___Win32_DebugStatic"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "NonLinSystems___Win32_DebugStatic"
# PROP Intermediate_Dir "NonLinSystems___Win32_DebugStatic"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x41a /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x41a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 MathLibDebug.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 MathLibDebug.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "NonLinSystems - Win32 ReleaseStatic"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "NonLinSystems___Win32_ReleaseStatic"
# PROP BASE Intermediate_Dir "NonLinSystems___Win32_ReleaseStatic"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "NonLinSystems___Win32_ReleaseStatic"
# PROP Intermediate_Dir "NonLinSystems___Win32_ReleaseStatic"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G5 /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x41a /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x41a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Mathlib.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"LIBCMT"
# ADD LINK32 Mathlib.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"LIBCMT"

!ENDIF 

# Begin Target

# Name "NonLinSystems - Win32 Release"
# Name "NonLinSystems - Win32 Debug"
# Name "NonLinSystems - Win32 DebugStatic"
# Name "NonLinSystems - Win32 ReleaseStatic"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BifPointsListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcEquSolutionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CalcLyapExpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\InputDiffEqDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\IpFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\LocateBifPointsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LyapExpListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\NLSysOptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NonLinSystems.cpp
# End Source File
# Begin Source File

SOURCE=.\hlp\NonLinSystems.hpj

!IF  "$(CFG)" == "NonLinSystems - Win32 Release"

USERDEP__NONLI="$(ProjDir)\hlp\AfxCore.rtf"	"$(ProjDir)\hlp\AfxPrint.rtf"	
# Begin Custom Build - Making help file...
OutDir=.\Release
ProjDir=.
TargetName=NonLinSystems
InputPath=.\hlp\NonLinSystems.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	call "$(ProjDir)\makehelp.bat"

# End Custom Build

!ELSEIF  "$(CFG)" == "NonLinSystems - Win32 Debug"

USERDEP__NONLI="$(ProjDir)\hlp\AfxCore.rtf"	"$(ProjDir)\hlp\AfxPrint.rtf"	
# Begin Custom Build - Making help file...
OutDir=.\Debug
ProjDir=.
TargetName=NonLinSystems
InputPath=.\hlp\NonLinSystems.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	call "$(ProjDir)\makehelp.bat"

# End Custom Build

!ELSEIF  "$(CFG)" == "NonLinSystems - Win32 DebugStatic"

USERDEP__NONLI="$(ProjDir)\hlp\AfxCore.rtf"	"$(ProjDir)\hlp\AfxPrint.rtf"	
# Begin Custom Build - Making help file...
OutDir=.\NonLinSystems___Win32_DebugStatic
ProjDir=.
TargetName=NonLinSystems
InputPath=.\hlp\NonLinSystems.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	call "$(ProjDir)\makehelp.bat"

# End Custom Build

!ELSEIF  "$(CFG)" == "NonLinSystems - Win32 ReleaseStatic"

USERDEP__NONLI="$(ProjDir)\hlp\AfxCore.rtf"	"$(ProjDir)\hlp\AfxPrint.rtf"	
# Begin Custom Build - Making help file...
OutDir=.\NonLinSystems___Win32_ReleaseStatic
ProjDir=.
TargetName=NonLinSystems
InputPath=.\hlp\NonLinSystems.hpj

"$(OutDir)\$(TargetName).hlp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	call "$(ProjDir)\makehelp.bat"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\NonLinSystems.rc
# End Source File
# Begin Source File

SOURCE=.\NonLinSystemsDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\NonLinSystemsView.cpp
# End Source File
# Begin Source File

SOURCE=.\PhasePlotParDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PlotEquSurfaceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PlotFourierSpectrumDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PlotLyapExpDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvrItem.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TrajectoryPlotParDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BifPointsListDlg.h
# End Source File
# Begin Source File

SOURCE=.\CalcEquSolutionDlg.h
# End Source File
# Begin Source File

SOURCE=.\CalcLyapExpDlg.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\InputDiffEqDlg.h
# End Source File
# Begin Source File

SOURCE=.\IpFrame.h
# End Source File
# Begin Source File

SOURCE=.\LocateBifPointsDlg.h
# End Source File
# Begin Source File

SOURCE=.\LyapExpListDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\NLSysDoc.h
# End Source File
# Begin Source File

SOURCE=.\NLSysOptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\NLSysView.h
# End Source File
# Begin Source File

SOURCE=.\NonLinSystems.h
# End Source File
# Begin Source File

SOURCE=.\PhasePlotParDlg.h
# End Source File
# Begin Source File

SOURCE=.\PlotEquSurfaceDlg.h
# End Source File
# Begin Source File

SOURCE=.\PlotFourierSpectrumDlg.h
# End Source File
# Begin Source File

SOURCE=.\PlotLyapExpDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SrvrItem.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TrajectoryPlotParDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\commands.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\IToolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\NonLinSystems.ico
# End Source File
# Begin Source File

SOURCE=.\res\NonLinSystems.rc2
# End Source File
# Begin Source File

SOURCE=.\res\NonLinSystemsDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# Begin Group "Help Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\hlp\AfxCore.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\AfxOleSv.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\AfxPrint.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\AppExit.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Bullet.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurArw2.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurArw4.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\CurHelp.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditCopy.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditCut.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditPast.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\EditUndo.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileNew.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileOpen.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FilePrnt.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\FileSave.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\HlpSBar.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\HlpTBar.bmp
# End Source File
# Begin Source File

SOURCE=.\MakeHelp.bat
# End Source File
# Begin Source File

SOURCE=.\hlp\NonLinSystems.cnt
# End Source File
# Begin Source File

SOURCE=.\hlp\RecFirst.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecLast.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecNext.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\RecPrev.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Scmax.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\ScMenu.bmp
# End Source File
# Begin Source File

SOURCE=.\hlp\Scmin.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\NonLinSystems.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
