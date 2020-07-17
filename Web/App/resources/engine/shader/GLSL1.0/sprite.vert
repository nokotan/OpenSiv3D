#version 100
				
//
// Uniforms
//
uniform vec4 g_transform0;
uniform vec4 g_transform1;
uniform vec4 g_colorMul;

//
// VSInput
//
attribute vec2 VertexPosition;
attribute vec2 VertexUV;
attribute vec4 VertexColor;

//
// VSOutput
//
varying vec4 Color;
varying vec2 UV;

vec4 StandardTransform(const vec2 pos)
{
	vec4 result;
	result.xy = g_transform0.zw + pos.x * g_transform0.xy + pos.y * g_transform1.xy;
	result.zw = g_transform1.zw;
	return result;
}
		
void main()
{
	gl_Position = StandardTransform(VertexPosition);

	Color = VertexColor * g_colorMul;
	
	UV = VertexUV;
}
