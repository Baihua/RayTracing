#include "LIght.h"
#include "World.h"

namespace BL {
	bool PointLight::InShaodws(const Ray& ray, const ShadeRec& sr) const {
		Float distance = (location - ray.o).Length();
		for (auto i = 0; i < GetWorldPtr->objects.size(); i++) {
			Float t = 0;
			if (GetWorldPtr->objects[i]->HitTest(ray, t) && t < distance) {
				return true;
			}
		}
		return false;
	}
}