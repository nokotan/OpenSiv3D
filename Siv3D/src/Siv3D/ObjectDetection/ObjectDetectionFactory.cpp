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

# include "CObjectDetection.hpp"

# define SIV3D_WITH_FEATURE_PRIVATE_DEFINITION_OPENCV() 1

namespace s3d
{
	ISiv3DObjectDetection* ISiv3DObjectDetection::Create()
	{
# if SIV3D_WITH_FEATURE(OPENCV)
		return new CObjectDetection;
# else
		return nullptr;
# endif
	}
}

