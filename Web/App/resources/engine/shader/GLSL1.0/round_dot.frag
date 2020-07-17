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
	float t = mod(UV.x, 2.0);
	vec2 tex = UV;
	tex.x = abs(1.0 - t) * 2.0;
	vec4 color = Color;
	color.a *= 1.0 - clamp(pow(dot(tex, tex), 8.0), 0.0, 1.0);
	
	gl_FragColor = color + g_colorAdd;
}
