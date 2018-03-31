# Soli Moisture Sensor

## Dependencies
This sketches depends on Adafruit_GFX and TFTLCD-Library to operate the display

## hardware
The soil moisture sensor itself is a resistor divider, read on ADC channel 5 and powered by arduino's D10 (power applied only when sensor is read, to avoid quick corrosion)

TFT touch screen (touch feature not used in the sketche) is wired to D2-D9 (data bus) and A0-A4 (control signals), plus power pins.

