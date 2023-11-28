/*
*	��������ҪShader�������߲��֣�
*	ע�⣺Work����ֱ�ӻ��Ƶ�ǰ������Ϣ����������壬����ÿ�λ��Ƶ�Shaderֻ����һ��
* 
*	class Shader;
*	��ɫ�����࣬����д�Ĳ����ж�����ɫ����Ƭ����ɫ����Ĭ����һ����ɫ��ɫ����
*/

#ifndef SRENDER_SHADER_H
#define SRENDER_SHADER_H
#include <thread>
#include <mutex>
#include "EngineTools/ResourceManager.h"
#include "EngineTools/Components/Camera.h"
#include "Layer.h"

typedef std::pair<int, int> Point2D;

// ��һ��Vector3ת����Point2D������z��Ϣ
#define VEC3TOPOINT2D(p)		{(p).x, (p.y)}

// Point2D���
#define POINT2D_DOT(p1, p2)		((p1.first) * (p2.first) + (p1.second) * (p2.second)) 

// Point2D���
#define POINT2D_CROSS(u, v)		(u.first * v.second - u.second * v.first)

namespace EngineTools 
{
	class GlobalSettings;	// ǰ������
}

namespace SRender
{
	class Shader
	{
		// ��ȡ͸�Ӿ���
		// @para fov	�ӽ�
		// @para aspect	���ݱ�
		// @para near	��ƽ��
		// @para far	Զ��Ļ
		static SRender::MathTools::Matrix<double> GetPerspectiveMatrix(double fov, double aspect, double near, double far);

		// �ж�Ŀ����Ƿ�����������
		// @para targetPoint	Ŀ���
		// @para p1				�����ζ���1
		// @para p2				�����ζ���2
		// @para p3				�����ζ���3
		// @ret	 bool			Ŀ����Ƿ��������������Ƿ���true
		static bool InTriangle(const Point2D& targetPoint, const Point2D& p1, const Point2D& p2, const Point2D& p3);

		// ����ĳ�����ķ����ж�Ŀ����Ƿ�����������
		// @para interpolate	���ķ���
		// @ret	 bool			Ŀ����Ƿ��������������Ƿ���true
		static bool InTriangle(const EngineTools::Vector3& interpolate);

		// �ж��������Ƿ�Ϊ��ʱ�뻷��
		// @para p1		�����ζ���1
		// @para p2		�����ζ���2
		// @para p3		�����ζ���3
		// @ret	 bool	�Ƿ���ʱ�뻷�ƣ�����Ƿ���true
		static bool IsAntiClockWise(const Point2D& p1, const Point2D& p2, const Point2D& p3);

		// ����Ŀ����������ε����ķ���
		// @para p1				�����ζ���1
		// @para p2				�����ζ���2
		// @para p3				�����ζ���3
		// @para targetPoint	Ŀ���
		static EngineTools::Vector3 BaryCentric(const Point2D& p1, const Point2D& p2, const Point2D& p3, const Point2D& targetPoint);

		// �������ķ�������ĳһ������ֵ
		// @para interpolate	���ķ���
		// @para z1				���1
		// @para z2				���2
		// @para z3				���3
		// @ret	 double			�õ������ֵ
		static double DoInterpolate(const EngineTools::Vector3& interpolate, double z1, double z2, double z3); 

		// �������ķ�������֪������4*1�ĵ����õ�Ŀ���ĵ����
		// @para interpolate						���ķ���
		// @para p1									����1�ĵ����
		// @para p2									����2�ĵ����
		// @para p3									����3�ĵ����
		// @ret  SRender::MathTools::Matrix<double>	�õ��ĵ����
		static SRender::MathTools::Matrix<double> CalcScreenPosWithBaryCentric(const EngineTools::Vector3& interpolate,
			const SRender::MathTools::Matrix<double>& p1,
			const SRender::MathTools::Matrix<double>& p2,
			const SRender::MathTools::Matrix<double>& p3);
	public:
		// ������ɫ��������MVP�任�õ������
		// @para point								Ŀ���ı���λ��
		// @para modelMat							ģ�;���
		// @para viewMat							�۲����
		// @para perspectiveMat						͸�Ӿ���
		// @ret  SRender::MathTools::Matrix<double> �õ��ĵ����
		virtual SRender::MathTools::Matrix<double> Vert(const EngineTools::Vector3& point,
			const SRender::MathTools::Matrix<double>& modelMat,
			const SRender::MathTools::Matrix<double>& viewMat,
			const SRender::MathTools::Matrix<double>& perspectiveMat);

		// Ƭ����ɫ����ȷ��ÿ�����ص����ɫ
		// @para interpolate	���ķ���
		// @para viewPos		�����λ��
		// @para fragPos		Ŀ��ƬԪ������λ��
		// @para normalIn		����������ռ��ֵ
		// @para glSet			ȫ����Ϣ�������
		// 
		// Note: ���������������Ĳ�������Ϊһ��ģ�͵Ĳ��ʡ����㻺�����Ϣ�Ƿֿ��洢��
		// @para nowModel		���ڻ��Ƶ�ģ��
		// @para nowMesh		���ڻ��Ƶ�������
		// @para nowFace		���ڻ��Ƶ���
		// 
		// @ret	 RGBAColor		�õ�����ɫ
		virtual RGBAColor Frag(
			const EngineTools::Vector3& interpolate,
			const EngineTools::Vector3& viewPos,
			const EngineTools::Vector3& fragPos,
			const EngineTools::Vector3& normalIn,
			EngineTools::GlobalSettings* glSet,
			EngineTools::ModelAsset* nowModel, 
			EngineTools::MeshAsset* nowMesh,
			EngineTools::FaceAsset* nowFace);

		// ������Ļ
		// Note: ��������ݲ�Ӱ�컭�����������ǰ�����������ȫ����ģʽ�¹���
		// @para screenWidth	��Ļ���
		// @para screenHeight	��Ļ�߶�
		// @para glSet			ȫ����Ϣ�������
		// @ret	 SRender::Layer	�����
		SRender::Layer Draw(int screenWidth, int screenHeight, EngineTools::GlobalSettings* glSet);
	};
}



#endif // !SRENDER_SHADER_H



