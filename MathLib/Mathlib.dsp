# Microsoft Developer Studio Project File - Name="Mathlib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Mathlib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Mathlib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Mathlib.mak" CFG="Mathlib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Mathlib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Mathlib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/MathLib", BAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Mathlib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G5 /MD /W3 /GX /O2 /I "d:\users\zvone\projects visual c++\mathlib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE RSC /l 0x41a
# ADD RSC /l 0x41a
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"c:\Program Files\Microsoft Visual Studio\VC98\Lib\MathLib.lib"

!ELSEIF  "$(CFG)" == "Mathlib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W2 /GX /Z7 /Od /I "d:\users\zvone\projects visual c++\mathlib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE RSC /l 0x41a
# ADD RSC /l 0x41a
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"d:\Program Files\Microsoft Visual Studio\VC98\Lib\MathLibDebug.lib"

!ENDIF 

# Begin Target

# Name "Mathlib - Win32 Release"
# Name "Mathlib - Win32 Debug"
# Begin Group "Header files"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\complex.h
# End Source File
# Begin Source File

SOURCE=.\DiffSystem.h
# End Source File
# Begin Source File

SOURCE=.\Function.h
# End Source File
# Begin Source File

SOURCE=.\Functions.h
# End Source File
# Begin Source File

SOURCE=.\geometry.h
# End Source File
# Begin Source File

SOURCE=.\Integrate.h
# End Source File
# Begin Source File

SOURCE=.\Interpolation.h
# End Source File
# Begin Source File

SOURCE=.\mathlib.h
# End Source File
# Begin Source File

SOURCE=.\matrix.h
# End Source File
# Begin Source File

SOURCE=.\MLibBasic.h
# End Source File
# Begin Source File

SOURCE=.\ODEIntegration.h
# End Source File
# Begin Source File

SOURCE=.\polynom.h
# End Source File
# Begin Source File

SOURCE=.\RootFinding.h
# End Source File
# Begin Source File

SOURCE=.\SparseMatrix.h
# End Source File
# Begin Source File

SOURCE=.\SpecFunc.h
# End Source File
# Begin Source File

SOURCE=.\vector.h
# End Source File
# Begin Source File

SOURCE=.\VectorFunction.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Calc.cpp
# End Source File
# Begin Source File

SOURCE=.\Complex.cpp
# End Source File
# Begin Source File

SOURCE=.\ComplexMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\DiffSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\Function.cpp
# End Source File
# Begin Source File

SOURCE=.\Geometry.cpp
# End Source File
# Begin Source File

SOURCE=.\Integration.cpp
# End Source File
# Begin Source File

SOURCE=.\Interpolation.cpp
# End Source File
# Begin Source File

SOURCE=.\mathlib.cpp
# End Source File
# Begin Source File

SOURCE=.\Matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\Minmization.cpp
# End Source File
# Begin Source File

SOURCE=.\ODEIntegration.cpp
# End Source File
# Begin Source File

SOURCE=.\Polynom.cpp
# End Source File
# Begin Source File

SOURCE=.\RootFinding.cpp
# End Source File
# Begin Source File

SOURCE=.\SparseMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\SpecFunc.cpp
# End Source File
# Begin Source File

SOURCE=.\Statistic.cpp
# End Source File
# Begin Source File

SOURCE=.\Vector.cpp
# End Source File
# Begin Source File

SOURCE=.\VectorFunction.cpp
# End Source File
# End Target
# End Project
