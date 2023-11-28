# SGENGINE - 一个简单的3D引擎

## 简介

Simple Game Engine是一个我自己用C++搭建的3D引擎，支持的功能正在不断拓展中，请耐心等待。

引擎采用类似Unity3D的“场景-物体-组件”结构，可以帮助您轻松地对场景内的物体或组件进行管理，也可以根据需求定制您自己的组件。

如果您有Unity3D、Cocos、Unreal或其他引擎基础，那么后面的内容对您来说将会十分简单。



## 更新日志

- 2023/7/16 2.0版本

&emsp;&emsp;版本特性：

1. 抛弃了OpenGL改用我自己实现的简单渲染器（SRender）（注意：该渲染器仅支持Windows平台），并基于该渲染器实现了相机组件和一些UI组件。
2. 实现了资源管理器以便于对已经加载的图片或模型资产进行管理。
3. 实现了ObjLoader和BmpLoader，用于加载简单的.obj文件和.bmp文件。
4. 重新实现了矩阵类，现在原来的矩阵类EngineTools::Matrix已经弃用，请使用SRender::MathTools::Matrix。
5. 将EngineTools::Vector3的基类型从float更改为double。



- 2023/6/13 1.0版本

&emsp;&emsp;版本特性：

1. 实现了基础引擎结构。
2. 基于OpenGL进行渲染操作。
3. 实现了动画控制器进行简单的动画控制。



## 致谢

- 感谢知乎用户Ekennis的文章[https://www.zhihu.com/people/eykenis/posts](https://www.zhihu.com/people/eykenis/posts)，新渲染器中有关三角形的知识大部分参考自这些文章。同时，渲染管线也在一定程度上参考了这些文章里的内容，但根据实际数据结构有一些改动。

- 感谢CSDN博主Jaihk662的文章[https://blog.csdn.net/jaihk662/category_9903113_2.html](https://blog.csdn.net/jaihk662/category_9903113_2.html) ，教会了我如何配置OpenGL环境以及一些关于OpenGL的知识。在1.0版本中，引擎源码中一些文件是直接引用的其文章中的代码，如Camera.h 、Shader.h、Model.h、Mesh.h，以及Render模块默认假定的着色器代码。在2.0版本中，Phong模型的着色器代码参照其GLSL代码改写为C++。下附1.0版本中主要使用的GLSL着色器代码：

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

## 第三方引用

- lodepng [lvandeve/lodepng: PNG encoder and decoder in C and C++. (github.com)](https://github.com/lvandeve/lodepng) 来自GitHub上的项目，在本项目中用于解析.png文件。

## 开始使用

### 环境配置

#### V2.0

1. 新建项目，并把V2.0文件夹中的代码添加到项目里。
2. 构建并运行main.cpp中的默认场景，如果程序成功显示出了类似下方图片的输出，运行成功。![SampleV2](SampleV2.png)
3. 修改main.cpp中InitScene()函数，现在可以开始构建您自己的场景了！

#### V1.0

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

   这段代码创建了一个简单的场景，并为场景的根物体下添加了一个子物体，并为子物体添加了Transform组件，然后把当前场景的信息输出出来。您看到的输出应该类似下面的输出：

   > -Scene
   > -GlobalSettings:
   > @EngineVersion: 1.0
   > -root
   > -Component:Transform
   > @localPosition: Vector3(0.000000 0.000000 0.000000)
   > @worldPosition: Vector3(0.000000 0.000000 0.000000)
   > @rotation: Vector3(0.000000 0.000000 0.000000)
   > @scale: Vector3(1.000000 1.000000 1.000000)
   > -child1
   > -Component:Transform
   > @localPosition: Vector3(0.000000 0.000000 0.000000)
   > @worldPosition: Vector3(0.000000 0.000000 0.000000)
   > @rotation: Vector3(0.000000 0.000000 0.000000)
   > @scale: Vector3(1.000000 1.000000 1.000000)

   如果使用OpenGL进行渲染，首先要先初始化OpenGL，然后才能进行其他操作，样例代码如下：

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

   现在如果您的项目文件夹里有正确的3D模型，您将可以看到在红色背景下，场景里有一个地板和一个正在缓缓旋转的立方体。您可以通过调整参数来获得更好的效果。

   如果您的项目正确输出了结果，那么环境配置就完成了，下面让我们开始创建属于我们自己的场景。   

## 基础概念

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

## 编写规范

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

## 文档

### 渲染器文档

##### 基础介绍

SRender渲染器只支持在Windows上工作。

SRender渲染器的工作顺序是：先绘制场景中各个摄像机的内容并把他们叠加在一起，然后绘制各个画布的内容并叠加在一起，最后把这两部分内容叠加到一起并输出到屏幕上。

其中，绘制摄像机和绘制画布的过程可能会造成性能问题，请确保不要频繁刷新UI或者在场景中放入过多物体，否则可能会造成严重卡顿。

如果您需要使用自己的着色器，请重载Shader类，并且重写顶点着色器Vert和片段着色器Frag。请注意，Vert输出的是一个4*1的点矩阵，其内容为
$$
\begin{bmatrix}
x''w\\
y''w\\
z''w\\
w
\end{bmatrix}
$$
其中x'',y''为[-1,1]的NDC坐标。

##### 命名空间

渲染器有关命名空间均在SRender空间内。

##### Matrix

矩阵类可能是渲染器部分会被外部使用的最多的数学类。当前矩阵实现使用一个一维数组模拟二维数组，并且必须指明矩阵大小。

注意：zero和one是一个函数指针，用于指定一个元素是0或1。

主要的公开成员有：

```c++
 			//Constructors
            Matrix();

            // @ para int row
            // @ para int col
            Matrix(int row, int col);

            // @ para int row
            // @ para int col
            // @ para T   val 矩阵元素的默认值
            Matrix(int row, int col, T val);

            // @ para int row
            // @ para int col
            // @ para T(*zero)()    指定一个得到零元素的方法
            // @ para T(*one)()     指定一个得到一元素的方法
            Matrix(int row, int col, T(*zero)(), T(*one)());

            // 复制构造函数
            Matrix(Matrix const& other);

            ~Matrix();

            // 更改矩阵大小并重新分配空间
            // @ para int row
            // @ para int col
            void ReSize(int row, int col);

            void ReSize(int row, int col, T val);

            void ReSize(int row, int col, int val);

            //Basic Get/Set methods
            int GetRow() const;

            int GetCol() const;

            T GetValue(int x, int y) const;

            T* const GetValuePtr() const;

            void SetValuePtr(T* _value);

            void SetValue(int x, int y, T val);

            void SetZeroFunc(T(*zero)());

            void SetOneFunc(T(*one)());

            //Math operation
            Matrix Add(Matrix const& other) const;

            Matrix Subtract(Matrix const& other) const;

            Matrix Multiply(Matrix const& other) const;

            Matrix Transfer() const;

            Matrix Inverse() const;

            //Operator overloading
            Matrix& operator=(Matrix const& other);

            Matrix operator+(Matrix const& other) const;

            Matrix operator-(Matrix const& other) const;

            Matrix operator*(Matrix const& other) const;

            Matrix operator+=(Matrix const& other);

            Matrix operator-=(Matrix const& other);

            Matrix operator*=(Matrix const& other);

            T* operator[](int x) const;

            std::string ToString() const;

            // static Methods, used to get special matrix
            
            // Get Zero Matrix
            // @ para int row
            // @ para int col
            static Matrix GetZeros(int row, int col);

            // Get Zero Matrix using zero function
            // @ para int row
            // @ para int col
            // @ para T(*zero)()    zero funtion
            static Matrix GetZeros(int row, int col, T(*zero)());

            // Get One Matrix
            // @ para int row
            // @ para int col
            static Matrix GetOnes(int row, int col);

            // Get One Matrix using one function
            // @ para int row
            // @ para int col
            // @ para T(*one)()    one funtion
            static Matrix GetOnes(int row, int col, T(*one)());

            // Get Unit Matrix
            // @ para int size
            static Matrix GetUnit(int size);

            // Get Unit Matrix using one function
            // @ para int size
            // @ para T(*one)()    one funtion
            static Matrix GetUnit(int size, T(*one)());
```



### 引擎文档

#### 命名空间

引擎有关内容均在EngineTools空间内。

#### 基础设施文档

##### Vector3

基础三维向量，主要公开成员有：

```c++
		double x, y, z;

		Vector3() = default;

        Vector3(double _x, double _y, double _z);

        Vector3(const Vector3 &_vec);

        Vector3 operator+(const Vector3 &vec3) const;

        Vector3 operator-(const Vector3& vec3) const;

		// 等价于Dot(const Vector3&)
        double operator*(const Vector3 &vec3) const;

        Vector3 operator*(double time) const;

        Vector3 operator/(double time) const;

		Vector3& Normalize();

        double Dot(const Vector3& other) const;

        Vector3 Cross(const Vector3& other) const;

        double Distance(const Vector3& other) const;
```

##### UIBase

所有UI组件都应该派生自UIBase类，同时派生自Component类。

作为所有UI组件的基类，UIBase拥有以下公开成员：

```c++
		int posX;		// UI的X坐标
		int posY;		// UI的Y坐标
		int width;		// UI的宽度
		int height;		// UI的高度
		float rotation; // UI的旋转值，以逆时针为正，注意：这个值暂时还没用

		// 该UI是否已经在画布注册
		bool isRegistered = false;

		// UI图像的原始数据
		ImageAsset* imgData = nullptr;

		// 输出给画布的像素数据
		SRender::MathTools::Matrix<RGBAColor>* out = nullptr;
```

##### Component

这是所有组件的基类，所有组件都应当是这个类的子类。

Component类提供了众多生命周期函数，判定组件类型的依据，组件状态等重要信息。注意：就当前引擎版本而言，仅Start和Update有效。

主要公开成员有：

```c++
		bool isActive = true; // 是否启用

        GameObject *gameObject = nullptr; // 父物体的指针

        size_t typeHashCode; // 类型判定依据，用于GetComponent方法，在被继承时应当在构造函数里更改初始设定值

        // Constructors:
        Component(); // 无参构造函数，必须在这里为typeHashCode赋值

        virtual ~Component() = default; // 虚析构函数，可以覆盖

        // Methods:
        void Enable(); // 启用组件

        void Disable(); // 禁用组件

        virtual void OnValidate() // 用于确认信息更改
        {};

        virtual void Awake() // 场景初调用，只会被调用一次
        {};

        virtual void OnEnable() // 被启用时调用
        {};

        virtual void OnDisable() // 被禁用时调用
        {};

        virtual void Start() // 场景开始运行的时候调用
        {}

        virtual void Update() // 每帧被调用
        {};

        virtual void OnRemove() // 组件被移除时调用
        {};
```

##### DebugHelper

用于输出日志信息到日志文件或控制台。在打印信息的时候会自动附带时间信息。

主要公开成员有：

```c++
		template<typename T>
        static void Log(T* object, const std::string& fileName = "");

        static void Log(const char* s, const char* fileName = "");

        static void LogWarning(const char* s, const char* fileName = "");

        static void LogError(const char* s, const char* fileName = "");

        static void LogError(std::exception& e, const char* fileName = "");
```

##### EngineTime

引擎时间用于读取每一帧的帧间隔，是一个单例类。

主要公开成员有：

```c++
		std::clock_t deltaTime;         // 每帧时间，以毫秒为单位
        float deltaTimef;               // 每帧时间，以秒为单位

        static EngineTime *GetInstance(); // 获取单例对象

        static void DeleteInstance();   // 清除对象

        void UpdateDeltaTime();         // 刷新每帧间隔
```

##### ResourceManager

资源管理器管理着当前已经加载的所有图片和模型资源（以后可能会有更多类型），它和XXXAsset一起工作。

资源管理器也是一个单例类。

关于不同的资产，请查阅对应源代码。

主要公开成员有：

```c++
		// 获取实例指针
		static ResourceManager* GetInstance();

		// 释放实例并清理已加载的所有资源
		static void DestoryInstance();

		// 加载在指定路径的图片资产
		// @para path			文件路径
		// @para imgType		图片类型
		// @ret  ImageAsset*	加载出来的图片资产指针
		ImageAsset* LoadImageAtPath(const std::string& path, ImageType imgType = ImageType::BMP);

		// 注册图片资产
		// @para path	文件路径
		// @para asset	资产指针
		void RegisterImageAsset(const std::string& path, ImageAsset* asset);

		// 查找图片资产
		// @para path			文件路径
		// @ret  ImageAsset*	资产指针
		ImageAsset* FindImageAsset(const std::string& path);

		// 加载对应路径的物体模型
		// @para path	文件路径
		// @para type	文件类型
		// @ret  int	模型资产的UID
		int LoadObjectAtPath(const std::string& path, ModelType type = ModelType::OBJ);

		// 注册模型资产
		// @para modelID	模型UID
		// @para asset		资产指针
		void RegisterModelAsset(int modelID, ModelAsset* asset);

		// 查找图片资产
		// @para path			文件路径
		// @ret  ImageAsset*	资产指针
		ModelAsset* FindModelAsset(int modelID);
```

##### SceneManager

场景管理器用于管理当前正在运行的场景栈。

注意：如果决定使用场景管理器，那么最好在整个项目中一直使用场景管理器。否则，就完全不使用场景管理器。这有助于避免不必要的困扰。

主要的公开成员有：

```c++
		Scene* NowActiveScene = nullptr; // 当前活动场景，默认为空，开始时需要Load或Push操作

		// 直接加载场景，该操作直接替换当前活动场景，不对场景栈操作，同时会释放旧的场景
		// @ para	Scene*	将要加载的场景指针
		void LoadScene(Scene* s);

		// 切换到新场景但不释放旧场景
		// @ para	Scene*	将要加载的场景指针
		void PushScene(Scene* s);

		// 释放当前活动中场景，并把栈顶场景设置为活动场景
		void PopScene();
```

##### StringTools

字符串工具用于处理字符串，目前已经完成的工具有分割工具和转换到数字类型。

##### MemoryPool

一个简单的内存池模型。

主要的公开成员有：

```c++
		// 从生存池里获取一个物体或创建并初始化一个物体（如果需要初始化）
        // @ para void (*InitFunc)(T* object) = nullptr 物体的初始化方法，如果不传入则为空
        // @ ret  std::pair<T*, bool>                   返回物体指针，以及这个物体是否为新创建的物体，如果是则为true
        std::pair<T*, bool> Create(void (*InitFunc)(T* object) = nullptr);

        // 将一个物体从生存池移入死亡池
        // @ para T* object 将要销毁的物体指针
        void Destroy(T* object);
```

##### XXXLoader

用于加载对应类型的文件到对应资产，目前实现了简单的.bmp和.obj文件的加载。具体内容请查看源码。

#### 组件文档

##### Transform

用于记录当前物体的位置信息，由于很多组件都需要访问Transform组件，所以我在GameObject中添加了一个指向当前物体挂载的Transform组件的快捷方式，使用方式为：gameObject->transform。

Transform的主要公开成员有：

```C++
		Vector3 localPosition{};    // 以父物体位置为原点的相对位置
        Vector3 worldPosition{};    // 世界位置
        // 旋转角度，注意：当父物体旋转时不影响子物体的旋转，如果需要跟随旋转，请在子物体使用RotationWithRoot方法
        Vector3 rotation{};         
        Vector3 scale{1, 1, 1};     // 缩放，同上，父物体缩放不影响子物体

		// 对物体进行平移变换
        // @ para const Vector3& vec3   移动的方向及距离
        void Translate(const Vector3& vec3);    

        // 旋转物体
        // @ para const Vector3& vec3 旋转角度，以度为单位
        void Rotation(const Vector3& vec3);

        // 跟随父物体旋转
        // @ para const Vector3& vec3 旋转角度，以度为单位
        // @ para bool changeSelfRotation = true    是否改变当前物体旋转角度，如果false，则仅更改位置而不改变旋转角度
        void RotationWithRoot(const Vector3& vec3, bool changeSelfRotation = true);

        // 获取模型矩阵
        // @ ret  SRender::MathTools::Matrix<double>    模型矩阵
        SRender::MathTools::Matrix<double> GetWorldModelMatrix() const;
```

Transform是一个十分重要的组件，几乎每一个物体都需要挂载这个组件。

##### RenderInfo

这个组件记录了模型信息，当模型绑定之后，它将在场景中的GlobalSettings组件中注册模型信息。

主要的公开成员有：

```c++
		// 绑定对应路径的模型
		// @para path	文件路径
		// @para type	模型文件类型
		void BindModel(const std::string& path, ModelType type = ModelType::OBJ);

		// 直接绑定对应指针的模型到自己
		// Note: 不检查空指针，故BindModel(nullptr) = UnBindModel()
		void BindModel(ModelAsset* _model);

		// 取消与当前模型的绑定
		void UnBindModel();
```

##### Light

用于记录光照信息的组件。每个灯光都应当占用一个Light组件。Light组件使用一个union来保存不同种类的灯光信息，因此，请确保您正确设置了灯光类型，这样灯光信息才能被正确读取。

主要的公开成员有：

```c++
		int lightID;                // 光照ID
        LightType lightType;        // 光照类型
        union LightInfo
        {
            SunLight sunLight;      // 太阳光
            PointLight pointLight;  // 点光源
            SpotLight spotLight;    // 聚光灯
            Vector3 ambient;        // 环境光
        } lightInfo;                // 光照信息
```

##### GlobalSettings

用于记录当前场景中需要记录的数据，如模型信息、光照信息、摄像机数据等。该组件只会被动接受其他组件的修改，而不主动进行数据校验。

主要的公开成员有：

```c++
		const std::string EngineVersion = "2.0";	// 当前引擎版本2.0
		std::map<const RenderInfo*, ModelAsset*> modelMap;		// 模型信息
		std::map<int, Camera*> cameras;							// 摄像机信息
		std::map<LightType, std::vector<Light*>> orderedLights;	// 光照信息
```

##### Camera

摄像机组件用于观察当前世界。

警告：摄像机窗口大小设置选项暂时无效！

主要的公开成员有：

```c++
		int cameraID;				// 相机ID
		float fov = 45.0f;			// 视角大小
		float cameraNear = 0.1f;	// 近平面
		float cameraFar = 100.0f;	// 远平面

		// 默认世界向上方向为y轴正方向
		Vector3 worldUp = Vector3(0.0, 1.0, 0.0);

		// 默认前方指向为z轴正方向
		Vector3 front = Vector3(0.0, 0.0, 1.0);

		// 获取LookAt矩阵
		SRender::MathTools::Matrix<double> GetLookAtMatrix();
```

##### Canvas

画布用来承载若干UI元素、获取对应UI的输出，并把这些图像整合后输出到渲染系统。

UI项需要主动绑定自己到画布上，Canvas不会去寻找其他UI项或者对其输出进行校验。

主要的公开成员有：

```c++
		// 获取当前画布的输出
		Layer* GetOutput() const;

		// 把目标UI注册到当前画布
		// @para UIID		UI的ID
		// @para UILayer	UI的指针
		void RegisterUIElement(unsigned int UIID, UIBase* UILayer);

		// 移除指定ID的UI
		// @para UIID		UI的ID
		void RemoveUIElement(unsigned int UIID);

		// 把当前画布注册到渲染系统
		void RegisterCanvasToRenderSystem();

		// 从渲染系统移除当前画布
		void RemoveCanvasFromRenderSystem();

		// UI项通知画布有变化，需要重绘
		void InformChange();
```

##### Image

Image是一个样例UI项组件，用于在指定位置显示一个固定像素大小的图片。在编写新的UI组件时可以供作参考。

主要的公开成员有：

```c++
		// 绑定图片
		// @para path	文件路径
		// @para type	文件类型
		void BindImage(const std::string& path, ImageType type);

		// 修改坐标，移动至对应坐标
		// @para position	新的坐标
		//		first:	x
		//		second: y
		void MoveTo(const std::pair<int, int>& position);

		// 平移对应距离
		// @para vec		移动距离
		//		first:	x
		//		second: y
		void Translate(const std::pair<int, int>& vec);

		// 缩放图片到新的大小
		// @para nHeight	新的高度
		// @para nWidth		新的宽度
		void ScaleTo(int nHeight, int nWidth);

		// 与对应画布绑定
		// @para pCanvas	要绑定的画布指针
		void BindCanvas(Canvas* pCanvas);

		// 与对应路径的画布绑定
		// @para path	画布的物体路径
		void BindCanvas(const std::string& path);
```



#### 其他功能

##### 动画Animation

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

## 未来的更新内容

- 粒子系统
- 音乐
- 输入系统
- 物理系统
- 引擎专有异常
- 更加丰富的动画及其他组件






