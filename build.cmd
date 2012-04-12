@echo off

call checkoutandpatch.cmd

mkdir build%Platform%
cd build%Platform%

call cmake -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX=stage -G "NMake Makefiles" ..
call nmake package

copy /Y zmq-private* ..
cd ..
