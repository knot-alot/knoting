$input v_wpos, v_view, v_normal, v_tangent, v_bitangent, v_texcoord0// in...

uniform vec4 m_skyboxData;
#define u_exposure  m_skyboxData.x
#define u_lodAmount m_skyboxData.y

#include <bgfx_shader.sh>
#include <common.sh>

SAMPLERCUBE(s_skyboxAlbedo,  	 0);
SAMPLERCUBE(s_skyboxIrraiance, 1);
SAMPLERCUBE(s_skyboxRadiance,  2);

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan2(v.z, v.x), asin(v.y));
    uv *= invAtan;
    //uv -= 0.5;
    return uv;
}


void main(){
	vec4 color;
    vec2 uv = SampleSphericalMap(normalize(v_wpos));
    vec3 dir = normalize(v_view);

	color = vec4(v_view,1);
	

	color *= exp2(u_exposure);
	gl_FragColor = toFilmic(color);




}