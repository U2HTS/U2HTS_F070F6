#!/bin/bash
[ -z $1 ] || [ -z $2 ] && echo "Usage: $0 <swd/dfu> [firmware.elf]" && exit 1

case $1 in
dfu)
    dfu-suffix -a $2 -v 0x0483 -d 0xdf11
    dfu-util -d=0483:df11 -a 0 -s 0x08000000:leave -D $2
    ;;
swd)
    openocd -f /usr/share/openocd/scripts/interface/cmsis-dap.cfg -f /usr/share/openocd/scripts/target/stm32f0x.cfg -c "program $2 verify reset exit"
    ;;
esac
