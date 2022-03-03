$input v_wpos, v_view, v_normal, v_tangent, v_bitangent, v_texcoord0// in...


#include <bgfx_shader.sh>
#include <common.sh>

SAMPLER2D(s_texColor,  0);
SAMPLER2D(s_texNormal, 1);

void main(){
	vec4 color = texture2D(s_texColor, v_view.xy);
	
	gl_FragColor = color;
}
