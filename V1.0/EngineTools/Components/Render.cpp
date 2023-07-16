//
// Created by tianlan on 2023/5/3.
//

#include "Render.h"

namespace EngineTools
{
	void Render::SetLight()
	{
		// 获取场景中的光照信息
		auto pLight = gameObject->scene->GetSharedComponent<Light>();

		// 如果光照或相机或着色器或模型中任意一个没有设置，不绘制该物体
		if (!pLight || !camera || !shaderObj || !pModel)
		{
			flag = false;
		}
		else
		{
			flag = true;
		}

		if (!flag)
		{
			return;
		}

		// 设置着色器数据
		glUniform3f(glGetUniformLocation(shaderObj->Program, "sunLight.direction"), pLight->sunLight.direction.x, pLight->sunLight.direction.y, pLight->sunLight.direction.z);
		glUniform3f(glGetUniformLocation(shaderObj->Program, "sunLight.diffuse"), pLight->sunLight.diffuse.x, pLight->sunLight.diffuse.y, pLight->sunLight.diffuse.z);
		glUniform3f(glGetUniformLocation(shaderObj->Program, "sunLight.specular"), pLight->sunLight.specular.x, pLight->sunLight.specular.y, pLight->sunLight.specular.z);
		glUniform3f(glGetUniformLocation(shaderObj->Program, "pointLights[0].position"), pLight->pointLight[0].position.x, pLight->pointLight[0].position.y, pLight->pointLight[0].position.z);
		glUniform3f(glGetUniformLocation(shaderObj->Program, "pointLights[0].diffuse"), pLight->pointLight[0].diffuse.x, pLight->pointLight[0].diffuse.y, pLight->pointLight[0].diffuse.z);
		glUniform3f(glGetUniformLocation(shaderObj->Program, "pointLights[0].specular"), pLight->pointLight[0].specular.x, pLight->pointLight[0].specular.y, pLight->pointLight[0].specular.z);
		glUniform1f(glGetUniformLocation(shaderObj->Program, "pointLights[0].k0"), pLight->pointLight[0].k0);
		glUniform1f(glGetUniformLocation(shaderObj->Program, "pointLights[0].k1"), pLight->pointLight[0].k1);
		glUniform1f(glGetUniformLocation(shaderObj->Program, "pointLights[0].k2"), pLight->pointLight[0].k2);
		glUniform3f(glGetUniformLocation(shaderObj->Program, "pointLights[1].position"), pLight->pointLight[1].position.x, pLight->pointLight[1].position.y, pLight->pointLight[1].position.z);
		glUniform3f(glGetUniformLocation(shaderObj->Program, "pointLights[1].diffuse"), pLight->pointLight[1].diffuse.x, pLight->pointLight[1].diffuse.y, pLight->pointLight[1].diffuse.z);
		glUniform3f(glGetUniformLocation(shaderObj->Program, "pointLights[1].specular"), pLight->pointLight[1].specular.x, pLight->pointLight[1].specular.y, pLight->pointLight[1].specular.z);
		glUniform1f(glGetUniformLocation(shaderObj->Program, "pointLights[1].k0"), pLight->pointLight[1].k0);
		glUniform1f(glGetUniformLocation(shaderObj->Program, "pointLights[1].k1"), pLight->pointLight[1].k1);
		glUniform1f(glGetUniformLocation(shaderObj->Program, "pointLights[1].k2"), pLight->pointLight[1].k2);
		glUniform3f(glGetUniformLocation(shaderObj->Program, "pointLights[2].position"), pLight->pointLight[2].position.x, pLight->pointLight[2].position.y, pLight->pointLight[2].position.z);
		glUniform3f(glGetUniformLocation(shaderObj->Program, "pointLights[2].diffuse"), pLight->pointLight[2].diffuse.x, pLight->pointLight[2].diffuse.y, pLight->pointLight[2].diffuse.z);
		glUniform3f(glGetUniformLocation(shaderObj->Program, "pointLights[2].specular"), pLight->pointLight[2].specular.x, pLight->pointLight[2].specular.y, pLight->pointLight[2].specular.z);
		glUniform1f(glGetUniformLocation(shaderObj->Program, "pointLights[2].k0"), pLight->pointLight[2].k0);
		glUniform1f(glGetUniformLocation(shaderObj->Program, "pointLights[2].k1"), pLight->pointLight[2].k1);
		glUniform1f(glGetUniformLocation(shaderObj->Program, "pointLights[2].k2"), pLight->pointLight[2].k2);
		glUniform3f(glGetUniformLocation(shaderObj->Program, "spotLight.position"), pLight->spotLight.position.x, pLight->spotLight.position.y, pLight->spotLight.position.z);
		glUniform3f(glGetUniformLocation(shaderObj->Program, "spotLight.direction"), pLight->spotLight.direction.x, pLight->spotLight.direction.y, pLight->spotLight.direction.z);
		glUniform3f(glGetUniformLocation(shaderObj->Program, "spotLight.diffuse"), pLight->spotLight.diffuse.x, pLight->spotLight.diffuse.y, pLight->spotLight.diffuse.z);
		glUniform3f(glGetUniformLocation(shaderObj->Program, "spotLight.specular"), pLight->spotLight.specular.x, pLight->spotLight.specular.y, pLight->spotLight.specular.z);
		glUniform1f(glGetUniformLocation(shaderObj->Program, "spotLight.k0"), pLight->spotLight.k0);
		glUniform1f(glGetUniformLocation(shaderObj->Program, "spotLight.k1"), pLight->spotLight.k1);
		glUniform1f(glGetUniformLocation(shaderObj->Program, "spotLight.k2"), pLight->spotLight.k2);
		glUniform1f(glGetUniformLocation(shaderObj->Program, "spotLight.cutOff"), pLight->spotLight.cutOff);
		glUniform1f(glGetUniformLocation(shaderObj->Program, "spotLight.outCutOff"), pLight->spotLight.outCutOff);
		glUniform3f(glGetUniformLocation(shaderObj->Program, "ambient"), pLight->ambient.x, pLight->ambient.y, pLight->ambient.z);
		glUniform3f(glGetUniformLocation(shaderObj->Program, "viewPos"), camera->Position.x, camera->Position.y, camera->Position.z);

		
		auto viewLoc = glGetUniformLocation(shaderObj->Program, "view");
		auto projLoc = glGetUniformLocation(shaderObj->Program, "projection");

		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (GLfloat)camera->width / (GLfloat)camera->height, 0.1f, 100.0f);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	}
	Render::Render()
	{
		typeHashCode = typeid(Render).hash_code();	// 设置组件哈希码
	}
	void Render::Update()
	{
		// 使用物体的Shader
		shaderObj->Use();
		// 设置光照
		SetLight();

		// 如果flag和模型均为true，绘制物体
		if (pModel && flag)
		{
			//shaderObj->Use();
			Transform* transform = gameObject->GetComponent<Transform>();
			glm::mat4 model = glm::mat4(1.0f);
			auto modelLoc = glGetUniformLocation(shaderObj->Program, "model");
			model = glm::translate(glm::mat4(1.0f), glm::vec3(transform->worldPosition.x, transform->worldPosition.y, transform->worldPosition.z));
			model = glm::rotate(model, glm::radians(transform->rotation.y), glm::vec3(0, 1.0f, 0));
			model = glm::rotate(model, glm::radians(transform->rotation.x), glm::vec3(1.0f, 0, 0));
			model = glm::rotate(model, glm::radians(transform->rotation.z), glm::vec3(0, 0, 1.0f));
			model = glm::scale(model, glm::vec3(transform->scale.x, transform->scale.y, transform->scale.z));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			pModel->Draw(*shaderObj);
		}
	}

	void Render::SetModel(std::string modelPath)
	{
		delete pModel;	// 删除旧模型
		pModel = new Model(modelPath.c_str()); // 设置模型
	}

	void Render::SetCamera(Camera* camera)
	{
		this->camera = camera;	// 设置相机
	}

	void Render::SetShader(Shader* shader)
	{
		delete shaderObj;	// 删除原先的Shader
		shaderObj = shader;	// 设置新的Shader
	}

	std::string Render::ToString(int depth) const
	{
		std::string ret = "";

		// 输出组件名
		for (int i = 0; i < depth; i++)
		{
			ret += ' ';
		}
		ret += "-Component:Render\n";

		return ret;
	}
} // EngineTools