# SGENGINE - 一个简单的3D引擎

## 在开始之前

### 致谢

感谢CSDN博主Jaihk662的文章[https://blog.csdn.net/jaihk662/category_9903113_2.html](https://blog.csdn.net/jaihk662/category_9903113_2.html) ，教会了我如何配置环境以及一些关于OpenGL的知识。引擎源码中也有一些文件是直接引用的其文章中的代码，如Camera.h 、Shader.h、Model.h、Mesh.h，以及Render模块默认假定的着色器代码也是来自于这些文章：

ObjVShader.vert

```glsl
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture;
out vec2 texIn;
out vec3 normalIn;
out vec3 fragPosIn;
uniform mat4 model;             //模型矩阵
uniform mat4 view;              //观察矩阵
uniform mat4 projection;        //投影矩阵
void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    texIn = texture;
    fragPosIn = vec3(model * vec4(position, 1.0f));
    normalIn = mat3(transpose(inverse(model))) * normal;
}

```

ObjFShader.frag

```glsl
#version 330 core
struct Material
{
    sampler2D texture_diffuse1;      //贴图
    sampler2D texture_specular1;     //镜面贴图
    sampler2D emission;     //放射贴图
    float shininess;        //反光度
};
struct SunLight             //平行光
{
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight           //点光源
{
    vec3 position;
    vec3 diffuse;
    vec3 specular;
    float k0, k1, k2;
};
struct SpotLight            //聚光灯
{
    vec3 position;
    vec3 direction;
    vec3 diffuse;
    vec3 specular;
    float k0, k1, k2;
    float cutOff, outCutOff;
};
uniform vec3 ambient;
uniform Material material;
uniform SunLight sunLight;
uniform PointLight pointLights[3];
uniform SpotLight spotLight;
vec3 CalcSunLight(SunLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
out vec4 color;
uniform vec3 viewPos;
in vec2 texIn;
in vec3 fragPosIn;
in vec3 normalIn;
void main()
{
    //环境光
    vec3 ambient = ambient * vec3(texture(material.texture_diffuse1, texIn));
    vec3 viewDir = normalize(viewPos - fragPosIn);
    vec3 normal = normalize(normalIn);
    vec3 result = CalcSunLight(sunLight, normal, viewDir);
    for (int i = 0; i <= 2; i++)
        result = result + CalcPointLight(pointLights[i], normal, fragPosIn, viewDir);
    result = result + CalcSpotLight(spotLight, normal, fragPosIn, viewDir);
    color = vec4(result, 1.0f);
}
 
vec3 CalcSunLight(SunLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.texture_diffuse1, texIn)));
 
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.texture_specular1, texIn)));
 
    return diffuse + specular;
}
 
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.texture_diffuse1, texIn)));
 
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.texture_specular1, texIn)));
 
    float dis = length(light.position - fragPos);
    float attenuation = 1.0f / (light.k0 + light.k1 * dis + light.k2 * (dis * dis));
 
    diffuse *= attenuation;
    specular *= attenuation;
    return diffuse + specular;
}
 
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float lightSoft = clamp((theta - light.outCutOff) / (light.cutOff - light.outCutOff), 0.0f, 1.0f);
 
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(material.texture_diffuse1, texIn)));
 
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.texture_specular1, texIn)));
 
    float dis = length(light.position - fragPos);
    float attenuation = 1.0f / (light.k0 + light.k1 * dis + light.k2 * (dis * dis));
 
    diffuse *= attenuation * lightSoft;
    specular *= attenuation * lightSoft;
    return diffuse + specular;
}
```



### 引擎简介

> 如果你有Unity、cocos、Unreal或其他引擎基础，那么后面的内容对你来说将很容易理解。

SGEngine是一款简单的3D引擎，由C++语言编写。由于采用了类似Unity的“场景-物体-组件”结构，您可以很轻松的对场景内的物体或组件进行查找。引擎默认可视化窗口使用OpenGL进行渲染，当然您也可以使用其他的图形库（如DirectX）对Render模块进行重写，或者不使用可视化窗口。

> PS：这个引擎只是我实验作业中的一个副产物，因此很多模块（如GUI、音乐）都还没有编写，请等待后续更新或者提交您编写的分支代码。感谢大家的支持。



## 开始使用

### 环境配置

1. 首先下载引擎源码，并将其放到一个空项目里。

2. 如果您需要使用OpenGL进行渲染，请按照[https://blog.csdn.net/jaihk662/category_9903113_2.html](https://blog.csdn.net/jaihk662/category_9903113_2.html)配置您的OpenGL环境，确保您正确配置了glew、glfw、glm、Assimp等依赖项的属性，否则可能导致编译失败或出现其他莫名其妙的BUG（如黑屏、窗口初始化失败等）。

3. 复制下方代码到一个空的cpp文件里，并且确保项目中只有一个main函数：

   如果只需要控制台，测试代码如下：

   ```c++
   #include "EngineTools.h"
   
   using namespace EngineTools;
   
   int main()
   {
   	Scene scene;
   	scene.AddSharedComponent(new GlobalSettings);
   
   	scene.AddChild("root/child1");
   	auto object = scene.GetChild("root/child1");
   	object->AddComponent(new Transform);
   
   	DebugHelper::Log(&scene);
   	DebugHelper::Log(&scene, "EngineLog_ToStringTest.txt");
   
   	return 0;
   }
   ```

   这段代码创建了一个简单的场景，并为场景的根物体下添加了一个子物体，并为子物体添加了Transform组件，然后把当前场景的信息输出出来。你看到的输出应该类似下面的输出：

   > -Scene
   >  -GlobalSettings:
   >  @EngineVersion: 1.0
   >  -root
   >   -Component:Transform
   >   @localPosition: Vector3(0.000000 0.000000 0.000000)
   >   @worldPosition: Vector3(0.000000 0.000000 0.000000)
   >   @rotation: Vector3(0.000000 0.000000 0.000000)
   >   @scale: Vector3(1.000000 1.000000 1.000000)
   >   -child1
   >    -Component:Transform
   >    @localPosition: Vector3(0.000000 0.000000 0.000000)
   >    @worldPosition: Vector3(0.000000 0.000000 0.000000)
   >    @rotation: Vector3(0.000000 0.000000 0.000000)
   >    @scale: Vector3(1.000000 1.000000 1.000000)

   并且你会发现，你的项目里多出来一个叫做EngineLog_ToStringTest.txt的文件，其内容应当和控制台输出相同。



​		如果使用OpenGL进行渲染，首先要先初始化OpenGL，然后才能进行其他操作，样例代码如下：

```c++
#include <Windows.h>
#include "EngineTools.h"

using namespace EngineTools;

bool keys[1024];
Camera camera({0, 0, 6.0f});
GLfloat lastX, lastY;
bool firstMouse = true;
bool mouseLeftButtonDown = false;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void cameraMove();
void InitLight(Light* light, Camera* camera);
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
static int fcount = 0;
int width, height;

int main()
{
    // 初始化OpenGL
    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "SGEngine", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glewExperimental = GL_TRUE;
	glewInit();

	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    
    // 创建Shader
    Shader shaderObj("ObjVShader.vert", "ObjFShader.frag");
    
    // 初始化场景
    Scene scene;
    scene.AddSharedComponent(new GlobalSettings);
    
    // 设置光照
    Light* pLight = new Light();
    InitLight(pLight, &camera);
    scene.AddSharedComponent(pLight);
    
    // 创建动画控制器
    AnimationController aniController;
    scene.AddSharedComponent(&aniController);
    
    // 添加地板
    scene.AddChild("root/BaseFloor");
    auto object = scene.GetChild("root/BaseFloor");
    
    // 设置地板的Transform组件数据，这里的数据仅作参考
    object->AddComponent(new Transform());
    object->GetComponent<Transform>()->rotation = { 0,0,0 };
    object->GetComponent<Transform>()->scale = { 25.0f ,0.01f,25.0f };
    object->GetComponent<Transform>()->localPosition = { 0.3f, -5.0f, -1.6f };
    
    // 为地板添加Render组件
    object->AddComponent(new Render());
    auto pRender = object->GetComponent<Render>();
    
    // 设置物体的模型，绑定相机和着色器
    pRender->SetModel("Model/floor/cube.fbx");
    pRender->SetCamera(&camera);
    pRender->SetShader(&shaderObj);
    
    // 添加一个立方体
    scene.AddChild("root/cube");
    object = scene.GetChild("root/cube");
    object->AddComponent(new Transform());
    object->GetComponent<Transform>()->rotation = { 0,0,0 };
    object->GetComponent<Transform>()->scale = { 0.75f ,0.75f,0.75f };
    object->GetComponent<Transform>()->localPosition = { 0, 0, 0 };
    
    object->AddComponent(new Render());
    pRender = object->GetComponent<Render>();
    pRender->SetModel("Model/cube.fbx");
    pRender->SetCamera(&camera);
    pRender->SetShader(&shaderObj);
    
    object->AddComponent(new RotationController());
    
    // 调用场景的Start方法
    scene.CallStart();
    
    // 获取时间（主要用来处理每帧时间）
    EngineTools::EngineTime* time = EngineTools::EngineTime::GetInstance();
    
    // 进入游戏主循环
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);

        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        camera.width = width;
        camera.height = height;
		cameraMove();
        
        // 调用场景的Update方法
        scene.CallUpdate();
        // 更新上一帧的耗时
		time->UpdateDeltaTime();
        // 把绘制结果显示到屏幕上
        glfwSwapBuffers(window);
    }
    
    return 0;
}

void cameraMove()
{
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    GLfloat cameraSpeed = 1.0f * deltaTime;
    if (keys[GLFW_KEY_W])
        camera.ProcessKeyboard(Camera_Movement(FORWARD), deltaTime);
    if (keys[GLFW_KEY_S])
        camera.ProcessKeyboard(Camera_Movement(BACKWARD), deltaTime);
    if (keys[GLFW_KEY_A])
        camera.ProcessKeyboard(Camera_Movement(LEFT), deltaTime);
    if (keys[GLFW_KEY_D])
        camera.ProcessKeyboard(Camera_Movement(RIGHT), deltaTime);
    if (keys[GLFW_KEY_E])
        camera.ProcessKeyboard(Camera_Movement(UP), deltaTime);
    if (keys[GLFW_KEY_Q])
        camera.ProcessKeyboard(Camera_Movement(DOWN), deltaTime);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (action == GLFW_PRESS)           //如果当前是按下操作
        keys[key] = true;
    else if (action == GLFW_RELEASE)            //松开键盘
        keys[key] = false;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (mouseLeftButtonDown)
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        GLfloat xoffset = xpos - lastX;
        GLfloat yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        GLfloat sensitivity = 0.05;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            mouseLeftButtonDown = true;
            printf("鼠标左键按下！！");
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            printf("鼠标中间按下！！");
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            printf("鼠标右键按下！！");
            break;
        default:
            return;
        }
    }
    if (action == GLFW_RELEASE)
    {
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            mouseLeftButtonDown = false;
            firstMouse = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        default:
            break;
        }
    }
    return;
}

void InitLight(Light* light, Camera* camera)
{
	light->sunLight = { {-0.2f, -1.0f, -0.3f},{0.4f, 0.4f, 0.4f},{0.5f, 0.5f, 0.5f} };
	
	light->pointLight[0] = { {-1.0f, 0.0f, -2.0f},{0.8f, 0.8f, 0.8f},{1.0f, 1.0f, 1.0f},1.0f,0.007f,0.0002f };
	light->pointLight[1] = { {0.0f, -1.0f, 2.0f},{0.8f, 0.8f, 0.8f},{1.0f, 1.0f, 1.0f},1.0f,0.007f,0.0002f };
	light->pointLight[2] = { {-5.0f, -1.0f, 1.0f},{0.8f, 0.8f, 0.8f},{1.0f, 1.0f, 1.0f},1.0f,0.007f,0.0002f };

	light->spotLight = { 
		{camera->Position.x, camera->Position.y, camera->Position.z},
		{camera->Front.x, camera->Front.y, camera->Front.z},
		{1.0f, 1.0f, 1.0f},{1.0f, 1.0f, 1.0f},
		1.0f ,0.09 ,0.032 ,
		glm::cos(glm::radians(45.0f)),
		glm::cos(glm::radians(90.0f)) 
	};

	light->ambient = { 0.2f, 0.2f, 0.2f };

}
```

现在如果你的项目文件夹里有正确的3D模型，你将可以看到在红色背景下，场景里有一个地板和一个正在缓缓旋转的立方体。你可以通过调整参数来获得更好的效果。

如果你的项目正确输出了结果，那么环境配置就完成了，下面让我们开始创建属于我们自己的场景。

### 基础概念

#### 场景Scene

也可以理解为关卡（Level）。场景是一个容器，它包含了游戏或应用程序中的所有游戏对象（GameObjects），用于组织和管理游戏对象，以便在游戏运行时进行交互和渲染。

> 注：同一时间只应该有一个活动中的场景。在本引擎中，你可以使用SceneManager来管理你的场景。当场景数量不太多的时候，你也可以尝试自己手动管理。

#### 游戏对象GameObject

游戏对象是场景中的所有实体的基本单位。它们可以代表场景中的各种物体，如3D模型。游戏对象本身并不包含任何功能，但可以通过添加组件（Components）来为其赋予功能和行为。

> 与Unity不同的是，Camera在本引擎中暂时不视为一个GameObject，尽管它的效果和Unity3D中的摄像机相同。
>
> 除此之外，由于光源在组件Light中管理，并且Render从场景中的共享组件里获得Light中的光照信息，所以光源也不视为GameObject。但是你可以编写自己的方法来手动管理光源的位置与参数。

#### 组件Compoenet

组件是一种可重用的代码片段，它们可以附加到对象上，以实现特定的功能和行为。组件的主要目的是将功能和行为分解成独立的、可重用的部分，以便更容易地组织和管理代码。

在本引擎中，组件分为两种类型：一种被附加到物体上，这时组件的行为与Unity3D中的组件相同；另一种是场景中的共享组件，常用于保存一些共享于场景中的信息，如上文我们提到过的Light组件和GlobalSettings组件。

### 编写规范

#### 场景

只需要继承自原本的Scene即可。如果需要，你可以重写Scene的ToString(int depth)方法。

一个样例如下：

```c++
	class MyScene : public Scene
	{
	public:
		MyScene()
		{
			// Do Init Operation
		}

		~MyScene()
		{
			// If needed, free the memory
		}

		std::string ToString(int depth = 0) const override
		{
			return "This is MyScene";
		}
	};
```

#### 游戏物体

同样只需要继承自原本的GameObject类即可。但是一般情况下，不建议对GameObject进行派生。

#### 组件

组件可能是最主要的工作部分。有很多操作都需要编写组件来进行。

新的组件不仅要继承自Compoenet类，还要编写其构造函数，并且必须修改基类中的typeHashCode这一项的值，否则使用GetCompoenet<>方法或其他类似的方法时可能会获取不到组件。

重写后的组件的构造函数应该类似如下：

```c++
	NewComponent::NewComponent()
    {
        typeHashCode = typeid(NewComponent).hash_code();
    }
```

除此之外，您还可以根据实际需要重写Start、Update或其他方法。截至目前，可以重写的方法如下：

|                      函数声明                      |                             作用                             |
| :------------------------------------------------: | :----------------------------------------------------------: |
|             virtual void OnValidate()              | 用于确认信息更改，例如Transform组件在此方法里确认世界坐标。  |
|                virtual void Awake()                | 场景初调用，只会被调用一次（目前这一项并没有被启用，如果需要，您可以在Scene里编写CallAwake方法并调用） |
|              virtual void OnEnable()               |                         被启用时调用                         |
|              virtual void OnDisable()              |                         被禁用时调用                         |
|                virtual void Start()                |                    场景开始运行的时候调用                    |
|               virtual void Update()                |                         每帧调用一次                         |
|              virtual void OnRemove()               |                       组件被移除时调用                       |
| virtual std::string ToString(int depth = 0) const; |                  用于Debug以及显示日志信息                   |

### 其他功能

#### 动画Animation

动画功能需要共享组件AnimationController、头文件ActionSequence、头文件BaseAnimation一起使用。具体使用的方法为：

1. 创建若干个基于BaseAnimation派生出来的动画对象。
2. 创建一个ActionSequence并把动画对象按照你希望的顺序加入到序列里。
3. 把动画序列与物体绑定。
4. 把动画序列添加到组件AnimationController。

下面是一个示例代码：

```c++
	Translation* trans1 = new Translation({ 10,0,0 }, 2.0f);
	Translation* trans2 = new Translation({ 0,5,0 }, 2.0f);
	Translation* trans3 = new Translation({ 0,0,20 }, 4.0f);

	ActionSequence seq;
	seq.AddAnimation(ActionSequence::MakeParallelAnimations({ trans1, trans2 }), 3);
	seq.AddAnimation({ trans3 });
	seq.BindWith(object);

	scene.GetSharedComponent<AnimationController>()->AddSequence(&seq);
```

这段代码把3个位移动画按照{{trans1, trans2}, trans3}的顺序绑定到某个物体上。

注意：

1. 绑定物体必须在序列添加完所有动画项之后才能进行，否则后加入的动画可能绑定不到物体。
2. 当需要多个动画同时进行时，使用ActionSequence::MakeParallelAnimations构造平行动画，请确保用于构造的这些动画的时长相等。
3. 当需要某个动画无限执行下去的时候，在AddAnimation的第二个参数中填入ENDLESS_LOOP，这个宏等价于0xffffffff。当一个序列中已经有无尽动画的时候，在这之后加入的动画都不会被执行。

##### Animation编写规范

当前的动画只提供了最基础的线性位移动画Translation，在很多情况下，您需要编写自己的动画类型来满足需求，比如修改位移函数为二次函数来控制移动速度，或者让物体按照某个新的轨迹移动。

当您编写新的动画类时：首先，确保新的类型继承自BaseAnimation，然后您需要根据以下规则重写以下几个方法：

|           函数声明            |                             说明                             |
| :---------------------------: | :----------------------------------------------------------: |
|   virtual bool NextFrame()    |   执行下一帧的操作，当动画结束时，返回true，否则返回false    |
|    virtual void Restart()     |   重新开始动画，当且仅当该动画被设置为无尽动画时才会被调用   |
| virtual BaseAnimation* Copy() | 生成一个当前动画对象的复制体，但不论当前对象是否已绑定，新动画对象总是未绑定的 |

最后，把新的头文件包含到Animation.h里，就大功告成了。

### 未来的更新内容

- 粒子系统
- 音乐
- 自己的渲染器
- 物理系统
- 引擎专有异常
- 更加丰富的动画及其他组件