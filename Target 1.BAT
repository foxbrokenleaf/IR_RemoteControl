SET C51INC=F:\Software\Develop\Software\Keil\C51\Inc
SET C51LIB=F:\Software\Develop\Software\Keil\C51\Lib
SET CPU_TYPE=8052 (all Variants)
SET CPU_VENDOR=Generic
SET UV2_TARGET=Target 1
SET CPU_XTAL=0x00B71B00
"F:\Software\Develop\Software\Keil\C51\BIN\A51.EXE" @"STARTUP._ia"
"F:\Software\Develop\Software\Keil\C51\BIN\C51.EXE" @"main.__i"
"F:\Software\Develop\Software\Keil\C51\BIN\BL51.EXE" @"project.lnp"
"F:\Software\Develop\Software\Keil\C51\BIN\OH51.EXE" "project" 
keil5_disp_size_bar.exe
