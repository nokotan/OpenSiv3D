#version 100

precision mediump float;
uniform sampler2D Texture0;

//
// PSInput
//
varying vec4 Color;
varying vec2 UV;
		
void main()
{
	gl_FragColor = texture2D(Texture0, UV);
}
