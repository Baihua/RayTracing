#pragma once
#include "math/Point.h"
//#define STB_IMAGE_WRITE_IMPLEMENTATION
//#include "stb/stb_image_write.h"
#include <vector>
namespace BL {
	class Sampler {
	public:
		Sampler() :numSamples(1), numSets(1) {}
		Sampler(int numSamples, int sets) :numSamples(numSamples), numSets(sets) {}
		virtual void GenerateSamples() = 0;

		void SetupShuffledIndices() {}

		void ShuffleSamples() {}

		Point2f SampleUnitSquare() {
			if (count % numSamples == 0) {
				jump = (randInt() % numSets) * numSamples;
			}
			return samples[jump + count++ % numSamples];
		}

		void MapSamplesToUnitDisk() {
			diskSamples.reserve(samples.size());
			Float r = 0;
			Float phi = 0;
			for (auto i = 0; i < samples.size(); i++)
			{
				Float x = 2 * samples[i].x - 1;
				Float y = 2 * samples[i].y - 1;

				if (x > -y)
				{
					if (x > y)
					{
						r = x;
						phi = PiOver4 * y / x;
					}
					else
					{
						r = y;
						phi = PiOver4 * (2 - x / y);
					}
				}
				else
				{
					if (x < y) {
						r = -x;
						phi = PiOver4 * (4 + y / x);
					}
					else {
						r = -y;
						phi = PiOver4 * (6 - x / y);
					}
				}
				diskSamples.push_back(Point2f((r * cos(phi) + 1) / 2, (r * sin(phi) + 1) / 2));
			}
		}

		void MapSamplesToHimishpere(const Float e)
		{
			hemisphereSamples.reserve(samples.size());
			for (auto i = 0; i < samples.size(); i++) {

				Float phi = 2 * Pi * samples[i].x;

				Float cosPhi = cos(phi);
				Float sinPhi = sin(phi);

				Float cosTheta = pow((1 - samples[i].y), 1.0f / (e + 1));
				Float sinTheta = sqrt(1 - cosTheta * cosTheta);
				Float px = sinTheta * cosPhi;
				Float py = cosTheta * sinPhi;
				Float pz = cosTheta;
				hemisphereSamples.push_back(Point3f(px, py, pz));
			}
		}

		void DebugToIntuition(const char* file, bool isDisk = false)//用1000*1000的png显示
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

		int GetNumSamples() { return numSamples; }
	protected:
		int numSamples;
		int numSets;
		std::vector<Point2f> samples;
		std::vector<Point2f> diskSamples;
		std::vector<Point3f> hemisphereSamples;

		size_t count;
		size_t jump;
	};

	class RandomSampler : public Sampler
	{
	public:

		RandomSampler() :Sampler() {}

		RandomSampler(int numSamples, int numSets) :Sampler(numSamples, numSets) {}

		void GenerateSamples() {
			for (size_t i = 0; i < numSets; i++) {
				for (size_t x = 0; x < numSamples; x++) {
					Point2f p(rand_float(), rand_float());
					samples.push_back(p);
				}
			}
		}
	};

	class JitteredSampler : public Sampler //如果采样数不是某个数的平方，则会改变
	{
	public:

		JitteredSampler() :Sampler() {}

		JitteredSampler(int numSamples, int numSets) :Sampler(numSamples, numSets) {}

		void GenerateSamples() {
			int n = (int)sqrt(numSamples);
			numSamples = n * n;
			for (size_t i = 0; i < numSets; i++) {
				for (size_t x = 0; x < n; x++) {
					for (size_t y = 0; y < n; y++) {
						Point2f p((x + rand_float()) / n, (y + rand_float()) / n);
						//Point2f p((x + 0.5) / n, (y + 0.5) / n);
						samples.push_back(p);
					}
				}
			}
		}
	};


	class NRooksSampler : public Sampler
	{
	public:

		NRooksSampler() :Sampler() {}

		NRooksSampler(int numSamples, int numSets) :Sampler(numSamples, numSets) {}

		void GenerateSamples() {
			for (size_t i = 0; i < numSets; i++) {
				for (size_t j = 0; j < numSamples; j++) {
					Point2f p((j + rand_float()) / numSamples, (j + rand_float()) / numSamples);
					samples.push_back(p);
				}
			}
			ShuffleXCoordinates();
			ShuffleYCoordinates();
		}
	private:
		void ShuffleXCoordinates() {
			for (size_t i = 0; i < numSets; i++) {
				for (size_t j = 0; j < numSamples; j++) {
					int t = randInt() % numSamples + i * numSamples;
					float temp = samples[j + i * numSamples].x;
					samples[j + i * numSamples].x = samples[t].x;
					samples[t].x = temp;
				}
			}
		}
		void ShuffleYCoordinates() {
			for (size_t i = 0; i < numSets; i++) {
				for (size_t j = 0; j < numSamples; j++) {
					int t = randInt() % numSamples + i * numSamples;
					float temp = samples[j + i * numSamples].y;
					samples[j + i * numSamples].y = samples[t].y;
					samples[t].y = temp;
				}
			}
		}
	};
	class HammersleySampler : public Sampler
	{
	public:

		HammersleySampler() :Sampler() {}

		HammersleySampler(int numSamples, int numSets) :Sampler(numSamples, numSets) {}

		void GenerateSamples() {
			for (size_t i = 0; i < numSets; i++) {
				for (size_t x = 0; x < numSamples; x++) {
					Point2f p(x / (float)numSamples, getPhi(x));
					samples.push_back(p);
				}
			}
		}
	private:
		Float getPhi(int j)
		{
			Float x = 0.0f;
			Float f = 0.5f;
			while (j != 0)
			{
				x += f * (j & 1);
				f *= 0.5f;
				j /= 2;
			}
			return x;
		}
	};

}

