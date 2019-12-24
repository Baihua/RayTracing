#pragma once
#include "math/Vector.h"
#include "math/Point.h"
namespace BL {
	class Camera
	{
	public:
		Camera();
		~Camera();

		void ComputeUVW() {
			w = (lookat - eyePosition).Normalize();
			u = Cross(w, u);
		}
		
		Vector3f u, v, w;
		Vector3f eyePosition;
		Vector3f lookat;
		Vector3f up;
		Float exposureTime;
	private:
	};

	Camera::Camera()
	{
	}

	Camera::~Camera()
	{
	}
}