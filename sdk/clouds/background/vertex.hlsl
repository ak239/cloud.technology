#version 330

layout (location = 0) in vec3 Position;

uniform mat4 gWVP;

out vec3 TexCoord0;

void main()
{
	vec4 WVP_Pos = gWVP * vec4(Position, 1.0);
	gl_Position = WVP_Pos.xyww;
	//TexCoord0 = 0.5 * (Position + vec3(1.0, 1.0, 1.0));
	TexCoord0 = Position;
}