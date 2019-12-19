#pragma once
#include "ViewPlane.h"
#include "math/Normal.h"
#include "ShadeRec.h"
#include "Shape/Shape.h"
#include <vector>
namespace BL {
	class World
	{

	public:
		ViewPlane viewPlane;
		RGBColor backgroundColor;
		Sphere* sphere;
		std::vector<Shape*> objects;

		World() {}

		void Build(void) {
			viewPlane.height = 200;
			viewPlane.width = 200;
			viewPlane.pixSize = 1.0f;
			viewPlane.gamma = 1.0f;

			backgroundColor = ColorBlack;
			sphere = new Sphere(Point3f(0, 0, 0), 85.0f);
			sphere->color = ColorBlue;
			objects.push_back(sphere);

			sphere = new Sphere(Point3f(0, 30, 0), 60);
			sphere->color = ColorRed;
			objects.push_back(sphere);

			Plane* p = new Plane(Point3f(0, 0, 0), Normal3f(0, 1, 1));
			p->color = ColorGreen;
			objects.push_back(p);
		}

		void RenderScene(unsigned char*& data, int& width, int& height) {

			height = viewPlane.height;
			width = viewPlane.width;
			data = new unsigned char[height * width * 3];

			RGBColor pixelColor;
			Ray ray;
			Float z = 100;
			ray.d = Vector3f(0, 0, -1);
			const int sampleNumPerPixer = 1;
			int n = (int)sqrt(sampleNumPerPixer);

			//Float halfH = 0.5f * (viewPlane.height - 1);
			//Float halfW = 0.5f * (viewPlane.width - 1);
			for (int h = 0; h < viewPlane.height; h++) {
				for (int w = 0; w < viewPlane.width; w++) {
					//pixelColor = _RegularSampling(ray, h, w, 5); 
					//pixelColor = _RandSampling(ray, h, w, 16); 
					pixelColor = _JitteredSampling(ray, h, w, 5);
					int p = (viewPlane.height - 1 - h) * (3 * width) + 3 * w;
					data[p] = pixelColor.r * 255.9f;
					data[p + 1] = pixelColor.g * 255.9f;
					data[p + 2] = pixelColor.b * 255.9f;
				}
			}
		}



		void openWindow(const int w, int h) {
		}

		void displayPixel(const int row, const int column, const RGBColor& pixelColor) const {
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
		RGBColor _RegularSampling(Ray& ray, int r, int c, int num) {
			RGBColor pixelColor = ColorBlack;
			for (int p = 0; p < num; p++) {
				for (int q = 0; q < num; q++) {
					Float x = viewPlane.pixSize * (c - 0.5f * viewPlane.width + (p + 0.5) / num);
					Float y = viewPlane.pixSize * (r - 0.5f * viewPlane.height + (q + 0.5) / num);
					ray.o = Point3f(x, y, 100);
					pixelColor += hitBareBonesObjects(ray).color;
				}
			}
			return pixelColor / (num * num);
		}

		RGBColor _RandSampling(Ray& ray, int r, int c, int num) {
			RGBColor pixelColor = ColorBlack;
			for (int p = 0; p < num; p++) {
				Float x = viewPlane.pixSize * (c - 0.5f * viewPlane.width + rand_float());
				Float y = viewPlane.pixSize * (r - 0.5f * viewPlane.height + rand_float());
				ray.o = Point3f(x, y, 100);
				pixelColor += hitBareBonesObjects(ray).color;
			}

			return pixelColor / num;
		}

		RGBColor _JitteredSampling(Ray& ray, int r, int c, int num) {
			RGBColor pixelColor = ColorBlack;
			for (int p = 0; p < num; p++) {
				for (int q = 0; q < num; q++) {
					Float x = viewPlane.pixSize * (c - 0.5f * viewPlane.width + (p + rand_float()) / num);
					Float y = viewPlane.pixSize * (r - 0.5f * viewPlane.height + (q + rand_float()) / num);
					ray.o = Point3f(x, y, 100);
					pixelColor += hitBareBonesObjects(ray).color;
				}
			}
			return pixelColor / (num * num);
		}
	};
}