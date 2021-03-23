﻿//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2021 Ryo Suzuki
//	Copyright (c) 2016-2021 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# pragma once

namespace s3d
{
	inline constexpr Vec2 P2Collision::normal() const noexcept
	{
		return m_normal;
	}

	inline std::array<P2Contact, 2>::const_iterator P2Collision::begin() const noexcept
	{
		return m_contacts.begin();
	}

	inline std::array<P2Contact, 2>::const_iterator P2Collision::end() const noexcept
	{
		return (m_contacts.begin() + m_num_contacts);
	}

	inline constexpr size_t P2Collision::num_contacts() const noexcept
	{
		return m_num_contacts;
	}

	inline const P2Contact& P2Collision::contact(const size_t index) const noexcept
	{
		assert(index < m_num_contacts);

		return m_contacts[index];
	}

	inline constexpr bool P2Collision::isEmpty() const noexcept
	{
		return (m_num_contacts == 0);
	}

	inline constexpr P2Collision::operator bool() const noexcept
	{
		return (m_num_contacts != 0);
	}
}