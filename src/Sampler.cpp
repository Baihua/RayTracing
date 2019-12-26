#include "Sampler.h"
#include "stb/stb_image_write.h"
namespace BL {

	void Sampler::DebugToIntuition(const char* file, bool isDisk)//”√1000*1000µƒpngœ‘ æ
	{
		std::vector<Point2f> s;
		if (isDisk)
			s = diskSamples;
		else
			s = samples;
		int n = (randInt() % numSets) * numSamples;
		unsigned char* data = new unsigned char[100 * 100];
		memset(data, 0, 100 * 100);
		for (size_t i = 0; i < numSamples; i++)
		{
			int x = s[n + i].x * 100;
			int y = s[n + i].y * 100;
			data[(100 - 1 - y) * 100 + x] = 255;
		}
		stbi_write_bmp(file, 100, 100, 1, data);
		delete[] data;
	}
}