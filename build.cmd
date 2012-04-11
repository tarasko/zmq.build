@echo off
call git clone https://github.com/zeromq/zeromq2-x.git
call git clone https://github.com/zeromq/jzmq.git

copy /Y zeromq.cmake zeromq2-x/CMakeLists.txt
copy /Y jzmq.cmake jzmq/CMakeLists.txt

mkdir build%Platform%
cd build%Platform%

call cmake -G "NMake Makefiles" ..
call nmake package

copy /Y zmq-private* ..
cd ..
