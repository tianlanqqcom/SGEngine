#ifndef SRENDER_PHONGSHADER_H
#define SRENDER_PHONGSHADER_H

#include "Shader.h"
#include "EngineTools/Components/GlobalSettings.h"

namespace SRender
{
	class PhongShader : public Shader
	{
		// 计算纹理坐标
		// @para interpolate				重心分量
		// @para vt1						顶点纹理坐标1
		// @para vt2						顶点纹理坐标2
		// @para vt3						顶点纹理坐标3
		// @ret  std::pair<double, double>	最终的顶点纹理坐标, 范围[0-1]
		std::pair<double, double> CalcTexturePos(const EngineTools::Vector3& interpolate,
			const std::pair<double, double>& vt1,
			const std::pair<double, double>& vt2,
			const std::pair<double, double>& vt3);

		// 计算太阳光的RGB贡献分量
		// @para sunLight				太阳光的指针
		// @para normal					法线
		// @para viewDirection			视角方向
		// @para material				材质的指针
		// @ret  EngineTools::Vector3	太阳光的RGB贡献分量
		EngineTools::Vector3 CalcSunLight(EngineTools::Light* sunLight,
			const EngineTools::Vector3& normal,
			const EngineTools::Vector3& viewDirection,
			EngineTools::MaterialAsset* material);

		// 计算点光源的RGB贡献分量
		// @para pointLight				点光源的指针
		// @para normal					法线
		// @para fragPos				片元的世界位置
		// @para viewDirection			视角方向
		// @para material				材质的指针
		// @ret  EngineTools::Vector3	点光源的RGB贡献分量
		EngineTools::Vector3 CalcPointLight(EngineTools::Light* pointLight,
			const EngineTools::Vector3& normal,
			const EngineTools::Vector3& fragPos,
			const EngineTools::Vector3& viewDirection,
			EngineTools::MaterialAsset* material);

		// 计算聚光灯的RGB贡献分量
		// @para spotLight				点光源的指针
		// @para normal					法线
		// @para fragPos				片元的世界位置
		// @para viewDirection			视角方向
		// @para material				材质的指针
		// @ret  EngineTools::Vector3	聚光灯的RGB贡献分量
		EngineTools::Vector3 CalcSpotLight(EngineTools::Light* spotLight,
			const EngineTools::Vector3& normal,
			const EngineTools::Vector3& fragPos,
			const EngineTools::Vector3& viewDirection,
			EngineTools::MaterialAsset* material);

		// 夹具函数，返回三个参数的中间值
		// @para x		目标值
		// @para minVal	最小值
		// @para maxVal	最大值
		// @ret  double	返回值
		double Clamp(double x, double minVal, double maxVal);

		// 计算反射光线的方向
		// @para I						入射光方向
		// @para N						法线
		// @ret  EngineTools::Vector3	反射光方向
		EngineTools::Vector3 Reflect(const EngineTools::Vector3& I, const EngineTools::Vector3& N);

	public:
		// 重载的片段着色器
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
		RGBAColor Frag(
			const EngineTools::Vector3& interpolate,
			const EngineTools::Vector3& viewPos,
			const EngineTools::Vector3& fragPos,
			const EngineTools::Vector3& normalIn,
			EngineTools::GlobalSettings* glSet,
			EngineTools::ModelAsset* nowModel,
			EngineTools::MeshAsset* nowMesh,
			EngineTools::FaceAsset* nowFace) override;
	};
}

#endif // !SRENDER_PHONGSHADER_H




