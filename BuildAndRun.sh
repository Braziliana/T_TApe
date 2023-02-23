#!/bin/bash
if [ ! -d "build" ]; then
  mkdir build
fi

cd build

cmake ..

make

if [ -f T_TApe ]; then
    sudo ./T_TApe
fi
