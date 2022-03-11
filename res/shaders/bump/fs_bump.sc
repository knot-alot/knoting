$input v_wpos, v_view, v_normal, v_tangent, v_bitangent,
    v_texcoord0  // in...

/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include <bgfx_shader.sh>
#include <common.sh>

        SAMPLER2D(s_texColor, 0);
SAMPLER2D(s_texNormal, 1);
uniform vec4 u_lightPosRadius[64];
uniform vec4 u_lightRgbInnerR[64];

vec2 blinn(vec3 _lightDir, vec3 _normal, vec3 _viewDir) {
    float ndotl = dot(_normal, _lightDir);
    // vec3 reflected = _lightDir - 2.0*ndotl*_normal; // reflect(_lightDir, _normal);
    vec3 reflected = 2.0 * ndotl * _normal - _lightDir;
    float rdotv = dot(reflected, _viewDir);
    return vec2(ndotl, rdotv);
}

float fresnel(float _ndotl, float _bias, float _pow) {
    float facing = (1.0 - _ndotl);
    return max(_bias + (1.0 - _bias) * pow(facing, _pow), 0.0);
}

vec4 lit(float _ndotl, float _rdotv, float _m) {
    float diff = max(0.0, _ndotl);
    float spec = step(0.0, _ndotl) * max(0.0, _rdotv * _m);
    return vec4(1.0, diff, spec, 1.0);
}

vec4 powRgba(vec4 _rgba, float _pow) {
    vec4 result;
    result.xyz = pow(_rgba.xyz, vec3_splat(_pow));
    result.w = _rgba.w;
    return result;
}

vec3 calcLight(int _idx, mat3 _tbn, vec3 _wpos, vec3 _normal, vec3 _view) {
    vec3 lp = u_lightPosRadius[_idx].xyz - _wpos;
    float attn = 1.0 - smoothstep(u_lightRgbInnerR[_idx].w, 1.0, length(lp) / u_lightPosRadius[_idx].w);
    vec3 lightDir = mul(normalize(lp), _tbn);
    vec2 bln = blinn(lightDir, _normal, _view);
    vec4 lc = lit(bln.x, bln.y, 1.0);
    vec3 rgb = u_lightRgbInnerR[_idx].xyz * saturate(lc.y) * attn;
    return rgb;
}

mat3 mtx3FromCols(vec3 c0, vec3 c1, vec3 c2) {
#if BGFX_SHADER_LANGUAGE_GLSL
    return mat3(c0, c1, c2);
#else
    return transpose(mat3(c0, c1, c2));
#endif
}

void main() {
    mat3 tbn = mtx3FromCols(v_tangent, v_bitangent, v_normal);

    vec3 normal;
    normal.xy = texture2D(s_texNormal, v_texcoord0).xy * 2.0 - 1.0;
    normal.z = sqrt(1.0 - dot(normal.xy, normal.xy));
    vec3 view = normalize(v_view);

    vec3 lightColor;
    lightColor = calcLight(0, tbn, v_wpos, normal, view);
    lightColor += calcLight(1, tbn, v_wpos, normal, view);
    lightColor += calcLight(2, tbn, v_wpos, normal, view);
    lightColor += calcLight(3, tbn, v_wpos, normal, view);
    lightColor += calcLight(4, tbn, v_wpos, normal, view);
    lightColor += calcLight(5, tbn, v_wpos, normal, view);
    lightColor += calcLight(6, tbn, v_wpos, normal, view);
    lightColor += calcLight(7, tbn, v_wpos, normal, view);
    lightColor += calcLight(8, tbn, v_wpos, normal, view);
    lightColor += calcLight(9, tbn, v_wpos, normal, view);
    lightColor += calcLight(10, tbn, v_wpos, normal, view);
    lightColor += calcLight(11, tbn, v_wpos, normal, view);
    lightColor += calcLight(12, tbn, v_wpos, normal, view);
    lightColor += calcLight(13, tbn, v_wpos, normal, view);
    lightColor += calcLight(14, tbn, v_wpos, normal, view);
    lightColor += calcLight(15, tbn, v_wpos, normal, view);
    lightColor += calcLight(16, tbn, v_wpos, normal, view);
    lightColor += calcLight(17, tbn, v_wpos, normal, view);
    lightColor += calcLight(18, tbn, v_wpos, normal, view);
    lightColor += calcLight(19, tbn, v_wpos, normal, view);
    lightColor += calcLight(20, tbn, v_wpos, normal, view);
    lightColor += calcLight(21, tbn, v_wpos, normal, view);
    lightColor += calcLight(22, tbn, v_wpos, normal, view);
    lightColor += calcLight(23, tbn, v_wpos, normal, view);
    lightColor += calcLight(24, tbn, v_wpos, normal, view);
    lightColor += calcLight(25, tbn, v_wpos, normal, view);
    lightColor += calcLight(26, tbn, v_wpos, normal, view);
    lightColor += calcLight(27, tbn, v_wpos, normal, view);
    lightColor += calcLight(29, tbn, v_wpos, normal, view);
    lightColor += calcLight(30, tbn, v_wpos, normal, view);
    lightColor += calcLight(31, tbn, v_wpos, normal, view);
    lightColor += calcLight(32, tbn, v_wpos, normal, view);
    lightColor += calcLight(33, tbn, v_wpos, normal, view);
    lightColor += calcLight(34, tbn, v_wpos, normal, view);
    lightColor += calcLight(35, tbn, v_wpos, normal, view);
    lightColor += calcLight(36, tbn, v_wpos, normal, view);
    lightColor += calcLight(37, tbn, v_wpos, normal, view);
    lightColor += calcLight(38, tbn, v_wpos, normal, view);
    lightColor += calcLight(39, tbn, v_wpos, normal, view);
    lightColor += calcLight(40, tbn, v_wpos, normal, view);
    lightColor += calcLight(50, tbn, v_wpos, normal, view);
    lightColor += calcLight(51, tbn, v_wpos, normal, view);
    lightColor += calcLight(52, tbn, v_wpos, normal, view);
    lightColor += calcLight(53, tbn, v_wpos, normal, view);
    lightColor += calcLight(54, tbn, v_wpos, normal, view);
    lightColor += calcLight(55, tbn, v_wpos, normal, view);
    lightColor += calcLight(56, tbn, v_wpos, normal, view);
    lightColor += calcLight(57, tbn, v_wpos, normal, view);
    lightColor += calcLight(58, tbn, v_wpos, normal, view);
    lightColor += calcLight(59, tbn, v_wpos, normal, view);
    lightColor += calcLight(60, tbn, v_wpos, normal, view);
    lightColor += calcLight(61, tbn, v_wpos, normal, view);
    lightColor += calcLight(62, tbn, v_wpos, normal, view);
    lightColor += calcLight(63, tbn, v_wpos, normal, view);

    vec4 color = toLinear(texture2D(s_texColor, v_texcoord0));

    gl_FragColor.xyz = max(vec3_splat(0.05), lightColor.xyz) * color.xyz;
    gl_FragColor.w = 1.0;
    gl_FragColor = toGamma(gl_FragColor);
}
