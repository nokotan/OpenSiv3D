# version 300 es

//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2021 Ryo Suzuki
//	Copyright (c) 2016-2021 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

//
//	VSInput
//
layout(location = 0) in vec4 VertexPosition;
layout(location = 1) in vec4 VertexColor;

//
//	VSOutput
//
out vec3 WorldPosition;
out vec4 Color;

//
//	Constant Buffer
//
layout(std140) uniform VSConstants3D
{
	mat4x4 g_localToWorld;
	mat4x4 g_worldToProjected;
};

//
//	Functions
//
void main()
{
	vec4 worldPosition = VertexPosition;

	gl_Position		= worldPosition * g_worldToProjected;
	WorldPosition	= worldPosition.xyz;
	Color			= VertexColor;
}