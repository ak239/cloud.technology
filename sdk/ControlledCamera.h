#pragma once

#include "gl.h"
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

class ControlledCamera : public Camera
{
public:
	ControlledCamera(GLint _width, GLint _height) : 
		isLeft(false), isRight(false), isUp(false), isDown(false),
		width(_width), height(_height){}

	void reshapeFunc(int _width, int _height){
		width  = _width;
		height = _height;
	}
	
	void keyboardFunc(unsigned char key, int x, int y){
		switch(key){
		case GLUT_KEY_UP:
			moveCamera(getTarget() * glm::vec3(-0.01f, -0.01f, -0.01f));
			break;
		case GLUT_KEY_DOWN:
			moveCamera(getTarget() * glm::vec3(0.01f, 0.01f, 0.01f));
			break;
		case GLUT_KEY_LEFT:
			{
				glm::vec3 left = glm::normalize(glm::cross(getTarget(), getUp()));
				moveCamera(left * glm::vec3(0.01f, 0.01f, 0.01f));
				break;
			}
		case GLUT_KEY_RIGHT:
			{
				glm::vec3 right = glm::normalize(glm::cross(getTarget(), getUp()));
				moveCamera(right * glm::vec3(-0.01f, -0.01f, -0.01f));
				break;
			}
		}	
	}

	void mouseMotionFunc(int x, int y){
		isLeft = isRight = isUp = isDown = false; 
		if (x < 100)
			isLeft = true;
		if (x > width - 100)
			isRight = true;
		if (y < 100)
			isUp = true;
		if (y > height - 100)
			isDown = true;
	}
	
	void idleFunc(){
		if (isLeft)
			setHorizontalAngle(getHorizontalAngle() - 0.001f);
		if (isRight)
			setHorizontalAngle(getHorizontalAngle() + 0.001f);
		if (isUp)
			setVerticalAngle(getVerticalAngle() + 0.001f);
		if (isDown)
			setVerticalAngle(getVerticalAngle() - 0.001f);
	}

private:
	bool isLeft;
	bool isRight;
	bool isUp;
	bool isDown;

	GLint width;
	GLint height;
};

