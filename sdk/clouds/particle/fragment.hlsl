#version 330

in vec4 outColor;
in vec2 outTex;

out vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
	FragColor = texture2D(gSampler, outTex) * outColor;
}