#include <iostream>
#include <Windows.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Shader.h"
#include "Camera.h"
#define WIDTH 800
#define HEIGHT 600
using namespace std;

/*三维物体的顶点信息和颜色信息*/
GLfloat Vertices[] =
{
    //第一个面
    //位置                   //颜色
    -0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,      1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, -0.5f,      1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, -0.5f,     1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 0.0f,

    //第二个面
    //位置                   //颜色
    -0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f,      0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,       0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,       0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,      0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.5f,     0.0f, 1.0f, 0.0f,

    //第三个面
    //位置                   //颜色
    -0.5f, 0.5f, 0.5f,      0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, -0.5f,     0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,     0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,      0.0f, 0.0f, 1.0f,

    //第四个面
    //位置                   //颜色
    0.5f, 0.5f, 0.5f,       1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, -0.5f,      1.0f, 1.0f, 0.0f,
    0.5f, -0.5f, -0.5f,     1.0f, 1.0f, 0.0f,
    0.5f, -0.5f, -0.5f,     1.0f, 1.0f, 0.0f,
    0.5f, -0.5f, 0.5f,      1.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,       1.0f, 1.0f, 0.0f,

    //第五个面
    //位置                   //颜色
    -0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f,      1.0f, 0.0f, 1.0f,
    0.5f, -0.5f, 0.5f,      1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.5f,     1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,    1.0f, 0.0f, 1.0f,

    //第六个面
    //位置                   //颜色
    -0.5f, 0.5f, -0.5f,     0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, -0.5f,      0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f,       0.0f, 1.0f, 1.0f,
    0.5f, 0.5f, 0.5f,       0.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,      0.0f, 1.0f, 1.0f,
    -0.5f, 0.5f, -0.5f,     0.0f, 1.0f, 1.0f,
};

bool keys[1024];  //存储按键信息
//按键触发关闭的函数
void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        int temp = MessageBox(NULL, "是否退出？", "提示", MB_YESNO);
        if (temp == IDYES)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        else if (temp == IDNO)
        {
            glfwSetWindowShouldClose(window, GL_FALSE);
        }
            
    }
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }
}

int main()
{
    /*初始化OPENGL*/
    //创建窗口前，对窗口进行设置
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  //向前兼容，MACOS系统必须加
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);  //禁止缩放

    //新建窗口
    GLFWwindow* Window = glfwCreateWindow(WIDTH, HEIGHT, "Learn OpenGL B19080618", nullptr, nullptr);
    //为高清屏做设置
    int ScreenWidth, ScreenHeight;
    glfwGetFramebufferSize(Window, &ScreenWidth, &ScreenHeight);  //获取帧缓存大小
    //检查屏幕是否创建成功
    if (nullptr == Window)
    {
        cout << "Faild to create GLFW window." << endl;
        glfwTerminate();  //GLFW关闭
        return -1;
    }

    //使用OPENGL画图
    glfwMakeContextCurrent(Window); //把当前窗口环境转到window
    glfwSetKeyCallback(Window, KeyCallBack);  //设置键盘

    glewExperimental = GL_TRUE;  //使用OPENGL前设置为true
    //初始化OpenGL,并检查初始化是否成功
    if (GLEW_OK != glewInit())
    {
        cout << "Faild to initialize GLEW" << endl;
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);  //开启深度测试
    glDepthFunc(GL_LESS);  //深度测试方程
    //设置透明度
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //实例化Shader
    Shader OurShader = Shader("res/shaders/core.vs", "res/shaders/core.fs");
    Camera OurCamera(glm::vec3(0.0f, 0.0f, 2.0f));

    /*着色器设置和纹理设置*/
    //将顶点数据传入显存
    GLuint VAO, VBO;  //顶点序列对象，顶点缓存对象
    //绑定VAO和VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //传输数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    //设置VAO、VBO和纹理
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);  //打开0通道
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);  //打开1通道

    //解绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /*显示窗口和内容*/
    //绘图循环
    while (!glfwWindowShouldClose(Window))  //只要当前窗口不关闭，就一直执行循环
    {
        glViewport(0, 0, ScreenWidth, ScreenHeight);  //设置视口大小
        glfwPollEvents();  //事件响应器
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);  //设置颜色，并设置透明度为不透明
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);  //赋值到颜色缓存,然后清空缓存
        
        //相机设置
        float Aspact = (float)ScreenWidth / (float)ScreenHeight;  //长宽比
        glm::mat4 Projection = glm::perspective(glm::radians(60.0f), Aspact, 0.1f, 100.0f);
        glm::mat4 View = OurCamera.GetViewMatrix();

        //绘制图形
        OurShader.Use();
        int VertexProjectionLocation = glGetUniformLocation(OurShader.Program, "projection");
        int VertexViewLocation = glGetUniformLocation(OurShader.Program, "view");
        glUniformMatrix4fv(VertexProjectionLocation, 1, GL_FALSE, glm::value_ptr(Projection));
        glUniformMatrix4fv(VertexViewLocation, 1, GL_FALSE, glm::value_ptr(View));
        glBindVertexArray(VAO);  //绑定VAO

        //地月系统绘制
        for (int i = 1; i <= 2; i++)
        {
            GLfloat Time = glfwGetTime();  //获取时间
            glm::mat4 Transform;  //旋转矩阵
            if (i == 1)
            {
                Transform = glm::scale(Transform, glm::vec3(0.6f, 0.6f, 0.6f));  //缩放
                Transform = glm::rotate(Transform, Time, glm::vec3(0.2f, 1.0f, 0.0f));  //绕轴旋转
            }
            else
            {
                Transform = glm::translate(Transform, glm::vec3(0.8f * cos(Time), 0.8f * sin(Time), 0.0f));  //平移
                float ScaleTransform = 0.15f - sin(Time) * 0.05f;
                Transform = glm::scale(Transform, glm::vec3(ScaleTransform, ScaleTransform, ScaleTransform));
                Transform = glm::rotate(Transform, Time, glm::vec3(0.5f, 0.5f, 0.0f));
            }
            int VertexTransformLocation = glGetUniformLocation(OurShader.Program, "transform");
            glUniformMatrix4fv(VertexTransformLocation, 1, GL_FALSE, glm::value_ptr(Transform));
            glDrawArrays(GL_TRIANGLES, 0, 36);  //绘制三角形 
        }
        glBindVertexArray(0);  //解绑定
        glfwSwapBuffers(Window);  //打开双缓存模式
    }

    //删除
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();  //结束
    return 0;
}
