// RayTracing.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
int main()
{
	std::cout << "Hello   ddddd World!\n";
	std::string st;
	int width = 200;
	int heigth = 200;
	int chanle = 3;
	unsigned char* data = new unsigned char[width * heigth * chanle];
	for (int h = 0; h < heigth; h++)
	{
		for (int w = 0; w < width; w++)
		{
			int  r = 255.99 * w / width;
			int  g =  255.99 * h / heigth;
			int  b = 0;// 255.99 * 0.2f;
			int p = h * (chanle * width) + chanle * w;
			data[p] = r;
			data[p + 1] = g;
			data[p + 2] = b;
		}
	}
	stbi_write_png("ret.png", width, heigth, chanle, data, width * chanle);
}
