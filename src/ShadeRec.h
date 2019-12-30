#pragma once
#include "math/Point.h"
#include "math/Normal.h"
#include "Utility/Utility.h"
#include "math/Ray.h"
//#include "World.h"
namespace BL
{
	class RGBColor
	{
	public:
		Float  r, g, b;
		RGBColor() :r(0.f), g(0.f), b(0.f) {}
		RGBColor(Float r, Float g, Float b) :r(r), g(g), b(b) {}

		RGBColor& operator=(const RGBColor c) {
			r = c.r; b = c.b; g = c.g;
			return *this;
		}

		RGBColor operator+(const RGBColor& c)const {
			return RGBColor(r + c.r, g + c.g, b + c.b);
		}

		RGBColor& operator+=(const RGBColor& c) {
			r += c.r; g += c.g; b += c.b;
			return *this;
		}

		RGBColor operator*(Float a)const {
			return RGBColor(a * r, a * g, a * b);
		}

		RGBColor& operator*=(Float a) {
			r *= a; g *= a; b *= a;
			return *this;
		}

		RGBColor operator*(const RGBColor& c)const {
			return RGBColor(c.r * r, c.g * g, c.b * b);
		}

		RGBColor& operator*=(const RGBColor& c) {
			r *= c.r; g *= c.g; b *= c.b;
			return *this;
		}

		RGBColor operator/(Float a)const {
			a = 1 / a;
			return RGBColor(a * r, a * g, a * b);
		}

		RGBColor& operator/=(Float a) {
			a = 1 / a;
			r *= a; g *= a; b *= a;
			return *this;
		}
	};

	static RGBColor operator*(Float a, const RGBColor& c)
	{
		return c * a;
	}

	const RGBColor ColorBlack(0, 0, 0);
	const RGBColor ColorWhite(1, 1, 1);
	const RGBColor ColorRed(1, 0, 0);
	const RGBColor ColorGreen(0, 1, 0);
	const RGBColor ColorBlue(0, 0, 1);
	const RGBColor ColorGray(0.5f, 0.5f, 0.5f);
	const RGBColor ColorYellow(1, 1, 0);

	class ShadeRec
	{
	public:
		bool hitAnObject;
		Material* material;
		Point3f worldHitPoint;
		Point3f localHitPoint;
		Normal3f normal;
		RGBColor color;
		Ray ray;
		int depth;                 //µ›πÈ…Ó∂»
		Vector3f dir;              //for Area lights;
		Float rayT;

		ShadeRec() :hitAnObject(false), material(NULL), localHitPoint(), normal(), color() {
		}
		ShadeRec(const ShadeRec& sr) :
			hitAnObject(sr.hitAnObject),
			material(sr.material),
			worldHitPoint(sr.worldHitPoint),
			localHitPoint(sr.localHitPoint),
			normal(sr.normal),
			color(sr.color), ray(sr.ray), depth(sr.depth), rayT(sr.rayT) {
		}

		ShadeRec& operator= (const ShadeRec& sr) {
			if (this != &sr) {
				hitAnObject = sr.hitAnObject;
				localHitPoint = sr.localHitPoint;
				normal = sr.normal;
				color = sr.color;
			}
			return *this;
		}
	};
}