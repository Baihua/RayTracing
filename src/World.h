#pragma once
#include "ViewPlane.h"
#include "math/Normal.h"
#include "ShadeRec.h"
#include "Shape/Shape.h"
#include "Sampler.h"
#include <vector>
namespace BL {
	class World
	{

	public:
		ViewPlane viewPlane;
		RGBColor backgroundColor;
		Sphere* sphere;
		std::vector<Shape*> objects;
		Sampler* sampler;
		unsigned char* dislayPixelData;
		World() :dislayPixelData(NULL) {
		}
		~World() {
			if (dislayPixelData != NULL) {
				delete[] dislayPixelData;
			}
		}
		void Build(void) {
			viewPlane.height = 200;
			viewPlane.width = 200;
			viewPlane.pixSize = 1.0f;
			viewPlane.gamma = 1.0f;

			backgroundColor = ColorBlack;
			sphere = new Sphere(Point3f(-40, 0, 0), 40);
			sphere->color = ColorBlue;
			objects.push_back(sphere);

			sphere = new Sphere(Point3f(40, 0, 0), 40);
			sphere->color = ColorRed;
			objects.push_back(sphere);

			Plane* p = new Plane(Point3f(0, 0, 0), Normal3f(0, 1, 1));
			p->color = ColorGreen;
			//objects.push_back(p);

			if (dislayPixelData != NULL) {
				delete[] dislayPixelData;
			}
			dislayPixelData = new unsigned char[viewPlane.height * viewPlane.width * 3];
			memset(dislayPixelData, 0, viewPlane.height * viewPlane.width * 3);
		}

		void RenderScene() {
			RGBColor pixelColor;
			Ray ray;
			Float z = 100;
			ray.d = Vector3f(0, 0, -1);
			const int sampleNumPerPixer = 1;
			int n = (int)sqrt(sampleNumPerPixer);
			for (int h = 0; h < viewPlane.height; h++) {
				for (int w = 0; w < viewPlane.width; w++) {
					pixelColor = _GetColor(ray, h, w);
					displayPixel(h, w, pixelColor);
				}
			}
		}

		void RenderPerspective() {
			RGBColor pixelColor;
			Ray ray;
			Float z = 100;
			Float eye = 100;
			ray.o = Point3f(0.0f, 0.0f, eye);

			const int sampleNumPerPixer = 1;
			int n = (int)sqrt(sampleNumPerPixer);
			for (int h = 0; h < viewPlane.height; h++) {
				for (int w = 0; w < viewPlane.width; w++) {
					ray.d = Vector3f(w - 0.5f * viewPlane.width + 0.5f, h - 0.5f * viewPlane.height + 0.5f, -50);
					ray.d.Normalize();
					pixelColor = hitBareBonesObjects(ray).color;
					displayPixel(h, w, pixelColor);
				}
			}
		}

		void GetDisplayPixelData(unsigned char*& data, int& width, int& height)const {
			data = dislayPixelData;
			width = viewPlane.width;
			height = viewPlane.height;
		}

		void openWindow(const int w, int h) {
		}

		void displayPixel(const int row, const int column, const RGBColor& pixelColor) {
			int p = (viewPlane.height - 1 - row) * (3 * viewPlane.width) + 3 * column;
			dislayPixelData[p] = pixelColor.r * 255.9f;
			dislayPixelData[p + 1] = pixelColor.g * 255.9f;
			dislayPixelData[p + 2] = pixelColor.b * 255.9f;
		}

		ShadeRec hitBareBonesObjects(const Ray& ray)const {
			Float t, tmin = MaxFloat;
			ShadeRec sr;
			for (int i = 0; i < objects.size(); i++) {
				if (objects[i]->Hit(ray, t, sr) && t < tmin) {
					sr.hitAnObject = true;
					tmin = t;
					sr.color = objects[i]->color;
				}
			}
			return sr;
		}
	private:
		RGBColor _GetColor(Ray& ray, int r, int c)
		{
			if (sampler == NULL)
			{
				Float x = viewPlane.pixSize * (c - 0.5f * viewPlane.width + 0.5f);
				Float y = viewPlane.pixSize * (r - 0.5f * viewPlane.height + 0.5f);
				ray.o = Point3f(x, y, 100);
				return hitBareBonesObjects(ray).color;
			}
			int num = sampler->GetNumSamples();
			RGBColor pixelColor ;
			for (int i = 0; i < num; i++) {
				Point2f p = sampler->SampleUnitSquare();
				Float x = viewPlane.pixSize * (c - 0.5f * viewPlane.width + p.x);
				Float y = viewPlane.pixSize * (r - 0.5f * viewPlane.height + p.y);
				ray.o = Point3f(x, y, 100);
				pixelColor += hitBareBonesObjects(ray).color;
			}
			return pixelColor / num;
		}

		//RGBColor _RegularSampling(Ray& ray, int r, int c, int num) {
		//	RGBColor pixelColor = ColorBlack;
		//	for (int p = 0; p < num; p++) {
		//		for (int q = 0; q < num; q++) {
		//			Float x = viewPlane.pixSize * (c - 0.5f * viewPlane.width + (p + 0.5) / num);
		//			Float y = viewPlane.pixSize * (r - 0.5f * viewPlane.height + (q + 0.5) / num);
		//			ray.o = Point3f(x, y, 100);
		//			pixelColor += hitBareBonesObjects(ray).color;
		//		}
		//	}
		//	return pixelColor / (num * num);
		//}

		//RGBColor _RandSampling(Ray& ray, int r, int c, int num) {
		//	RGBColor pixelColor = ColorBlack;
		//	for (int p = 0; p < num; p++) {
		//		Float x = viewPlane.pixSize * (c - 0.5f * viewPlane.width + rand_float());
		//		Float y = viewPlane.pixSize * (r - 0.5f * viewPlane.height + rand_float());
		//		ray.o = Point3f(x, y, 100);
		//		pixelColor += hitBareBonesObjects(ray).color;
		//	}

		//	return pixelColor / num;
		//}

		//RGBColor _JitteredSampling(Ray& ray, int r, int c, int num) {
		//	RGBColor pixelColor = ColorBlack;
		//	for (int p = 0; p < num; p++) {
		//		for (int q = 0; q < num; q++) {
		//			Float x = viewPlane.pixSize * (c - 0.5f * viewPlane.width + (p + rand_float()) / num);
		//			Float y = viewPlane.pixSize * (r - 0.5f * viewPlane.height + (q + rand_float()) / num);
		//			ray.o = Point3f(x, y, 100);
		//			pixelColor += hitBareBonesObjects(ray).color;
		//		}
		//	}
		//	return pixelColor / (num * num);
		//}
	};
}