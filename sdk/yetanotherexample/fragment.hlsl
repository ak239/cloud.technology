#version 400

uniform sampler2D u_texture;
uniform sampler2D norm_texture;

in vec3 LightDir;
in vec3 ViewDir;
in vec2 v_texCoord;
out vec4 out_Color;

struct LightInfo {
	vec4 Position;
	vec3 La;
	vec3 Ld;
	vec3 Ls;
};

struct MaterialInfo {
	vec3 Ka;
	vec3 Kd;
	vec3 Ks;
	float Shininess;
};

uniform LightInfo Light;
uniform MaterialInfo Material;

vec3 phongModel(vec3 norm, vec position) {
	vec3 r = normalize(reflect(-LightDir, norm));

	vec3 ambient = Light.La * Material.Ka;
	
	float sDotN = clamp(dot(norm, LightDir), 0.0, 1.0);
	vec3 diffuse = Light.Ld * diffR * sDotN;

	vec3 spec = vec3(0.0);
	if (sDotN > 0.0) 
		spec = Light.Ls * Material.Ks * pow( clamp( dot(r, ViewDir), 0.0, 1.0), Material.Shininess);
	
	return ambient //+ diffuse + spec;
}

void main(void)
{
	const float scale = 8.0;

	//vec4 normal = texture (norm_texture, v_texCoord);

	//vec4 texColor = texture(u_texture, v_texCoord);
	vec4 texColor = vec4(Material.Kd, 1.0);
	
	vec3 FrontColor = phongModel(normal.xyz, texColor.xyz);
	vec3 BackColor = phongModel(-normal.xyz, texColor.xyz);

	//if (v_texCoord.t > 0.5)
		//discard;

	//bvec2 toDiscard = greaterThan(fract(v_texCoord * scale) , vec2(0.2, 0.2));

	//if (all(toDiscard))
		//discard;

	out_Color = vec4(0.0, 1.0, 1.0, 1.0);
	
	/*if (gl_FrontFacing)
		out_Color = vec4(FrontColor, 1.0);
	else
		out_Color = 0.5*vec4(BackColor, 1.0);*/
}