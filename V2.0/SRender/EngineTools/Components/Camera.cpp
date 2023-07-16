#include "Camera.h"
#include "../GameObject.h"
#include "../Scene.h"
#include "GlobalSettings.h"
#include "../EngineTime.h"

int EngineTools::Camera::cameraCount = 0;

SRender::MathTools::Matrix<double> EngineTools::Camera::GetLookAtMatrix(Vector3 eye, Vector3 lookAt, Vector3 wUp)
{
	SRender::MathTools::Matrix<double> Result(4, 4, 0);

	Vector3 f = (lookAt - eye).Normalize();
	Vector3 s = f.Cross(wUp).Normalize();
	Vector3 u = s.Cross(f).Normalize();

	Result[0][0] = s.x;
	Result[1][0] = s.y;
	Result[2][0] = s.z;
	Result[0][1] = u.x;
	Result[1][1] = u.y;
	Result[2][1] = u.z;
	Result[0][2] = -f.x;
	Result[1][2] = -f.y;
	Result[2][2] = -f.z;
	Result[3][0] = s.Dot(eye);
	Result[3][1] = u.Dot(eye);
	Result[3][2] = f.Dot(eye);
	Result[3][3] = 1;

	return Result;
}

EngineTools::Camera::Camera()
{
	typeHashCode = typeid(Camera).hash_code();
	cameraID = cameraCount++;
}

void EngineTools::Camera::Start()
{
	if (!gameObject->transform)
	{
		gameObject->transform = gameObject->GetComponent<Transform>();
	}
	RegisterToGlobalSettings();
}

void EngineTools::Camera::Update()
{
	if (!gameObject->transform)
	{
		gameObject->transform = gameObject->GetComponent<Transform>();
	}
	
	// For Camera, pitch should not be bigger than +-90 degree.
	if (gameObject->transform->rotation.x >= 90.0)
	{
		gameObject->transform->rotation.x = 89.0;
	}
	else if (gameObject->transform->rotation.x <= -90.0)
	{
		gameObject->transform->rotation.x = -89.0;
	}

	// gameObject->transform->RotationWithRoot({ 0, 20 * EngineTime::GetInstance()->deltaTimef, 0 }, true);

	front.x = sin(gameObject->transform->rotation.y * 0.0174532925199433f) * cos(0.0174532925199433f * gameObject->transform->rotation.x);
	front.y = sin(0.0174532925199433f * gameObject->transform->rotation.x);
	front.z = cos(0.0174532925199433f * gameObject->transform->rotation.x) * cos(0.0174532925199433f * gameObject->transform->rotation.y);
	front.Normalize();
}

SRender::MathTools::Matrix<double> EngineTools::Camera::GetLookAtMatrix()
{
	return GetLookAtMatrix(gameObject->transform->worldPosition, gameObject->transform->worldPosition + front, worldUp).Transfer();
}

void EngineTools::Camera::RegisterToGlobalSettings()
{
	gameObject->scene->GetSharedComponent<GlobalSettings>()->cameras[cameraID] = this;
}
