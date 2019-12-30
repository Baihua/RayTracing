#pragma once
#include "ViewPlane.h"
#include "math/Normal.h"
#include "ShadeRec.h"
#include "Shape/Shape.h"
#include "Sampler.h"
#include <vector>
#include "Camera.h"
#include "Light.h"
#include "Material.h";
namespace BL {
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

		World() :dislayPixelData(NULL) {

		}
		~World() {
			if (dislayPixelData != NULL) {
				delete[] dislayPixelData;
			}
		}


		void AddObject(Shape* shape) {
			objects.push_back(shape);
		}

		void AddLight(Light* light) {
			lights.push_back(light);
		}

		void Build(void) {
			viewPlane.height = 200;
			viewPlane.width = 200;
			viewPlane.pixSize = 1.0f;
			viewPlane.gamma = 1.0f;

			//camera = new ThinLen(2.0f, 300.0f);0
			//camera = new Fisheye(180 * Pi / 180);
			camera = new BL::Pinhole();
			camera->SetEyePosition(Vector3f(0, 40, 100));
			camera->SetLookAt(Vector3f(0, 0, 0));
			camera->SetUp(Vector3f(0, 1, 0));
			camera->SetViewDistance(100);
			camera->ComputeUVW();

			backgroundColor = ColorBlack;


			Ambient* ambiant = new Ambient();
			this->ambiant = ambiant;

			PointLight* ptLight = new PointLight();
			ptLight->SetPosition(Point3f(100, 50, 1500));
			lights.push_back(ptLight);

			Matte* matte = new Matte();
			matte->SetKOfAmbient(0.25f);
			matte->SetKOfDiffuse(0.65);
			matte->SetCd(ColorBlue);
			Sphere* sphere = new Sphere(Point3f(-40, 0, -20), 40);
			AddObject(sphere);


			matte = new Matte();
			matte->SetKOfAmbient(0.25f);
			matte->SetKOfDiffuse(0.65);
			matte->SetCd(ColorRed);
			sphere = new Sphere(Point3f(0, 0, -140), 40);
			sphere->material = matte;
			AddObject(sphere);

			matte = new Matte();
			matte->SetKOfAmbient(0.25f);
			matte->SetKOfDiffuse(0.65);
			matte->SetCd(ColorGray);
			sphere = new Sphere(Point3f(80, 0, -260), 40);
			sphere->material = matte;
			AddObject(sphere);

			matte = new Matte();
			matte->SetKOfAmbient(0.25f);
			matte->SetKOfDiffuse(0.65);
			matte->SetCd(ColorGray);
			Plane* p = new Plane(Point3f(0, -50, 0), Normal3f(0, 100, 1));
			p->material = matte;
			AddObject(p);

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
			camera->RenderScene(this);
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
		
		RGBColor GetTraceRayColor(const Ray& ray, const int depth = 1) const {
			ShadeRec sr = HitObjects(ray);
			if (sr.hitAnObject) {
				sr.ray = ray;
				return sr.material->Shade(sr);
			}
			else
				return backgroundColor;
		}

		ShadeRec HitObjects(const Ray& ray) const {
			ShadeRec sr;
			sr.color = backgroundColor;
			Float t, tMin = MaxFloat;
			Normal3f normal;
			Point3f localHitPoint;
			for (int i = 0; i < objects.size(); i++) {
				if (objects[i]->Hit(ray, t, sr) && t < tMin) {
					sr.hitAnObject = true;
					tMin = t;
					sr.material = objects[i]->material;
					if (sr.material == NULL)
					{
						printf("nllllllll");
					}
					sr.normal = sr.normal;
					sr.localHitPoint = sr.localHitPoint;
					sr.rayT = tMin;
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

	static World* GetWorldPtr = new World();
}