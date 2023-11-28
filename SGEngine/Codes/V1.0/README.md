# SGENGINE - A Simple 3D Game Engine

## Before Starting

### Thanks

Thanks for the CSDN Blogger Jaihk662, his article [https://blog.csdn.net/jaihk662/category_9903113_2.html](https://blog.csdn.net/jaihk662/category_9903113_2.html)  taught me a lot about OpenGL and how to configure environment.  There are also some files in the engine source code that are directly referenced from the code in the articles, such as Camera.h, Shader.h, Model.h, Mesh.h and the default shader source code of Render Module.

The default shader source code are:

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



### Introduction To SGEngine

> If you have Unity, Cocos, Unreal, or other engine foundations, then the following content will be easy for you to understand.

SGEngine is a simple 3D Engine written in C++.  Because SGEngine has adopted a "Scene-Object-Component" structure similar to Unity, it is easy for you to manage Game Objects and Components in a Scene. The default visualization window use OpenGL for rendering. Of course, you can also use other graphics libraries (such as DirectX) to rewrite the Render module, or not use visualization windows.

> Besides, the engine is just a by-product of my experimental homework. And many modules (such as GUI and music) have not been written yet. Please wait for further updates or submit your branch codes. Thanks for your support.

## Get Started

### Environment Configuration

1. Clone the source code to an empty project.
2. If you need to render using OpenGL, please follow the steps in article [https://blog.csdn.net/jaihk662/category_9903113_2.html](https://blog.csdn.net/jaihk662/category_9903113_2.html) to configure your OpenGL environment. Make sure you correctly configured glew, glfw, glm, assimp and other dependencies. Otherwise, it may cause compilation failure or other bugs (such as black screen, window initialization failure, etc.).
3. Copy the following codes to an empty cpp file and make sure there is only one main function in your project.

> If you believe that your environment is okay, you can skip the step.

If console only, the test codes are:

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

This code creates a simple scene and adds a sub object under the root object, and adds a Transform component to the sub object, then outputs the information of the current scene. You may see similar output like these:

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

And you can find a new file named EngineLog_ToStringTest.txt in your project. The contents should be the same as stdout.

If you need to render with OpenGL, you need to initialize OpenGL before other operations. The sample codes are:

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
    // Init OpenGL
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
    
    // Create Shader
    Shader shaderObj("ObjVShader.vert", "ObjFShader.frag");
    
    // Init scene
    Scene scene;
    scene.AddSharedComponent(new GlobalSettings);
    
    // Set Light
    Light* pLight = new Light();
    InitLight(pLight, &camera);
    scene.AddSharedComponent(pLight);
    
    // Create Animation Controller
    AnimationController aniController;
    scene.AddSharedComponent(&aniController);
    
    // Add floor
    scene.AddChild("root/BaseFloor");
    auto object = scene.GetChild("root/BaseFloor");
    
    // Set Transform, Data is for reference only
    object->AddComponent(new Transform());
    object->GetComponent<Transform>()->rotation = { 0,0,0 };
    object->GetComponent<Transform>()->scale = { 25.0f ,0.01f,25.0f };
    object->GetComponent<Transform>()->localPosition = { 0.3f, -5.0f, -1.6f };
    
    // Add Render
    object->AddComponent(new Render());
    auto pRender = object->GetComponent<Render>();
    
    // Bind model, camera and shader
    pRender->SetModel("Model/floor/cube.fbx");
    pRender->SetCamera(&camera);
    pRender->SetShader(&shaderObj);
    
    // Add a cube
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
    
    // Call Start() in the scene
    scene.CallStart();
    
    // Get Time(Mainly used for delta time)
    EngineTools::EngineTime* time = EngineTools::EngineTime::GetInstance();
    
    // Enter main loop 
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
        
        // Call Update() in a scene
        scene.CallUpdate();
        // Update delta time
		time->UpdateDeltaTime();
        // Output in your screen
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
    if (action == GLFW_PRESS)           //if press
        keys[key] = true;
    else if (action == GLFW_RELEASE)            //if release
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
            printf("Mouse Left Button Pressed!!");
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            printf("Mouse Middle Button Pressed!!");
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            printf("Mouse Right Button Pressed!!");
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

Now if there are correct model in your project, you may see a floor and a rotating cube with red background in your scene. You may adjust the arguments to get better effects.

Now let's create your own scene.

### Basic Concepts

#### Scene

It can also be understood as a level(in Unreal). A scene is a container that contains all GameObjects in a game or application. It is used to organize game objects for interaction and rendering during game runtime.

> Note: There should only be one active scene at the same time. In SGEngine, you can use SceneManager to manage your scenes. When the number of scenes is not too large, you can also try to manually manage them yourself.

#### GameObject

Game objects are the basic units of all entities in the scene. They can represent various objects in the scene, such as 3D models. The game object itself does not contain any functionality, but it can be endowed with functionality and behavior by adding components.

> Unlike Unity, Camera is temporarily not considered a GameObject now, although its effect is the same as the camera in Unity3D.
>
> In addition, since the light source is managed in the component Light and Render obtains lighting information from shared components in the scene, the light source is not considered a GameObject. You can also write your own methods to manage the position and parameters of the light source.

#### Component

Components are reusable code fragments that can be attached to objects to achieve specific functions and behaviors. The main purpose of components is to decompose functionality and behavior into independent and reusable parts, making it easier to organize and manage code.

In SGEngine, components are divided into two types: one is attached to an object, where the behavior of the component is the same as that of the component in Unity3D; Another type is shared components in the scene, commonly used to save information shared in the scene, such as the Light component and GlobalSettings component mentioned earlier.

### Writing Specifications

#### Scene

Just inherit from the original Scene. If necessary, you can override the ToString (int depth) method of Scene.

Here is an example:

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

#### GameObject

Similarly, it only needs to inherit from the original GameObject class. However, in general, it is not recommended to derive GameObjects.

#### Component

Components may be the most important part of your coding work. There are many operations that require writing components to perform.

The new component not only needs to inherit from the Compoenet class, but also needs to rewrite its constructor, and the value of the typeHashCode item in the base class must be modified. Otherwise, using the GetCompoenet<>method or other similar methods may not obtain the component.

The constructor of the rewritten component should be similar to the following code:

```c++
	NewComponent::NewComponent()
    {
        typeHashCode = typeid(NewComponent).hash_code();
    }
```

Apart from that, you can also override Start, Update, or other methods according to actual needs. As of now, the methods that can be rewritten are as follows:



|                Function Declaration                |                         Description                          |
| :------------------------------------------------: | :----------------------------------------------------------: |
|             virtual void OnValidate()              | Used to confirm information changes.  For example, the Transform component confirms world coordinates in this method. |
|                virtual void Awake()                | Called at the beginning of the scene, it will only be called once. (At present, this option is not enabled. If necessary, you can write the CallAwake method in Scene and call it.) |
|              virtual void OnEnable()               |                     Called when enabled                      |
|              virtual void OnDisable()              |                     Called when disabled                     |
|                virtual void Start()                |             Called when the scene starts running             |
|               virtual void Update()                |                        Call per frame                        |
|              virtual void OnRemove()               |              Called when a component is removed              |
| virtual std::string ToString(int depth = 0) const; |            Used for debug and show Log Infomation            |

### Other Functions

#### Animation

Animation should be used with SharedComponent-AnimationController, Header-ActionSequence, Header-BaseAnimation. The concrete usage are:

1. Create several animation objects derived from BaseAnimation.
2. Create an ActionSequence and add animation objects to the sequence in the order you want.
3. Bind the sequence to a gameobject.
4. Add the sequence to AnimationController.

Here is an example:

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

The code binds three Translation to an object in the order {{trans1, trans2}, trans3}.

Notes:

1. Binding objects must be called after all animation items have been added to the sequence, otherwise the animation added later may not be bound to the object.
2. When multiple animations need to be executed simultaneously, use ActionSequence:: MakeParallelAnimations to construct parallel animations. Please ensure that the duration of these animations used for construction is equal.
3. When an animation needs to be executed indefinitely, fill in ENDLESS_LOOP in the second parameter (times) of AddAnimation , this macro is equivalent to 0xffffff. When there is already endless animation in a sequence, any animation added after that will not be executed.

#### Writing Specifications of Animation

The current animation only provides the most basic linear displacement animation -- Translation. Thus, you should write your own animation type to meet your needs. For example, change the displacement function to a quadratic function to control the moving speed, or let the object move according to a new trajectory.

When writing a new animation class, first ensure that the new type inherits from BaseAnimation, and then you need to rewrite the following methods according to the following rules:

|     Function Declaration      |                         Description                          |
| :---------------------------: | :----------------------------------------------------------: |
|   virtual bool NextFrame()    | Execute the next frame operation, return true when the animation ends, otherwise return false |
|    virtual void Restart()     | Restart the animation and it will only be called when and only when the animation is set to Endless Animation |
| virtual BaseAnimation* Copy() | Generate a replica of the current animated object, but regardless of whether the current object is bound or not, the new animated object is always unbound |

Finally, include the new header file in Animation. h. You are done!

### Future Updates

- Particle System
- Music
- My own Render pipe
- Physics
- Engine Exceptions
- More Animations And Components