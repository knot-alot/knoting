$input v_wpos, v_view, v_normal, v_tangent, v_bitangent, v_texcoord0// in...

uniform vec4 m_skyboxData;
#define u_exposure  m_skyboxData.x
#define u_lodAmount m_skyboxData.y

#include <bgfx_shader.sh>
#include <common.sh>

SAMPLERCUBE(s_skyboxAlbedo,  	 0);
SAMPLERCUBE(s_skyboxIrraiance, 1);
SAMPLERCUBE(s_skyboxRadiance,  2);

void main(){
	vec4 color;
    vec3 dir = normalize(v_view);

	
	if(u_lodAmount == 0.0){
		color = toLinear(textureCube(s_skyboxAlbedo, dir));
	}
	else if(u_lodAmount == 1.0){
		color = toLinear(textureCube(s_skyboxIrraiance, dir));
	}
	else{
		color = toLinear(textureCube(s_skyboxRadiance, dir));
	}

	color *= exp2(u_exposure);
	gl_FragColor = toFilmic(color);




}