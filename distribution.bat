@ECHO off
ECHO ===========================================================
ECHO ======================= MEGALOADER ========================
ECHO ===========================================================

SET ver=_v1.2
SET rar_path="C:\Program Files\WinRAR\rar.exe"

REM ============================================================================================================

IF EXIST MegaLoader\*.dol rm MegaLoader/*.*
IF EXIST MegaLoader\*.iso rm MegaLoader/*.*
IF EXIST MegaLoader*.rar rm MegaLoader*.rar
IF NOT EXIST MegaLoader MKDIR MegaLoader
IF NOT EXIST megaloader.dol GOTO ERROR1
IF NOT EXIST iso\dollz3.exe GOTO ERROR2
IF NOT EXIST iso\mkisofs.exe GOTO ERROR3

COPY megaloader.dol MegaLoader
IF EXIST MegaLoader\megaloader.dol SET /a errorcheck=%errorcheck%+1

iso\dollz3 megaloader.dol MegaLoader\megaloader-lz.dol -m
IF EXIST MegaLoader\megaloader-lz.dol SET /a errorcheck=%errorcheck%+1

iso\dollz3 megaloader.dol MegaLoader\megaloader-lz-viper.dol -m -v
IF EXIST MegaLoader\megaloader-lz-viper.dol SET /a errorcheck=%errorcheck%+1

COPY megaloader.dol iso\disc\
iso\mkisofs -R -J -G iso/gbi.hdr -no-emul-boot -b megaloader.dol -o MegaLoader\megaloader.iso iso/disc/
IF EXIST MegaLoader\megaloader.iso SET /a errorcheck=%errorcheck%+1

IF %errorcheck%==4 (goto :RAR) else goto :NORAR

REM ============================================================================================================

:NORAR
ECHO   STATUS: ERROR! RAR package will not be built!!
IF NOT EXIST MegaLoader\megaloader.dol ECHO   FILE NOT FOUND: MegaLoader\megaloader.dol
IF NOT EXIST MegaLoader\megaloader-lz.dol ECHO   FILE NOT FOUND: MegaLoader\megaloader-lz.dol
IF NOT EXIST MegaLoader\megaloader-lz-viper.dol ECHO   FILE NOT FOUND: MegaLoader\megaloader-lz-viper.dol
IF NOT EXIST MegaLoader\megaloader.iso ECHO   FILE NOT FOUND: MegaLoader\megaloader.iso
GOTO EOF

:RAR
IF EXIST %rar_path% %rar_path% a MegaLoader%ver%.rar MegaLoader
IF NOT EXIST %rar_path% ECHO   STATUS: ERROR!  FILE NOT FOUND: rar.exe && ECHO   EXPECTED PATH = %rar_path%
GOTO EOF

:ERROR1
ECHO. && ECHO STATUS: ERROR!  FILE NOT FOUND: megaloader.dol
GOTO EOF

:ERROR2
ECHO. && ECHO STATUS: ERROR!  FILE NOT FOUND: iso\dollz3.exe
GOTO EOF

:ERROR3
ECHO. && ECHO STATUS: ERROR!  FILE NOT FOUND: iso\mkisofs.exe
GOTO EOF

:EOF
ECHO. && ECHO Press any key or close window to exit
PAUSE