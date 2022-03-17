$input v_view

uniform vec4 m_skyboxData;
#define u_exposure  m_skyboxData.x
#define u_skybox_type m_skyboxData.y

#include <bgfx_shader.sh>
#include <common.sh>

SAMPLER2D(s_skyboxAlbedo,  	 0);
SAMPLER2D(s_skyboxIrraiance, 1);
SAMPLER2D(s_skyboxRadiance,  2);

#define PI 3.14159265

vec2 equirectangular_to_view_uv(vec3 view){
    view = normalize(view);
    return vec2(atan2(view.z, view.x) + PI, acos(-view.y)) / vec2(2.0 * PI, PI);
}

vec2 flip_uv(vec2 uv, bool x, bool y){
    if(x){
        uv.x = -uv.x;
    }
    if(y){
        uv.y = -uv.y;
    }
    return uv;
}


void main(){

	vec4 color;

    vec2 uv = equirectangular_to_view_uv(v_view);
    
    uv = flip_uv(uv, false, false);
    
    if(u_skybox_type == 0.0){
        color = toLinear(texture2D(s_skyboxAlbedo, uv));   
    }
    else if(u_skybox_type == 1.0){
        color = toLinear(texture2D(s_skyboxIrraiance, uv));   
    }
    else if(u_skybox_type == 2.0){
        color = toLinear(texture2D(s_skyboxRadiance, uv));   
    }

	color *= exp2(u_exposure);

	gl_FragColor = toFilmic(color);
}