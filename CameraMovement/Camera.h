#pragma once
#include <iostream>
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;

//����ĸ����ǡ�ˮƽ�ǡ��ӽ��Լ��ٶ�
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat ZOOM = 45.0f;
const GLfloat SPEED = 6.0f;
const GLfloat SENSITIVITY = 0.1f;

//����ƶ�����
enum CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

//Camera��
class Camera
{
	private:
		glm::vec3 position;
		glm::vec3 front;  //��ǰ
		glm::vec3 up;  //����
		glm::vec3 right;  //����
		glm::vec3 worldup;
		GLfloat yaw;
		GLfloat pitch;
		GLfloat zoom;
		GLfloat MovementSpeed;
		GLfloat MouseSencitivity;
		void UpdateCameraVectors()
		{
			glm::vec3 front;
			//��ǰ�������Ǹ��ݸ����Ǻ�ˮƽ����������
			front.x = cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
			front.y = sin(glm::radians(this->pitch));
			front.z = cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));
			this->front = glm::normalize(front);
			this->right = glm::normalize(glm::cross(this->front, this->worldup));
			this->up = glm::normalize(glm::cross(this->right, this->front));  //��ǰ�����ҵ�����������õ����ϵ�����
		}
	public:
		//���캯��
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
			   glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
			   glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f),
			   GLfloat yaw = YAW, GLfloat pitch = PITCH, GLfloat zoom = ZOOM, 
			   GLfloat speed = SPEED, GLfloat mouse_sencitivity = SENSITIVITY)
		{
			this->position = position;
			this->worldup = up;
			this->yaw = yaw;
			this->pitch = pitch;
			this->zoom = zoom;
			this->MovementSpeed = speed;
			this->MouseSencitivity = mouse_sencitivity;
			this->UpdateCameraVectors();
		}

		//��ȡ����
		glm::mat4 GetViewMatrix()
		{
			return glm::lookAt(this->position, this->position + this->front, this->up);
		}

		//��ȡ�ӽ�
		GLfloat GetZoom()
		{
			return this->zoom;
		}

		//����ƶ�����
		void ProcessKeyboard(CameraMovement direction, GLfloat DeltaTime)
		{
			GLfloat Velocity = this->MovementSpeed * DeltaTime;
			//�����ƶ����
			if (direction == FORWARD)
			{
				this->position += this->front * Velocity;
			}
			if (direction == BACKWARD)
			{
				this->position -= this->front * Velocity;
			}
			if (direction == LEFT)
			{
				this->position -= this->right * Velocity;
			}
			if (direction == RIGHT)
			{
				this->position += this->right * Velocity;
			}
		}

		//�������ӽ�
		void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset)
		{
			xOffset *= this->MouseSencitivity;
			yOffset *= this->MouseSencitivity;
			this->yaw += xOffset;
			this->pitch += yOffset;
			this->UpdateCameraVectors();
		}
};