@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by NONLINSYSTEMS.HPJ. >"hlp\NonLinSystems.hm"
echo. >>"hlp\NonLinSystems.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\NonLinSystems.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\NonLinSystems.hm"
echo. >>"hlp\NonLinSystems.hm"
echo // Prompts (IDP_*) >>"hlp\NonLinSystems.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\NonLinSystems.hm"
echo. >>"hlp\NonLinSystems.hm"
echo // Resources (IDR_*) >>"hlp\NonLinSystems.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\NonLinSystems.hm"
echo. >>"hlp\NonLinSystems.hm"
echo // Dialogs (IDD_*) >>"hlp\NonLinSystems.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\NonLinSystems.hm"
echo. >>"hlp\NonLinSystems.hm"
echo // Frame Controls (IDW_*) >>"hlp\NonLinSystems.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\NonLinSystems.hm"
REM -- Make help for Project NONLINSYSTEMS


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\NonLinSystems.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\NonLinSystems.hlp" goto :Error
if not exist "hlp\NonLinSystems.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\NonLinSystems.hlp" Debug
if exist Debug\nul copy "hlp\NonLinSystems.cnt" Debug
if exist Release\nul copy "hlp\NonLinSystems.hlp" Release
if exist Release\nul copy "hlp\NonLinSystems.cnt" Release
echo.
goto :done

:Error
echo hlp\NonLinSystems.hpj(1) : error: Problem encountered creating help file

:done
echo.
