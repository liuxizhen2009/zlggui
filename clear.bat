@echo off
echo del *.d *.crf *.__i *.o
pause
del /S *.d
del /S *.axf

del /S *.crf

del /S *.__i
del /S *.o
del /S *.bak

::start explorer "./"
::copy %SRC_PATH_DAT% %DST_PATH% 
::copy %SRC_PATH_TXT% %DST_PATH% 
::for %%a in (ab xy aaa bbb) do copy "f:\music\%%a.mp3" d:\mp3\
pause
