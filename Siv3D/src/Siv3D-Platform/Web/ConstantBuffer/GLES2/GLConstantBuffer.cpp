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

# include <Siv3D/Vector4D.hpp>
# include "GLConstantBuffer.hpp"

namespace s3d
{
	namespace detail
	{
		ConstantBufferBase::ConstantBufferDetail::ConstantBufferDetail(const size_t size)
		: m_bufferSize(size), m_uniformBuffer(m_bufferSize / sizeof(Float4))
		{

		}
		
		ConstantBufferBase::ConstantBufferDetail::~ConstantBufferDetail()
		{

		}
		
		bool ConstantBufferBase::ConstantBufferDetail::update(const void* const data, const size_t size)
		{			
			assert(size <= m_bufferSize);
		
			::memcpy(m_uniformBuffer.data(), data, size);
			
			return true;
		}
		
		const Float4* ConstantBufferBase::ConstantBufferDetail::getData() const
		{
			return m_uniformBuffer.data();
		}
	}
}
