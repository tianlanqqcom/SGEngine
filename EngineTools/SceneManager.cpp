#include "SceneManager.h"

void EngineTools::SceneManager::LoadScene(Scene* s)
{
	// 释放旧场景
	delete NowActiveScene;

	// 设置为新场景并初始化
	NowActiveScene = s;
	NowActiveScene->CallStart();
}

void EngineTools::SceneManager::PushScene(Scene* s)
{
	// 如果参数为nullptr,拒绝该次操作
	if (!s)
	{
		throw std::exception(); // 后续会替换成引擎专有异常
	}

	// 如果当前活动场景有效，推入到场景栈
	if (NowActiveScene)
	{
		sceneStack.emplace(NowActiveScene);
	}
	
	// 设置为新场景并初始化
	NowActiveScene = s;
	NowActiveScene->CallStart();
	
}

void EngineTools::SceneManager::PopScene()
{
	// 释放旧场景
	delete NowActiveScene;

	// 获取栈顶场景
	NowActiveScene = sceneStack.top();
	sceneStack.pop();
}
