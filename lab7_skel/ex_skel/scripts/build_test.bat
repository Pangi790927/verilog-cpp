@echo off

SET nodup=

if "%2"=="nodup" SET nodup=nodup
if "%3"=="nodup" SET nodup=nodup

SET build_mode=call build_base.bat %nodup%

if "%2"=="nobase" SET build_mode=
if "%3"=="nobase" SET build_mode=

%build_mode%

mkdir ..\build 2> NUL
pushd ..\build

popd

echo Linking test %1...

mkdir ..\build\bin\%1 2> NUL
pushd ..\build\bin\%1

fuse -incremental tester.tester -L unisims_ver -L unimacro_ver -L xilinxcorelib_ver -L tester=..\..\lib\tester -L reference=..\..\lib\reference -L uut=..\..\lib\uut -o tester.exe > NUL

popd