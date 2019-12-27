#include "Camera.h"
#include "World.h"
namespace BL {

	void Pinhole::RenderScene(World* world) {
		Float width = world->viewPlane.width;
		Float height = world->viewPlane.height;

		Sampler* sampler = world->sampler;
		Point2f p;
		int numSamples = sampler->GetNumSamples();
		Ray ray;
		ray.o = Point3f(eyePosition.x, eyePosition.y, eyePosition.z);
		for (int r = 0; r < height; r++) {
			for (int c = 0; c < width; c++) {
				RGBColor color(0, 0, 0);
				for (auto s = 0; s < numSamples; s++) {

					Point2f samplerPoint = sampler->SampleUnitSquare();
					Float x = c - 0.5f * width + samplerPoint.x;
					Float y = r - 0.5f * height + samplerPoint.y;
					ray.d = x * u + y * v - d * w;
					ray.d.Normalize();
					color += world->hitBareBonesObjects(ray).color;
				}
				color /= numSamples;
				world->displayPixel(r, c, color);
			}
		}
	}

	void ThinLen::RenderScene(World* world) {
		Float width = world->viewPlane.width;
		Float height = world->viewPlane.height;

		Sampler* sampler = world->sampler;
		sampler->MapSamplesToUnitDisk();

		Point2f p;
		int numSamples = sampler->GetNumSamples();
		Ray ray;
		ray.o = Point3f(eyePosition.x, eyePosition.y, eyePosition.z);
		Point2f pixelPt;
		for (int r = 0; r < height; r++) {
			for (int c = 0; c < width; c++) {
				RGBColor color(0, 0, 0);
				for (auto s = 0; s < numSamples; s++) {

					Point2f samplerPoint = sampler->SampleUnitSquare();
					pixelPt.x = c - 0.5f * width + samplerPoint.x;
					pixelPt.y = r - 0.5f * height + samplerPoint.y;

					Point2f lenPt = lenRadius * sampler->SampleUnitDisk();//镜头位置

					ray.o = (Point3f)(eyePosition + lenPt.x * u + lenPt.y * v);//

					//--计算在焦点平面的位置。
					Point2f p = (FocusPlenFar / d) * pixelPt;
					//然后计算出射线方向。
					ray.d = (p.x - lenPt.x) * u + (p.y - lenPt.y) * v - FocusPlenFar * w;
					ray.d.Normalize();

					color += world->hitBareBonesObjects(ray).color;
				}
				color /= numSamples;
				world->displayPixel(r, c, color);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	void Fisheye::RenderScene(World* world) {
		Float width = world->viewPlane.width;
		Float height = world->viewPlane.height;

		Sampler* sampler = world->sampler;
		sampler->MapSamplesToUnitDisk();

		Point2f p;
		int numSamples = sampler->GetNumSamples();
		Ray ray;
		ray.o = Point3f(eyePosition.x, eyePosition.y, eyePosition.z);
		Point2f pixelPt;
		for (int r = 0; r < height; r++) {
			for (int c = 0; c < width; c++) {
				RGBColor color(0, 0, 0);
				for (auto s = 0; s < numSamples; s++) {

					Point2f samplerPoint = sampler->SampleUnitSquare();
					pixelPt.x = c - 0.5f * width + samplerPoint.x;
					pixelPt.y = r - 0.5f * height + samplerPoint.y;
					Vector2f pn(2*pixelPt.x / width, 2*pixelPt.y / height);
					Float rsqr = pn.LengthSquared();
					if (rsqr <= 1.0f) {
						Float r = sqrt(rsqr);
						float psi = r * psiMax;
						float sinPsi = sin(psi);
						float cosPsi = cos(psi);

						float sinAlpha = pn.y / r;
						float cosAlpha = pn.x / r;
						ray.d = sinPsi * cosAlpha * u + sinPsi * sinAlpha * v - cosPsi * w;
						ray.d.Normalize();
					}
					else
					{
						ray.d = V3Zroe;
					}
					
					color += world->hitBareBonesObjects(ray).color;
				}
				color /= numSamples;
				world->displayPixel(r, c, color);
			}
		}
	}
}