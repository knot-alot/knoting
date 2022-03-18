$input a_position, a_normal, a_tangent, a_texcoord0

$output v_texcoord0,v_fragPos,v_normal,v_view,v_tangent,v_bitangent,v_fragPosLightSpace

#include <bgfx_shader.sh>
#include <common.sh>

uniform mat4 lightSpaceMatrix;
uniform vec4 textureScale;

mat3 mtx3FromCols(vec3 c0, vec3 c1, vec3 c2)
{
#ifdef BGFX_SHADER_LANGUAGE_GLSL
	return mat3(c0, c1, c2);
#else
	return transpose(mat3(c0, c1, c2));
#endif
}

void main() {

    vec2 texScale = textureScale.xy;
    texScale.x = 1.0;
    texScale.y = 1.0;

    v_texcoord0 = vec2(a_texcoord0.x * texScale.x, a_texcoord0.y * texScale.y);

    v_fragPos = mul(u_model[0], vec4(a_position, 1.0)).xyz;

    vec4 normal = a_normal * 2.0 - 1.0;
	vec4 tangent = a_tangent * 2.0 - 1.0;

	vec3 wnormal = mul(u_model[0], vec4(normal.xyz, 0.0) ).xyz;
	vec3 wtangent = mul(u_model[0], vec4(tangent.xyz, 0.0) ).xyz;

	v_normal = normalize(wnormal);
	v_tangent = normalize(wtangent);

    v_bitangent = cross(v_normal, v_tangent) * a_tangent.w;

    v_fragPosLightSpace = mul(lightSpaceMatrix, vec4(v_fragPos, 1.0)).xyz;

	gl_Position = mul(u_viewProj, vec4(v_fragPos, 1.0) );

	mat3 tbn = mtx3FromCols(v_tangent, v_bitangent, v_normal);

    // eye position in world space
	vec3 weyepos = mul(vec4(0.0, 0.0, 0.0, 1.0), u_view).xyz;	
	// tangent space view dir
	v_view = mul(weyepos - v_fragPos, tbn);	
}
