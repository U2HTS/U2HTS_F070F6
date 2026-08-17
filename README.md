# U2HTS_F070F6
[U2HTS](https://github.com/U2HTS/U2HTS) `STM32F070F6P6` Port.  
[zh_CN(简体中文)](./README_zh.md)  
![PCB](https://github.com/user-attachments/assets/2f6e9a71-b0eb-4756-abe4-4c37f9a0a067)

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
LED, persistent storage and key are enabled via [CMakePresets.json](./CMakePresets.json).  
*Note*: This MCU has small memories (6 KB RAM / 32 KB Flash), so you would like to disable unused drivers to save memory.  
[CMakeLists.txt](./CMakeLists.txt#L57)

# Flash
Press `USR button` while power on the board.
```bash
sudo apt install dfu-util
sudo ./flash.sh dfu build/MinSizeRel/U2HTS_F070F6.bin
```

# Config
[main.c](./Core/Src/main.c#L128)

# Debug
`SWCLK` can be reconfigured as UART TX to print logs by following cmake command:
```shell
# Turn on log print via UART2_TX
cmake -B build -DCMAKE_TOOLCHAIN_FILE=cmake/gcc-arm-none-eabi.cmake -DCMAKE_BUILD_TYPE=MinSizeRel -DU2HTS_LOG_LEVEL=U2HTS_LOG_LEVEL_INFO -DU2HTS_F070F6_SWCLK_AS_UART=ON
```
