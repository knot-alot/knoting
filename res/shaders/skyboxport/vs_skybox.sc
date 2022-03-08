$input a_position, a_normal, a_tangent, a_texcoord0
$output v_wpos, v_view, v_normal, v_tangent, v_bitangent, v_texcoord0

/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */
#include <bgfx_shader.sh>
#include <common.sh>

mat3 mtx3FromCols(vec3 c0, vec3 c1, vec3 c2)
{
#ifdef BGFX_SHADER_LANGUAGE_GLSL
	return mat3(c0, c1, c2);
#else
	return transpose(mat3(c0, c1, c2));
#endif
}

void main()
{
	vec3 pos = a_position;
	mat4 invproj = u_invProj;
	pos.xy *= (invproj[0][0], invproj[1][1]);
	pos.z = -1.0;

	vec3 viewMatR0 = vec3(u_view[0][0],u_view[0][1],u_view[0][2]);
	vec3 viewMatR1 = vec3(u_view[1][0],u_view[1][1],u_view[1][2]);
	vec3 viewMatR2 = vec3(u_view[2][0],u_view[2][1],u_view[2][2]);

	//mat3 view = mat3(viewMatR0,viewMatR1,viewMatR2);
	mat3 transposeView = mtx3FromCols(viewMatR0,viewMatR1,viewMatR2);
	vec3 viewDir = mul(transposeView , normalize(pos));
	v_view = viewDir;
	
	gl_Position = vec4(a_position.xyz, 1.0);

}
