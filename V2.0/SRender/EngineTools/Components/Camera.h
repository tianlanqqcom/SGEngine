/*
*	Created by tianlan.
*	�������������ڹ۲�ͻ��Ƶ�ǰ�����ڵ�����
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
	// ���ڿ����������Ӧ�Ĵ��ڴ�С����
	// ���棺��ǰѡ����Ч
	enum class CameraType
	{
		FULLWINDOW,	// ȫ����״̬�£���������ݻ�������Ļ
		AREA
	};

	// ���������Ŀ�괰�ڴ�С
	// �ýṹ���ֵ��Χ����0-1֮��
	// �ߺͿ�Ϊ�������εĸߺͿ�
	struct CameraRect
	{
		float center_x = 0.5f;
		float center_y = 0.5f;
		float height = 1.0f;
		float width = 1.0f;
	};

	class Camera : public Component
	{
		static int cameraCount;		// �Ѿ����������������

		// ��ȡLookAt����
		// @para eye		�������ǰ��������
		// @para lookAt		�����ָ��ķ���
		// @para wUp		������ָ�����ϵķ���
		// @ret  SRender::MathTools::Matrix<double>	�õ���lookAt����
		SRender::MathTools::Matrix<double> GetLookAtMatrix(Vector3 eye, Vector3 lookAt, Vector3 wUp);
	public:
		int cameraID;				// ���ID

		float fov = 45.0f;			// �ӽǴ�С
		float cameraNear = 0.1f;	// ��ƽ��
		float cameraFar = 100.0f;	// Զƽ��

		// ��������
		// ���棺��ǰѡ����Ч
		CameraType cameraType = CameraType::FULLWINDOW;

		// ���ڴ�С
		// ���棺��ǰѡ����Ч
		CameraRect cameraRect;

		// Ĭ���������Ϸ���Ϊy��������
		Vector3 worldUp = Vector3(0.0, 1.0, 0.0);

		// Ĭ��ǰ��ָ��Ϊz��������
		Vector3 front = Vector3(0.0, 0.0, 1.0);

		Camera();

		void Start() override;

		// ���ڸ���front
		void Update() override;

		// ��ȡLookAt����Ŀ�ݷ�ʽ
		SRender::MathTools::Matrix<double> GetLookAtMatrix();

		// �ѵ�ǰ���ע�ᵽȫ����Ϣ
		void RegisterToGlobalSettings();
	};
}
#endif // !SGENGINE_CAMERA_H



