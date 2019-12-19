#pragma once
namespace BL
{
	template <typename T>
	class Vector2;
	template <typename T>
	class Vector3;
	template <typename T>
	class Point3;
	template <typename T>
	class Point2;
	template <typename T>
	class Normal3;
	class World;
	typedef float Float;
	static const Float Pi = 3.14159265358979323846;
	static const Float InvPi = 0.31830988618379067154;
	static const Float Inv2Pi = 0.15915494309189533577;
	static const Float Inv4Pi = 0.07957747154594766788;
	static const Float PiOver2 = 1.57079632679489661923;
	static const Float PiOver4 = 0.78539816339744830961;
	static const Float Sqrt2 = 1.41421356237309504880;

	static float rand_float()
	{

		return rand() / double(RAND_MAX);
	}
}