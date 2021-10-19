#pragma once
#include <iostream>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;

//俯仰角、水平角和视角
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat ZOOM = 45.0f;

//Camera类
class Camera
{
	private:
		glm::vec3 position;
		glm::vec3 front;  //向前
		glm::vec3 up;  //向上
		glm::vec3 right;  //向右
		glm::vec3 worldup;
		GLfloat yaw;
		GLfloat pitch;
		GLfloat zoom;
		void UpdateCameraVectors()
		{
			glm::vec3 front;
			//向前的向量是根据俯仰角和水平角来决定的
			front.x = cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
			front.y = sin(glm::radians(this->pitch));
			front.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));
			this->front = glm::normalize(front);
			this->right = glm::normalize(glm::cross(this->front, this->worldup));
			this->up = glm::normalize(glm::cross(this->right, this->front));  //向前和向右的向量做叉积得到向上的向量
		}
	public:
		//构造函数
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
			   GLfloat yaw = YAW, GLfloat pitch = PITCH): front(glm::vec3(0.0f, 0.0f, -1.0f)), zoom(ZOOM)
		{
			this->position = position;
			this->worldup = up;
			this->yaw = yaw;
			this->pitch = pitch;
			this->UpdateCameraVectors();
		}
		//获取矩阵
		glm::mat4 GetViewMatrix()
		{
			return glm::lookAt(this->position, this->position + this->front, this->up);
		}
		//获取视角
		GLfloat GetZoom()
		{
			return this->zoom;
		}
};
