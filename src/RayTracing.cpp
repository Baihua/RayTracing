﻿// RayTracing.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#include <iostream>
#include <string>
#include "math/Vector.h"
#include "math/Point.h"
#include "Shape/Shape.h"
#include "Sampler.h"
#include "Camera.h"
#include "World.h"
#include "bl.h"

int main()
{


	std::cout << "Begin Tracing\n";

	

	BL::Sampler* s = new BL::JitteredSampler(BL::SampleNumPerPixes,2);
	//BL::Sampler* s = new BL::RandomSampler(36,2);
	//BL::Sampler* s = new BL::NRooksSampler(36,2);
	//BL::Sampler* s = new BL::HammersleySampler(100, 2);
	s->GenerateSamples();
	//s->MapSamplesToUnitDisk();
	//s->DebugToIntuition("hammersaley_s.bmp", false);
	//s->DebugToIntuition("hammersaley_disk.bmp", true);
	
	BL::GetWorldPtr->sampler = s;
	BL::GetWorldPtr->Build();


	unsigned char* colorData = NULL;
	int size = 0, w = 0, h=0;
	BL::GetWorldPtr->RenderPerspective();
	//BL::GetWorldPtr->RenderScene();
	BL::GetWorldPtr->GetDisplayPixelData(colorData, w, h);
	stbi_write_png("testOutput/arealight.png", w, h, 3, colorData, w * 3);
	colorData = NULL;
	std::cout << "End Check your output\n";

}