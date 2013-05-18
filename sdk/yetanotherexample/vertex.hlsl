#version 400

layout(location=0) in vec4 in_Position;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 a_texCoord;
layout(location=3) in vec4 tangent;

out vec2 v_texCoord;

out vec3 LightDir;
out vec3 ViewDir;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat3 NormalMatrix;

struct LightInfo {
	vec4 Position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
};
uniform LightInfo Light;


void main(void)
{
	vec3 tnorm = normalize(NormalMatrix * normal);
	vec3 tang = normalize(NormalMatrix * vec3(tangent));
	vec3 binormal = normalize( cross( tnorm, tang) ) * tangent.w;

	mat3 toObjectLocal = mat3(
		tang.x, binormal.x, tnorm.x,
		tang.y, binormal.y, tnorm.y,
		tang.z, binormal.z, tnorm.z
		);

	vec3 eyeCoords = vec3((ViewMatrix * ModelMatrix) * in_Position);

	LightDir = normalize(toObjectLocal * (Light.Position.xyz - eyeCoords));

	ViewDir = toObjectLocal * normalize(-eyeCoords);
	
	v_texCoord = a_texCoord;
	gl_Position = ProjectionMatrix * vec4(eyeCoords, 1.0);
}