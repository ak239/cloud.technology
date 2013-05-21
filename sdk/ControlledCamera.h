#pragma once

#include "gl.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

class ControlledCamera : public Camera
{
public:
	ControlledCamera(GLint _width, GLint _height) : 
		width(_width), height(_height){
		
		m_mousePos.x  = width / 2;
		m_mousePos.y  = height / 2;

		glutWarpPointer(m_mousePos.x, m_mousePos.y);
		m_Speed = 0.1f;
		m_IsDragging = false;
		m_IsPassiveMotion = false;
	}

	void reshapeFunc(int _width, int _height){
		width  = _width;
		height = _height;

	//	if (!m_IsPassiveMotion)
		{
			m_mousePos.x  = width / 2;
			m_mousePos.y  = height / 2;

			glutWarpPointer(m_mousePos.x, m_mousePos.y);
		}
	}
	
	void keyboardFunc(unsigned char key, int x, int y){
		switch(key){
		case GLUT_KEY_UP:
			moveCamera(getTarget() * (glm::vec3(m_Speed)));
			break;
		case GLUT_KEY_DOWN:
			moveCamera(getTarget() * (-glm::vec3(m_Speed)));
			break;
		case GLUT_KEY_LEFT:
			{
				glm::vec3 left = glm::normalize(glm::cross(getTarget(), getUp()));
				moveCamera(left * (-glm::vec3(m_Speed)));
				break;
			}
		case GLUT_KEY_RIGHT:
			{
				glm::vec3 right = glm::normalize(glm::cross(getTarget(), getUp()));
				moveCamera(right * (glm::vec3(m_Speed)));
				break;
			}
		}	
	}

	void setSpeed(float speed) {
		m_Speed = speed;
	}

	void mouseClickFunc(int button, int state, int x, int y) {
		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				if (!m_IsPassiveMotion)
				{
					m_mousePos = glm::ivec2(x, y);
					m_IsDragging = true;
				}
				else {
					m_mousePos = glm::ivec2(width /2, height/ 2);
					glutWarpPointer(m_mousePos.x, m_mousePos.y);
				}
				
			}
			else if (state == GLUT_UP)
			{
				m_IsDragging = false;
			}

		}
	}

	void mouseMotionFunc(int x, int y){
		if (!m_IsPassiveMotion && m_IsDragging)
		{
			if (( x == m_mousePos.x)&&(y == m_mousePos.y)) return;
			move(x, y);
			m_mousePos = glm::ivec2(x, y);
		}
	}

	void mousePassiveMotionFunc(int x, int y){
		if (m_IsPassiveMotion)
		{
			if (( x == m_mousePos.x)&&(y == m_mousePos.y)) return;
			move(x, y);
			glutWarpPointer(m_mousePos.x, m_mousePos.y);
		}
	}

	void move(int x, int y) {
		
		int DeltaX = x - m_mousePos.x;
		int DeltaY = y - m_mousePos.y;

		setHorizontalAngle(getHorizontalAngle() - (float)DeltaX / 60.0f);
		setVerticalAngle(getVerticalAngle() - (float)DeltaY / 60.0f);
	}

	void setMode(bool isPassiveMotion) {
		m_IsPassiveMotion = isPassiveMotion;
		if (m_IsPassiveMotion)
		{
			m_mousePos.x  = width / 2;
			m_mousePos.y  = height / 2;

			glutWarpPointer(m_mousePos.x, m_mousePos.y);
		}
		m_IsDragging = false;
	}
	
	void idleFunc(){
	}

private:
	GLint width;
	GLint height;

	float m_Speed;
	bool m_IsDragging;
	bool m_IsPassiveMotion;

	glm::ivec2 m_mousePos;
};

