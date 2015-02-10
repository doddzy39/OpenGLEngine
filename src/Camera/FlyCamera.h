#ifndef _FLY_CAMERA_H_
#define _FLY_CAMERA_H_

#include "Engine/BaseCamera.h"
#include <GLFW\glfw3.h>

class FlyCamera : public BaseCamera
{
public:
	FlyCamera() : FlyCamera(10.0f, 5.0f)
	{
		m_bViewButtonClicked = false;
	}
	FlyCamera(float flySpeed, float rotSpeed) : BaseCamera(), m_fFlySpeed(flySpeed), m_fRotationSpeed(rotSpeed)
	{
		
	}
	~FlyCamera() {};

	virtual void Update(double dt);

	void SetFlySpeed(float fSpeed);
	float GetFlySpeed() const { return m_fFlySpeed;  }

	float GetRotationSpeed() const { return m_fRotationSpeed; }
	void SetRotationSpeed(float val) { m_fRotationSpeed = val; }

	//TEMP - Pass the window to be used to query the keyboard
	//This will get changed to an Input Manager later
	void SetInputWindow(GLFWwindow* pWindow) { m_pWindow = pWindow; }


protected:
	void HandleKeyboardInput(double dt);
	void HandleMouseInput(double dt);

	void CalculateRotation(double dt, double xOffset, double yOffset);

	GLFWwindow* m_pWindow;
	float m_fFlySpeed;
	float m_fRotationSpeed;
	bool m_bViewButtonClicked;
	double m_dCursorX, m_dCursorY;

};

#endif