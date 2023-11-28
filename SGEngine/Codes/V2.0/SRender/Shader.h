/*
*	这里是主要Shader工作管线部分，
*	注意：Work函数直接绘制当前场景信息里的所有物体，所以每次绘制的Shader只能有一个
* 
*	class Shader;
*	着色器基类，可重写的部分有顶点着色器和片段着色器（默认是一个纯色着色器）
*/

#ifndef SRENDER_SHADER_H
#define SRENDER_SHADER_H
#include <thread>
#include <mutex>
#include "EngineTools/ResourceManager.h"
#include "EngineTools/Components/Camera.h"
#include "Layer.h"

typedef std::pair<int, int> Point2D;

// 把一个Vector3转换到Point2D，丢弃z信息
#define VEC3TOPOINT2D(p)		{(p).x, (p.y)}

// Point2D点积
#define POINT2D_DOT(p1, p2)		((p1.first) * (p2.first) + (p1.second) * (p2.second)) 

// Point2D叉积
#define POINT2D_CROSS(u, v)		(u.first * v.second - u.second * v.first)

namespace EngineTools 
{
	class GlobalSettings;	// 前置声明
}

namespace SRender
{
	class Shader
	{
		// 获取透视矩阵
		// @para fov	视角
		// @para aspect	横纵比
		// @para near	近平面
		// @para far	远屏幕
		static SRender::MathTools::Matrix<double> GetPerspectiveMatrix(double fov, double aspect, double near, double far);

		// 判断目标点是否在三角形里
		// @para targetPoint	目标点
		// @para p1				三角形顶点1
		// @para p2				三角形顶点2
		// @para p3				三角形顶点3
		// @ret	 bool			目标点是否在三角形里，如果是返回true
		static bool InTriangle(const Point2D& targetPoint, const Point2D& p1, const Point2D& p2, const Point2D& p3);

		// 根据某点重心分量判断目标点是否在三角形里
		// @para interpolate	重心分量
		// @ret	 bool			目标点是否在三角形里，如果是返回true
		static bool InTriangle(const EngineTools::Vector3& interpolate);

		// 判断三角形是否为逆时针环绕
		// @para p1		三角形顶点1
		// @para p2		三角形顶点2
		// @para p3		三角形顶点3
		// @ret	 bool	是否逆时针环绕，如果是返回true
		static bool IsAntiClockWise(const Point2D& p1, const Point2D& p2, const Point2D& p3);

		// 计算目标点在三角形的重心分量
		// @para p1				三角形顶点1
		// @para p2				三角形顶点2
		// @para p3				三角形顶点3
		// @para targetPoint	目标点
		static EngineTools::Vector3 BaryCentric(const Point2D& p1, const Point2D& p2, const Point2D& p3, const Point2D& targetPoint);

		// 根据重心分量计算某一点的深度值
		// @para interpolate	重心分量
		// @para z1				深度1
		// @para z2				深度2
		// @para z3				深度3
		// @ret	 double			得到的深度值
		static double DoInterpolate(const EngineTools::Vector3& interpolate, double z1, double z2, double z3); 

		// 根据重心分量和已知的三个4*1的点矩阵得到目标点的点矩阵
		// @para interpolate						重心分量
		// @para p1									顶点1的点矩阵
		// @para p2									顶点2的点矩阵
		// @para p3									顶点3的点矩阵
		// @ret  SRender::MathTools::Matrix<double>	得到的点矩阵
		static SRender::MathTools::Matrix<double> CalcScreenPosWithBaryCentric(const EngineTools::Vector3& interpolate,
			const SRender::MathTools::Matrix<double>& p1,
			const SRender::MathTools::Matrix<double>& p2,
			const SRender::MathTools::Matrix<double>& p3);
	public:
		// 顶点着色器，根据MVP变换得到点矩阵
		// @para point								目标点的本地位置
		// @para modelMat							模型矩阵
		// @para viewMat							观察矩阵
		// @para perspectiveMat						透视矩阵
		// @ret  SRender::MathTools::Matrix<double> 得到的点矩阵
		virtual SRender::MathTools::Matrix<double> Vert(const EngineTools::Vector3& point,
			const SRender::MathTools::Matrix<double>& modelMat,
			const SRender::MathTools::Matrix<double>& viewMat,
			const SRender::MathTools::Matrix<double>& perspectiveMat);

		// 片段着色器，确定每个像素点的颜色
		// @para interpolate	重心分量
		// @para viewPos		摄像机位置
		// @para fragPos		目标片元的世界位置
		// @para normalIn		法线在世界空间的值
		// @para glSet			全局信息保存组件
		// 
		// Note: 下面这三个连续的参数是因为一个模型的材质、顶点缓冲等信息是分开存储的
		// @para nowModel		正在绘制的模型
		// @para nowMesh		正在绘制的网格体
		// @para nowFace		正在绘制的面
		// 
		// @ret	 RGBAColor		得到的颜色
		virtual RGBAColor Frag(
			const EngineTools::Vector3& interpolate,
			const EngineTools::Vector3& viewPos,
			const EngineTools::Vector3& fragPos,
			const EngineTools::Vector3& normalIn,
			EngineTools::GlobalSettings* glSet,
			EngineTools::ModelAsset* nowModel, 
			EngineTools::MeshAsset* nowMesh,
			EngineTools::FaceAsset* nowFace);

		// 绘制屏幕
		// Note: 摄像机数据不影响画布的输出，当前摄像机仅能在全窗口模式下工作
		// @para screenWidth	屏幕宽度
		// @para screenHeight	屏幕高度
		// @para glSet			全局信息保存组件
		// @ret	 SRender::Layer	结果层
		SRender::Layer Draw(int screenWidth, int screenHeight, EngineTools::GlobalSettings* glSet);
	};
}



#endif // !SRENDER_SHADER_H



