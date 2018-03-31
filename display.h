#ifndef MOISTURE_SENSOR_DISPLAY
#define MOISTURE_SENSOR_DISPLAY

// comment next line to disable debug logs
#define DISPLAY_DEBUG 1

#ifdef __cplusplus
extern "C" {
#endif 

#ifndef FILE
  #include <stdio.h>
#endif 
    
// init display module.
// NB if f!=NULL, it is used to printf logs, when DISPLAY_BEBUG enabled    
void display_init(FILE* f);

// Call with NULL to reset error.
// if called with a message, display is "blocked" until reset.
void display_error(char * str);

// NB : two metgods provided : 
// 1 - clear & update all steps of display
void display_update(uint8_t *data, uint16_t size, uint16_t offset);

// 2 - manually clearing and step by step update 
//     (NB : 'step' MUST BE in [0..'tft.width()'] ; 
//     while 'offset' SHOULD BE within [O..'size-1'] (anyway, modulo is used) 
void display_clear();  // clear & draw background
void display_update_step(uint8_t *data, uint16_t size, uint16_t offset, uint16_t step); 

uint16_t display_width();
uint16_t display_height();

#ifdef __cplusplus
}
#endif 
#endif
