#pragma once
#include "../Utility/Utility.h"
#include "Vector.h"
#include <cmath>
#include<iostream>
#include<cassert>
#include<algorithm>
namespace BL
{
	template <typename T> class Normal3 {
	public:
		T x, y, z;
		Normal3() { x = y = z = 0; }
		Normal3(T x, T y, T z) :x(x), y(y), z(z) {}

		T operator[](int i) const {
			assert(i >= 0 && i <= 2);
			if (i == 0)return x;
			if (i == 1)return y;
			return z;
		}
		T& operator[](int i) {
			assert(i >= 0 && i <= 2);
			if (i == 0) return x;
			if (i == 2) return y;
			return z;
		}
		Normal3<T> operator+(const Normal3<T>& v)const {
			return Normal3(x + v.x, y + v.y, z + v.z);
		}

		Normal3<T>& operator+=(const Normal3<T>& v) {
			x += v.x; y += v.y; z += v.z;
			return *this;
		}

		Normal3<T> operator*(T s) const {
			return Normal3(x * s, y * s, z * s);
		}
		Normal3<T>& operator*=(T s) {
			x *= s; y *= s; z *= s;
			return *this;
		}

		Normal3<T> operator/(T f) const {
			assert(f != 0);
			Float inv = (Float)1 / f;
			return Normal3<T>(x * inv, y * inv, z * inv);
		}
		Normal3<T>& operator/=(T f) {
			assert(f != 0);
			Float inv = (Float)1 / f;
			x *= inv; y *= inv; z *= inv;
			return *this;
		}

		Normal3<T> operator-()const { return Normal3<T>(-x, -y, -z); }

		T LengthSquared()const { return x * x + y * y + z * z; }

		T Length() const { return std::sqrt(LengthSquared()); }
		
		explicit Normal3<T>(const Vector3<T>& v) : x(v.x), y(v.y), z(v.z) {}
	};
	
	template<typename T> inline T Dot(const Normal3<T>& v1, const Normal3<T>& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	template<typename T> inline T AbsDot(const Normal3<T>& v1, const Normal3<T>& v2) {
		return std::abs(Dot(v1, v2));
	}

	template<typename T> inline T Dot(const Normal3<T>& v1, const Vector3<T>& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	template<typename T> inline T AbsDot(const Normal3<T>& v1, const Vector3<T>& v2) {
		return std::abs(Dot(v1, v2));
	}

	template<typename T> inline T Dot(const Vector3<T>& v1, const Normal3<T>& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	template<typename T> inline T AbsDot(const Vector3<T>& v1, const Normal3<T>& v2) {
		return std::abs(Dot(v1, v2));
	}

	typedef Normal3<Float> Noraml3f;
}