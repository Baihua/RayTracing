#include "World.h"
#include "Camera.h"
namespace BL {
	World* World::instance = NULL;
	
	World* World::GetInstance(){
		if (instance == NULL){
			instance = new World;
		}
		return instance;
	}

	void World::Build() {
		viewPlane.height = 200;
		viewPlane.width = 200;
		viewPlane.pixSize = 1.0f;
		viewPlane.gamma = 1.0f;

		camera = new BL::Pinhole();
		camera->SetEyePosition(Vector3f(0, 40, 100));
		camera->SetLookAt(Vector3f(0, 0, 0));
		camera->SetUp(Vector3f(0, 1, 0));
		camera->SetViewDistance(100);
		camera->ComputeUVW();

		backgroundColor = ColorBlack;

		SetAmbiant(new Ambient());

		PointLight* ptLight = new PointLight();
		ptLight->SetPosition(Point3f(100, 50, 150));
		ptLight->SetScaleRadiance(3.0f);
		lights.push_back(ptLight);

		Matte* matte = new Matte();
		matte->SetKOfAmbient(0.25f);
		matte->SetKOfDiffuse(0.65);
		matte->SetCd(ColorBlue);
		Sphere* sphere = new Sphere(Point3f(-40, 0, -20), 40);
		sphere->material = matte;
		AddObject(sphere);


		matte = new Matte();
		matte->SetKOfAmbient(0.25f);
		matte->SetKOfDiffuse(0.65);
		matte->SetCd(ColorRed);
		sphere = new Sphere(Point3f(0, 0, -140), 40);
		sphere->material = matte;
		AddObject(sphere);

		matte = new Matte();
		matte->SetKOfAmbient(0.25f);
		matte->SetKOfDiffuse(0.65);
		matte->SetCd(ColorGray);
		sphere = new Sphere(Point3f(80, 0, -260), 40);
		sphere->material = matte;
		AddObject(sphere);

		matte = new Matte();
		matte->SetKOfAmbient(0.25f);
		matte->SetKOfDiffuse(0.65);
		matte->SetCd(ColorGreen);
		Plane* p = new Plane(Point3f(0, -50, 0), Normal3f(0, 100, 1));
		p->material = matte;
		AddObject(p);

		if (dislayPixelData != NULL) {
			delete[] dislayPixelData;
		}

		dislayPixelData = new unsigned char[viewPlane.height * viewPlane.width * 3];
		memset(dislayPixelData, 0, viewPlane.height * viewPlane.width * 3);

	}

	void World::RenderPerspective() {
		camera->RenderScene(this);
	}

}