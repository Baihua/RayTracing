#pragma once
#include "bl.h"
#include "Light.h"

namespace BL {
	class AreaLight :public Light {
	public:
		Vector3f GetDirection(ShadeRec& sr);
		bool InShaodws(const Ray& ray, const ShadeRec& sr)const;
		RGBColor L(ShadeRec& sr);

		virtual RGBColor TraceRay(const Ray& ray, const int depth)const;
		Float G(ShadeRec& sr);
		Float Pdf(ShadeRec& sr);
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