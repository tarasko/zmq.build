@echo off
call git clone https://github.com/zeromq/zeromq2-x.git
call git clone https://github.com/zeromq/jzmq.git

cmake -E copy zeromq.cmake zeromq2-x/CMakeLists.txt
cmake -E copy jzmq.cmake jzmq/CMakeLists.txt

mkdir build%Platform%
cd build%Platform%

call cmake -DCMAKE_BUILD_TYPE=RELEASE -DCMAKE_INSTALL_PREFIX=stage -G "NMake Makefiles" ..
call nmake package

copy /Y zmq-private* ..
cd ..
