#include "LIght.h"
#include "World.h"
#include "Sampler.h"
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

	//////////////////////////////////////////////////////////////////////////
	void AmbientOccluder::SetSampler(Sampler* s){
		this->sampler = s;
		s->MapSamplesToHimishpere(1);
	}

	Vector3f AmbientOccluder::GetDirection(ShadeRec& sr) {
		Point3f p = sampler->SamplerHemisphere();
		return (p.x * u + p.y * v + p.z * w);
	}

	bool AmbientOccluder::InShaodws(const Ray& ray, const ShadeRec& sr) const {
		Float t;
		int num = GetWorldPtr->objects.size();
		for (int i = 0; i < num; i++) {
			if (GetWorldPtr->objects[i]->HitTest(ray, t))
				return true;
		}
		return false;
	}

	RGBColor AmbientOccluder::L(ShadeRec& sr) {
		//生成一个向量基
		w = (Vector3f)sr.normal;
		v = Cross(w, Vector3f(0.0722, 1, 0.0235));
		v.Normalize();
		u = Cross(v, w);
		u.Normalize();
		Ray ray;
		ray.o = sr.localHitPoint;
		ray.d = GetDirection(sr);
		//return ls * color;
		if (InShaodws(ray, sr))
			return minAmount *ls* color;
		else
			return ls * color;
	}
}