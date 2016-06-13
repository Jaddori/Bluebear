@echo off

pushd ..\build\

echo Building BlueFilesystem.
cl.exe -EHsc -Zi -nologo -c -Febluefilesystem.obj ..\code\bluefilesystem.cpp -link /INCREMENTAL:NO user32.lib
lib.exe /NOLOGO /OUT:bluefilesystem.lib bluefilesystem.obj

echo Removing .obj files.
del *.obj

popd ..\build\