#pragma once

struct PersProjInfo
{
	PersProjInfo(float _FOV, float _width, float _height, float _zNear, float _zFar):
		FOV(_FOV), Width(_width), Height(_height), zNear(_zNear), zFar(_zFar)
	{}

	PersProjInfo(){}

	float FOV;
	float Width; 
	float Height;
	float zNear;
	float zFar;
};