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

# include <cassert>
# include <Siv3D/ConstantBuffer.hpp>
# include <Siv3D/Array.hpp>
# include <GL/glew.h>
# include <GLFW/glfw3.h>

namespace s3d
{
	namespace detail
	{
		class ConstantBufferBase::ConstantBufferDetail
		{
		private:

			size_t m_bufferSize = 0;

			Array<Float4> m_uniformBuffer;

		public:

			explicit ConstantBufferDetail(size_t size);

			~ConstantBufferDetail();

			bool update(const void* data, size_t size);

			const Float4* getData() const;
		};
	}
}
