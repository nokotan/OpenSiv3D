#version 100

precision mediump float;

//
// Uniform
//
uniform vec4 g_colorAdd;
uniform vec4 g_sdfParam;
uniform vec4 g_internal;	

//
// PSInput
//
varying vec4 Color;
		
void main()
{
	gl_FragColor = Color + g_colorAdd;
}
