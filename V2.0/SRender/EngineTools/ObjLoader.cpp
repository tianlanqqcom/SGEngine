#include "ObjLoader.h"
#include "ResourceManager.h"

using namespace EngineTools;

EngineTools::MeshAsset* EngineTools::ObjLoader::nowOperatingMesh = nullptr;
EngineTools::MaterialAsset* EngineTools::ObjLoader::nowOperatingMaterial = nullptr;
std::string EngineTools::ObjLoader::nowUsingMaterialName = "";

EngineTools::ModelAsset* EngineTools::ObjLoader::LoadObj(const std::string& path)
{
	// ���ļ������ʧ��ֱ�ӷ���
	std::ifstream file(path, std::ios::in);
	if (!file.is_open() || !file.good())
	{
		return nullptr;
	}

	// ÿһ�е����ݵĻ�����
	char line[256]{};
	// �ָ�õ�����
	std::vector<std::string> spLine;

	// ���ģ��
	ModelAsset* res = new ModelAsset;
	
	// ѭ����ȡÿһ��
	while (!file.eof())
	{
		file.getline(line, 256);

		// ����
		if (strcmp("", line) == 0)
		{
			continue;
		}

		// �ָ�����
		spLine.clear();
		Stringsplit(std::string(line), ' ', spLine);

		// �����ע��
		if (spLine[0] == "#")
		{
			continue;
		}

		// ���в���
		DoOperation(res, spLine);
	}
	return res;
}

void EngineTools::ObjLoader::DoOperation(EngineTools::ModelAsset* model, std::vector<std::string> cmd)
{
	// �򿪲��ʿ⣬��ȡ����
	if (cmd[0] == "mtllib" && cmd.size() == 2)
	{
		ReadMaterial(cmd[1], model);
	}
	// ����������
	else if (cmd[0] == "o" && cmd.size() == 2)
	{
		MeshAsset* newMesh = new MeshAsset;
		nowOperatingMesh = newMesh;
		newMesh->meshName = cmd[1];
		newMesh->parentModel = model;
		model->meshes[cmd[1]] = newMesh;
	}
	// ��������
	else if (cmd[0] == "v" && cmd.size() == 4)
	{
		if (nowOperatingMesh)
		{
			nowOperatingMesh->vertex.emplace_back(
				Vector3(stringToNum<float>(cmd[1]), stringToNum<float>(cmd[2]), stringToNum<float>(cmd[3])));
		}
	}
	// ���㷨��
	else if (cmd[0] == "vn" && cmd.size() == 4)
	{
		if (nowOperatingMesh)
		{
			nowOperatingMesh->vertexNormal.emplace_back(
				Vector3(stringToNum<float>(cmd[1]), stringToNum<float>(cmd[2]), stringToNum<float>(cmd[3])));
		}
	}
	// ������������
	else if (cmd[0] == "vt" && cmd.size() == 3)
	{
		if (nowOperatingMesh)
		{
			nowOperatingMesh->vertexTexture.emplace_back(stringToNum<float>(cmd[1]), stringToNum<float>(cmd[2]));
		}
	}
	// ʹ�ò���
	else if (cmd[0] == "usemtl" && cmd.size() == 2)
	{
		nowUsingMaterialName = cmd[1];
	}
	// ����������
	else if (cmd[0] == "f" && cmd.size() == 4)
	{
		if (nowOperatingMesh)
		{
			FaceAsset face;
			face.connectedMaterialName = nowUsingMaterialName;
			for (int i = 1; i <= 3; i++)
			{
				std::vector<std::string> faceItem;
				Stringsplit(cmd[i], '/', faceItem);
				if (faceItem.size() == 3)
				{
					face.vertexIndex[i - 1] = stringToNum<int>(faceItem[0]) - 1;
					face.vertexTextureIndex[i - 1] = stringToNum<int>(faceItem[1]) - 1;
					face.vertexNormalIndex[i - 1] = stringToNum<int>(faceItem[2]) - 1;
				}
			}
			nowOperatingMesh->faces.emplace_back(face);
		}
	}
	// �����棬��ֳ�����������
	else if (cmd[0] == "f" && cmd.size() == 5)
	{
		if (nowOperatingMesh)
		{
			FaceAsset face1;
			FaceAsset face2;
			face1.connectedMaterialName = nowUsingMaterialName;
			face2.connectedMaterialName = nowUsingMaterialName;
			for (int i = 1; i <= 4; i++)
			{
				std::vector<std::string> faceItem;
				Stringsplit(cmd[i], '/', faceItem);
				if (faceItem.size() == 3)
				{
					if (i < 4)
					{
						face1.vertexIndex[i - 1] = stringToNum<int>(faceItem[0]) - 1;
						face1.vertexTextureIndex[i - 1] = stringToNum<int>(faceItem[1]) - 1;
						face1.vertexNormalIndex[i - 1] = stringToNum<int>(faceItem[2]) - 1;
					}

					if (i != 2)
					{
						if (i == 1)
						{
							face2.vertexIndex[0] = stringToNum<int>(faceItem[0]) - 1;
							face2.vertexTextureIndex[0] = stringToNum<int>(faceItem[1]) - 1;
							face2.vertexNormalIndex[0] = stringToNum<int>(faceItem[2]) - 1;
						}
						else
						{
							face2.vertexIndex[i - 2] = stringToNum<int>(faceItem[0]) - 1;
							face2.vertexTextureIndex[i - 2] = stringToNum<int>(faceItem[1]) - 1;
							face2.vertexNormalIndex[i - 2] = stringToNum<int>(faceItem[2]) - 1;
						}
					}
				}
			}

			nowOperatingMesh->faces.emplace_back(face1);
			nowOperatingMesh->faces.emplace_back(face2);
		}
	}
	// �رչ⻬��
	else if (cmd[0] == "s" && cmd[1] == "off")
	{
		;
	}
}

void EngineTools::ObjLoader::ReadMaterial(const std::string& path, EngineTools::ModelAsset* model)
{
	// �򿪲����ļ�
	std::ifstream file(path, std::ios::in);
	if (!file.is_open() || !file.good())
	{
		return;
	}

	char line[256]{};

	std::vector<std::string> spLine;

	// ���ж�ȡ������
	while (!file.eof())
	{
		file.getline(line, 256);
		if (strcmp("", line) == 0)
		{
			continue;
		}
		spLine.clear();
		Stringsplit(std::string(line), ' ', spLine);
		if (spLine[0] == "#")
		{
			continue;
		}

		MaterialOperation(model, spLine);
	}
}

void EngineTools::ObjLoader::MaterialOperation(ModelAsset* model, std::vector<std::string> cmd)
{
	// �½�����
	if (cmd[0] == "newmtl" && cmd.size() == 2)
	{
		MaterialAsset* newMtl = new MaterialAsset;
		newMtl->materialName = cmd[1];
		model->materials[cmd[1]] = newMtl;
		nowOperatingMaterial = newMtl;
	}
	else if (cmd[0] == "Ns" && cmd.size() == 2)
	{
		if (nowOperatingMaterial)
		{
			nowOperatingMaterial->Ns = stringToNum<float>(cmd[1]);
		}
	}
	else if (cmd[0] == "Ka" && cmd.size() == 4)
	{
		if (nowOperatingMaterial)
		{
			nowOperatingMaterial->Ka =
				Vector3(stringToNum<float>(cmd[1]), stringToNum<float>(cmd[2]), stringToNum<float>(cmd[3]));
		}
	}
	else if (cmd[0] == "Kd" && cmd.size() == 4)
	{
		if (nowOperatingMaterial)
		{
			nowOperatingMaterial->Kd =
				Vector3(stringToNum<float>(cmd[1]), stringToNum<float>(cmd[2]), stringToNum<float>(cmd[3]));
		}
	}
	else if (cmd[0] == "Ks" && cmd.size() == 4)
	{
		if (nowOperatingMaterial)
		{
			nowOperatingMaterial->Ks =
				Vector3(stringToNum<float>(cmd[1]), stringToNum<float>(cmd[2]), stringToNum<float>(cmd[3]));
		}
	}
	else if (cmd[0] == "Ke" && cmd.size() == 4)
	{
		if (nowOperatingMaterial)
		{
			nowOperatingMaterial->Ke =
				Vector3(stringToNum<float>(cmd[1]), stringToNum<float>(cmd[2]), stringToNum<float>(cmd[3]));
		}
	}
	else if (cmd[0] == "Ni" && cmd.size() == 2)
	{
		if (nowOperatingMaterial)
		{
			nowOperatingMaterial->Ni = stringToNum<float>(cmd[1]);
		}
	}
	else if (cmd[0] == "Ns" && cmd.size() == 2)
	{
		if (nowOperatingMaterial)
		{
			nowOperatingMaterial->Ns = stringToNum<float>(cmd[1]);
		}
	}
	else if (cmd[0] == "d" && cmd.size() == 2)
	{
		if (nowOperatingMaterial)
		{
			nowOperatingMaterial->d = stringToNum<float>(cmd[1]);
		}
	}
	else if (cmd[0] == "illum" && cmd.size() == 2)
	{
		if (nowOperatingMaterial)
		{
			nowOperatingMaterial->illum = stringToNum<int>(cmd[1]);
		}
	}
	else if (cmd[0] == "map_Ka" && cmd.size() == 2)
	{
		if (nowOperatingMaterial)
		{
			nowOperatingMaterial->map_Ka = cmd[1];

			auto resmgr = ResourceManager::GetInstance();
			if (resmgr)
			{
				if (!resmgr->FindImageAsset(cmd[1]))
				{
					resmgr->LoadImageAtPath(cmd[1], ImageType::PNG);
				}
			}
		}
	}
	else if (cmd[0] == "map_Kd" && cmd.size() == 2)
	{
		if (nowOperatingMaterial)
		{
			nowOperatingMaterial->map_Kd = cmd[1];

			auto resmgr = ResourceManager::GetInstance();
			if (resmgr)
			{
				if (!resmgr->FindImageAsset(cmd[1]))
				{
					resmgr->LoadImageAtPath(cmd[1], ImageType::PNG);
				}
			}
		}
	}
	else if (cmd[0] == "map_Ks" && cmd.size() == 2)
	{
		if (nowOperatingMaterial)
		{
			nowOperatingMaterial->map_Ks = cmd[1];

			auto resmgr = ResourceManager::GetInstance();
			if (resmgr)
			{
				if (!resmgr->FindImageAsset(cmd[1]))
				{
					resmgr->LoadImageAtPath(cmd[1], ImageType::PNG);
				}
			}
		}
	}
	else if (cmd[0] == "map_Ns" && cmd.size() == 2)
	{
		if (nowOperatingMaterial)
		{
			nowOperatingMaterial->map_Ns = cmd[1];

			auto resmgr = ResourceManager::GetInstance();
			if (resmgr)
			{
				if (!resmgr->FindImageAsset(cmd[1]))
				{
					resmgr->LoadImageAtPath(cmd[1], ImageType::PNG);
				}
			}
		}
	}
	else if (cmd[0] == "map_d" && cmd.size() == 2)
	{
		if (nowOperatingMaterial)
		{
			nowOperatingMaterial->map_d = cmd[1];

			auto resmgr = ResourceManager::GetInstance();
			if (resmgr)
			{
				if (!resmgr->FindImageAsset(cmd[1]))
				{
					resmgr->LoadImageAtPath(cmd[1], ImageType::PNG);
				}
			}
		}
	}
	else if (cmd[0] == "map_bump" && cmd.size() == 2)
	{
		if (nowOperatingMaterial)
		{
			nowOperatingMaterial->map_bump = cmd[1];

			auto resmgr = ResourceManager::GetInstance();
			if (resmgr)
			{
				if (!resmgr->FindImageAsset(cmd[1]))
				{
					resmgr->LoadImageAtPath(cmd[1], ImageType::PNG);
				}
			}
		}
	}
}