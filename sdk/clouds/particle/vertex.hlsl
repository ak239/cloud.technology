#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 Offset;
layout (location = 2) in vec2 Tex;
layout (location = 3) in vec4 Color;


uniform mat4 gWVP;
uniform vec4 rightNormal;
uniform vec4 upNormal;

out vec4 outColor;
out vec2 outTex;

void main()
{
	vec4 outPosition = vec4(Position.xyz, 1.0) + Offset.x*rightNormal + Offset.y*upNormal;
	outPosition.w = 1.0;
	outPosition = outPosition * gWVP;
	outColor = Color;
	outTex = Tex;

	gl_Position = outPosition;
}