#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Camera
{
public:
	Camera():verticalAngle(0.0f), horizontalAngle(-glm::pi<float>()){
		cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		target = glm::vec3(0.0f, 0.0f, 1.0f);
		init();
	}

	virtual ~Camera(){}

	void init() {
		glm::vec3 HTarget = glm::normalize(glm::vec3(target.x, 0.0, target.z));

		if (HTarget.z >= 0.0f)
		{
			if (HTarget.x >= 0.0f)
			{
				horizontalAngle = (GLfloat)(2*glm::pi<GLfloat>() - asin(HTarget.z));
			}
			else
			{
				horizontalAngle = (GLfloat)(glm::pi<GLfloat>() + asin(HTarget.z));
			}
		}
		else
		{
			if (HTarget.x >= 0.0f)
			{
				horizontalAngle = asin(-HTarget.z);
			}
			else
			{
				horizontalAngle = (GLfloat)(0.5*glm::pi<GLfloat>() + asin(-HTarget.z));
			}
		}

		verticalAngle = -asin(target.y);

		
	}

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
		Update();
		return glm::lookAt(getPos(), getPos()+target, cameraUp);
	}

	void Update() {
		glm::vec3 vAxies(0.0, 1.0, 0.0);
		glm::vec3 view(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle));

		glm::vec3 right = glm::vec3(sin(horizontalAngle - 0.5f * glm::pi<float>()), 
			0,
			cos(horizontalAngle - 0.5f * glm::pi<float>()));

		target = glm::normalize(view);
		cameraUp = glm::normalize(glm::cross(right, view));
		glm::vec3 up = glm::normalize(glm::cross(right, target));
	}

	GLfloat getHorizontalAngle() const{ return horizontalAngle; }
	GLfloat getVerticalAngle()   const{ return verticalAngle;   }

	void setVerticalAngle(GLfloat verAngle) {
		verticalAngle = glm::clamp(verAngle, -0.5f*glm::pi<float>(), 0.5f*glm::pi<float>());
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

