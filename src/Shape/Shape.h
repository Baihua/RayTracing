#pragma once
#include"../bl.h"
#include"../math/Ray.h"
#include"../math/Normal.h"
#include "../ShadeRec.h"
#include"../math/Vector.h"
#include "../math/Point.h"

namespace BL {

	class Shape {
	public:
		virtual bool Hit(const Ray& ray, Float& tMin, ShadeRec& sr) const = 0;
		RGBColor color;
		Material* material;
	};

	class Plane : public Shape
	{
	public:
		Plane(const Point3f& p, const Normal3f& n) :point(p), normal(n) {
		}

		bool Hit(const Ray& ray, Float& tMin, ShadeRec& sr) const 
		{
			Float t = Dot((point - ray.o), normal) / Dot(ray.d, normal);
			if (t > 0) {
				tMin = t;
				sr.localHitPoint = ray.o + ray.d * t;
				sr.hitAnObject = true;
			}
			else {
				return false;
			}

		}
	protected:
		Point3f point;
		Normal3f normal;
	};

	class Sphere : public Shape {
	public:
		Sphere(const Point3f& p, Float r) :position(p), radious(r) {}
		virtual bool Hit(const Ray& ray, Float& tMin, ShadeRec& sr) const
		{
			Vector3f temp = ray.o - position;
			Float a = Dot(ray.d, ray.d); 
			Float b = 2 * Dot(temp, ray.d); 
			Float c = Dot(temp, temp) - radious* radious;
			Float dis = b* b - 4 * a * c;
			if (dis < 0.0)
				return false;
			else
			{
				Float e = sqrt(dis);
				Float t = (-b - e) / (2 * a);
				if (t > 0)
				{
					tMin = t;
					Vector3f v = (temp + t * ray.d) / radious;
					sr.normal = (Normal3f)v;// (temp + t * ray.d) / radious;
					sr.localHitPoint = ray.o + t * ray.d;
					return true;
				}
				t = (-b + e) / (2 * a);
				if (t > 0)
				{
					tMin = t;
					sr.normal = (Normal3f)(temp + t * ray.d) / radious;
					sr.localHitPoint = ray.o + t * ray.d;
					return true;
				}
			}
			return false;
		}
	protected:
		Point3f position;
		Float radious;
	};
}