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
				L += diffuseBRDF->F(sr, wi, wo) * GetWorldPtr->lights[i]->L(sr) * ndotWi;
		}
		return L;
	}

	//////////////////////////////////////////////////////////////////////////
	Phong::Phong() :ambientBRDF(new Lambertian), diffuseBRDF(new Lambertian), specularBRDF(new GlossySpeecular) {}
	RGBColor Phong::Shade(ShadeRec& sr)
	{
		Vector3f wo = -sr.ray.d;
		Light* a = GetWorldPtr->GetAmbient();
		RGBColor L = ambientBRDF->Rho(sr, wo) * GetWorldPtr->ambiant->L(sr);
		int lightsNum = GetWorldPtr->lights.size();
		for (auto i = 0; i < lightsNum; i++)
		{
			Light* light = GetWorldPtr->lights[i];
			Vector3f wi = light->GetDirection(sr);
			bool inShadows = false;
			if (light->CastShadows())
			{
				Ray shadowR(sr.localHitPoint, wi);
				inShadows = light->InShaodws(shadowR, sr);
			}
			if (!inShadows)
			{
				Float ndotWi = Dot(sr.normal, wi);
				if (ndotWi > 0.0)
					L += (diffuseBRDF->F(sr, wi, wo) + specularBRDF->F(sr, wi, wo)) * GetWorldPtr->lights[i]->L(sr) * ndotWi;
			}
		}
		return L;
	}
	void Phong::SetKOfAmbient(const Float k) {
		ambientBRDF->SetK(k);
	}
	void Phong::SetKOfDiffuse(const Float k) {
		diffuseBRDF->SetK(k);
	}
	void Phong::SetKOfSpecular(const Float k) {
		specularBRDF->SetK(k);
	}
	void Phong::SetExp(const Float exp) {
		specularBRDF->SetExp(exp);
	}
	void Phong::SetColor(const RGBColor& c) {
		ambientBRDF->SetColor(c);
		diffuseBRDF->SetColor(c);
		specularBRDF->SetColor(ColorWhite);
	}
}