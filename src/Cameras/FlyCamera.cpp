#include "FlyCamera.h"
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <iostream>

void FlyCamera::Update(double dt)
{
	HandleKeyboardInput(dt);
	HandleMouseInput(dt);
}

void FlyCamera::HandleKeyboardInput(double dt)
{
	//Get the cameras forward/up/right
	glm::mat4 trans = GetTransform();
	glm::vec3 vRight = trans[0].xyz;
	glm::vec3 vUp = trans[1].xyz;
	glm::vec3 vForward = trans[2].xyz;
	glm::vec3 moveDir(0.0f);

	if (glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		moveDir -= vForward;
	}

	if (glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		moveDir += vForward;
	}

	if (glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		moveDir -= vRight;
	}

	if (glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		moveDir += vRight;
	}

	if (glfwGetKey(m_pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		moveDir += glm::vec3(0.0f, 1.0f, 0.0f);
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		moveDir -= glm::vec3(0.0f, 1.0f, 0.0f);
	}

	float fLength = glm::length(moveDir);
	if (fLength > 0.01f)
	{
		moveDir = ((float)dt * m_fFlySpeed) * glm::normalize(moveDir);
		SetPosition(GetPosition() + moveDir);
	}
}

void FlyCamera::HandleMouseInput(double dt)
{
	if (glfwGetMouseButton(m_pWindow, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		if (m_bViewButtonClicked == false)
		{
			int width, height;
			glfwGetFramebufferSize(m_pWindow, &width, &height);

			m_dCursorX = width / 2.0;
			m_dCursorY = height / 2.0;

			glfwSetCursorPos(m_pWindow, width / 2, height / 2);

			m_bViewButtonClicked = true;
		}
		else
		{
			double mouseX, mouseY;
			glfwGetCursorPos(m_pWindow, &mouseX, &mouseY);

			double xOffset = mouseX - m_dCursorX;
			double yOffset = mouseY - m_dCursorY;

			CalculateRotation(dt, xOffset, yOffset);

		}

		int width, height;
		glfwGetFramebufferSize(m_pWindow, &width, &height);
		glfwSetCursorPos(m_pWindow, width / 2, height / 2);
	}
	else
	{
		m_bViewButtonClicked = false;
	}
}

void FlyCamera::CalculateRotation(double dt, double xOffset, double yOffset)
{
	if (xOffset != 0.0)
	{
		glm::mat4 rot = glm::rotate((float)(m_fRotationSpeed * dt * -xOffset), glm::vec3(0, 1, 0));
		
		SetTransform(GetTransform() * rot);
	}

	if (yOffset != 0.0)
	{
		glm::mat4 rot = glm::rotate((float)(m_fRotationSpeed * dt * -yOffset), glm::vec3(1, 0, 0));

		SetTransform(GetTransform() * rot);
	}

	//Clean up rotation
	glm::mat4 oldTrans = GetTransform();
	
	glm::mat4 trans;
	
	glm::vec3 worldUp = glm::vec3(0, 1, 0);
	
	//Right
	trans[0] = glm::normalize( glm::vec4(glm::cross(worldUp, oldTrans[2].xyz()), 0));
	//Up
	trans[1] = glm::normalize(glm::vec4(glm::cross(oldTrans[2].xyz(), trans[0].xyz()), 0));
	//Forward
	trans[2] = glm::normalize(oldTrans[2]);
	
	//Position
	trans[3] = oldTrans[3];

	SetTransform(trans);
}