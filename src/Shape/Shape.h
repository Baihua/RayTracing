#pragma once
#include"../math/Ray.h"
#include"../math/Normal.h"
#include "../ShadeRec.h"
#include"../math/Vector.h"
#include "../math/Point.h"

namespace BL {

	class Shape {
	public:
		Shape() :color(ColorBlack), material(NULL) {}
		virtual bool Hit(const Ray& ray, Float& tMin, ShadeRec& sr) const = 0;
		virtual bool HitTest(const Ray& ray, Float& tMin) const = 0;
		void SetSampler(Sampler* sampler) {
			this->sampler = sampler;
		}
		RGBColor color;
		Material* material;

		//只是给面积灯用的
		virtual Point3f Sample();
		virtual Float Pdf();

		virtual Normal3f GetNormal(const Point3f& point);
	protected:
		Sampler* sampler;
	};

	class Plane : public Shape
	{
	public:
		Plane(const Point3f& p, const Normal3f& n) :point(p), normal(Normalize(n)) {
		}

		bool Hit(const Ray& ray, Float& tMin, ShadeRec& sr) const
		{
			Float t = Dot((point - ray.o), normal) / Dot(ray.d, normal);
			if (t > kEpsilon) {
				tMin = t;
				sr.localHitPoint = ray.o + ray.d * t;
				sr.hitAnObject = true;
				sr.normal = normal;
				sr.material = material;
			}
			else {
				return false;
			}
			return true;
		}

		bool HitTest(const Ray& ray, Float& tMin) const
		{
			Float t = Dot((point - ray.o), normal) / Dot(ray.d, normal);
			if (t > kEpsilon) {
				tMin = t;
				return true;
			}
			else {
				return false;
			}
			return true;
		}
	protected:
		static  const Float kEpsilon;
		Point3f point;
		Normal3f normal;
	};

	class Rectangle : public Shape
	{
	public:
		Rectangle(const Point3f& p, const Normal3f& n, const Vector3f& aSide, const Vector3f& bSide) :
			point(p), 
			normal(Normalize(n)), aSide(aSide), bSide(bSide){
			aSideLenSquare = aSide.LengthSquared(); bSideLenSquare = bSide.LengthSquared();
			invAea = 1 / (aSide.Length()*bSide.Length());
		}

		bool Hit(const Ray& ray, Float& tMin, ShadeRec& sr) const
		{
			Float t = Dot((point - ray.o), normal) / Dot(ray.d, normal);
			if (t < kEpsilon)
				return false;
			Point3f hitpoint = ray.o + ray.d * t;
			Vector3f d = hitpoint - point;
			//计算在a边的投影长度
			Float da = Dot(d,aSide);
			if (da < 0 ||(da - aSideLenSquare)  > -kEpsilon)
				return false;

			Float db = Dot(d,bSide);
			if (db < 0 || (db-bSideLenSquare) > -kEpsilon) {
				return false;
			}

			tMin = t;
			sr.localHitPoint = hitpoint;
			sr.hitAnObject = true;
			sr.normal = normal;
			sr.material = material;
			return true;
		}

		bool HitTest(const Ray& ray, Float& tMin) const
		{
			Float t = Dot((point - ray.o), normal) / Dot(ray.d, normal);
			if (t < kEpsilon)
				return false;
			Point3f hitpoint = ray.o + ray.d * t;
			Vector3f d = hitpoint - point;
			//计算在a边的投影长度
			Float da = Dot(aSide, d);
			if (da < 0 || da > aSideLenSquare)
				return false;

			Float db = Dot(bSide, d);
			if (db < 0 || db > bSideLenSquare) {
				return false;
			}
			return true;
		}
		Point3f Sample();
		Normal3f GetNormal(const Point3f& point);
		Float Pdf();
	protected:
		static  const Float kEpsilon;
		Point3f point;
		Normal3f normal;
		Vector3f aSide;
		Vector3f bSide;
		Float aSideLenSquare;
		Float bSideLenSquare;
		Float invAea;
	};

	class Sphere : public Shape {
	public:
		Sphere(const Point3f& p, Float r) :position(p), radious(r) {}
		virtual bool Hit(const Ray& ray, Float& tMin, ShadeRec& sr) const
		{
			Vector3f temp = ray.o - position;
			Float a = Dot(ray.d, ray.d);
			Float b = 2 * Dot(temp, ray.d);
			Float c = Dot(temp, temp) - radious * radious;
			Float dis = b * b - 4 * a * c;
			if (dis < 0.0)
				return false;
			else
			{
				Float e = sqrt(dis);
				Float t = (-b - e) / (2 * a);
				if (t > kEpsilon)
				{
					tMin = t;
					Vector3f v = (temp + t * ray.d) / radious;
					sr.normal = (Normal3f)v;// (temp + t * ray.d) / radious;
					sr.localHitPoint = ray.o + t * ray.d;
					sr.material = material;
					return true;
				}
				t = (-b + e) / (2 * a);
				if (t > kEpsilon)
				{
					tMin = t;
					sr.normal = (Normal3f)(temp + t * ray.d) / radious;
					sr.localHitPoint = ray.o + t * ray.d;
					sr.material = material;
					return true;
				}
			}
			return false;
		}
		virtual bool HitTest(const Ray& ray, Float& tMin) const
		{
			Vector3f temp = ray.o - position;
			Float a = Dot(ray.d, ray.d);
			Float b = 2 * Dot(temp, ray.d);
			Float c = Dot(temp, temp) - radious * radious;
			Float dis = b * b - 4 * a * c;
			if (dis < 0.0)
				return false;
			else
			{
				Float e = sqrt(dis);
				Float t = (-b - e) / (2 * a);
				if (t > kEpsilon)
				{
					tMin = t;
					return true;
				}
				t = (-b + e) / (2 * a);
				if (t > kEpsilon)
				{
					tMin = t;
					return true;
				}
			}
			return false;
		}
	protected:
		static  const Float kEpsilon;
		Point3f position;
		Float radious;
	};
}