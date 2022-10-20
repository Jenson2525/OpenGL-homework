#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
using namespace std;

//Shader��
class Shader
{
private:
	GLuint Vertex, Fragment;
public:
	GLuint Program;
	//�������������
	Shader(const GLchar* VertexPath, const GLchar* FragmentPath)
	{
		//�����ɫ��Դ��
		string VertexCode, FragmentCode;  //�ַ���
		ifstream VertexShaderFile, FragmentShaderFile;  //�ļ���
		//�쳣����
		VertexShaderFile.exceptions(ifstream::badbit);
		FragmentShaderFile.exceptions(ifstream::badbit);
		try
		{
			//���ļ�
			VertexShaderFile.open(VertexPath);
			FragmentShaderFile.open(FragmentPath);
			stringstream VertexShaderStream, FragmentShaderStream;
			VertexShaderStream << VertexShaderFile.rdbuf();
			FragmentShaderStream << FragmentShaderFile.rdbuf();
			//�ر��ļ�
			VertexShaderFile.close();
			FragmentShaderFile.close();
			//ת�����ַ�����ʽ
			VertexCode = VertexShaderStream.str();
			FragmentCode = FragmentShaderStream.str();
		}
		catch (ifstream::failure e)
		{
			//����
			cout << "ERROR::SHADER::FILE_NOT_SUCCESSUILLY_READ\n" << endl;
		}
		const GLchar* VertexShaderCode = VertexCode.c_str();
		const GLchar* FragmentShaderCode = FragmentCode.c_str();

		/*������ɫ��*/
		GLint SuccessCompile;  //����״̬
		GLchar InfoLogCompile[512];  //������Ϣ
		//���붥����ɫ��������
		Vertex = glCreateShader(GL_VERTEX_SHADER);  //����������ɫ���Ķ���
		glShaderSource(Vertex, 1, &VertexShaderCode, NULL);  //��VertexCode�Ĵ��봫�붥����ɫ��
		glCompileShader(Vertex);  //���붥����ɫ��,����Ŀ�����
		//�������Ƿ����
		glGetShaderiv(Vertex, GL_COMPILE_STATUS, &SuccessCompile);  //��ȡ����״̬��1��0
		//����������ʾ������Ϣ
		if (!SuccessCompile)
		{
			glGetShaderInfoLog(Vertex, 512, NULL, InfoLogCompile);
			cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << InfoLogCompile << endl;
		}

		//����ƬԪ��ɫ�������룬����ͬ��
		Fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(Fragment, 1, &FragmentShaderCode, NULL);
		glCompileShader(Fragment);
		//�������Ƿ����
		glGetShaderiv(Fragment, GL_COMPILE_STATUS, &SuccessCompile);
		//����������ʾ������Ϣ
		if (!SuccessCompile)
		{
			glGetShaderInfoLog(Fragment, 512, NULL, InfoLogCompile);
			cout << "ERROR::SHADER::FRAGMENTSHADER::COMPILATION_FAILED\n" << InfoLogCompile << endl;
		}

		this->Program = glCreateProgram();
		glAttachShader(this->Program, Vertex);
		glAttachShader(this->Program, Fragment);
		glLinkProgram(this->Program);
		//��������Ƿ�ɹ�
		GLint SuccessLink;  //����״̬
		GLchar InfoLogLink[512];  //������Ϣ
		glGetProgramiv(this->Program, GL_LINK_STATUS, &SuccessLink);  //��ȡ����״̬��1��0
		//�����ɹ�������ʾ������Ϣ
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