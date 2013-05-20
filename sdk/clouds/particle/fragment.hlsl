#version 330

in vec4 outColor;
in vec2 outTex;

out vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
	//FragColor = outColor;
	vec4 tex = texture2D(gSampler, outTex);
	//FragColor = vec4(tex.xyz, 1.0) * outColor;
	FragColor = tex * outColor;
	//FragColor = vec4(outTex, 1.0, 0.0);
}