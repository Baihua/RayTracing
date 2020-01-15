#pragma once
#include "math/Vector.h"
#include "ShadeRec.h"
#include "bl.h"
namespace BL {
	class Light
	{
	public:
		Light() {};
		~Light() {};
		virtual Vector3f GetDirection(ShadeRec& sr) = 0;
		virtual RGBColor L(ShadeRec& sr) = 0;
		virtual Float G(const ShadeRec& sr) { return 0; }
		virtual Float Pdf(const ShadeRec& sr) { return 0; }
		virtual bool InShadows(const Ray& ray, const ShadeRec& sr)const {
			return false;
		}

		bool CastShadows() {
			return shadows;
		}
		void SetCastShadows(bool shadows) {
			this->shadows = shadows;
		}
	protected:
		bool shadows;
	};
	//////////////////////////////////////////////////////////////////////////
	class Ambient : public Light {
	public:
		Ambient() :ls(1.0), color(ColorWhite) {}

		virtual Vector3f GetDirection(ShadeRec& sr) {
			return V3Zroe;
		}

		virtual RGBColor L(ShadeRec& sr) {
			return (ls * color);
		}
	private:
		Float ls;
		RGBColor color;

	};
	//////////////////////////////////////////////////////////////////////////
	class PointLight : public Light {
	public:
		PointLight() :ls(1.0), color(ColorWhite) {}
		virtual Vector3f GetDirection(ShadeRec& sr) {
			return (location - sr.localHitPoint).Normalize();
		}

		virtual RGBColor L(ShadeRec& sr) {
			return (ls * color);
		}

		void SetPosition(Point3f p) {
			location = p;
		}

		void SetScaleRadiance(Float ls) {
			this->ls = ls;
		}

		virtual bool InShaodws(const Ray& ray, const ShadeRec& sr)const;

	private:
		Float    ls;
		RGBColor color;
		Point3f  location;
	};
	//////////////////////////////////////////////////////////////////////////
	class AmbientOccluder : public Light
	{
	public:
		AmbientOccluder() :ls(1), color(ColorWhite) {};
		virtual Vector3f GetDirection(ShadeRec& sr);
		virtual RGBColor L(ShadeRec& sr);
		virtual bool InShaodws(const Ray& ray, const ShadeRec& sr)const;
		void SetSampler(Sampler* s);
		void SetMinAmount(Float v) {
			minAmount = v;
		}
	private:
		Vector3f u, v, w;
		Sampler* sampler;
		float minAmount;
		Float ls;
		RGBColor color;
	};
}
