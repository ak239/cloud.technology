#pragma once

#include "gl.h"

class TransformMatrix
{
public:
	TransformMatrix(void) : 
		scale(1.0f, 1.0f, 1.0f),
		worldPos(0.0f, 0.0f, 0.0f),
		rotate(0.0f, 0.0f, 0.0f)
	{}

	void setScale(const glm::vec3& _scale){
		scale = _scale;
	}

	void setWorldPos(const glm::vec3& _worldPos){
		worldPos = _worldPos;
	}

	void setRotate(const glm::vec3& _rotate){
		rotate = _rotate;
	}

	void setProjection(const glm::mat4& _projection){
		projection = _projection;
	}

	void setCamera(const glm::mat4& _camera){
		camera = _camera;
	}

	const glm::mat4& getWVP() const{
		WVP = glm::translate(glm::mat4(), worldPos);

		WVP = glm::rotate(WVP, rotate.x, glm::vec3(1.0f, 0.0f, 0.0f));
		WVP = glm::rotate(WVP, rotate.y, glm::vec3(0.0f, 1.0f, 0.0f));
		WVP = glm::rotate(WVP, rotate.z, glm::vec3(0.0f, 0.0f, 1.0f));

		WVP = glm::scale(WVP, scale);
		return WVP;		
	}

	const glm::mat4& getTransform() const{
		transform = projection * camera * getWVP();
		return transform;
	}

private:
	glm::vec3 scale;
	glm::vec3 worldPos;
	glm::vec3 rotate;

	glm::mat4 projection;
	glm::mat4 camera;

	mutable glm::mat4 transform;
	mutable glm::mat4 WVP;
};

