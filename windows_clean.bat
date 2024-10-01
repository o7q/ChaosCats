@echo off
for /r %cd% %%G in (*.o *.d) do del /f "%%G"
del /f "ChaosCats.exe"