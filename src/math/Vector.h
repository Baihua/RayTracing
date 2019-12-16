#pragma once
#include "../Utility/Utility.h"
#include <cmath>
#include<iostream>
#include<cassert>
#include<algorithm>
namespace BL
{
	template<typename T> class Vector2 {
	public:
		T x, y;

		Vector2() { x = y = 0; }
		Vector2(T x, T y) :x(x), y(y) {}

		T operator[](int i) const {
			assert(i >= 0 && i <= 1);
			if (i == 0)return x;
			return y;
		}
		T& operator[](int i) {
			assert(i >= 0 && i < 2);
			if (i == 0) return x;
			return y;
		}
	};

	template<typename T> class Vector3
	{
	public:
		T x, y, z;
		Vector3() { x = y = z = 0; }
		Vector3(T x, T y, T z) :x(x), y(y), z(z) {}

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

		Vector3<T> operator+(const Vector3<T>& v)const {
			return Vector3(x + v.x, y + v.y, z + v.z);
		}

		Vector3<T>& operator+=(const Vector3<T>& v) {
			x += v.x; y += v.y; z += v.z;
			return *this;
		}

		Vector3<T> operator*(T s) const {
			return Vector3(x * s, y * s, z * s);
		}
		Vector3<T>& operator*=(T s) {
			x *= s; y *= s; z *= s;
			return *this;
		}

		Vector3<T> operator/(T f) const {
			assert(f != 0);
			Float inv = (Float)1 / f;
			return Vector3<T>(x * inv, y * inv, z * inv);
		}
		Vector3<T>& operator/=(T f) {
			assert(f != 0);
			Float inv = (Float)1 / f;
			x *= inv; y *= inv; z *= inv;
			return *this;
		}

		Vector3<T> operator-()const { return Vector3<T>(-x, -y, -z); }
		T LengthSquared()const { return x * x + y * y + z * z; }

		T Length() const { return std::sqrt(LengthSquared()); }

		explicit Vector3(const Normal3<T>& n);
	};

	template <typename T> inline Vector3<T> operator*(T s, const Vector3<T>& v) {
		return v * s;
	}

	template <typename T> inline Vector3<T> Abs(const Vector3<T>& v) {
		return Vector3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
	}

	template<typename T> inline T Dot(const Vector3<T>& v1, const Vector3<T>& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	template<typename T> inline T AbsDot(const Vector3<T>& v1, const Vector3<T>& v2) {
		return std::abs(Dot(v1, v2));
	}

	template<typename T> inline Vector3<T> Cross(const Vector3<T>& v1, const Vector3<T>& v2) {
		return new Vector3<T>(v1.y * v2.z - v1.z * v2.z,
			v1.z * v2.x - v1.x * v2.y,
			v1.y * v2.z - v1.z * v2.z);
	}

	template <typename T> inline Vector3<T> Normalize(const Vector3<T>& v)
	{
		return v / v.Length();
	}

	template <typename T> T MinComponent(const Vector3<T>& v) {
		return std::min(v.x, std::min(v.y, v.z));
	}

	template <typename T> T MaxComponent(const Vector3<T>& v) {
		return std::max(v.x, std::max(v.y, v.z));
	}

	typedef Vector2<int> Vector2i;
	typedef Vector2<Float> Vector2f;
	typedef Vector3<Float> Vector3f;
	typedef Vector3<int> Vector3i;
}