#pragma once
#include "math/Vector.h"
#include "math/Point.h"

//#include "ShadeRec.h"
#include "math/Ray.h"

namespace BL {
	class Camera
	{
	public:
		Camera() {};
		~Camera() {};


		void SetEyePosition(const Vector3f& eye) {
			eyePosition = eye;
		}
		void SetLookAt(const Vector3f& lookat) {
			this->lookat = lookat;
		}
		void SetUp(const Vector3f& up) {
			this->up = up;
		}
		void SetViewDistance(Float d) {
			this->d = d;
		}

		void ComputeUVW() {

			w = -(lookat - eyePosition).Normalize();//w 是dir 的反方向

			u = Cross(up, w).Normalize();

			v = Cross(w, u).Normalize();
		}
		virtual void RenderScene(World* w) = 0;

		Vector3f u, v, w;
		Vector3f eyePosition;
		Vector3f lookat;
		Vector3f up;
		Float d;
		Float exposureTime;
	private:
	};

	class Pinhole :public Camera {
	public:
		virtual void RenderScene(World* world);
	};

	class ThinLen :public Camera {
	public:
		ThinLen(Float lenRaduis, Float FocuFar):lenRadius(lenRaduis), FocusPlenFar(FocuFar){}
		virtual void RenderScene(World* world);
		
	private:
		Float lenRadius;
		Float FocusPlenFar; 
	};
}