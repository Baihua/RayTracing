#pragma once
#include "math/Point.h"
#include "math/Normal.h"
#include "World.h"
namespace BL
{
	class RGBColor
	{
	public:
		int r, g, b;
	};

	class ShadeRec
	{
	public:
		bool hitAnObject;
		Point3f localHitPoint;
		Noraml3f normal;
		RGBColor color;
		World& world;

		ShadeRec(World& world) :hitAnObject(false), localHitPoint(), normal(), color(), world(world) {

		}
		ShadeRec(const ShadeRec& sr) : hitAnObject(sr.hitAnObject), localHitPoint(sr.localHitPoint), normal(sr.normal), color(sr.color), world(sr.world) {

		}

		ShadeRec& operator= (const ShadeRec& sr) {
			if (this != &sr) {
				hitAnObject = sr.hitAnObject;
				localHitPoint = sr.localHitPoint;
				normal = sr.normal;
				color = sr.color;
				world = sr.world;
			}
			return *this;
		}
	};
}