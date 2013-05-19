#version 330

in vec4 Color;
in vec3 Tex;

out vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
	FragColor = texture2D(gSampler, Tex) * Color;
}