set /p filename=Please enter a file name:
set /p path=Please enter a Path:
set result1=%path%\%filename%.c
set result2=%path%\%filename%.h
set file1=#ifndef __%filename%_H  #define __%filename%_H   #endif
echo #include "%filename%.h"> %result1%
echo %file1%> %result2%