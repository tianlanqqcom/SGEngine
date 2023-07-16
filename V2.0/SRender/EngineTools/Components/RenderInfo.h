/*
*	Created by tianlan.
*	用于存储物体的模型等渲染信息的组件
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
		ModelAsset* model = nullptr;	// 模型资产指针
		bool hasChange = true;			// 标记是否有变化
	public:
		RenderInfo();

		// 绑定对应路径的模型
		// @para path	文件路径
		// @para type	模型文件类型
		void BindModel(const std::string& path, ModelType type = ModelType::OBJ);

		// 直接绑定对应指针的模型到自己
		// Note: 不检查空指针，故BindModel(nullptr) = UnBindModel()
		void BindModel(ModelAsset* _model);

		// 取消与当前模型的绑定
		void UnBindModel();

		void Start() override;

		void Update() override;
	};
}
#endif // !SENGINE_RENDERINFO_H


