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
		RGBColor L = ambientBRDF->Rho(sr, wo) * GetWorldPtr->ambiant->L(sr);
		int lightsNum = GetWorldPtr->lights.size();
		for (auto i = 0; i < lightsNum; i++)
		{
			Light* light = GetWorldPtr->lights[i];
			Vector3f wi = light->GetDirection(sr);
			Float ndotWi = Dot(sr.normal, wi);
			bool inShadows = false;
			if (light->CastShadows())
			{
				Ray shadowR(sr.localHitPoint, wi);
				inShadows = light->InShaodws(shadowR, sr);
			}
			if (!inShadows) {
				if (ndotWi > 0.0)
					L += diffuseBRDF->F(sr, wi, wo) * GetWorldPtr->lights[i]->L(sr) * ndotWi;
			}
		}
		return L;
	}

	RGBColor Matte::AreaLightShade(ShadeRec& sr) {
		Vector3f wo = -sr.ray.d;
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
			Float ndotWi = Dot(sr.normal, wi);
			if (ndotWi > 0) {
				if (!inShadows)
				{
					L += diffuseBRDF->F(sr, wi, wo) * 
						light->L(sr)*light->G(sr) * ndotWi/ light->Pdf(sr);
				}
			}
		}
		return L;
	}
	//////////////////////////////////////////////////////////////////////////
	Phong::Phong() :ambientBRDF(new Lambertian), diffuseBRDF(new Lambertian), specularBRDF(new GlossySpeecular) {}
	RGBColor Phong::Shade(ShadeRec& sr)
	{
		Vector3f wo = -sr.ray.d;
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

	RGBColor Phong::AreaLightShade(ShadeRec& sr)
	{
		Vector3f wo = -sr.ray.d;
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
					L += (diffuseBRDF->F(sr, wi, wo) + specularBRDF->F(sr, wi, wo)) * 
					(light->L(sr) * light->G(sr) * ndotWi/light->Pdf(sr));
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

	//////////////////////////////////////////////////////////////////////////
	Emissive::Emissive() {

	}

	RGBColor Emissive::Shade(ShadeRec& sr) {
		if (Dot((Vector3f)-sr.normal, sr.ray.d) > 0.0) {
			return ls * color;
		}
		else
			return ColorBlack;
	}

	RGBColor Emissive::AreaLightShade(ShadeRec& sr) {
		if (Dot((Vector3f)-sr.normal, sr.ray.d) > 0.0) {
			return ls * color;
		}
		else
			return ColorBlack;
	}
	RGBColor Emissive::GetLe(ShadeRec& sr) const {
		return ls * color;
	}
	void Emissive::SetColor(const RGBColor& color) {
		this->color = color;
	}
	void Emissive::SetScaleRadiance(const Float ls) {
		this->ls = ls;
	}
}