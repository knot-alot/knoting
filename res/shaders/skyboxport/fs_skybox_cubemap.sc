$input v_wpos, v_view, v_normal, v_tangent, v_bitangent, v_texcoord0// in...

uniform vec4 m_skyboxData;
#define u_exposure  m_skyboxData.x
#define u_lodAmount m_skyboxData.y

#include <bgfx_shader.sh>
#include <common.sh>

SAMPLER2D(s_skyboxAlbedo,    0);
SAMPLER2D(s_skyboxIrraiance, 1);
SAMPLER2D(s_skyboxRadiance,  2);

void main(){
    vec3 envColor
	
	if(u_lodAmount == 0.0){
		envColor = toLinear(texture2D(s_skyboxAlbedo, v_view.xy));
	}
	else if(u_lodAmount == 1.0){
		envColor = toLinear(texture2D(s_skyboxIrraiance, v_view.xy));	
	}
	else{
		envColor = toLinear(texture2D(s_skyboxRadiance,v_view.xy));
	}

    envColor = envColor / (envColor + vec3(1.0,1.0, 1.0));
    envColor = pow(envColor, vec3(1.0/2.2,1.0/2.2,1.0/2.2)); 

    gl_FragColor = vec4(envColor, 1.0);
}