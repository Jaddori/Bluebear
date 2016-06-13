@echo off

pushd ..\build\

echo Building BlueFilesystem.
cl.exe -EHsc -Zi -nologo -c -Febluefilesystem.obj ..\code\bluefilesystem.cpp -link /INCREMENTAL:NO user32.lib
lib.exe /NOLOGO /OUT:bluefilesystem.lib bluefilesystem.obj

echo Building BlueWindow.
cl.exe -EHsc -Zi -nologo -c -Febluewindow.obj ..\code\bluewindow.cpp -link /INCREMENTAL:NO user32.lib
lib.exe /NOLOGO /OUT:bluewindow.lib bluewindow.obj

echo Building Tests
cl.exe -EHsc -Zi -nologo -Febluefilesystem_test.exe ..\code\bluefilesystem_test.cpp -link /INCREMENTAL:NO user32.lib bluefilesystem.lib
cl.exe -EHsc -Zi -nologo -Febluewindow_test.exe ..\code\bluewindow_test.cpp -link /INCREMENTAL:NO user32.lib bluewindow.lib opengl32.lib winmm.lib gdi32.lib

echo Removing .obj files.
del *.obj

popd ..\build\