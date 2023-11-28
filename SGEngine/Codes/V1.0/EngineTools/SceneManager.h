//
// Created by tianlan.
// 场景管理器，当存在多个场景时用于辅助管理场景，Unity中SceneManager的简化版本
//
#ifndef SGENGINE_SCENEMANAGER
#define SGENGINE_SCENEMANAGER
#include <stack>
#include "Scene.h"

namespace EngineTools 
{
	class SceneManager
	{
	private:
		std::stack<Scene*> sceneStack; // 场景堆栈
	public:
		Scene* NowActiveScene = nullptr; // 当前活动场景，默认为空，开始时需要Load或Push操作

		// 直接加载场景，该操作直接替换当前活动场景，不对场景栈操作，同时会释放旧的场景
		// @ para	Scene*	将要加载的场景指针
		void LoadScene(Scene* s);

		// 切换到新场景但不释放旧场景
		// @ para	Scene*	将要加载的场景指针
		void PushScene(Scene* s);

		// 释放当前活动中场景，并把栈顶场景设置为活动场景
		void PopScene();
	};
}

#endif // SGENGINE_SCENEMANAGER

