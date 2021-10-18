#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
using namespace std;

//Shader类
class Shader
{
private:
	GLuint Vertex, Fragment;
public:
	GLuint Program;
	//构造和析构函数
	Shader(const GLchar* VertexPath, const GLchar* FragmentPath)
	{
		//获得着色器源码
		string VertexCode, FragmentCode;  //字符串
		ifstream VertexShaderFile, FragmentShaderFile;  //文件流
		//异常处理
		VertexShaderFile.exceptions(ifstream::badbit);
		FragmentShaderFile.exceptions(ifstream::badbit);
		try
		{
			//打开文件
			VertexShaderFile.open(VertexPath);
			FragmentShaderFile.open(FragmentPath);
			stringstream VertexShaderStream, FragmentShaderStream;
			VertexShaderStream << VertexShaderFile.rdbuf();
			FragmentShaderStream << FragmentShaderFile.rdbuf();
			//关闭文件
			VertexShaderFile.close();
			FragmentShaderFile.close();
			//转换成字符串形式
			VertexCode = VertexShaderStream.str();
			FragmentCode = FragmentShaderStream.str();
		}
		catch (ifstream::failure e)
		{
			//报错
			cout << "ERROR::SHADER::FILE_NOT_SUCCESSUILLY_READ\n" << endl;
		}
		const GLchar* VertexShaderCode = VertexCode.c_str();
		const GLchar* FragmentShaderCode = FragmentCode.c_str();

		/*编译着色器*/
		GLint SuccessCompile;  //编译状态
		GLchar InfoLogCompile[512];  //编译信息
		//导入顶点着色器并编译
		Vertex = glCreateShader(GL_VERTEX_SHADER);  //创建顶点着色器的对象
		glShaderSource(Vertex, 1, &VertexShaderCode, NULL);  //将VertexCode的代码传入顶点着色器
		glCompileShader(Vertex);  //编译顶点着色器,生成目标代码
		//检查编译是否出错
		glGetShaderiv(Vertex, GL_COMPILE_STATUS, &SuccessCompile);  //获取编译状态：1或0
		//若出错，则显示错误信息
		if (!SuccessCompile)
		{
			glGetShaderInfoLog(Vertex, 512, NULL, InfoLogCompile);
			cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << InfoLogCompile << endl;
		}

		//导入片元着色器并编译，步骤同上
		Fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(Fragment, 1, &FragmentShaderCode, NULL);
		glCompileShader(Fragment);
		//检查编译是否出错
		glGetShaderiv(Fragment, GL_COMPILE_STATUS, &SuccessCompile);
		//若出错，则显示错误信息
		if (!SuccessCompile)
		{
			glGetShaderInfoLog(Fragment, 512, NULL, InfoLogCompile);
			cout << "ERROR::SHADER::FRAGMENTSHADER::COMPILATION_FAILED\n" << InfoLogCompile << endl;
		}

		this->Program = glCreateProgram();
		glAttachShader(this->Program, Vertex);
		glAttachShader(this->Program, Fragment);
		glLinkProgram(this->Program);
		//检查链接是否成功
		GLint SuccessLink;  //链接状态
		GLchar InfoLogLink[512];  //链接信息
		glGetProgramiv(this->Program, GL_LINK_STATUS, &SuccessLink);  //获取链接状态：1或0
		//若不成功，则显示错误信息
		if (!SuccessLink)
		{
			glGetShaderInfoLog(this->Program, 512, NULL, InfoLogLink);
			cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << InfoLogLink << endl;
		}
		glDeleteShader(Vertex);
		glDeleteShader(Fragment);
	}
	~Shader()
	{
		glDetachShader(this->Program, Vertex);
		glDetachShader(this->Program, Fragment);
		glDeleteShader(Vertex);
		glDeleteShader(Fragment);
		glDeleteProgram(this->Program);
	}
	void Use()
	{
		glUseProgram(this->Program);
	}
};
