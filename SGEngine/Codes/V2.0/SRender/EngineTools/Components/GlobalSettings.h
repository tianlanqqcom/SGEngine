// 
// Created by tianlan.
// ȫ�����ã�������һ�������еĲ�ͬ�����������ݣ�Ҳ�����ڳ������������
//
// ������־��
// V1.0 -> V2.0:
// ����˶Թ��ա��������������Ϣ���ռ�


#ifndef SGENGINE_GLOBALSETTINGS
#define SGENGINE_GLOBALSETTINGS

#include "../ResourceManager.h"
#include "../Component.h"
#include "Camera.h"
#include "Light.h"

namespace EngineTools
{
	class RenderInfo;

	class GlobalSettings : public Component
	{
	public:
		GlobalSettings();

		const std::string EngineVersion = "2.0";	// ��ǰ����汾2.0

		std::map<const RenderInfo*, ModelAsset*> modelMap;		// ģ����Ϣ
		std::map<int, Camera*> cameras;							// �������Ϣ
		std::map<LightType, std::vector<Light*>> orderedLights;	// ������Ϣ

		std::string ToString(int depth = 0) const override;

		void Start() override;
	};
}

#endif
