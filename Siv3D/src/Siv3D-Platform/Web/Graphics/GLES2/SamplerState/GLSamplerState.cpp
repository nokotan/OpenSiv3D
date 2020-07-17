//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2019 Ryo Suzuki
//	Copyright (c) 2016-2019 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# include <Siv3D/Platform.hpp>
# include <GL/glew.h>
# include <GLFW/glfw3.h>
# include "GLSamplerState.hpp"

namespace s3d
{
	namespace detail
	{
		static constexpr GLint minmipTable[4] =
		{
			GL_NEAREST_MIPMAP_NEAREST,
			GL_NEAREST_MIPMAP_LINEAR,
			GL_LINEAR_MIPMAP_NEAREST,
			GL_LINEAR_MIPMAP_LINEAR
		};
	}
	
	GLSamplerState::GLSamplerState()
	{
		m_currentStates.fill(NullSamplerState);
	}
	
	void GLSamplerState::setPS(const uint32 slot, const SamplerState& state)
	{
		assert(slot < SamplerState::MaxSamplerCount);
		
		if (state == m_currentStates[slot])
		{
			return;
		}

		m_currentStates[slot] = state;
	}
	
	void GLSamplerState::setPS(const uint32 slot, None_t)
	{
		assert(slot < SamplerState::MaxSamplerCount);
		
		m_currentStates[slot] = NullSamplerState;
	}

	void GLSamplerState::applyState()
	{
		for (uint32 i = 0; i < SamplerState::MaxSamplerCount; i++)
		{
			const auto& state = m_currentStates[i];

			::glActiveTexture(GL_TEXTURE0 + i);

			GLint boundTexture;
			::glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);

			if (!boundTexture)
			{
				continue;
			}
			
			static const GLfloat border[] = { state.borderColor[0], state.borderColor[1], state.borderColor[2], state.borderColor[3] };
			static const GLuint wraps[] = { GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER };

			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			// ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			//					detail::minmipTable[(static_cast<int32>(state.min) << 1) | (static_cast<int32>(state.mip))]);
			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<bool>(state.mag) ? GL_LINEAR : GL_NEAREST);
			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wraps[static_cast<int32>(state.addressU)]);
			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wraps[static_cast<int32>(state.addressV)]);
			// ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wraps[static_cast<int32>(state.addressW)]);
			// ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, state.lodBias);
			// ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
			::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, state.maxAnisotropy);
			// ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
			// ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, -1000.0f);
			// ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, 1000.0f);
		}
	}
	
	GLSamplerState::SamplerStateList::iterator GLSamplerState::create(const SamplerState& state)
	{
		std::unique_ptr<SamplerState_GL> samplerState = std::make_unique<SamplerState_GL>();
		
		if (m_states.size() >= 1024)
		{
			m_states.clear();
		}

		return m_states.emplace(state, std::move(samplerState)).first;
	}
}
