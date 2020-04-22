#version 300 es

precision mediump float;
uniform sampler2D Texture0;

layout(std140) uniform PSConstants2D
{
	vec4 g_colorAdd;
	vec4 g_sdfParam;
	vec4 g_internal;	
};

//
// PSInput
//
in vec4 Color;
in vec2 UV;
		
//
// PSOutput
//
layout(location = 0) out vec4 FragColor;
		
void main()
{
	vec4 texColor = texture(Texture0, UV);

	FragColor = (texColor * Color) + g_colorAdd;
}
