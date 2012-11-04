@ECHO off
ECHO ===========================================================
ECHO ======================= MEGALOADER ========================
ECHO ===========================================================

SET var1==no
SET var2==no
SET var3==no

IF EXIST distro\*.dol rm distro/*.*
IF EXIST distro\*.iso rm distro/*.*
IF EXIST distribution.rar rm distribution.rar
IF NOT EXIST distro MKDIR distro
IF NOT EXIST megaloader.dol GOTO ERROR1
IF NOT EXIST iso\dollz3.exe GOTO ERROR2
IF NOT EXIST iso\mkisofs.exe GOTO ERROR3

COPY megaloader.dol distro
iso\dollz3 megaloader.dol distro\megaloader-lz.dol -m
ECHO.
IF EXIST distro\megaloader-lz.dol ECHO   STATUS: megaloader-lz.dol DONE!!

ECHO.
iso\dollz3 megaloader.dol distro\megaloader-lz-viper.dol -m -v
ECHO.
IF EXIST distro\megaloader-lz-viper.dol ECHO   STATUS: megaloader-lz-viper.dol DONE!!
ECHO.
iso\dollz3 megaloader.dol iso\disc\megaloader.dol -m
iso\mkisofs -R -J -G iso/gbi.hdr -no-emul-boot -b megaloader.dol -o distro\megaloader.iso iso/disc/
ECHO.
IF EXIST distro\megaloader.iso ECHO   STATUS: megaloader.iso DONE!!
ECHO.
"C:\Program Files\WinRAR\rar.exe" a distribution.rar distro
GOTO EOF

:ERROR1
ECHO.
ECHO ERROR: megaloader.dol NOT FOUND!!
ECHO.
GOTO EOF

:ERROR2
ECHO.
ECHO ERROR: iso\dollz3.exe NOT FOUND!!
ECHO.
GOTO EOF

:ERROR3
ECHO.
ECHO ERROR: iso\mkisofs.exe NOT FOUND!!
ECHO.
GOTO EOF


:EOF
ECHO Press any key or close window to exit
PAUSE

rem COPY megaloader.dol distro
rem iso\dollz3 megaloader.dol distro\megaloader-lz.dol -m
rem iso\dollz3 megaloader.dol distro\megaloader-lz-viper.dol -m -v
rem iso\dollz3 megaloader.dol iso\disc\megaloader.dol -m
rem iso\mkisofs -R -J -G iso/gbi.hdr -no-emul-boot -b megaloader.dol -o distro\megaloader.iso iso/disc/
rem ECHO Press any key or close window to exit
rem PAUSE





rem & rmdir distro
rem rm distro/*.*