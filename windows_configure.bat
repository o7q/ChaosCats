@echo off
mkdir build 2> nul
cd build
cmake .. -G "MinGW Makefiles"
pause