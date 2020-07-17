#version 100

precision mediump float;

//
// Uniforms
//
uniform vec4 g_colorAdd;
uniform vec4 g_sdfParam;
uniform vec4 g_internal;	

//
// PSInput
//
varying vec4 Color;
varying vec2 UV;
	
void main()
{
	float tr = UV.y;
	float d = abs(mod(UV.x, 3.0) - 1.0);
	float range = 1.0 - tr;
	vec4 color = Color;
	color.a *= (d < range) ? 1.0 : (d < 1.0) ? ((1.0 - d) / tr) : 0.0;
	
	gl_FragColor = color + g_colorAdd;
}
