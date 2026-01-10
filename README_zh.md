# U2HTS_F070F6
[U2HTS](https://github.com/U2HTS/U2HTS)的`STM32F070F6P6`移植版。

# 构建
## 借助GitHub actions构建
点击`fork`按钮，将本仓库fork到你的账号下。  
点击`Actions`标签页，点击`Enable workflow`。  
点击左边的`Build firmware`，点击`Run workflow`。  
构建完成后下载产物即可。
## 手动构建
```bash
sudo apt install gcc-arm-none-eabi libnewlib-dev libnewlib-arm-none-eabi make cmake
git clone https://github.com/U2HTS/U2HTS_F070F6.git --recursive --depth 1
cd U2HTS_F070F6
cmake --preset MinSizeRel
cmake --build build/MinSizeRel
```
*注意*：由于MCU的存储器较小 (6 KB RAM / 32 KB Flash)，只能集成数量有限的驱动。请在CMakeLists.txt中选择你需要的驱动。  
[CMakeLists.txt](./CMakeLists.txt#L74)

# 刷写
按下`用户按键`再给板子上电。
```bash
sudo apt install dfu-util
sudo ./flash.sh dfu build/MinSizeRel/U2HTS_F070F6.bin
```

# 配置
[main.c](./Core/Src/main.c#L128)

# Debug
通过在[CMakeLists.txt](./CMakeLists.txt#L40)中配置相关选项，可以将`SWCLK`复用为UART TX来打印日志: 
```cmake
# Turn on log print via UART2_TX
set(U2HTS_F070F6_SWCLK_AS_UART ON)
set(U2HTS_LOG_LEVEL U2HTS_LOG_LEVEL_DEBUG)
```