@echo off

mkdir build%Platform%
cd build%Platform%

call cmake -DSKIP_EXAMPLES=1 -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX=stage -G "NMake Makefiles" ..
call nmake package

copy /Y zmq.build* ..
cd ..
