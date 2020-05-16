cmake_minimum_required (VERSION 3.0)

project(OpenSiv3D_Web CXX)
enable_language(C)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Wno-unknown-pragmas -s ERROR_ON_UNDEFINED_SYMBOLS=0 -s ALLOW_MEMORY_GROWTH=1 -s FULL_ES3 -s USE_GLFW=3 -s USE_LIBPNG=1 -s USE_FREETYPE=1 -s USE_HARFBUZZ=1 -s USE_OGG=1 -s USE_VORBIS=1 -s USE_ZLIB=1 --js-library '${CMAKE_SOURCE_DIR}/Siv3D.js' --preload-file '${CMAKE_SOURCE_DIR}/resources@/resources'")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -pg -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -Og -pg")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")

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
set(CMAKE_EXECUTABLE_SUFFIX ".html")

target_link_libraries(Siv3D_App
	${PROJECT_SOURCE_DIR}/../Build/libSiv3D.a
)

