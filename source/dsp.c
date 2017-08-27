#include "dsp.h"


double sRGB_to_linear(double x) {
	if (x < 0.04045) return x / 12.92;
	return pow((x + 0.055) / 1.055, 2.4);
}

double linear_to_sRGB(double y) {
	if (y <= 0.0031308) return 12.92 * y;
	return 1.055 * pow(y, 1 / 2.4) - 0.055;
}

uint32_t RGB565_to_RGB888(uint16_t RGB)
{
	//split the 16 bits to 5 - 6 - 5 RGB components
	uint8_t R, R8 = 0;
	uint8_t G, G8 = 0;
	uint8_t B, B8 = 0;

	R = (RGB & 0xf800) >> 11;
	G = (RGB & 0x07e0) >> 5;
	B = (RGB & 0x001f);

	//convert to linear space

	R8 = (R * 527 + 23) >> 6;
	G8 = (G * 259 + 33) >> 6;
	B8 = (B * 527 + 23) >> 6;

	uint32_t rgb888 = 0;

	rgb888 = rgb888 | R8;
	rgb888 = rgb888 << 8;

	rgb888 = rgb888 | G8;
	rgb888 = rgb888 << 8;

	rgb888 = rgb888 | B8;
	rgb888 = rgb888 << 8;


	return rgb888;
}


uint16_t RGB888_to_RGB565(uint32_t RGB)
{

	//split the 24 bit components

	uint8_t R8 = (RGB & 0xff000000) >> 24;
	uint8_t B8 = (RGB & 0x00ff0000) >> 16;
	uint8_t G8 = (RGB & 0x0000ff00) >> 8;

	uint8_t R5 = (R8 * 249 + 1014) >> 11;
	uint8_t G6 = (G8 * 253 + 505) >> 10;
	uint8_t B5 = (B8 * 249 + 1014) >> 11;

	uint16_t result = 0;

	result |= R5;
	result = result << 6;
	result = result | G6;
	result = result << 5;
	result = result | B5;

	return result;
}


uint16_t RGB565_to_Grayscale(uint16_t RGB)
{
	//split the 16 bits to 5 - 6 - 5 RGB components
//	uint8_t R = 0;
//	uint8_t G = 0;
//	uint8_t B = 0;

//	R = (RGB & 0xf800) >> 11;
//	G = (RGB & 0x07e0) >> 5;
//	B = (RGB & 0x001f);

//	//conver to linear space
//	

//	double R_linear = sRGB_to_linear(R / 255.0);
//	double G_linear = sRGB_to_linear(G / 255.0);
//	double B_linear = sRGB_to_linear(B / 255.0);

//	double gray_linear = 0.2126 * R_linear + 0.7152 * G_linear + 0.0722 * B_linear;

//	double gray_color = round(linear_to_sRGB(gray_linear) * 255);

//	return (uint16_t)gray_color;
	return 0;
}

void grayscale(volatile uint16_t * image)
{
	
	for(int i = 0; i < 76800; i++)
	{
		uint32_t rgba888 = RGB565_to_RGB888(image[i]);
		
		uint8_t red = 	((rgba888 & 0xff000000) >> 24 );
		uint8_t green = ((rgba888 & 0x00ff0000) >> 16 );
		uint8_t blue = 	((rgba888 & 0x0000ff00) >> 8 );
		
		double gray_linear = 0.2126 * red + 0.7152 * green + 0.0722 * blue;
		uint32_t grayRGB = (uint32_t) gray_linear;
		
		uint32_t rgbGrayscaled = 0;
		
		rgbGrayscaled = rgbGrayscaled | grayRGB;
		rgbGrayscaled = rgbGrayscaled << 8;
		
		rgbGrayscaled = rgbGrayscaled | grayRGB;
		rgbGrayscaled = rgbGrayscaled << 8;
		
		rgbGrayscaled = rgbGrayscaled | grayRGB;
		rgbGrayscaled = rgbGrayscaled << 8;
		
		image[i] = RGB888_to_RGB565(rgbGrayscaled);
	}
	
}

void test(volatile uint16_t * image)
{
	memset((uint16_t *)image,0,320 * 240 * 2);
}

void convolution(volatile uint16_t * image, int16_t * kernel)
{
	//uint32_t image_size = width * height;
	//int image_byte_size = image_size * 16; // 5 6 5 RGB
	
	uint16_t kernelPointNumber = 3 * 3;
	uint16_t centerPointIndex = 1;
	uint16_t IMG_ROWS = 100;
	uint16_t IMG_COLUMNS = 240;
	uint16_t normalization = 16;

	for(uint16_t i = 0; i < IMG_ROWS - 3; i++)
	{
		uint16_t tmp_counter = 0;
		volatile uint16_t tmp_buffer[IMG_COLUMNS * IMG_ROWS];
		
		for (uint16_t j = 0; j < IMG_COLUMNS - 3; j++)
		{
			volatile int32_t accomulatorR = 0;
			volatile int32_t accomulatorG = 0;
			volatile int32_t accomulatorB = 0;
			
			for (uint8_t k = 0; k < 3; k++) 
			{
				for (uint8_t m = 0; m < 3; m++) 
				{
					uint16_t kernelOffset = k * 3 + m;
					
					volatile uint32_t rgba888 = RGB565_to_RGB888(image[(i + k) * IMG_COLUMNS + j + m]);
					
					accomulatorR += kernel[kernelOffset] * ((rgba888 & 0xff000000) >> 24 );
					accomulatorG += kernel[kernelOffset] * ((rgba888 & 0x00ff0000) >> 16 );
					accomulatorB += kernel[kernelOffset] * ((rgba888 & 0x0000ff00) >> 8 );
					
				}
			}
			
			uint8_t averageR = (uint8_t) accomulatorR / normalization;
			uint8_t averageG = (uint8_t) accomulatorG / normalization;
			uint8_t averageB = (uint8_t) accomulatorB / normalization;
			
			uint32_t tmp = 0;
			
			tmp = (tmp | averageR) << 8;
			tmp = (tmp | averageG) << 8;
			tmp =(tmp | averageB) << 8;
			
			tmp_buffer[tmp_counter++] = RGB888_to_RGB565(tmp);
			
		}
		// copy to image buffer
		memcpy((uint16_t*)&image[0], (uint16_t *)tmp_buffer, sizeof(tmp_buffer));

	}
	
}