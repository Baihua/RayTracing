﻿// RayTracing.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#include "math/Vector.h"
#include "math/Point.h"
#include "Shape/Shape.h"
#include "World.h"

int main()
{
	

	std::cout << "what is s , Hello World! honge edd test\n";

	std::cout << "2Hello World! honge edd test\n";


	BL::World world;
	
	world.Build();
	unsigned char* colorData = NULL;
	int size = 0, w = 0, h=0;
	world.RenderScene(colorData, w, h);
	stbi_write_png("ret.png", w, h, 3, colorData, w * 3);
	delete colorData;
	
	//int width = 200;
	//int heigth = 200;
	//int chanle = 3;
	//unsigned char* data = new unsigned char[width * heigth * chanle];
	//for (int h = 0; h < heigth; h++)
	//{
	//	for (int w = 0; w < width; w++)
	//	{
	//		int  r = 255.99 * w / width;
	//		int  g = 255.99 * h / heigth;
	//		int  b = 0;// 255.99 * 0.2f;
	//		int p = h * (chanle * width) + chanle * w;
	//		data[p] = r;
	//		data[p + 1] = g;
	//		data[p + 2] = b;
	//	}
	//}
	//stbi_write_png("ret.png", width, heigth, chanle, data, width * chanle);
}