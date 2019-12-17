#pragma once
#include "ViewPlane.h"
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
		std::vector<Sphere*> objects;

		World() {}

		void Build(void) {
			viewPlane.height = 200;
			viewPlane.width = 200;
			viewPlane.pixSize = 1.0f;
			viewPlane.gamma = 1.0f;

			backgroundColor = ColorBlack;
			sphere = new Sphere(Point3f(0, 0, 0), 85.0f);
			sphere->color = ColorRed;
			objects.push_back(sphere);
		}

		void RenderScene(unsigned char* &data, int& width, int& height) const {

			height = viewPlane.height;
			width = viewPlane.width;
			data = new unsigned char[height * width * 3];

			RGBColor pixelColor;
			Ray ray;
			Float z = 100;
			ray.d = Vector3f(0, 0, -1);

			Float halfH = 0.5f * (viewPlane.height - 1);
			Float halfW = 0.5f * (viewPlane.width - 1);
			for (int h = 0; h < viewPlane.height; h++) {
				for (int w = 0; w < viewPlane.width; w++) {
					Float x = viewPlane.pixSize * (w - halfW);
					Float y = viewPlane.pixSize * (h - halfH);
					ray.o = Point3f(x, y, 100);
					pixelColor = hitBareBonesObjects(ray).color;
					int p = h * (3 * width) + 3 * w;
					data[p] = pixelColor.r*255.9f;
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
	};
}