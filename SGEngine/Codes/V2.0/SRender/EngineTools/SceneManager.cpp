#include "SceneManager.h"

void EngineTools::SceneManager::LoadScene(Scene* s)
{
	// �ͷžɳ���
	delete NowActiveScene;

	// ����Ϊ�³�������ʼ��
	NowActiveScene = s;
	NowActiveScene->CallStart();
}

void EngineTools::SceneManager::PushScene(Scene* s)
{
	// �������Ϊnullptr,�ܾ��ôβ���
	if (!s)
	{
		throw std::exception(); // �������滻������ר���쳣
	}

	// �����ǰ�������Ч�����뵽����ջ
	if (NowActiveScene)
	{
		sceneStack.emplace(NowActiveScene);
	}
	
	// ����Ϊ�³�������ʼ��
	NowActiveScene = s;
	NowActiveScene->CallStart();
	
}

void EngineTools::SceneManager::PopScene()
{
	// �ͷžɳ���
	delete NowActiveScene;

	// ��ȡջ������
	NowActiveScene = sceneStack.top();
	sceneStack.pop();
}
