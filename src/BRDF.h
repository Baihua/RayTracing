#pragma once
#include "math/Vector.h"
#include "ShadeRec.h"
namespace BL {
	class BRDF {
	public:
		virtual RGBColor F(const ShadeRec& sr, const Vector3f& wi, const Vector3f& wo) const = 0;
		virtual RGBColor SampleF(ShadeRec& sr, const Vector3f& wi, const Vector3f& wo) const = 0;
		virtual RGBColor Rho(const ShadeRec& sr, const Vector3f wo) const = 0;
	protected:
		;
	};

	//////////////////////////////////////////////////////////////////////////
	class Lambertian : public BRDF {
	public:
		virtual RGBColor F(const ShadeRec& sr, const Vector3f& wi, const Vector3f& wo) const {
			return (kd * cd * InvPi);
		}

		virtual RGBColor SampleF(ShadeRec& sr, const Vector3f& wi, const Vector3f& wo)const {
			return cd;//还没实现
		}

		virtual RGBColor Rho(const ShadeRec& sr, const Vector3f wo)const {
			return (kd * cd);
		}

		void SetK(const Float k) { kd = k; }
		void SetColor(const RGBColor cd) { this->cd = cd; }
	private:
		Float kd;
		RGBColor cd;
	};

	class GlossySpeecular : public BRDF {
	public:
		GlossySpeecular() :ks(0.0), exp(1.0), color(ColorWhite) {}

		virtual RGBColor F(const ShadeRec& sr, const Vector3f& wi, const Vector3f& wo) const {
			RGBColor l;
			Float nDotWi = Dot(sr.normal, wi);
			Vector3f r = -wi + (Vector3f)sr.normal * (2.0 * nDotWi);
			Float rDotWo = Dot(r, wo);
			if (rDotWo > 0.0f)
				l = color * ks * pow(rDotWo, exp);
			return l;
		}

		virtual RGBColor SampleF(ShadeRec& sr, const Vector3f& wi, const Vector3f& wo)const {
			return ColorBlack;
		}

		virtual RGBColor Rho(const ShadeRec& sr, const Vector3f wo)const {
			return ColorBlack;
		}

		void SetColor(const RGBColor& c) {
			this->color = c;
		}

		void SetExp(Float exp) {
			this->exp = exp;
		}

		void SetK(Float k) {
			this->ks = k;
		}
	private:
		Float ks;
		RGBColor color;
		Float exp;
	};
}