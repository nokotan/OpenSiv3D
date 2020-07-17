#version 100

//
// VSInput
//
attribute vec2 VertexPosition;
attribute vec2 UVPosition;

//
// VSOutput
//
varying vec2 UV;
				
void main()
{
	float x = VertexPosition.x;
	float y = VertexPosition.y;
	gl_Position = vec4(x, y, 0, 1);
	
	UV = UVPosition;
}
