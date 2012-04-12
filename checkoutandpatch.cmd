@echo off
call git clone https://github.com/zeromq/zeromq2-x.git
call git clone https://github.com/zeromq/jzmq.git

cmake -E copy zeromq.cmake zeromq2-x/CMakeLists.txt
cmake -E copy jzmq.cmake jzmq/CMakeLists.txt
