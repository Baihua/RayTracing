#pragma once
#include "../Utility/Utility.h"
#include "Vector.h"
namespace BL
{
	template<typename T> class Point2 {
	public:
		T x, y;
		Point2() { x = y = 0; }
		Point2(T x, T y) :x(x), y(y) {}

		explicit Point2(const Point3<T>& p) :x(p.x), y(p.y) {}
	};

	template<typename T>class Point3 {
	public:
		T x, y, z;
		Point3() { x = y = z = 0; }
		Point3(T x, T y, T z) :x(x), y(y), z(z) {}

		template<typename U> explicit Point3(const Point3<U>& p) :
			x((T)p.x), y((T)p.y), z((T)p.z) {
		}

		template<typename U> explicit operator Vector3<U>() const {
			return Vector3<U>(x, y, z);
		}

		Point3<T> operator+(const Vector3<T>& v)const {
			return Point3<T>(x + v.x, y + v.y, z + v.z);
		}

		Point3<T>& operator+=(const Vector3<T>& v) {
			x += v.x; y += v.y; z += v.z;
			return *this;
		}

		Vector3<T> operator-(const Point3<T>& v)const {
			return Vector3<T>(x - v.x, y - v.y, z - v.z);
		}

		Point3<T> operator-(const Vector3<T>& v) {
			return Point3<T>(x - v.x, y - v.y, z - v.z);
		}

		Point3<T>& operator-=(const Vector3<T>& v) {
			x -= v.x; y -= v.y; z -= v.z;
			return *this;
		}

		explicit Point3<T>(const Vector3<T>& v) : x(v.x), y(v.y), z(v.z) {}
	};

	template <typename T> inline Point2<T> operator*(T s, const Point2<T>& p) {
		return Point2<T>(p.x*s, p.y*s);
	}

	template<typename T> inline Float Distance(const Point3<T>& p1, const Point3<T>& p2)
	{
		return (p1 - p2).Length();
	}
	template<typename T> inline Float Distance(const Point2<T>& p1, const Point3<T>& p2)
	{
		return (p1 - p2).Length();
	}
	template<typename T> inline Float DistanceSquared(const Point3<T>& p1, const Point3<T>& p2)
	{
		return (p1 - p2).LengthSquared();
	}

	typedef Point2<Float> Point2f;
	typedef Point2<int> Point2i;
	typedef Point3<Float> Point3f;
	typedef Point3<int> Point3i;
}