ARDUINO_LIBS = 
BOARD_TAG    = uno
MONITOR_PORT = /dev/ttyACM0
USER_LIB_PATH = /home/johann/Arduino/libs/

CFLAGS = -g -Os -std=gnu99
LDFLAGS = -Wl,-u,vfprintf -lprintf_flt

ARDMK_VENDOR=archlinux-arduino
include /home/johann/Arduino/Arduino-Makefile/Arduino.mk


