ARDUINO_LIBS = Adafruit_GFX TFTLCD-Library
ARDMK_VENDOR = archlinux-arduino
BOARD_TAG    = uno
MONITOR_PORT = /dev/ttyACM0
USER_LIB_PATH = /home/johann/Arduino/libs/

CFLAGS = -g -O0 -std=gnu99
include /home/johann/Arduino/Arduino-Makefile/Arduino.mk


