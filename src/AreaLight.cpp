#include "AreaLight.h"
#include "World.h"
namespace BL {
	Vector3f AreaLight::GetDirection(ShadeRec& sr) {
		samplerPoint = shape->Sample();
		lightNormal = shape->GetNormal(samplerPoint);
		wi = samplerPoint - sr.localHitPoint;
		wi.Normalize();
		return wi;
	}

	RGBColor AreaLight::L(ShadeRec& sr) {
		float ndotd = Dot(-lightNormal, wi);
		if (ndotd > 0.0)
			return material->Le(sr);
		else
			return ColorBlack;
	}

	Float AreaLight::G(const ShadeRec& sr) {
		float ndotd = Dot(-lightNormal, wi);
		float d2 = 1;
		d2 = (samplerPoint - sr.localHitPoint).LengthSquared();
		return ndotd / d2;
	}

	Float AreaLight::Pdf(const ShadeRec& sr) {
		return shape->Pdf();
	}

	bool AreaLight::InShadows(const Ray& ray, const ShadeRec& sr)const {
		Float distance = (samplerPoint - ray.o).Length();
		for (auto i = 0; i < GetWorldPtr->objects.size(); i++) {
			Float t = 0;
			if (GetWorldPtr->objects[i]->HitTest(ray, t) && t < distance) {
				return true;
			}
		}
		return false;
	}
	
	void AreaLight::SetShape(Shape* shape) {
		this->shape = shape;
	}

	void AreaLight::SetMaterial(Material* mat) {
		this->material = mat;
	}
}