#include "Material.h"
#include "BRDF.h"
#include "World.h"
namespace BL {

	Matte::Matte() :ambientBRDF(new Lambertian), diffuseBRDF(new Lambertian) {

	}

	void Matte::SetCd(const RGBColor& c) {
		ambientBRDF->SetColor(c);
		diffuseBRDF->SetColor(c);
	}

	void Matte::SetKOfAmbient(const Float k) {
		ambientBRDF->SetK(k);
	}

	void Matte::SetKOfDiffuse(const Float k) {
		diffuseBRDF->SetK(k);
	}

	RGBColor Matte::Shade(ShadeRec& sr) {
		Vector3f wo = -sr.ray.d;
		Light* a = GetWorldPtr->GetAmbient();
		RGBColor L = ambientBRDF->Rho(sr, wo) * GetWorldPtr->ambiant->L(sr);
		int lightsNum = GetWorldPtr->lights.size();
		for (auto i = 0; i < lightsNum; i++)
		{
			Vector3f wi = GetWorldPtr->lights[i]->GetDirection(sr);
			Float ndotWi = Dot(sr.normal, wi);
			if (ndotWi > 0.0)
				L += diffuseBRDF->F(sr, wo, wi) * GetWorldPtr->lights[i]->L(sr)*ndotWi;
		}
		return L;
	}
}