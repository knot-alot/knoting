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
	vec3 pos = a_position ;
	mat4 invproj = u_invProj;
    pos.xy *= vec2 (invproj[0][0] , invproj [1][1]);
    pos.z = -1.0f ;
	pos = normalize(pos);

	mat4 transposeView = transpose(u_view);
	vec4 tvpos = mul(transposeView, vec4(pos, 1.0));
	tvpos.y = -tvpos.y;
    v_view = tvpos.xyz;

    gl_Position = vec4 ( a_position.xyz , 1.0);

}
