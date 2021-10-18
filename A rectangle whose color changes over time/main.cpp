#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
using namespace std;

const GLint WIDTH = 800, HEIGHT = 600;  //设置窗口的大小

//使用四个顶点信息进行正方形的绘制
//顶点的位置信息
GLfloat Vertices[] =
{
    //位置                   //颜色
    0.5f, 0.5f, 0.0f,       //1.0f, 0.0f, 0.0f,   //右上角顶点
    0.5f, -0.5f, 0.0f,      //1.0f, 0.0f, 0.0f,   //右下角顶点
    -0.5f, -0.5f, 0.0f,     //1.0f, 0.0f, 0.0f,   //左下角顶点
    -0.5f, 0.5f, 0.0f       //1.0f, 0.0f, 0.0f    //左上角顶点
};
//顶点的连接信息
unsigned int Indices[] =
{
    0, 1, 3,
    1, 2, 3
};

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
    glewExperimental = GL_TRUE;  //使用OPENGL前设置为true
    //初始化OpenGL,并检查初始化是否成功
    if (GLEW_OK != glewInit())
    {
        cout << "Faild to initialize GLEW" << endl;
        glfwTerminate();
        return -1;
    }


    //实例化Shader
    Shader OurShader = Shader("res/shaders/core.vs", "res/shaders/core.fs");  

    /*使用编译顶点着色器和片元着色器画三角形*/
    //将顶点数据传入显存
    GLuint VAO, VBO;  //顶点序列对象，顶点缓存对象
    //绑定VAO和VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //传输数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    //设置VAO和VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);  //打开0通道

    //设置索引缓冲对象
    //将顶点的连接信息传入显存
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    //解绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /*显示窗口和内容*/
    //绘图循环
    while (!glfwWindowShouldClose(Window))  //只要当前窗口不关闭，就一直执行循环
    {
        glViewport(0, 0, ScreenWidth, ScreenHeight);  //设置视口大小
        glfwPollEvents();  //事件响应器
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  //设置颜色，并设置透明度为不透明
        glClear(GL_COLOR_BUFFER_BIT);  //赋值到颜色缓存

        //绘制图形
        OurShader.Use();
        float Time = glfwGetTime();  //获取时间
        float GreenTime = sin(Time) / 2.0f + 0.5f;  //设置绿色颜色值范围0--1
        float RedTime = 1 - GreenTime;  //设置红色颜色值范围0--1
        glUniform4f(glGetUniformLocation(OurShader.Program, "time"), GreenTime, RedTime, 0.0f, 1.0f);

        glBindVertexArray(VAO);  //绑定VAO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);  //绑定EBO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  //绘制正方形
        //解绑定
        glBindVertexArray(0);  
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glfwSwapBuffers(Window);  //打开双缓存模式
    }

    //删除
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();  //结束
    return 0;
}
