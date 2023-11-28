/*
*	Created by tianlan.
*	摄像机组件，用于观察和绘制当前世界内的物体
* 
*	class Camera;
*/
#ifndef SGENGINE_CAMERA_H
#define SGENGINE_CAMERA_H

#include "../../MathTools/Matrix.h"
#include "../Vector3.h"
#include "../Component.h"
#include "Transform.h"


namespace EngineTools
{
	// 用于控制摄像机对应的窗口大小类型
	// 警告：当前选项无效
	enum class CameraType
	{
		FULLWINDOW,	// 全窗口状态下，摄像机内容会铺满屏幕
		AREA
	};

	// 控制摄像机目标窗口大小
	// 该结构体的值范围均在0-1之间
	// 高和宽为完整矩形的高和宽
	struct CameraRect
	{
		float center_x = 0.5f;
		float center_y = 0.5f;
		float height = 1.0f;
		float width = 1.0f;
	};

	class Camera : public Component
	{
		static int cameraCount;		// 已经创建的摄像机个数

		// 获取LookAt矩阵
		// @para eye		摄像机当前世界坐标
		// @para lookAt		摄像机指向的方向
		// @para wUp		世界中指明向上的方向
		// @ret  SRender::MathTools::Matrix<double>	得到的lookAt矩阵
		SRender::MathTools::Matrix<double> GetLookAtMatrix(Vector3 eye, Vector3 lookAt, Vector3 wUp);
	public:
		int cameraID;				// 相机ID

		float fov = 45.0f;			// 视角大小
		float cameraNear = 0.1f;	// 近平面
		float cameraFar = 100.0f;	// 远平面

		// 窗口类型
		// 警告：当前选项无效
		CameraType cameraType = CameraType::FULLWINDOW;

		// 窗口大小
		// 警告：当前选项无效
		CameraRect cameraRect;

		// 默认世界向上方向为y轴正方向
		Vector3 worldUp = Vector3(0.0, 1.0, 0.0);

		// 默认前方指向为z轴正方向
		Vector3 front = Vector3(0.0, 0.0, 1.0);

		Camera();

		void Start() override;

		// 用于更新front
		void Update() override;

		// 获取LookAt矩阵的快捷方式
		SRender::MathTools::Matrix<double> GetLookAtMatrix();

		// 把当前相机注册到全局信息
		void RegisterToGlobalSettings();
	};
}
#endif // !SGENGINE_CAMERA_H



