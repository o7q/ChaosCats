@echo off
mingw32-make compile-windows -f "shared.makefile"
ChaosCats.exe host 60000
pause