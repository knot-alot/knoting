$input v_wpos, v_view, v_normal, v_tangent, v_bitangent, v_texcoord0, v_localPos// in...


#include <bgfx_shader.sh>
#include <common.sh>

SAMPLER2D(m_albedo,  0);
SAMPLER2D(m_normal, 1);
SAMPLER2D(m_mask, 2);
SAMPLER2D(m_red, 3);
SAMPLER2D(m_blue, 4);
SAMPLER2D(m_redNormal, 5);
SAMPLER2D(m_blueNormal, 6);

uniform vec4 m_paintData0[100];
uniform vec4 m_paintData1;

#define paintStart m_paintData1.x
#define paintEnd m_paintData1.y

uniform vec4 u_lightPosRadius[30];
uniform vec4 u_lightRgbInnerR[30];
uniform vec4 m_albedoColor;

vec2 blinn(vec3 _lightDir, vec3 _normal, vec3 _viewDir)
{
	float ndotl = dot(_normal, _lightDir);
	//vec3 reflected = _lightDir - 2.0*ndotl*_normal; // reflect(_lightDir, _normal);
	vec3 reflected = 2.0*ndotl*_normal - _lightDir;
	float rdotv = dot(reflected, _viewDir);
	return vec2(ndotl, rdotv);
}

float fresnel(float _ndotl, float _bias, float _pow)
{
	float facing = (1.0 - _ndotl);
	return max(_bias + (1.0 - _bias) * pow(facing, _pow), 0.0);
}

vec4 lit(float _ndotl, float _rdotv, float _m)
{
	float diff = max(0.0, _ndotl);
	float spec = step(0.0, _ndotl) * max(0.0, _rdotv * _m);
	return vec4(1.0, diff, spec, 1.0);
}

vec4 powRgba(vec4 _rgba, float _pow)
{
	vec4 result;
	result.xyz = pow(_rgba.xyz, vec3_splat(_pow) );
	result.w = _rgba.w;
	return result;
}

vec3 calcLight(int _idx, mat3 _tbn, vec3 _wpos, vec3 _normal, vec3 _view)
{
	vec3 lp = u_lightPosRadius[_idx].xyz - _wpos;
	float attn = 1.0 - smoothstep(u_lightRgbInnerR[_idx].w, 1.0, length(lp) / u_lightPosRadius[_idx].w);
	vec3 lightDir = mul( normalize(lp), _tbn );
	vec2 bln = blinn(lightDir, _normal, _view);
	vec4 lc = lit(bln.x, bln.y, 1.0);
	vec3 rgb = u_lightRgbInnerR[_idx].xyz * saturate(lc.y) * attn;
	return rgb;
}

mat3 mtx3FromCols(vec3 c0, vec3 c1, vec3 c2)
{
#if BGFX_SHADER_LANGUAGE_GLSL
	return mat3(c0, c1, c2);
#else
	return transpose(mat3(c0, c1, c2));
#endif
}



void main()
{
	mat3 tbn = mtx3FromCols(v_tangent, v_bitangent, v_normal);
	
	vec4 paintCol = vec4_splat(0.0);
	
	float radius = 2.0;
	vec3 paintPos = v_wpos.xyz;
	for (int pointi = 0; pointi < 50; ++pointi) {
			if(m_paintData0[pointi].w == 0.0){
				continue;
			}
			float radiusSq = radius * radius;
			vec3 pointPos = m_paintData0[pointi].xyz;
			vec3 relativePos = pointPos - paintPos;
			float distanceSq= relativePos.x * relativePos.x + relativePos.y* relativePos.y + relativePos.z * relativePos.z;
			if(distanceSq > radiusSq){
				continue;
			}
			if(m_paintData0[pointi].w == 1.0){
				paintCol = (vec4(1.0,0.0,0.0,1.0));
			} else{
				paintCol = (vec4(0.0,0.0,1.0,1.0));
			}
	}

	for (int pointj = 50; pointj < 100; ++pointj) {
			if(m_paintData0[pointj].w == 0.0){
				continue;
			}
			float radiusSq = radius * radius;
			vec3 pointPos = m_paintData0[pointj].xyz;
			vec3 relativePos = pointPos - paintPos;
			float distanceSq= relativePos.x * relativePos.x + relativePos.y* relativePos.y + relativePos.z * relativePos.z;
			if(distanceSq > radiusSq){
				continue;
			}
			if(m_paintData0[pointj].w == 1.0){
				paintCol = (vec4(1.0,0.0,0.0,1.0));
			} else{
				paintCol = (vec4(0.0,0.0,1.0,1.0));
			}
	}

	

	vec4 maskColor = paintCol;
	
	float mixTargetAmount = 0.1;
	float mixMinAmount = 0.8;
	float paintNormalDamping = 0.15;

	vec3 normal;
	normal.xy = texture2D(m_normal, v_texcoord0).xy * 2.0 - 1.0;
	
	if(maskColor.x >= mixTargetAmount && maskColor.y <= mixMinAmount && maskColor.z <= mixMinAmount){
		normal.xy *= paintNormalDamping;
		normal.xy += texture2D(m_redNormal, v_texcoord0).xy * 2.0 - 1.0;
	}
	else if(maskColor.x <= mixMinAmount && maskColor.y <= mixMinAmount && maskColor.z >= mixTargetAmount){
		normal.xy *= paintNormalDamping;
		normal.xy += texture2D(m_blueNormal, v_texcoord0).xy * 2.0 - 1.0;
	}
	
	normal.z = sqrt(1.0 - dot(normal.xy, normal.xy) );
	vec3 view = normalize(v_view);

	vec3 lightColor = vec3(0.0,0.0,0.0);

	for (int i = 0; i < 30; ++i) {
    	lightColor += calcLight(i, tbn, v_wpos, normal, view);
	}
	
	vec4 color = toLinear(texture2D(m_albedo, v_texcoord0));
	color *= m_albedoColor;

	if(maskColor.x >= mixTargetAmount && maskColor.y <= mixMinAmount && maskColor.z <= mixMinAmount){
		color = toLinear(texture2D(m_red, v_texcoord0));
	}
	
	if(maskColor.x <= mixMinAmount && maskColor.y <= mixMinAmount && maskColor.z >= mixTargetAmount){
		color = toLinear(texture2D(m_blue, v_texcoord0));
	}

	gl_FragColor.xyz = max(vec3_splat(0.05), lightColor.xyz)*color.xyz;
	gl_FragColor.w = 1.0;
	gl_FragColor = toGamma(gl_FragColor);
}
