#ifndef DSP_H
#define DSP_H

#include <stdint.h>
#include <math.h>
#include <string.h>

//#define KERNEL_SIZE 5;
//#define WINDOW_WIDTH 100 - KERNEL_SIZE;
//#define WINDOW_HEIGHT 100 - KERNEL_SIZE;

//helper methods

uint32_t RGB565_to_RGB888(uint16_t RGB);
uint16_t RGB888_to_RGB565(uint32_t RGB);

//image manipulation
void grayscale(volatile uint16_t * image);

void convolution(volatile uint16_t * image, int16_t * kernel, uint16_t * tmp_buffer, uint16_t startRow, uint16_t startColumn);

void convolutionGray(volatile uint16_t * image, int16_t * kernel, uint16_t * tmp_buffer, uint16_t startRow, uint16_t startColumn);

void test(volatile uint16_t * image);
#endif
