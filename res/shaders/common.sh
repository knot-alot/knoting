#ifndef __SHADERLIB_SH__
#define __SHADERLIB_SH__

float decodeRE8(vec4 _re8) {
    float exponent = _re8.w * 255.0 - 128.0;
    return _re8.x * exp2(exponent);
}

#endif // __SHADERLIB_SH__
