#version 330

in vec4 Color;
in vec2 Tex;

out vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
	FragColor = texture2D(gSampler, Tex) * Color;
	//FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}