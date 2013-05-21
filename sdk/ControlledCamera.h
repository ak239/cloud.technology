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

		m_mousePos.x  = width / 2;
		m_mousePos.y  = height / 2;

		glutWarpPointer(m_mousePos.x, m_mousePos.y);
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
					glutWarpPointer(m_mousePos.x, m_mousePos.y);
					m_IsDragging = true;
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
			move(x, y);
		}
	}

	void mousePassiveMotionFunc(int x, int y){
		if (m_IsPassiveMotion)
		{
			move(x, y);
		}
	}

	void move(int x, int y) {
		if (( x == m_mousePos.x)&&(y == m_mousePos.y)) return;

		int DeltaX = x - m_mousePos.x;
		int DeltaY = y - m_mousePos.y;

		setHorizontalAngle(getHorizontalAngle() - (float)DeltaX / 60.0f);
		setVerticalAngle(getVerticalAngle() - (float)DeltaY / 60.0f);

		glutWarpPointer(m_mousePos.x, m_mousePos.y);
	}

	void setMode(bool isPassiveMotion) {
		m_IsPassiveMotion = isPassiveMotion;
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

