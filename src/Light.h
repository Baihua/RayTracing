#pragma once
#include "math/Vector.h"
#include "ShadeRec.h"
namespace BL {
	class Light
	{
	public:
		Light() {};
		~Light() {};
		virtual Vector3f GetDirection(ShadeRec& sr) = 0;
		virtual RGBColor L(ShadeRec& sr) = 0;
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

	class PointLight : public Light {
	public:
		PointLight():ls(1.0), color(ColorWhite){}
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

	private:
		Float    ls;
		RGBColor color;
		Point3f  location;
	};
}
