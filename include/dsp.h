#ifndef DSP_H
#define DSP_H

#include <stdint.h>
#include <math.h>
#include <string.h>

#define KERNEL_SIZE 3;

//helper methods

uint32_t RGB565_to_RGB888(uint16_t RGB);
uint16_t RGB888_to_RGB565(uint32_t RGB);

//image manipulation
void grayscale(volatile uint16_t * image);

void convolution(volatile uint16_t * image, int16_t * kernel, uint16_t * tmp_buffer);

void test(volatile uint16_t * image);
#endif
