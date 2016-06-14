@echo off

pushd ..\build\

echo Debug builds:
echo.

echo Building BlueFilesystem.
cl.exe -EHsc -Zi -nologo -c ..\code\bluefilesystem.cpp -link /INCREMENTAL:NO user32.lib
lib.exe /NOLOGO /OUT:bluefilesystem_d.lib bluefilesystem.obj

echo Building BlueWindow.
cl.exe -EHsc -Zi -nologo -c ..\code\bluewindow.cpp -link /INCREMENTAL:NO user32.lib
lib.exe /NOLOGO /OUT:bluewindow_d.lib bluewindow.obj

echo Building BluePNG.
cl.exe -EHsc -Zi -nologo -c ..\code\bluepng.cpp /IC:\LIBPNG\ /IC:\ZLIB\ -link /INCREMENTAL:NO /LIBPATH:C:\LIBPNG\bin\ /LIBPATH:C:\ZLIB\bin\ user32.lib libpng.lib zlib.lib
lib.exe /NOLOGO /OUT:bluepng_d.lib bluepng.obj

echo Building BlueController.
cl.exe -EHsc -Zi -nologo -c ..\code\bluecontroller.cpp -link /INCREMENTAL:NO user32.lib xinput9.lib
lib.exe /NOLOGO /OUT:bluecontroller_d.lib bluecontroller.obj

echo.
echo Release builds:
echo.

echo Building BlueFilesystem.
cl.exe -EHsc -nologo -c -Febluefilesystem.obj ..\code\bluefilesystem.cpp -link /INCREMENTAL:NO user32.lib
lib.exe /NOLOGO /OUT:bluefilesystem.lib bluefilesystem.obj

echo Building BlueWindow.
cl.exe -EHsc -nologo -c -Febluewindow.obj ..\code\bluewindow.cpp -link /INCREMENTAL:NO user32.lib
lib.exe /NOLOGO /OUT:bluewindow.lib bluewindow.obj

echo Building BluePNG.
cl.exe -EHsc -nologo -c -Febluepng.obj ..\code\bluepng.cpp /IC:\LIBPNG\ /IC:\ZLIB\ -link /INCREMENTAL:NO /LIBPATH:C:\LIBPNG\bin\ /LIBPATH:C:\ZLIB\bin\ user32.lib libpng.lib zlib.lib
lib.exe /NOLOGO /OUT:bluepng.lib bluepng.obj

echo Building BlueController.
cl.exe -EHsc -nologo -c -Febluecontroller.obj ..\code\bluecontroller.cpp -link /INCREMENTAL:NO user32.lib xinput9.lib
lib.exe /NOLOGO /OUT:bluecontroller.lib bluecontroller.obj

echo.
echo Building Tests
echo.
cl.exe -EHsc -Zi -nologo -Febluefilesystem_test.exe ..\code\bluefilesystem_test.cpp -link /INCREMENTAL:NO user32.lib bluefilesystem.lib
cl.exe -EHsc -Zi -nologo -Febluewindow_test.exe ..\code\bluewindow_test.cpp -link /INCREMENTAL:NO user32.lib bluewindow.lib opengl32.lib winmm.lib gdi32.lib

echo.
echo Removing .obj files.
del *.obj

popd ..\build\