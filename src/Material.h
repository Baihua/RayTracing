#pragma once
#include "ShadeRec.h"
namespace BL {
	class Material {
	public:
		virtual RGBColor Shade(ShadeRec& sr) = 0;
		virtual RGBColor AreaLightShade(ShadeRec& sr) {
			return ColorBlack;
		};

		virtual RGBColor PathShade(ShadeRec& sr) {
			return ColorBlack;
		};
	};

	class Matte :public Material {
	public:
		Matte();
		void SetKOfAmbient(const Float k);
		void SetKOfDiffuse(const Float k);
		void SetCd(const RGBColor& c);
		virtual RGBColor Shade(ShadeRec& sr);
	private:
		Lambertian* ambientBRDF;
		Lambertian* diffuseBRDF;
	};

	class Phong : public Material {
	public:
		Phong();
		virtual RGBColor Shade(ShadeRec& sr);
		void SetKOfAmbient(const Float k);
		void SetKOfDiffuse(const Float k);
		void SetKOfSpecular(const Float k);
		void SetExp(const Float exp);
		void SetColor(const RGBColor& c);
	protected:
		Lambertian* ambientBRDF;
		Lambertian* diffuseBRDF;
		GlossySpeecular* specularBRDF;
	};
}
