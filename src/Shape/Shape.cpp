#include "Shape.h"
#include "../Sampler.h"
namespace BL {
	Point3f Shape::Sample() {
		return Point3f(0, 0, 0);
	}

	Float Shape:: Pdf() {
		return 0.0;
	}

	Normal3f Shape::GetNormal(const Point3f& point) {
		return Normal3f(0, 0, 0);
	}

	const Float Plane::kEpsilon = 0.0001;

	const  Float Sphere::kEpsilon = 0.0001;

	
	//////////////////////////////////////////////////////////////////////////
	const Float Rectangle::kEpsilon = 0.00001;
	Point3f Rectangle::Sample() {
		if (sampler != NULL)
		{
			Point2f  pt = sampler->SampleUnitSquare();
			return point + pt.x * aSide + pt.y * bSide;
		}
		return point;
	}
	Normal3f Rectangle::GetNormal(const Point3f& point) {
		return normal;
	}
	Float Rectangle::Pdf() {
		return invAea;
	}
}