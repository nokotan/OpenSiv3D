cmake_minimum_required (VERSION 3.0)

project(OpenSiv3D_Web CXX)
enable_language(C)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -pg -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -O2 -pg")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")

set(CMAKE_EXE_LINKER_FLAGS "-s ERROR_ON_UNDEFINED_SYMBOLS=0 -s ALLOW_MEMORY_GROWTH=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -s FULL_ES3 -s USE_GLFW=3 -s USE_LIBPNG=1 -s USE_OGG=1 -s USE_VORBIS=1 -s USE_ZLIB=1 --js-library \"${CMAKE_SOURCE_DIR}/Siv3D.js\" --preload-file \"${CMAKE_SOURCE_DIR}/resources@/resources\"")
set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "--closure 1")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_compile_options ("-fcolor-diagnostics")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	add_compile_options ("-fdiagnostics-color=always")
endif()

#set(CMAKE_BUILD_TYPE Release)

include_directories(
	"../../Siv3D/include"
	"../../Siv3D/include/ThirdParty"
)

set(SOURCE_FILES
	"./Main.cpp"
)

add_executable(Siv3D_App ${SOURCE_FILES})

if (SIV3D_BUILD_WITH_SHARED_WASM)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s MAIN_MODULE=1 -s RUNTIME_LINKED_LIBS=['Siv3D.wasm']")
	set(CMAKE_EXECUTABLE_SUFFIX ".html")
elseif (SIV3D_BUILD_WITH_SHARED_MAIN)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s SIDE_MODULE=1")
	set(CMAKE_EXECUTABLE_SUFFIX ".wasm")
elseif (SIV3D_BUILD_WITH_FASTCOMP_SUPPORT)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSIMDE_NO_VECTOR")

	target_link_directories(Siv3D_App
	PRIVATE
		${PROJECT_SOURCE_DIR}/../Build
		${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/asmjs/opencv
		${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/asmjs/harfbuzz
		${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/asmjs/freetype
		${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/asmjs/turbojpeg
		${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/asmjs/giflib
		${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/asmjs/webp
	)

	target_link_libraries(Siv3D_App
		libSiv3D.a
		libopencv_objdetect.a
		libopencv_photo.a
		libopencv_imgproc.a
		libopencv_core.a
		libharfbuzz.a
		libfreetype.a
		libturbojpeg.a
		libgif.a
		libwebp.a
	)

	set(CMAKE_EXECUTABLE_SUFFIX ".html")
else ()
	if (EMSCRIPTEN_VERSION VERSION_GREATER_EQUAL "2.0.22")
		target_link_directories(Siv3D_App
		PRIVATE
			${PROJECT_SOURCE_DIR}/../Build
			${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm-2.0.22/opencv
			${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm-2.0.22/harfbuzz
			${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm-2.0.22/freetype
			${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm-2.0.22/turbojpeg
			${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm-2.0.22/giflib
			${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm-2.0.22/webp
		)
	else ()
		target_link_directories(Siv3D_App
		PRIVATE
			${PROJECT_SOURCE_DIR}/../Build
			${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm/opencv
			${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm/harfbuzz
			${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm/freetype
			${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm/turbojpeg
			${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm/giflib
			${PROJECT_SOURCE_DIR}/../../Siv3D/lib/Web/wasm/webp
		)
	endif ()

	target_link_libraries(Siv3D_App
		libSiv3D.a
		libopencv_objdetect.a
		libopencv_photo.a
		libopencv_imgproc.a
		libopencv_core.a
		libharfbuzz.a
		libfreetype.a
		libturbojpeg.a
		libgif.a
		libwebp.a
	)

	set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()
