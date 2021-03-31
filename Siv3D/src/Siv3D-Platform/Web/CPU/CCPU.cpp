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

# include <Siv3D/EngineLog.hpp>
# include <Siv3D/Array.hpp>
# include "CCPU.hpp"

namespace s3d
{
	CCPU::CCPU()
		: m_feature()
	{

	}

	CCPU::~CCPU()
	{
		LOG_TRACE(U"CCPU::~CCPU()");
	}

	void CCPU::init()
	{
		LOG_TRACE(U"CCPU::init()");

		m_feature.vendor = CPUVendor::Unknown;

		m_feature.vendorName = U"Unknown";

		LOG_INFO(U"ℹ️ CPU vendor: {0}"_fmt(m_feature.vendorName));
		LOG_INFO(U"ℹ️ CPU name: {0}"_fmt(m_feature.name));

		m_feature.SSE		= false;
		m_feature.SSE2		= false;
		m_feature.SSE3		= false;
		m_feature.SSSE3		= false;
		m_feature.SSE41		= false;
		m_feature.SSE42		= false;
		m_feature.SSE4a		= false;
		m_feature.AES		= false;
		m_feature.SHA		= false;
		m_feature.AVX		= false;
		m_feature.XOP		= false;
		m_feature.FMA4		= false;
		m_feature.FMA3		= false;
		m_feature.AVX2		= false;
		m_feature.AVX512_F	= false;

		Array<String> features;

		if (m_feature.SSE)
		{
			features.emplace_back(U"SSE");
		}

		if (m_feature.SSE2)
		{
			features.emplace_back(U"SSE2");
		}

		if (m_feature.SSE3)
		{
			features.emplace_back(U"SSE3");
		}

		if (m_feature.SSSE3)
		{
			features.emplace_back(U"SSSE3");
		}

		if (m_feature.SSE41)
		{
			features.emplace_back(U"SSE4.1");
		}

		if (m_feature.SSE42)
		{
			features.emplace_back(U"SSE4.2");
		}

		if (m_feature.SSE4a)
		{
			features.emplace_back(U"SSE4a");
		}

		if (m_feature.AES)
		{
			features.emplace_back(U"AES-NI");
		}

		if (m_feature.SHA)
		{
			features.emplace_back(U"SHA");
		}

		if (m_feature.AVX)
		{
			features.emplace_back(U"AVX");
		}

		if (m_feature.XOP)
		{
			features.emplace_back(U"XOP");
		}

		if (m_feature.FMA4)
		{
			features.emplace_back(U"FMA4");
		}

		if (m_feature.FMA3)
		{
			features.emplace_back(U"FMA3");
		}

		if (m_feature.AVX2)
		{
			features.emplace_back(U"AVX2");
		}

		if (m_feature.AVX512_F)
		{
			features.emplace_back(U"AVX-512 F");
		}

		LOG_INFO(U"ℹ️ Instruction set extensions: {0}"_fmt(features.join(U", "_sv, U""_sv, U""_sv)));
		LOG_INFO(U"ℹ️ CCPU initialized");
	}

	void CCPU::setFeature(const CPUFeature& feature)
	{
		m_feature = feature;
	}

	const CPUFeature& CCPU::getFeature() const
	{
		return m_feature;
	}
}
