@echo off

pushd scripts

call C:\Xilinx\14.6\ISE_DS\settings32.bat
call clean.bat noref
call build.bat nodup
call run.bat nobuild

popd

pause
