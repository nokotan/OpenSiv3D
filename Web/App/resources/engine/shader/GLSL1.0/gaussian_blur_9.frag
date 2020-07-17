#version 100

precision mediump float;
uniform sampler2D Texture0;

//
// Uniforms
//
uniform vec4 g_colorAdd;
uniform vec4 g_sdfParam;
uniform vec2 g_pixelSize;
uniform vec2 g_direction;

//
// PSInput
//
varying vec4 Color;
varying vec2 UV;
	
//
// http://rastergrid.com/blog/2010/09/efficient-gaussian-blur-with-linear-sampling/
//
void main()
{
	vec2 offset1 = 1.38461538461538 * g_direction;
	vec2 offset2 = 3.23076923076923 * g_direction;

	vec4 color = texture2D(Texture0, UV) * 0.227027027027027;
	color += texture2D(Texture0, UV + (offset1 * g_pixelSize)) * 0.316216216216216;
	color += texture2D(Texture0, UV - (offset1 * g_pixelSize)) * 0.316216216216216;
	color += texture2D(Texture0, UV + (offset2 * g_pixelSize)) * 0.070270270270270;
	color += texture2D(Texture0, UV - (offset2 * g_pixelSize)) * 0.070270270270270;

	gl_FragColor = color;
}
