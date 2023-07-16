#include "Shader.h"
#include "EngineTools/GameObject.h"
#include "EngineTools/Components/GlobalSettings.h"
#include "EngineTools/Components/RenderInfo.h"
#include "EngineTools/Components/Transform.h"
#include <cmath>
#include <functional>
#include <memory>

std::mutex zLocker;
std::mutex resLocker;

SRender::MathTools::Matrix<double> SRender::Shader::GetPerspectiveMatrix(double fov, double aspect, double near, double far)
{
    SRender::MathTools::Matrix<double> res(4, 4, 0);
    double tan2fov = tan(fov * 0.01745329251994329576923690768489 / 2);
    res[0][0] = 1.0f / (aspect * tan2fov);
    res[1][1] = 1.0f / tan2fov;
    res[2][2] = -(far + near) / (far - near);
    res[2][3] = -(2 * far * near) / (far - near);
    res[3][2] = -1;

    return res;
}

bool SRender::Shader::InTriangle(const Point2D& targetPoint, const Point2D& p1, const Point2D& p2, const Point2D& p3)
{
    Point2D v1(p2.first - p1.first, p2.second - p1.second);
    Point2D v2(p3.first - p2.first, p3.second - p2.second);
    Point2D v3(p1.first - p3.first, p1.second - p3.second);

    Point2D t1(targetPoint.first - p1.first, targetPoint.second - p1.second);
    Point2D t2(targetPoint.first - p2.first, targetPoint.second - p2.second);
    Point2D t3(targetPoint.first - p3.first, targetPoint.second - p3.second);
    int r1 = POINT2D_CROSS(v1, t1);
    int r2 = POINT2D_CROSS(v2, t2);
    int r3 = POINT2D_CROSS(v3, t3);

    if (!r1 || !r2 || !r3) return true;
    if (r1 > 0 && r2 > 0 && r3 > 0) return true;
    if (r1 < 0 && r2 < 0 && r3 < 0) return true;
    return false;
}

bool SRender::Shader::InTriangle(const EngineTools::Vector3& interpolate)
{
    return interpolate.x >= 0.0 && interpolate.x <= 1.0 &&
        interpolate.y >= 0.0 && interpolate.y <= 1.0 &&
        interpolate.z >= 0.0 && interpolate.z <= 1.0;
}

bool SRender::Shader::IsAntiClockWise(const Point2D& p1, const Point2D& p2, const Point2D& p3)
{
    return ((p2.first - p1.first) * (p3.second - p1.second) - (p2.second - p1.second) * (p3.first - p1.first)) > 0;
}

EngineTools::Vector3 SRender::Shader::BaryCentric(const Point2D& p1, const Point2D& p2, const Point2D& p3, const Point2D& targetPoint)
{
    Point2D v0 = { p2.first - p1.first, p2.second - p1.second };
    Point2D v1 = { p3.first - p1.first, p3.second - p1.second };
    Point2D v2 = { targetPoint.first - p1.first, targetPoint.second - p1.second };

    int d00 = POINT2D_DOT(v0, v0);
    int d01 = POINT2D_DOT(v0, v1);
    int d11 = POINT2D_DOT(v1, v1);
    int d20 = POINT2D_DOT(v2, v0);
    int d21 = POINT2D_DOT(v2, v1);
    int denom = d00 * d11 - d01 * d01;
    float v, w;
    if (denom != 0)
    {
        v = (1.0 * d11 * d20 - d01 * d21) / denom;
        w = (1.0 * d00 * d21 - d01 * d20) / denom;
    }
    else
    {
        v = w = 0.0;
    }

    return { 1.0 - v - w, v, w };
}

double SRender::Shader::DoInterpolate(const EngineTools::Vector3& interpolate, double z1, double z2, double z3)
{
    return interpolate.x * z1 + interpolate.y * z2 + interpolate.z * z3;
}

SRender::MathTools::Matrix<double> SRender::Shader::CalcScreenPosWithBaryCentric(
    const EngineTools::Vector3& interpolate, 
    const SRender::MathTools::Matrix<double>& p1, 
    const SRender::MathTools::Matrix<double>& p2, 
    const SRender::MathTools::Matrix<double>& p3)
{
    SRender::MathTools::Matrix<double> res(4, 1, 0);
    res.SetValue(0, 0, 
        interpolate.x * p1.GetValue(0, 0) + interpolate.y * p2.GetValue(0, 0) + interpolate.z * p3.GetValue(0, 0));
    res.SetValue(1, 0,
        interpolate.x * p1.GetValue(1, 0) + interpolate.y * p2.GetValue(1, 0) + interpolate.z * p3.GetValue(1, 0));
    res.SetValue(2, 0,
        interpolate.x * p1.GetValue(2, 0) + interpolate.y * p2.GetValue(2, 0) + interpolate.z * p3.GetValue(2, 0));
    res.SetValue(3, 0,
        interpolate.x * p1.GetValue(3, 0) + interpolate.y * p2.GetValue(3, 0) + interpolate.z * p3.GetValue(3, 0));
    return res;

}

SRender::MathTools::Matrix<double> SRender::Shader::Vert(const EngineTools::Vector3& point,
    const SRender::MathTools::Matrix<double>& modelMat, 
    const SRender::MathTools::Matrix<double>& viewMat,
    const SRender::MathTools::Matrix<double>& perspectiveMat)
{
    SRender::MathTools::Matrix<double> position(4, 1, 0);
    position[0][0] = point.x;
    position[1][0] = point.y;
    position[2][0] = point.z;
    position[3][0] = 1;
    return perspectiveMat * viewMat * modelMat * position;
}

RGBAColor SRender::Shader::Frag(
    const EngineTools::Vector3& interpolate, 
    const EngineTools::Vector3& viewPos, 
    const EngineTools::Vector3& fragPos,
    const EngineTools::Vector3& normalIn,
    EngineTools::GlobalSettings* glSet, 
    EngineTools::ModelAsset* nowModel, 
    EngineTools::MeshAsset* nowMesh,
    EngineTools::FaceAsset* nowFace)
{
    return RGBAColor(0, 255, 255, 255);
}

SRender::Layer SRender::Shader::Draw(int screenWidth, int screenHeight, EngineTools::GlobalSettings* glSet)
{
    // �����
    SRender::Layer res(screenWidth, screenHeight, 0);
    auto resBufferPtr = res.GetLayerBufferPtr();
    
    for (auto& camera : glSet->cameras)
    {
        // ������δ���ã���һ��
        if (!camera.second->isActive)
        {
            continue;
        }

        // ÿ���������һ���Լ�����Ȼ���
        SRender::MathTools::Matrix<double> zBuffer(screenHeight, screenWidth, -1e10);

        // ��ȡ͸�Ӿ����View����
        SRender::MathTools::Matrix<double> perspectiveMatrix;
        SRender::MathTools::Matrix<double> viewMatrix;

        // ����ȫ����������������ƫ��
        if (camera.second->cameraType == EngineTools::CameraType::FULLWINDOW)
        {
            // ֱ��д��͸�Ӿ����LookAt����
            perspectiveMatrix = GetPerspectiveMatrix(
                camera.second->fov, static_cast<double>(screenWidth) / screenHeight, camera.second->cameraNear, camera.second->cameraFar);
            viewMatrix = camera.second->GetLookAtMatrix();
        }
        // �����������ƫ��
        // ���棺��һ�������⣬��ʹ��ȫ�������
        else
        {
            int cWidth = camera.second->cameraRect.width * screenWidth;
            int cHeight = camera.second->cameraRect.height * screenHeight;
            int dispX = (camera.second->cameraRect.center_x - 0.5);
            int dispY = (camera.second->cameraRect.center_y - 0.5);
            perspectiveMatrix = GetPerspectiveMatrix(
                camera.second->fov, static_cast<double>(cWidth) / cHeight, camera.second->cameraNear, camera.second->cameraFar);
            perspectiveMatrix[0][3] = dispX;
            perspectiveMatrix[1][3] = dispY;
            viewMatrix = camera.second->GetLookAtMatrix();
        }

        // ����ģ�ͣ�����
        for (auto& model : glSet->modelMap)
        {
            // �����ǰģ��������ı��棬ֱ���Թ�
            if ((model.first->gameObject->transform->worldPosition - camera.second->gameObject->transform->worldPosition).Dot(camera.second->front) <= 0)
            {
                continue;
            }

            // ��ȡģ�;���
            SRender::MathTools::Matrix<double> modelMatrix = model.first->gameObject->transform->GetWorldModelMatrix();

            // ����ÿ���������ÿ����
            for (auto& mesh : model.second->meshes)
            {
                for (auto& face : mesh.second->faces)
                {
                   

                    // ��ȡ�����������ĵ����
                    auto p1 = Vert(mesh.second->vertex[face.vertexIndex[0]], modelMatrix, viewMatrix, perspectiveMatrix);
                    auto p2 = Vert(mesh.second->vertex[face.vertexIndex[1]], modelMatrix, viewMatrix, perspectiveMatrix);
                    auto p3 = Vert(mesh.second->vertex[face.vertexIndex[2]], modelMatrix, viewMatrix, perspectiveMatrix);

                    // ת����NDC[0,1]
                    EngineTools::Vector3 realP1(1.0 * p1[0][0] / -p1[3][0] / 2 + 0.5, 1.0 * p1[1][0] / -p1[3][0] / 2 + 0.5, 1.0 * p1[2][0] / p1[3][0]);
                    EngineTools::Vector3 realP2(1.0 * p2[0][0] / -p2[3][0] / 2 + 0.5, 1.0 * p2[1][0] / -p2[3][0] / 2 + 0.5, 1.0 * p2[2][0] / p2[3][0]);
                    EngineTools::Vector3 realP3(1.0 * p3[0][0] / -p3[3][0] / 2 + 0.5, 1.0 * p3[1][0] / -p3[3][0] / 2 + 0.5, 1.0 * p3[2][0] / p3[3][0]);
                    
                    // ת������Ļ���� x:0 - screenWidth, y:0 - screenHeight
                    realP1.x *= screenWidth;
                    realP2.x *= screenWidth;
                    realP3.x *= screenWidth;
                    realP1.y *= screenHeight;
                    realP2.y *= screenHeight;
                    realP3.y *= screenHeight;

                    // ��������㲻����ʱ��˳������
                    if (!IsAntiClockWise(VEC3TOPOINT2D(realP1), VEC3TOPOINT2D(realP2),VEC3TOPOINT2D(realP3)))
                    {
                        continue;
                    }

                    // ��ʼ����������
                    // ��ȡ�����α߽����
                    float xl = std::min(realP1.x, std::min(realP2.x, realP3.x));
                    float xr = std::max(realP1.x, std::max(realP2.x, realP3.x));
                    float yd = std::min(realP1.y, std::min(realP2.y, realP3.y));
                    float yu = std::max(realP1.y, std::max(realP2.y, realP3.y));
                    double z;

                    // ���������α߽�����ڵ�ÿһ����
                    for (int i = std::max((int)xl, 0); i <= std::min((int)xr, screenHeight-1); i++)
                    {
                        // ���������Ļ�����
                        if (i < 0 || i >= screenWidth)
                        {
                            continue;
                        }

                        for (int j = std::max((int)yd, 0); j <= std::min((int)yu, screenWidth - 1); j++)
                        {
                            // ���������Ļ�����
                            if (j < 0 || j >= screenHeight)
                            {
                                continue;
                            }

                            // ��һ�����ķ���
                            auto interpolate = BaryCentric(
                                { realP1.x, realP1.y },
                                { realP2.x, realP2.y },
                                { realP3.x, realP3.y },
                                { i,j });

                            // ����õ�����������
                            if(InTriangle(interpolate))
                            {
                                // ����õ����ֵ
                                z = DoInterpolate(interpolate, realP1.z, realP2.z, realP3.z);
                                if (z <= zBuffer[j][i])
                                {
                                    continue;
                                }
                                zBuffer[j][i] = z;
                                // Frag Shader
                                {
                                    // ��ģ�;�����������������λ��
                                    SRender::MathTools::Matrix<double> localP1(4, 1, 0);
                                    localP1[0][0] = mesh.second->vertex[face.vertexIndex[0]].x;
                                    localP1[1][0] = mesh.second->vertex[face.vertexIndex[0]].y;
                                    localP1[2][0] = mesh.second->vertex[face.vertexIndex[0]].z;
                                    localP1[3][0] = 1;

                                    auto worldP1 = modelMatrix * localP1;

                                    SRender::MathTools::Matrix<double> localP2(4, 1, 0);
                                    localP2[0][0] = mesh.second->vertex[face.vertexIndex[1]].x;
                                    localP2[1][0] = mesh.second->vertex[face.vertexIndex[1]].y;
                                    localP2[2][0] = mesh.second->vertex[face.vertexIndex[1]].z;
                                    localP2[3][0] = 1;

                                    auto worldP2 = modelMatrix * localP2;

                                    SRender::MathTools::Matrix<double> localP3(4, 1, 0);
                                    localP3[0][0] = mesh.second->vertex[face.vertexIndex[2]].x;
                                    localP3[1][0] = mesh.second->vertex[face.vertexIndex[2]].y;
                                    localP3[2][0] = mesh.second->vertex[face.vertexIndex[2]].z;
                                    localP3[3][0] = 1;

                                    auto worldP3 = modelMatrix * localP3;

                                    // �������3������㵱ǰƬԪ����������
                                    auto fragPosIn = EngineTools::Vector3(
                                        worldP1[0][0] * interpolate.x +
                                        worldP2[0][0] * interpolate.x +
                                        worldP3[0][0] * interpolate.x,

                                        worldP1[1][0] * interpolate.y +
                                        worldP2[1][0] * interpolate.y +
                                        worldP3[1][0] * interpolate.y,

                                        worldP1[2][0] * interpolate.z +
                                        worldP2[2][0] * interpolate.z +
                                        worldP3[2][0] * interpolate.z
                                    );

                                    // �������������еķ���
									auto normal = mesh.second->vertexNormal[face.vertexNormalIndex[0]] * interpolate.x +
										mesh.second->vertexNormal[face.vertexNormalIndex[1]] * interpolate.y +
										mesh.second->vertexNormal[face.vertexNormalIndex[2]] * interpolate.z;
                                    SRender::MathTools::Matrix<double> normalMat(4, 1, 0);
                                    normalMat[0][0] = normal.x;
                                    normalMat[1][0] = normal.y;
                                    normalMat[2][0] = normal.z;
                                    normalMat[3][0] = 1;

                                    auto realWorldNormalMat = modelMatrix.Inverse().Transfer() * normalMat;
                                    EngineTools::Vector3 normalIn(realWorldNormalMat[0][0],
                                        realWorldNormalMat[1][0],
                                        realWorldNormalMat[2][0]);

                                    // ���߱�׼��
                                    normalIn = normalIn.Normalize();

                                    // ��Ƭ����ɫ��������ɫ
                                    resBufferPtr[j * screenWidth + i] = Frag(interpolate,
                                        camera.second->gameObject->transform->worldPosition,
                                        fragPosIn,
                                        normalIn,
                                        glSet,
                                        model.second,
                                        mesh.second,
                                        &face);
                                }
                            }
                        }
                    }
                }

            }
        }
    }
    
    return res;
}
