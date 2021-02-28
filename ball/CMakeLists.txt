cmake_minimum_required(VERSION 3.0.2)

project(ball)

file(COPY "resources" DESTINATION "${PROJECT_BINARY_DIR}/bin")
file(COPY "deps/shaders" DESTINATION "${PROJECT_BINARY_DIR}/bin/resources")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

if(MSVC)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/bin")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/bin")
endif(MSVC)

set(DEPS_PATH "${CMAKE_SOURCE_DIR}/deps")
set(CMAKE_PREFIX_PATH ${DEPS_PATH})

if(MSVC)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
elseif(WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
  set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
elseif(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++")
elseif(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif(MSVC)

find_package(Vulkan REQUIRED) # Delete for OpenGL
# Uncomment for OpenGL:
# find_package(OpenGL REQUIRED)
# find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)
find_package(PNG REQUIRED)
find_package(GLM)
find_package(OGG REQUIRED)
find_package(VORBIS REQUIRED)
find_package(Portaudio REQUIRED)
find_package(Freetype REQUIRED)
find_package(SMALL3D REQUIRED)

if(UNIX)
  find_package(BZip2 REQUIRED)
endif(UNIX)

subdirs(src)
