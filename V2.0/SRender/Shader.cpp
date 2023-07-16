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
    // 结果层
    SRender::Layer res(screenWidth, screenHeight, 0);
    auto resBufferPtr = res.GetLayerBufferPtr();
    
    for (auto& camera : glSet->cameras)
    {
        // 如果相机未启用，下一个
        if (!camera.second->isActive)
        {
            continue;
        }

        // 每个相机都有一个自己的深度缓冲
        SRender::MathTools::Matrix<double> zBuffer(screenHeight, screenWidth, -1e10);

        // 获取透视矩阵和View矩阵
        SRender::MathTools::Matrix<double> perspectiveMatrix;
        SRender::MathTools::Matrix<double> viewMatrix;

        // 对于全窗口相机，无需计算偏移
        if (camera.second->cameraType == EngineTools::CameraType::FULLWINDOW)
        {
            // 直接写入透视矩阵和LookAt矩阵
            perspectiveMatrix = GetPerspectiveMatrix(
                camera.second->fov, static_cast<double>(screenWidth) / screenHeight, camera.second->cameraNear, camera.second->cameraFar);
            viewMatrix = camera.second->GetLookAtMatrix();
        }
        // 计算相机窗口偏移
        // 警告：这一步有问题，请使用全窗口相机
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

        // 遍历模型，绘制
        for (auto& model : glSet->modelMap)
        {
            // 如果当前模型在相机的背面，直接略过
            if ((model.first->gameObject->transform->worldPosition - camera.second->gameObject->transform->worldPosition).Dot(camera.second->front) <= 0)
            {
                continue;
            }

            // 获取模型矩阵
            SRender::MathTools::Matrix<double> modelMatrix = model.first->gameObject->transform->GetWorldModelMatrix();

            // 遍历每个网格体的每个面
            for (auto& mesh : model.second->meshes)
            {
                for (auto& face : mesh.second->faces)
                {
                   

                    // 获取面的三个顶点的点矩阵
                    auto p1 = Vert(mesh.second->vertex[face.vertexIndex[0]], modelMatrix, viewMatrix, perspectiveMatrix);
                    auto p2 = Vert(mesh.second->vertex[face.vertexIndex[1]], modelMatrix, viewMatrix, perspectiveMatrix);
                    auto p3 = Vert(mesh.second->vertex[face.vertexIndex[2]], modelMatrix, viewMatrix, perspectiveMatrix);

                    // 转换到NDC[0,1]
                    EngineTools::Vector3 realP1(1.0 * p1[0][0] / -p1[3][0] / 2 + 0.5, 1.0 * p1[1][0] / -p1[3][0] / 2 + 0.5, 1.0 * p1[2][0] / p1[3][0]);
                    EngineTools::Vector3 realP2(1.0 * p2[0][0] / -p2[3][0] / 2 + 0.5, 1.0 * p2[1][0] / -p2[3][0] / 2 + 0.5, 1.0 * p2[2][0] / p2[3][0]);
                    EngineTools::Vector3 realP3(1.0 * p3[0][0] / -p3[3][0] / 2 + 0.5, 1.0 * p3[1][0] / -p3[3][0] / 2 + 0.5, 1.0 * p3[2][0] / p3[3][0]);
                    
                    // 转换到屏幕坐标 x:0 - screenWidth, y:0 - screenHeight
                    realP1.x *= screenWidth;
                    realP2.x *= screenWidth;
                    realP3.x *= screenWidth;
                    realP1.y *= screenHeight;
                    realP2.y *= screenHeight;
                    realP3.y *= screenHeight;

                    // 如果三个点不是逆时针顺序，跳过
                    if (!IsAntiClockWise(VEC3TOPOINT2D(realP1), VEC3TOPOINT2D(realP2),VEC3TOPOINT2D(realP3)))
                    {
                        continue;
                    }

                    // 开始绘制三角形
                    // 获取三角形边界矩形
                    float xl = std::min(realP1.x, std::min(realP2.x, realP3.x));
                    float xr = std::max(realP1.x, std::max(realP2.x, realP3.x));
                    float yd = std::min(realP1.y, std::min(realP2.y, realP3.y));
                    float yu = std::max(realP1.y, std::max(realP2.y, realP3.y));
                    double z;

                    // 对于三角形边界矩形内的每一个点
                    for (int i = std::max((int)xl, 0); i <= std::min((int)xr, screenHeight-1); i++)
                    {
                        // 如果不在屏幕里，跳过
                        if (i < 0 || i >= screenWidth)
                        {
                            continue;
                        }

                        for (int j = std::max((int)yd, 0); j <= std::min((int)yu, screenWidth - 1); j++)
                        {
                            // 如果不在屏幕里，跳过
                            if (j < 0 || j >= screenHeight)
                            {
                                continue;
                            }

                            // 算一下重心分量
                            auto interpolate = BaryCentric(
                                { realP1.x, realP1.y },
                                { realP2.x, realP2.y },
                                { realP3.x, realP3.y },
                                { i,j });

                            // 如果该点在三角形里
                            if(InTriangle(interpolate))
                            {
                                // 计算该点深度值
                                z = DoInterpolate(interpolate, realP1.z, realP2.z, realP3.z);
                                if (z <= zBuffer[j][i])
                                {
                                    continue;
                                }
                                zBuffer[j][i] = z;
                                // Frag Shader
                                {
                                    // 用模型矩阵计算三个点的世界位置
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

                                    // 用上面的3个点计算当前片元的世界坐标
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

                                    // 计算世界坐标中的法线
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

                                    // 法线标准化
                                    normalIn = normalIn.Normalize();

                                    // 用片段着色器计算颜色
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
