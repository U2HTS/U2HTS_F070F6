# U2HTS_F070F6
[U2HTS](https://github.com/CNflysky/U2HTS)的`STM32F070F6P6`移植版。

# 构建
## 借助GitHub actions构建
点击`fork`按钮，将本仓库fork到你的账号下。  
点击`Actions`标签页，点击`Enable workflow`。  
点击左边的`Build firmware`，点击`Run workflow`。  
构建完成后下载产物即可。
## 手动构建
```bash
sudo apt install gcc-arm-none-eabi libnewlib-dev libnewlib-arm-none-eabi make cmake
git clone https://github.com/CNflysky/U2HTS_F070F6.git --recursive --depth 1
cd U2HTS_F070F6
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=MinSizeRel -DCMAKE_TOOLCHAIN_FILE=cmake/gcc-arm-none-eabi.cmake
make -j16
```
*注意*：由于MCU的存储器较小 (6 KB RAM / 32 KB Flash)，只能集成数量有限的驱动。请在CMakeLists.txt中选择你需要的驱动。  
[CMakeLists.txt](./CMakeLists.txt#L56)

# 刷写
按下`用户按键`(靠近Type-C口的那个按键)，再接上Type-C线缆。
```bash
sudo apt install dfu-util
sudo ./flash.sh dfu build/U2HTS_F070F6.bin
```

# 配置
[main.c](./Core/Src/main.c#L98)