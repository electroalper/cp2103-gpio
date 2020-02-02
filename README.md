# cp2103-gpio
Windows command line tool to control CP2103 gpio pins

Usage: ```cp2103-gpio.exe [COMx] [mask] [latch]```

Examples:
- Set gpio 0 pin          : ```cp2103-gpio COM3 1 1```
- Clear gpio 2 pin        : ```cp2103-gpio COM3 4 0```
- Set all gpio pins       : ```cp2103-gpio COM3 0x0F 0x0F```
- Clear all gpio pins     : ```cp2103-gpio COM3 15 0```
- Set gpio 1,3 clear 0,2  : ```cp2103-gpio COM3 0x0F 0x0A```
- Read all gpio pins      : ```cp2103-gpio COM3 0 0```
  
