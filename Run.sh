if [ ! -d "build" ]; then
  mkdir build
fi

cd build

if [ -f T_TApe ]; then
    sudo ./T_TApe
fi
