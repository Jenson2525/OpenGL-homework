#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/SOIL2.h>
#include <SOIL2/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
using namespace std;

const GLint WIDTH = 600, HEIGHT = 600;  //设置窗口的大小

//使用四个顶点信息进行正方形的绘制
//顶点的位置信息、颜色信息和纹理坐标信息
GLfloat Vertices[] =
{
    //位置                   //颜色                  //纹理坐标
    1.0f, 3.0f, 0.0f,       1.0f, 0.0f, 0.0f,       1.0f, 2.0f,  //右上角顶点0
    1.0f, -3.0f, 0.0f,      0.0f, 1.0f, 0.0f,       1.0f, -1.0f,  //右下角顶点1
    -1.0f, -3.0f, 0.0f,     0.0f, 0.0f, 1.0f,       0.0f, -1.0f,  //左下角顶点2
    -1.0f, 3.0f, 0.0f,      0.0f, 0.0f, 0.0f,       0.0f, 2.0f   //左上角顶点3
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
    //设置VAO、VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);  //打开0通道
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);  //打开1通道
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);  //打开2通道

    //设置索引缓冲对象
    //将顶点的连接信息传入显存
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    //设置纹理对象
    //传入图片
    int Width, Height;
    unsigned char* image1 = SOIL_load_image("res/images/snow1.png", &Width, &Height, 0, SOIL_LOAD_RGBA);
    unsigned char* image2 = SOIL_load_image("res/images/snow2.png", &Width, &Height, 0, SOIL_LOAD_RGBA);

    //生成第一个纹理
    GLuint Texture1;
    glGenTextures(1, &Texture1);  //生成纹理
    glBindTexture(GL_TEXTURE_2D, Texture1);  //绑定
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
    glGenerateMipmap(GL_TEXTURE_2D);  //多层渐进纹理
    //生成第二个纹理
    GLuint Texture2;
    glGenTextures(1, &Texture2);  //生成纹理
    glBindTexture(GL_TEXTURE_2D, Texture2);  //绑定
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2);
    glGenerateMipmap(GL_TEXTURE_2D);  //多层渐进纹理

    //释放图像内存
    SOIL_free_image_data(image1);  
    SOIL_free_image_data(image2);
    
    //纹理参数设置
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /*解绑定*/
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);

    /*显示窗口和内容*/
    int count = 0;  //计数器
    int flag = 1;  //纹理切换标志
    int N = 100;  //N越大，位移/交替的速度都会变慢

    //绘图循环
    while (!glfwWindowShouldClose(Window))  //只要当前窗口不关闭，就一直执行循环
    {
        glViewport(0, 0, ScreenWidth, ScreenHeight);  //设置视口大小
        glfwPollEvents();  //事件响应器
        glClearColor(0.5f, 0.8f, 0.5f, 1.0f);  //设置颜色，并设置透明度为不透明
        glClear(GL_COLOR_BUFFER_BIT);  //赋值到颜色缓存

        //绘制图形
        OurShader.Use();
        glActiveTexture(GL_TEXTURE0);  //开启texture0
        glUniform1i(glGetUniformLocation(OurShader.Program, "OurTexture1"), 0);
        glBindTexture(GL_TEXTURE_2D, Texture1);

        glActiveTexture(GL_TEXTURE1);  //开启texture1
        glBindTexture(GL_TEXTURE_2D, Texture2);
        glUniform1i(glGetUniformLocation(OurShader.Program, "OurTexture2"), 1);

        //每一次循环引起的反应
        count++;// 计数器加1
        if (count >= 2 * N)
        {
            count = 0;		// 设置重复循环
        } 
        if (count == 0 || count == N)
        {
            flag = -flag;	// 设置纹理循环交替标志
        }
        
        //平移变换的实现
        glm::mat4 Matrix = glm::mat4(1.0f); //初始化为单位矩阵
        glm::mat4 Transform = glm::translate(Matrix, glm::vec3(0.0f, 1.0f - count * 1.0 / N, 0.0f));

        /* 交替变换的实现 */
        int vertexColorLocation = glGetUniformLocation(OurShader.Program, "time");	//找到“time”的索引
        if (flag == 1)
            glUniform1f(vertexColorLocation, count * 1.0 / N); //从纹理1逐渐交替到纹理2
        else
            glUniform1f(vertexColorLocation, 2.0 - count * 1.0 / N);  //从纹理2逐渐交替到纹理1

        unsigned int TransformLoc = glGetUniformLocation(OurShader.Program, "transform");
        glUniformMatrix4fv(TransformLoc, 1, GL_FALSE, glm::value_ptr(Transform));

        glBindVertexArray(VAO);  //绑定VAO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);  //绑定EBO
        //glDrawArrays(GL_TRIANGLES, 0, 6);  //绘制三角形  
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);  //绘制正方形
        //解绑定
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        glfwSwapBuffers(Window);  //打开双缓存模式
    }

    //删除
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glDeleteTextures(1, &Texture1);
    glDeleteTextures(1, &Texture2);

    glfwTerminate();  //结束
    return 0;
}
