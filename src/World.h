#pragma once
#ifndef __World_H__
#define __World_H__


#include "ViewPlane.h"
#include "math/Normal.h"
#include "ShadeRec.h"
#include "Shape/Shape.h"
#include "Sampler.h"
#include <vector>
#include "Light.h"
#include "Material.h";
namespace BL {

//#define  AREALIGHT_TRACE

	class World
	{

	public:
		ViewPlane viewPlane;
		RGBColor backgroundColor;
		std::vector<Shape*> objects;
		Sampler* sampler;
		unsigned char* dislayPixelData;
		Camera* camera;

		Light* ambiant;

		std::vector<Light*> lights;

		static World* GetInstance();

		~World() {
			if (dislayPixelData != NULL) {
				delete[] dislayPixelData;
			}
		}

		Light* GetAmbient() {
			return ambiant;
		}

		void SetAmbiant(Light* ambient) {
			this->ambiant = ambient;
		}

		void AddObject(Shape* shape) {
			objects.push_back(shape);
		}

		void AddLight(Light* light) {
			lights.push_back(light);
		}

		void Build(void);

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

		void RenderPerspective();

		void GetDisplayPixelData(unsigned char*& data, int& width, int& height)const {
			data = dislayPixelData;
			width = viewPlane.width;
			height = viewPlane.height;
		}

		void openWindow(const int w, int h) {
		}

		void displayPixel(const int row, const int column, RGBColor& pixelColor) {
			int p = (viewPlane.height - 1 - row) * (3 * viewPlane.width) + 3 * column;
			pixelColor.Clamp();
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
		
		RGBColor GetTraceRayColor(const Ray& ray, const int depth = 1) const {
			ShadeRec sr = HitObjects(ray);
			if (sr.hitAnObject) {
				sr.ray = ray;
#ifdef AREALIGHT_TRACE
				return sr.material->AreaLightShade(sr);
#else
				return sr.material->Shade(sr);
#endif // AREALIGHT_TRACE

			}
			else
				return backgroundColor;
		}

		ShadeRec HitObjects(const Ray& ray) const {
			ShadeRec sr;
			sr.color = backgroundColor;
			Float t, tMin = MaxFloat;
			Normal3f normal;
			Material* mat = NULL;
			Point3f localHitPoint;
			for (int i = 0; i < objects.size(); i++) {
				if (objects[i]->Hit(ray, t, sr) && t < tMin) {
					sr.hitAnObject = true;
					
					tMin = t;
					normal = sr.normal;
					mat = sr.material;
					localHitPoint = sr.localHitPoint;
				}
			}
			if (sr.hitAnObject)
			{
				sr.ray = ray;
				sr.localHitPoint = localHitPoint;
				sr.rayT = tMin;
				sr.normal = normal;
				sr.material = mat;
			}
			return sr;
		}

	private:
		static World* instance;
		World() :dislayPixelData(NULL) {
		}

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
			RGBColor pixelColor;
			for (int i = 0; i < num; i++) {
				Point2f p = sampler->SampleUnitSquare();
				Float x = viewPlane.pixSize * (c - 0.5f * viewPlane.width + p.x);
				Float y = viewPlane.pixSize * (r - 0.5f * viewPlane.height + p.y);
				ray.o = Point3f(x, y, 100);
				pixelColor += hitBareBonesObjects(ray).color;
			}
			return pixelColor / num;
		}
	};


	
#define GetWorldPtr  World::GetInstance()
}
#endif//__World_H__