// 
// Created by tianlan.
// ȫ�����ã�������һ�������еĲ�ͬ�����������ݣ�Ҳ�����ڳ������������

#ifndef SGENGINE_GLOBALSETTINGS
#define SGENGINE_GLOBALSETTINGS
#include "../Component.h"

namespace EngineTools
{
	class GlobalSettings : public Component
	{
	public:
		const std::string EngineVersion = "1.0";	// ��ǰ����汾

		std::string ToString(int depth = 0) const override;
	};
}

#endif
