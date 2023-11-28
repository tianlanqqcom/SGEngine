/*
*	Created by tianlan.
*	���ڴ洢�����ģ�͵���Ⱦ��Ϣ�����
* 
*	class RenderInfo;
*/
#ifndef SENGINE_RENDERINFO_H
#define SENGINE_RENDERINFO_H
#include "../Component.h"
#include "../ResourceManager.h"

namespace EngineTools 
{
	class RenderInfo: public Component
	{
		ModelAsset* model = nullptr;	// ģ���ʲ�ָ��
		bool hasChange = true;			// ����Ƿ��б仯
	public:
		RenderInfo();

		// �󶨶�Ӧ·����ģ��
		// @para path	�ļ�·��
		// @para type	ģ���ļ�����
		void BindModel(const std::string& path, ModelType type = ModelType::OBJ);

		// ֱ�Ӱ󶨶�Ӧָ���ģ�͵��Լ�
		// Note: ������ָ�룬��BindModel(nullptr) = UnBindModel()
		void BindModel(ModelAsset* _model);

		// ȡ���뵱ǰģ�͵İ�
		void UnBindModel();

		void Start() override;

		void Update() override;
	};
}
#endif // !SENGINE_RENDERINFO_H


