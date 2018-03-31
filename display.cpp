// #include <cstdint>

#include "display.h"
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin


#define	BLACK   0x0000
#define	BLUE    0x001F
#define DARK_BLUE 0x000F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);


FILE *debug_stream=NULL;
#ifdef DISPLAY_DEBUG
  #define debug(...) if (debug_stream) {fprintf(debug_stream, __VA_ARGS__);}
#else 
  #define debug(...)
#endif 

uint16_t tft_width = 0xFFFF;
uint16_t tft_height = 0xFFFF;

uint8_t error = 0;

const char* driver_names[5] = {     "ILI9325",
                                    "ILI9328",
                                    "HX8347G",
                                    "ILI9341",
                                    "HX8357D"
};
                              
void display_init(FILE *dbg_stream) {
    if (dbg_stream) {debug_stream = dbg_stream;}

    #ifdef USE_ADAFRUIT_SHIELD_PINOUT
        debug("Using Adafruit 2.8\" TFT Arduino Shield Pinout");
    #else
        debug("Using Adafruit 2.8\" TFT Breakout Board Pinout");
    #endif

    debug("TFT size is %d x %d\n", tft.width(), tft.height());

    tft.reset();

    uint16_t identifier = tft.readID();
    const char *driver_name = NULL;
    if(identifier == 0x9325) {
        driver_name = driver_names[0];
    } else if(identifier == 0x9328) {
        driver_name = driver_names[1];
    } else if(identifier == 0x7575) {
        driver_name = driver_names[2];
    } else if(identifier == 0x9341 || identifier == 0x0101) {
        driver_name = driver_names[3];
        identifier = 0x9341;
    } else if(identifier == 0x8357) {
        driver_name = driver_names[4];
    } 
    if (driver_name == NULL) {
        debug("Unknown LCD driver chip: %x\n", identifier);
        debug("If using the Adafruit 2.8\" TFT Arduino shield, the line:\n");
        debug("  #define USE_ADAFRUIT_SHIELD_PINOUT\n");
        debug("should appear in the library header (Adafruit_TFT.h).\n");
        debug("If using the breakout board, it should NOT be #defined!\n");
        debug("Also if using the breakout, double-check that all wiring\n");
        debug("matches the tutorial.\n");
        return;
    }
    debug("Found %s LCD Driver", driver_name);
    tft.begin(identifier);
    tft.setRotation(3);
    debug("Driver started");
    tft_width = tft.width();
    tft_height = tft.height();

    error = 0;
}

void display_update(uint8_t *data, uint16_t size, uint16_t offset) {
    if (error) return;
    uint16_t stop = (size>(uint16_t)tft.width()) ? (uint16_t)tft.width() : size;
    
//     display_clear();
    for (uint16_t dspl_idx = 0; dspl_idx < stop; dspl_idx++) {
        display_update_step(data, size, offset, dspl_idx);
    }
}

void display_clear() {
    if (error) return;

/*    int16_t tft_height = tft.height();
    uint8_t levelline_step = 64; //tft_height >> 2;
    
    tft.fillScreen(BLACK);
    
    tft.drawLine(0, levelline_step, tft.width(), levelline_step, GREEN);
    tft.drawLine(0, 2*levelline_step, tft.width(), 2*levelline_step, YELLOW);
    tft.drawLine(0, 3*levelline_step, tft.width(), 3*levelline_step, RED);
*/
}    

void display_update_step(uint8_t *data, uint16_t size, uint16_t offset, uint16_t step) {
    if (error) return;
    if (step >= tft_width) return;
    
    tft.drawFastVLine(step, 0, tft_height, BLACK);
    tft.drawPixel(step, 64, GREEN);
    tft.drawPixel(step, 128, YELLOW);
    tft.drawPixel(step, 192, RED);
    
    uint16_t h = data[(step+offset)%size];
    if (h <= tft_height) {
        tft.drawPixel(step, h-1, BLUE);
        tft.drawPixel(step, h,   0x03FF);
        tft.drawPixel(step, h+1, BLUE);
    }
//     debug("(%d, %d)\n", step, h);
}

void display_error(char * str) {
    if (!str) {
        tft.fillScreen(BLACK);
        error = 0;
        debug("error : NONE\n");
        tft.setRotation(3);
    } else {
        tft.setRotation(1);
        tft.fillScreen(RED);
        error=1;
        tft.setCursor(0, 0);
        tft.setTextColor(WHITE);  
        tft.setTextSize(1);
        tft.println(str);
        debug("error : %s\n", str);
    }
}


uint16_t display_width() {
    return tft_width;
}
uint16_t display_height() {
    return tft_height;
}
