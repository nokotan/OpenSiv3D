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
# include <Siv3D/Common.hpp>
# include <Siv3D/StringView.hpp>
# include <Siv3D/ModelObject.hpp>
# include <Siv3D/Material.hpp>

namespace s3d
{
	class ModelData
	{
	public:

		SIV3D_NODISCARD_CXX20
		explicit ModelData();

		SIV3D_NODISCARD_CXX20
		ModelData(FilePathView path);

		~ModelData();

		[[nodiscard]]
		bool isInitialized() const noexcept;

		[[nodiscard]]
		const Array<ModelObject>& getObjects() const noexcept;

		[[nodiscard]]
		const Array<Material>& getMaterials() const noexcept;

	private:

		Array<ModelObject> m_objects;

		Array<Material> m_materials;

		bool m_initialized = false;
	};
}