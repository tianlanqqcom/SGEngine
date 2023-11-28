#include "GlobalSettings.h"
#include "../GameObject.h"
#include "../Scene.h"
#include "RenderInfo.h"
#include "../EngineTime.h"

EngineTools::RenderInfo::RenderInfo()
{
	typeHashCode = typeid(RenderInfo).hash_code();
}

void EngineTools::RenderInfo::BindModel(const std::string& path, ModelType type)
{
	int modelID = ResourceManager::GetInstance()->LoadObjectAtPath(path, type);
	if (modelID != -1)
	{
		model = ResourceManager::GetInstance()->FindModelAsset(modelID);
		hasChange = true;
	}
}

inline void EngineTools::RenderInfo::BindModel(ModelAsset* _model)
{
	model = _model;
	hasChange = true;
}

void EngineTools::RenderInfo::UnBindModel()
{
	model = nullptr;
	hasChange = true;
}

void EngineTools::RenderInfo::Start()
{
	// ����Ѱ�ģ�ͣ�ע�ᵽȫ����Ϣ
	if (model)
	{
		gameObject->scene->GetSharedComponent<GlobalSettings>()->modelMap.insert({ this, model });
	}
}

void EngineTools::RenderInfo::Update()
{
	// ����б仯������ע��
	if (hasChange)
	{
		gameObject->scene->GetSharedComponent<GlobalSettings>()->modelMap[this] = model;
	}

	// Used for debug
	// gameObject->transform->Rotation({ 0,0 * EngineTools::EngineTime::GetInstance()->deltaTimef, 10 * EngineTools::EngineTime::GetInstance()->deltaTimef });
}
