//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2023 Ryo Suzuki
//	Copyright (c) 2016-2023 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# include "Siv3DTest.hpp"
# include <Siv3D/DLL.hpp>

TEST_CASE("DLL")
{
# if SIV3D_PLATFORM(WEB)

	SECTION("No support")
	{
		REQUIRE_THROWS_MATCHES(DLL::Load(U"libsub.so"), s3d::EngineError, EngineErrorMatcher(U"To use dlopen, you need enable dynamic linking, see https://github.com/emscripten-core/emscripten/wiki/Linking"));
	}

	SECTION("Undefined Symbol")
	{
		extern void NonExistentFunction();

		// FIXME: -1 is thrown when unresolved function is called in emscripten 3.1.20.
		// We should migrate to emscripten 3.1.53, in which unresolved symbol report will be thrown.
		// 
		// abort(-1); // emscripten 3.1.20, not user friendly abort
		// abort("missing function: _Z19NonExistentFunctionv"); // emscripten 3.1.53
		REQUIRE_THROWS_MATCHES(NonExistentFunction(), s3d::EngineError, EngineErrorMatcher(U"-1"));
	}

# else

	SECTION("Existent Module")
	{
		auto handle = DLL::Load(U"libsub.so");

		REQUIRE(handle != nullptr);
	}

	SECTION("Non Existent Module")
	{
		auto handle = DLL::Load(U"libsub2.so");

		REQUIRE(handle == nullptr);
	}

# endif
}
