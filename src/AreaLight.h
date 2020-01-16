#pragma once
#include "bl.h"
#include "Light.h"

namespace BL {
	class AreaLight :public Light {
	public:
		Vector3f GetDirection(ShadeRec& sr);
		bool InShadows(const Ray& ray, const ShadeRec& sr)const;
		RGBColor L(ShadeRec& sr);
		Float G(const ShadeRec& sr);
		Float Pdf(const ShadeRec& sr);
		void SetShape(Shape* shape);
		void SetMaterial(Material* mat);
	protected:
		Shape* shape;
		Material* material;
		Point3f samplerPoint;
		Normal3f lightNormal;
		Vector3f wi;
	};
}