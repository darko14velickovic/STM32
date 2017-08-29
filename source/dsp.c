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
	for(int i = 0; i < 76800; i++)
	{
		uint32_t rgba888 = RGB565_to_RGB888(image[i]);
		
		uint8_t red = 	((rgba888 & 0xff000000) >> 24 );
		uint8_t green = ((rgba888 & 0x00ff0000) >> 16 );
		uint8_t blue = 	((rgba888 & 0x0000ff00) >> 8 );
		
	
		uint32_t rgbGrayscaled = 0;
		
		rgbGrayscaled = rgbGrayscaled | red;
		rgbGrayscaled = rgbGrayscaled << 8;
		
		rgbGrayscaled = rgbGrayscaled | green;
		rgbGrayscaled = rgbGrayscaled << 8;
		
		rgbGrayscaled = rgbGrayscaled | blue;
		rgbGrayscaled = rgbGrayscaled << 8;
		
		image[i] = RGB888_to_RGB565(rgbGrayscaled);
	}
}

void convolution(volatile uint16_t * image, int16_t * kernel, uint16_t * tmp_buffer)
{
	
	uint16_t IMG_ROWS = 240;
	uint16_t IMG_COLUMNS = 100;
	int16_t normalization = 0;
	uint8_t kernelSize = 5;
	uint16_t centerPointIndex = 2;
	
	for(uint8_t i = 0; i < kernelSize * kernelSize; i++)
	{
		normalization += kernel[i];
	}
	
	volatile uint8_t rgbRed = 0;
	volatile uint8_t rgbGreen = 0;
	volatile uint8_t rgbBlue = 0;
	
	for(uint16_t i = 0; i < IMG_ROWS - kernelSize; i++)
	{
		//uint16_t tmp_counter = 0;
		
		
		for (uint16_t j = 0; j < IMG_COLUMNS - kernelSize; j++)
		{
			volatile int32_t accomulatorR = 0;
			volatile int32_t accomulatorG = 0;
			volatile int32_t accomulatorB = 0;
			
			for (uint8_t k = 0; k < kernelSize; k++) 
			{
				for (uint8_t m = 0; m < kernelSize; m++) 
				{
					uint16_t kernelOffset = k * kernelSize + m;
					
					volatile uint32_t rgba888 = RGB565_to_RGB888(image[(i + k) * IMG_COLUMNS + j + m]);
					
					rgbRed = ((rgba888 & 0xff000000) >> 24 );
					rgbGreen = ((rgba888 & 0x00ff0000) >> 16 );
					rgbBlue = ((rgba888 & 0x0000ff00) >> 8 );
					
					accomulatorR += kernel[kernelOffset] * rgbRed;
					accomulatorG += kernel[kernelOffset] * rgbGreen;
					accomulatorB += kernel[kernelOffset] * rgbBlue;
					
				}
			}
			
			uint8_t averageR = (uint8_t) (accomulatorR / normalization);
			uint8_t averageG = (uint8_t) (accomulatorG / normalization);
			uint8_t averageB = (uint8_t) (accomulatorB / normalization);
			
			uint32_t tmp = 0;
			
			tmp = (tmp | averageR) << 8;
			tmp = (tmp | averageG) << 8;
			tmp =(tmp | averageB) << 8;
			
			tmp_buffer[ (i+centerPointIndex) * IMG_COLUMNS + j + centerPointIndex] = RGB888_to_RGB565(tmp);
			
		}

	}
	//memcpy((uint16_t*)image, tmp_buffer, sizeof(tmp_buffer));
}
