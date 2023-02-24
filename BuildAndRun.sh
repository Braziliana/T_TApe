#!/bin/bash
if [ -d "build" ]; then
  rm -r build
fi

mkdir build

cd build

cmake ..

make

if [ -f T_TApe ]; then
  #openssl rand -out key.bin 16
  #openssl enc -aes-256-cbc -salt -in plaintext.txt -out ciphertext.bin -pass file:key.bin
  #sudo chmod 700 T_TApe
  #sudo ./T_TApe
  #sudo unshare --user --map-root-user --mount-proc /bin/bash -c "./T_TApe"

  filename=$(openssl rand -hex $((RANDOM % 11 + 10)))
  mv T_TApe $filename
  sudo chmod 700 $filename
  sudo ./$filename
  rm $filename
fi
