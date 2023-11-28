// main.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "SRender.h"
#include "RenderSystem.h"
#include "EngineTools.h"

#define MAX_LOADSTRING 1000
#define IDT_WINDOWREPAINT 1

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
bool timerIsSet = false;                        // 是否已设置更新计时器
EngineTools::Scene mainScene;                   // 游戏主场景

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                InitScene();

// Timer的回调函数
VOID CALLBACK TimerProcWindowRepaint(
    _In_  HWND hwnd,
    _In_  UINT uMsg,
    _In_  UINT_PTR idEvent,
    _In_  DWORD dwTime
)
{
    RECT rect;

    // 设置窗口立即重绘
    GetClientRect(hwnd, &rect);
    InvalidateRect(hwnd, &rect, FALSE);
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
    InitScene();

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SRENDER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SRENDER));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SRENDER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    // wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.hbrBackground  = CreateSolidBrush(RGB(0, 0, 0));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SRENDER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 将实例句柄存储在全局变量中
    
    // HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
    //     CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
           CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);
    
    unsigned int nWidth = GetSystemMetrics(SM_CXSCREEN);  //屏幕宽度    
    unsigned int nHeight = GetSystemMetrics(SM_CYSCREEN);
    SRender::RenderSystem::GetInstance()->Resize(nWidth, nHeight);
    
    if (!hWnd)
    {
        return FALSE;
    }
    
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    
    return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EngineTools::EngineTime::GetInstance()->UpdateDeltaTime(); // 更新帧间隔
            mainScene.CallUpdate(); // 调用Update更新世界
            // 绘制世界
            SRender::RenderSystem::GetInstance()->Draw(hdc, mainScene.GetSharedComponent<EngineTools::GlobalSettings>());

            if (!timerIsSet)
            {
                timerIsSet = true;
                SetTimer(hWnd, IDT_WINDOWREPAINT, 15, TimerProcWindowRepaint);
            }

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_SIZE:
    {
        UINT width = LOWORD(lParam);
        UINT height = HIWORD(lParam);
        SRender::RenderSystem::GetInstance()->Resize(width, height);
        RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
    }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_ERASEBKGND:
        // Return non-zero to indicate that we have erased the background
        return 1;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


// 初始化场景
void InitScene()
{
    // 添加全局信息保存点
    mainScene.AddSharedComponent(new EngineTools::GlobalSettings());

    // 添加画布
    mainScene.AddChild("root/MainCanvas");
    auto canvasObj = mainScene.GetChild("root/MainCanvas");
    canvasObj->AddComponent(new EngineTools::Canvas());
    auto canvas = canvasObj->GetComponent<EngineTools::Canvas>();
    canvas->RegisterCanvasToRenderSystem();

    // 在画布下添加两个Image
    mainScene.AddChild("root/MainCanvas/ImageObject");
    auto imgObj = mainScene.GetChild("root/MainCanvas/ImageObject");
    imgObj->AddComponent(new EngineTools::Image());

    // 设置Image信息
    auto imgComponent = imgObj->GetComponent<EngineTools::Image>();
    imgComponent->posX = 500;
    imgComponent->posY = 200;
    imgComponent->width = 256;
    imgComponent->height = 256;
    imgComponent->BindCanvas(canvas);
    std::string path = "test.bmp";
    imgComponent->BindImage(path, EngineTools::ImageType::BMP);

    mainScene.AddChild("root/MainCanvas/ImageObject2");
    imgObj = mainScene.GetChild("root/MainCanvas/ImageObject2");
    imgObj->AddComponent(new EngineTools::Image());
    imgComponent = imgObj->GetComponent<EngineTools::Image>();
    imgComponent->posX = 0;
    imgComponent->posY = 128;
    imgComponent->width = 256;
    imgComponent->height = 256;
    imgComponent->BindCanvas(canvas);
    path = "test3.png";
    imgComponent->BindImage(path, EngineTools::ImageType::PNG);

    // 添加一个立方体
    mainScene.AddChild("root/Objects");
    mainScene.AddChild("root/Objects/cube1");
    EngineTools::GameObject* cube = mainScene.GetChild("root/Objects/cube1");
    cube->AddComponent(new EngineTools::Transform);
    cube->GetComponent<EngineTools::Transform>()->Rotation({ 0,45,0 });
    cube->GetComponent<EngineTools::Transform>()->scale = { 1,1,1 };

    // 设置立方体渲染信息
    auto renderInfo = new EngineTools::RenderInfo;
    path = "stdcube.obj";
    renderInfo->BindModel(path);
    cube->AddComponent(renderInfo);

    // 添加相机
    mainScene.AddChild("root/Camera");
    EngineTools::GameObject* camera = mainScene.GetChild("root/Camera");
    camera->AddComponent(new EngineTools::Camera);
    auto cmpCamera = camera->GetComponent<EngineTools::Camera>();
    cmpCamera->cameraType = EngineTools::CameraType::AREA;
    cmpCamera->cameraRect = { 0.5,0.5,1.0,1.0 };
    camera->AddComponent(new EngineTools::Transform);
    camera->GetComponent<EngineTools::Transform>()->Translate({ 0,0,-20 });
    camera->GetComponent<EngineTools::Transform>()->Rotation({ 0,0,0 });

    // 添加灯光组，
    // 该灯光组包含：
    // 环境光 * 1，
    // 太阳光 * 1，
    // 点光源 * 3，
    // 聚光灯 * 1

    mainScene.AddChild("root/Lights");
    mainScene.AddChild("root/Lights/AmbientLight");
    auto ambientObj = mainScene.GetChild("root/Lights/AmbientLight");
    EngineTools::Light* ambiLight = new EngineTools::Light;
    ambiLight->lightType = EngineTools::LightType::Ambient;
    ambiLight->lightInfo.ambient = EngineTools::Vector3(0.2f, 0.2f, 0.2f);
    ambientObj->AddComponent(ambiLight);

    mainScene.AddChild("root/Lights/SunLight");
    ambientObj = mainScene.GetChild("root/Lights/SunLight");
    EngineTools::Light* pSunLight = new EngineTools::Light;
    pSunLight->lightType = EngineTools::LightType::SunLight;
    pSunLight->lightInfo.sunLight = { {-0.3f, -0.4f, 0.5f},{0.5f, 0.5f, 0.5f},{0.5f, 0.5f, 0.5f} };
    ambientObj->AddComponent(pSunLight);

    mainScene.AddChild("root/Lights/PointLight1");
    ambientObj = mainScene.GetChild("root/Lights/PointLight1");
    EngineTools::Light* pointL1 = new EngineTools::Light;
    pointL1->lightType = EngineTools::LightType::PointLight;
    pointL1->lightInfo.pointLight = { {-5.0f, 0.0f, 0.0f},{0.8f, 0.8f, 0.8f},{1.0f, 1.0f, 1.0f},1.0f,0.007f,0.0002f };
    ambientObj->AddComponent(pointL1);

    mainScene.AddChild("root/Lights/PointLight2");
    ambientObj = mainScene.GetChild("root/Lights/PointLight2");
    EngineTools::Light* pointL2 = new EngineTools::Light;
    pointL2->lightType = EngineTools::LightType::PointLight;
    pointL2->lightInfo.pointLight = { {5.0f, 5.0f, 5.0f},{0.8f, 0.8f, 0.8f},{1.0f, 1.0f, 1.0f},1.0f,0.007f,0.0002f };
    ambientObj->AddComponent(pointL2);

    mainScene.AddChild("root/Lights/PointLight3");
    ambientObj = mainScene.GetChild("root/Lights/PointLight3");
    EngineTools::Light* pointL3 = new EngineTools::Light;
    pointL3->lightType = EngineTools::LightType::PointLight;
    pointL3->lightInfo.pointLight = { {5.0f, -5.0f, 0.0f},{0.8f, 0.8f, 0.8f},{1.0f, 1.0f, 1.0f},1.0f,0.007f,0.0002f };
    ambientObj->AddComponent(pointL3);

    mainScene.AddChild("root/Lights/SpotLight");
    ambientObj = mainScene.GetChild("root/Lights/SpotLight");
    EngineTools::Light* spL = new EngineTools::Light;
    spL->lightType = EngineTools::LightType::SpotLight;
    auto pCmpCamera = camera->GetComponent<EngineTools::Camera>();
    auto pCmpTrans = camera->GetComponent<EngineTools::Transform>();
    spL->lightInfo.spotLight = {
        {pCmpTrans->worldPosition.x, pCmpTrans->worldPosition.y, pCmpTrans->worldPosition.z},
        {pCmpCamera->front.x, pCmpCamera->front.y, pCmpCamera->front.z},
        {1.0f, 1.0f, 1.0f},{1.0f, 1.0f, 1.0f},
        1.0f, 0.03f, 0.0025f,
        cos(45.0f * 0.01745329251994329576923690768489f),
        cos(90.0f * 0.01745329251994329576923690768489f)
    };
    ambientObj->AddComponent(spL);

    // 前期准备完成，调用Start函数
    mainScene.CallStart();
}




//#include "framework.h"
//#include <windows.h>
//
//LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
//{
//    switch (msg)
//    {
//    case WM_DESTROY:
//        PostQuitMessage(0);
//        return 0;
//    }
//    return DefWindowProc(hwnd, msg, wparam, lparam);
//}
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
//{
//    const wchar_t CLASS_NAME[] = L"Sample Window Class";
//
//    WNDCLASS wc = { };
//
//    wc.lpfnWndProc = WindowProcedure;
//    wc.hInstance = hInstance;
//    wc.lpszClassName = CLASS_NAME;
//
//    RegisterClass(&wc);
//
//    HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"Sample Window", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 740, 480, NULL, NULL, hInstance, NULL);
//
//    if (hwnd == NULL)
//    {
//        return 0;
//    }
//
//    ShowWindow(hwnd, SW_SHOWDEFAULT);
//
//    // Get the device context of the window
//    HDC hdc = GetDC(hwnd);
//
//    // Create a bitmap
//    BITMAPINFO bmi = { 0 };
//    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
//    bmi.bmiHeader.biWidth = 640;
//    bmi.bmiHeader.biHeight = -480; // top-down
//    bmi.bmiHeader.biPlanes = 1;
//    bmi.bmiHeader.biBitCount = 32; // each pixel is 4 bytes (RGBA)
//    bmi.bmiHeader.biCompression = BI_RGB;
//
//    COLORREF* pPixels;
//    HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&pPixels, NULL, 0);
//
//    // Fill the bitmap with pixel data
//    for (int i = 0; i < 640 * 480; i++)
//    {
//        pPixels[i] = RGB(255, 0, 0); // fill with red color
//    }
//
//    // Draw the bitmap to the window
//    HDC hdcMem = CreateCompatibleDC(hdc);
//    SelectObject(hdcMem, hBitmap);
//    BitBlt(hdc, 0, 0, 640, 480, hdcMem, 0, 0, SRCCOPY);
//    DeleteDC(hdcMem);
//
//    // Release the device context
//    ReleaseDC(hwnd, hdc);
//
//    MSG msg = { };
//    while (GetMessage(&msg, NULL, 0, 0))
//    {
//        TranslateMessage(&msg);
//        DispatchMessage(&msg);
//    }
//
//    return 0;
//}