cmake_minimum_required(VERSION 3.6.0)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_library(log-lib
        log)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/*.cpp)
file(GLOB HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/include/*.hpp)

MESSAGE(STATUS "Sources: ${SOURCE_FILES} ${HEADER_FILES}")

add_library(avoidthebug
        SHARED
        ${SOURCE_FILES}
        ${HEADER_FILES})

find_package(Vulkan REQUIRED)
message(STATUS "Vulkan FOUND = ${Vulkan_FOUND}")
message(STATUS "Vulkan Include = ${Vulkan_INCLUDE_DIR}")
message(STATUS "Vulkan Libs = ${Vulkan_LIBRARIES}")

target_include_directories(avoidthebug PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/include
        ${Vulkan_INCLUDE_DIRS}
        ${ANDROID_NDK}/sources/android/native_app_glue
        )

target_link_libraries(avoidthebug
        android
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/lib/${ANDROID_ABI}/libnative_app_glue.a
        aaudio
        ${log-lib}
        ${Vulkan_LIBRARIES}
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/lib/${ANDROID_ABI}/libpng.a
        z
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/lib/${ANDROID_ABI}/libvorbis.a
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/lib/${ANDROID_ABI}/libvorbisfile.a
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/lib/${ANDROID_ABI}/libogg.a
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/lib/${ANDROID_ABI}/libfreetype.a
        ${CMAKE_CURRENT_SOURCE_DIR}/deps/lib/${ANDROID_ABI}/libsmall3d.a
        )
