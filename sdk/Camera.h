#pragma once

#include <glm/glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	Camera():verticalAngle(0.0f), horizontalAngle(0.0f){}
	virtual ~Camera(){}

	void setPos(const glm::vec3& _cameraPos){
		cameraPos = _cameraPos;
	}

	void moveCamera(const glm::vec3& delta){
		cameraPos += delta;
	}

	void changeAngles(const glm::vec2& delta){
		verticalAngle   += delta[0];
		horizontalAngle += delta[1];
	}

	const glm::vec3& getPos() const{ return cameraPos; }
	const glm::vec3& getUp() const{ return cameraUp; }

	void setUp(const glm::vec3& _cameraUp){
		cameraUp = glm::normalize(_cameraUp);
	}

	glm::mat4 cameraMat(){
		glm::vec3 vAxies(0.0f, 1.0f, 0.0f);
		glm::vec3 view(1.0f, 0.0f, 0.0f);

		const GLfloat sinHorHalfAngle = sinf(horizontalAngle / 2.0f);
		const GLfloat cosHorHalfAngle = cosf(horizontalAngle / 2.0f);
		glm::quat quat(vAxies[0] * sinHorHalfAngle,
			vAxies[1] * sinHorHalfAngle,
			vAxies[2] * sinHorHalfAngle,
			cosHorHalfAngle);

		glm::quat w = glm::rotate(quat, horizontalAngle, vAxies);
		view = glm::vec3(w.x, w.y, w.z);
		view = glm::normalize(view);

		glm::vec3 hAxis = glm::normalize(glm::cross(vAxies, view));
		//hAxis = 
		const GLfloat sinVerHalfAngle = sinf(verticalAngle / 2.0f);
		const GLfloat cosVerHalfAngle = cosf(verticalAngle / 2.0f);
		glm::quat quatVer(hAxis[0] * sinVerHalfAngle,
			hAxis[1] * sinVerHalfAngle,
			hAxis[2] * sinVerHalfAngle,
			cosVerHalfAngle);

		glm::quat wVer = glm::rotate(quatVer, verticalAngle, hAxis);
		view = glm::vec3(wVer.x, wVer.y, wVer.z);
		view = glm::normalize(view);

		target = view;
		glm::vec3 up     = glm::normalize(glm::cross(target, hAxis));

		glm::mat4 translate = glm::translate(glm::mat4(), glm::vec3(-1.0f * cameraPos[0], -1.0f * cameraPos[1], -1.0f * cameraPos[2]));
		glm::vec3 cross  = glm::cross(target, up);

		glm::mat4 rotate = glm::mat4(
			up[0],     up[1],     up[2],     0.0f,
			cross[0],  cross[1],  cross[2],  0.0f,
			target[0], target[1], target[2], 0.0f,
			0.0f,      0.0f,      0.0f,      1.0f
			);
		return translate * rotate;
	}

	GLfloat getHorizontalAngle() const{ return horizontalAngle; }
	GLfloat getVerticalAngle()   const{ return verticalAngle;   }

	void setVerticalAngle(GLfloat verAngle){
		verticalAngle = verAngle;
	}

	void setHorizontalAngle(GLfloat horAngle){
		horizontalAngle = horAngle;
	}

	const glm::vec3& getTarget() const{
		return target;
	}

private:
	glm::vec3 cameraPos;
	glm::vec3 cameraUp;
	glm::vec3 target;

	GLfloat verticalAngle;
	GLfloat horizontalAngle;
};

