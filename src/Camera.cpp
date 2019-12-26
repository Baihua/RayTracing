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
}