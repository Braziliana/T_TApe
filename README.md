# T_TApe
<br />

**Disclaimer: This code is for educational purposes only. It is not intended for use in any games or competitions where it may violate the terms and conditions or be considered unfair to other players. Please use this code responsibly and at your own risk.**

<br />
<br />

## Dependency instalation script
```bash
#!/bin/bash

# Update package lists
sudo apt update

# Install required packages
sudo apt install -y libudev-dev xorg-dev libglu1-mesa-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libxkbcommon-dev libwayland-dev git cmake g++ gcc libinput-dev libsoil-dev nlohmann-json3-dev


# Clone GLFW from GitHub
git clone https://github.com/glfw/glfw.git

# Navigate to the GLFW directory
cd glfw

# Create build directory
mkdir build

# Navigate to the build directory
cd build

# Run CMake
cmake ..

# Build GLFW
make

# Install GLFW
sudo make install

# Navigate back to the parent directory
cd ../..

# Remove the GLFW directory
rm -rf glfw
```

## 1. Install Cinnamon desktop environment on Ubuntu
```bash
sudo apt update
```
```bash
sudo apt install cinnamon-desktop-environment
```
```bash
reboot
```

select cinnamon when you log in

## 2. Install dependencies
```bash
sudo apt update
```
```bash
sudo apt-get install -y libudev-dev
```
```bash
sudo apt install cmake xorg-dev libglu1-mesa-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

## 3. remove from /usr/lib/x86_64-linux-gnu/ files starting with libglfw you can run command below. If u get error u can mannualy check if files with name starting with libglfw if not you can go to next step
```bash
    sudo rm /usr/lib/x86_64-linux-gnu/libglfw*
```
## 4. build glfw
```bash
git clone https://github.com/glfw/glfw.git
```
```bash
cd glfw
```
```bash
mkdir build
```
```bash
cd build
```
```bash
cmake ..
```
```bash
make
```
```bash
sudo make install
```

## 5. Clone repo
```bash
git clone https://github.com/Braziliana/T_TApe.git
```
```bash
cd T_TApe
```

## 6. Build and run
### using commands
```bash
mkdir build
```
```bash
cd build
```
```bash
cmake ..
```
```bash
make
```
```bash
sudo sudo ./Apex
```

## 7. Menu key
* Insert