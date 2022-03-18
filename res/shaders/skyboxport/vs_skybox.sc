$input a_position
$output v_view

#include <bgfx_shader.sh>
#include <common.sh>

void main()
{
	vec3 pos = a_position ;

	mat4 invproj = u_invProj;
    pos.xy *= vec2 (invproj[0][0] , invproj [1][1]);

    pos.z = -1.0f ;
	pos = normalize(pos);

	mat4 transposeView = transpose(u_view);
	vec4 transposeViewPos = mul(transposeView, vec4(pos, 1.0));
    v_view = transposeViewPos.xyz;

    gl_Position = vec4 ( a_position.xyz , 1.0);

}
