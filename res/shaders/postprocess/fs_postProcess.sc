$input v_wpos, v_view, v_normal, v_tangent, v_bitangent, v_texcoord0// in...

/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include <bgfx_shader.sh>
#include <common.sh>

SAMPLER2D(s_texColor,  0);
SAMPLER2D(s_texNormal, 1);

void main()
{
	vec4 color = vec4(1,1,1,1);

	vec4 img_0 = texture2D(s_texColor, vec2(v_texcoord0.x,-v_texcoord0.y));
	vec4 img_1 = texture2D(s_texNormal, vec2(v_texcoord0.x,-v_texcoord0.y));

	vec4 mixImage = img_0;
	if(img_1.a > 0.0){
		mixImage = img_1;
	}

	vec4 outCol = mixImage;

	gl_FragColor = outCol;
}
