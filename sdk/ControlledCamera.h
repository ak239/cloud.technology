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
			moveCamera(getTarget() * (-glm::vec3(m_Speed)));
			break;
		case GLUT_KEY_DOWN:
			moveCamera(getTarget() * glm::vec3(m_Speed));
			break;
		case GLUT_KEY_LEFT:
			{
				glm::vec3 left = glm::normalize(glm::cross(getTarget(), getUp()));
				moveCamera(left * glm::vec3(m_Speed));
				break;
			}
		case GLUT_KEY_RIGHT:
			{
				glm::vec3 right = glm::normalize(glm::cross(getTarget(), getUp()));
				moveCamera(right * (-glm::vec3(m_Speed)));
				break;
			}
		}	
	}

	void setSpeed(float speed) {
		m_Speed = speed;
	}

	void mouseMotionFunc(int x, int y){
		if (( x == m_mousePos.x)&&(y == m_mousePos.y)) return;

		int DeltaX = x - m_mousePos.x;
		int DeltaY = y - m_mousePos.y;

		setHorizontalAngle(getHorizontalAngle() - (float)DeltaX / 60.0f);
		setVerticalAngle(getVerticalAngle() - (float)DeltaY / 60.0f);

		glutWarpPointer(m_mousePos.x, m_mousePos.y);
	}
	
	void idleFunc(){
	}

private:
	GLint width;
	GLint height;

	float m_Speed;

	glm::ivec2 m_mousePos;
};

