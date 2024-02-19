#!/bin/bash
if [ ! -d "build" ]; then
  #rm -r build
  mkdir build
fi

#mkdir build

cd build

cmake ..

make

if [ -f Apex ]; then
  # filename=$(openssl rand -hex $((RANDOM % 11 + 10)))
  # mv T_TApe $filename
  # sudo chmod 700 $filename
  # sudo ./$filename
  # rm $filename

  sudo chmod 700 Apex
  sudo ./Apex
fi
