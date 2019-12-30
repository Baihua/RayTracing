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

}