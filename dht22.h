#ifndef __DHT_22_H__
#define __DHT_22_H__

void dht22_init();

#define DHT22_OK 0
#define DHT22_ERR_NOLOWSTATE 1
#define DHT22_ERR_CHECKSUM 2
#define DHT22_ERR_TIMEOUT 3

uint8_t dht22_read(int16_t *temperature, int16_t *humidity);

#endif
