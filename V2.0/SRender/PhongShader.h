#ifndef SRENDER_PHONGSHADER_H
#define SRENDER_PHONGSHADER_H

#include "Shader.h"
#include "EngineTools/Components/GlobalSettings.h"

namespace SRender
{
	class PhongShader : public Shader
	{
		// ������������
		// @para interpolate				���ķ���
		// @para vt1						������������1
		// @para vt2						������������2
		// @para vt3						������������3
		// @ret  std::pair<double, double>	���յĶ�����������, ��Χ[0-1]
		std::pair<double, double> CalcTexturePos(const EngineTools::Vector3& interpolate,
			const std::pair<double, double>& vt1,
			const std::pair<double, double>& vt2,
			const std::pair<double, double>& vt3);

		// ����̫�����RGB���׷���
		// @para sunLight				̫�����ָ��
		// @para normal					����
		// @para viewDirection			�ӽǷ���
		// @para material				���ʵ�ָ��
		// @ret  EngineTools::Vector3	̫�����RGB���׷���
		EngineTools::Vector3 CalcSunLight(EngineTools::Light* sunLight,
			const EngineTools::Vector3& normal,
			const EngineTools::Vector3& viewDirection,
			EngineTools::MaterialAsset* material);

		// ������Դ��RGB���׷���
		// @para pointLight				���Դ��ָ��
		// @para normal					����
		// @para fragPos				ƬԪ������λ��
		// @para viewDirection			�ӽǷ���
		// @para material				���ʵ�ָ��
		// @ret  EngineTools::Vector3	���Դ��RGB���׷���
		EngineTools::Vector3 CalcPointLight(EngineTools::Light* pointLight,
			const EngineTools::Vector3& normal,
			const EngineTools::Vector3& fragPos,
			const EngineTools::Vector3& viewDirection,
			EngineTools::MaterialAsset* material);

		// ����۹�Ƶ�RGB���׷���
		// @para spotLight				���Դ��ָ��
		// @para normal					����
		// @para fragPos				ƬԪ������λ��
		// @para viewDirection			�ӽǷ���
		// @para material				���ʵ�ָ��
		// @ret  EngineTools::Vector3	�۹�Ƶ�RGB���׷���
		EngineTools::Vector3 CalcSpotLight(EngineTools::Light* spotLight,
			const EngineTools::Vector3& normal,
			const EngineTools::Vector3& fragPos,
			const EngineTools::Vector3& viewDirection,
			EngineTools::MaterialAsset* material);

		// �оߺ��������������������м�ֵ
		// @para x		Ŀ��ֵ
		// @para minVal	��Сֵ
		// @para maxVal	���ֵ
		// @ret  double	����ֵ
		double Clamp(double x, double minVal, double maxVal);

		// ���㷴����ߵķ���
		// @para I						����ⷽ��
		// @para N						����
		// @ret  EngineTools::Vector3	����ⷽ��
		EngineTools::Vector3 Reflect(const EngineTools::Vector3& I, const EngineTools::Vector3& N);

	public:
		// ���ص�Ƭ����ɫ��
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




