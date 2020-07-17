#version 100

#extension GL_OES_standard_derivatives : enable

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

float median(float r, float g, float b)
{
	return max(min(r, g), min(max(r, g), b));
}
		
void main()
{
	float pxRange = g_sdfParam.x;
	// vec2 size = vec2(textureSize(Texture0, 0));
	vec2 size = vec2(1024, 1024);
	vec2 msdfUnit = pxRange / size;
	vec3 s = texture2D(Texture0, UV).rgb;
	float sigDist = median(s.r, s.g, s.b) - 0.5 + g_sdfParam.y;
	sigDist *= dot(msdfUnit, 0.5 / fwidth(UV));
	
	float a = clamp(sigDist + 0.5, 0.0, 1.0);
	vec4 color = vec4(Color.rgb, Color.a * a);

	gl_FragColor = color + g_colorAdd;
}
