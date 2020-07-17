#version 100

precision mediump float;
uniform sampler2D Texture0;

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
	vec4 texColor = texture2D(Texture0, UV);

	gl_FragColor = (texColor * Color) + g_colorAdd;
}
