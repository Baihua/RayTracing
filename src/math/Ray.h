#pragma once
#include "..\bl.h"
#include "Vector.h"
#include "Point.h"
namespace BL
{
	class Ray {
	public:
		Point3f o;
		Vector3f d;
		mutable Float tMax;
		Float time;

		Ray():tMax(Infinity), time(0.0f){}
		Ray(const Point3f& o, const Vector3f& d, Float tMax = Infinity, Float time = 0.f)
			:o(o), d(d), tMax(tMax), time(time) {

		}
		Point3f operator()(Float t)const {
			return o + d * t;
		}

	};
}