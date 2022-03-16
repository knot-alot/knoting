$input v_wpos, v_view, v_normal, v_tangent, v_bitangent, v_texcoord0// in...

uniform vec4 m_skyboxData;
#define u_exposure  m_skyboxData.x
#define u_lodAmount m_skyboxData.y

#include <bgfx_shader.sh>
#include <common.sh>

SAMPLER2D(s_skyboxAlbedo,  	 0);
SAMPLER2D(s_skyboxIrraiance, 1);
SAMPLER2D(s_skyboxRadiance,  2);

void main(){
//	vec4 color;
	
//	if(u_lodAmount == 0.0){
//		color = toLinear(texture2D(s_skyboxAlbedo, v_view.xy));
//	}
//	else if(u_lodAmount == 1.0){
//		color = toLinear(texture2D(s_skyboxIrraiance, v_view.xy));	
//	}
//	else{
//		color = toLinear(texture2D(s_skyboxRadiance,v_view.xy));
//	}
//
//	color *= exp2(u_exposure);
//	gl_FragColor = toFilmic(color);


	vec3 envColor = texture2D(s_skyboxAlbedo, v_view.xy).rgb;
    
    envColor = envColor / (envColor + vec3(1.0,1.0, 1.0));
    envColor = pow(envColor, vec3(1.0/2.2,1.0/2.2,1.0/2.2)); 


    gl_FragColor = vec4(envColor, 1.0);



}