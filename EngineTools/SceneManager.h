//
// Created by tianlan.
// �����������������ڶ������ʱ���ڸ�����������Unity��SceneManager�ļ򻯰汾
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
		std::stack<Scene*> sceneStack; // ������ջ
	public:
		Scene* NowActiveScene = nullptr; // ��ǰ�������Ĭ��Ϊ�գ���ʼʱ��ҪLoad��Push����

		// ֱ�Ӽ��س������ò���ֱ���滻��ǰ����������Գ���ջ������ͬʱ���ͷžɵĳ���
		// @ para	Scene*	��Ҫ���صĳ���ָ��
		void LoadScene(Scene* s);

		// �л����³��������ͷžɳ���
		// @ para	Scene*	��Ҫ���صĳ���ָ��
		void PushScene(Scene* s);

		// �ͷŵ�ǰ��г���������ջ����������Ϊ�����
		void PopScene();
	};
}

#endif // SGENGINE_SCENEMANAGER

