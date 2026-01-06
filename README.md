# U2HTS_F070F6
[U2HTS](https://github.com/U2HTS/U2HTS) `STM32F070F6P6` Port.  
[zh_CN(简体中文)](./README_zh.md)

# Build
## GitHub action
Fork this repository to your account.  
Click `Actions` page, click `enable workflow`.  
Click `Build firmware` on the left，click `Run workflow`.  
Wait until build complete and grab the artifacts.  
## Manual
```bash
sudo apt install gcc-arm-none-eabi libnewlib-dev libnewlib-arm-none-eabi ninja-build cmake
git clone https://github.com/U2HTS/U2HTS_F070F6.git --recursive --depth 1
cd U2HTS_F070F6
cmake --preset MinSizeRel
cmake --build build/MinSizeRel
```
*Note*: This MCU has small memories (6 KB RAM / 32 KB Flash), so you would like to disable unused drivers to save memory.  
[CMakeLists.txt](./CMakeLists.txt#L56)

# Flash
Press `USR button` while power on the board.
```bash
sudo apt install dfu-util
sudo ./flash.sh dfu build/MinSizeRel/U2HTS_F070F6.bin
```

# Config
[main.c](./Core/Src/main.c#L98)
