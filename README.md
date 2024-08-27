# netlib

netlib is a toy network library built around io_uring using the proactor pattern

## compiling / linking

```bash
mkdir build
cd build
cmake -S ../netlib -B -DCMAKE_INSTALL_PREFIX=/opt/netlib-install/ # replace with whatever install dir
cmake --build .
cmake --install . 
```

In your cmake configuration:
```cmake
find_package(netlib REQUIRED PATHS /opt/netlib-install/)
target_link_libraries(your_target PRIVATE netlib::netlib)
```

It requires C++20 since eventually I'll use those features, probably maybe.

## usage

This library sucks, since it's just for fun. Don't use it. You can see an example of connecting and reading data 
from a socket in sandbox/src/main.cpp if you really want to.

