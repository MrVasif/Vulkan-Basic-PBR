#pragma once
#include "Essentials.h"
typedef glm::vec3 Vector3;
typedef glm::mat4 Matrix4;

class Camera
{
private:
	float fieldOfView = 0.0f;
	float zNear = 0.0f, zFar = 0.0f;
public:
	enum CameraType { LookAt,FirstPerson };
	CameraType cameraType = CameraType::FirstPerson;
	Vector3 rotation = Vector3();
	Vector3 position = Vector3();
	float rotationSpeed = 1.0f;
	float movementSpeed = 1.0f;
	bool updated = false;
	bool flipY = false;

	struct
	{
		Matrix4 perspective;
		Matrix4 view;
	} Matrices;

	struct
	{
		bool left = false;
		bool right = false;
		bool up = false;
		bool down = false;
	} Keys;
private:

	void UpdateViewMatrix()
	{
		Matrix4 rotationMatrix = Matrix4();
		Matrix4 translationMatrix = Matrix4();

		rotationMatrix = glm::rotate(rotationMatrix,rotation.x * (flipY ? -1.0f : 1.0f), Vector3(1.0f, 0.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.y), Vector3(0.0f, 1.0f, 0.0f));
		rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation.z), Vector3(0.0f, 0.0f, 1.0f));
	
		Vector3 translation = position;

		if (flipY)
			translation.y *= -1;

		translationMatrix = glm::translate(Matrix4(1.0f), translation);

		switch (cameraType)
		{
			case CameraType::LookAt:
				Matrices.view = translationMatrix * rotationMatrix;
			case CameraType::FirstPerson:
				Matrices.view = rotationMatrix * translationMatrix;
		}
	
		updated = true;
	}
public:
	bool IsMoving()
	{
		return Keys.left || Keys.right || Keys.up || Keys.down;
	}

	float GetNearClip()
	{
		return zNear;
	}

	float GetFarClip()
	{
		return zFar;
	}

	void SetPerspective(float fieldOfView, float aspect, float zNear, float zFar)
	{
		this->fieldOfView = fieldOfView;
		this->zNear = zNear;
		this->zFar = zFar;
		Matrices.perspective = glm::perspective(glm::radians(fieldOfView), aspect, zNear, zFar);

		if (flipY)
			Matrices.perspective[1, 1] *= -1.0f;
	}

	void UpdateAspectRatio(float aspect)
	{
		Matrices.perspective = glm::perspective(glm::radians(fieldOfView), aspect, zNear, zFar);

		if (flipY)
			Matrices.perspective[1, 1] *= -1.0f;
	}

	void SetPosition(Vector3 position)
	{
		this->position = position;
		UpdateViewMatrix();
	}

	void SetRotation(Vector3 rotation)
	{
		this->rotation = rotation;
		UpdateViewMatrix();
	}

	void Rotate(Vector3 delta)
	{
		this->rotation += delta;
		UpdateViewMatrix();
	}

	void Translate(Vector3 delta)
	{
		this->position += delta;
		UpdateViewMatrix();
	}

	Matrix4 GetViewMatrix()
	{
		return this->Matrices.view;
	}

	void Update(float deltaTime)
	{
		updated = false;
		if (cameraType == CameraType::FirstPerson)
		{
			if (IsMoving())
			{
				Vector3 cameraFront;
				cameraFront.x = -cos(glm::radians(rotation.x)) * sin(glm::radians(rotation.y));
				cameraFront.y = sin(glm::radians(rotation.x));
				cameraFront.z = cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
				cameraFront = glm::normalize(cameraFront);

				float moveSpeed = deltaTime * movementSpeed;

				if (Keys.up)
					position += cameraFront * movementSpeed;
				if(Keys.down)
					position -= cameraFront * movementSpeed;
				if (Keys.left)
					position -= glm::normalize(glm::cross(cameraFront, Vector3(0.0f, 1.0f, 0.0f))) * movementSpeed;
				if(Keys.right)
					position += glm::normalize(glm::cross(cameraFront, Vector3(0.0f, 1.0f, 0.0f))) * movementSpeed;
			
				UpdateViewMatrix();
			}
		}
	}

	void InputHandler(GLFWwindow* window)
	{
		if (cameraType != CameraType::FirstPerson)
			return;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			this->Keys.up = true;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			this->Keys.down = true;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			this->Keys.left = true;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			this->Keys.right = true;
		
	}
};