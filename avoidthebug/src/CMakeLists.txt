add_executable(avoidthebug3d main.cpp GameLogic.cpp GameLogic.hpp KeyInput.hpp)

target_include_directories(avoidthebug3d PUBLIC "${CMAKE_SOURCE_DIR}/include")

target_include_directories(avoidthebug3d PUBLIC
  ${SMALL3D_INCLUDE_DIR}
  ${GLFW_INCLUDE_DIRS}
  ${PNG_INCLUDE_DIRS}
  ${GLM_INCLUDE_DIRS}
  ${OGG_INCLUDE_DIRS}
  ${VORBIS_INCLUDE_DIR}
  ${PORTAUDIO_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
  )

if(NOT SMALL3D_OPENGL) # Vulkan
  target_include_directories(avoidthebug3d PUBLIC ${Vulkan_INCLUDE_DIR})
  target_link_libraries(avoidthebug3d PUBLIC
    ${SMALL3D_LIBRARY}
    ${GLFW_LIBRARIES}
    ${Vulkan_LIBRARIES}
    ${PNG_LIBRARIES}
    ${VORBIS_LIBRARIES}
    ${OGG_LIBRARIES}
    ${PORTAUDIO_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    )
else() # OpenGL
  target_include_directories(avoidthebug3d PUBLIC ${OPENGL_INCLUDE_DIR})
  target_link_libraries(avoidthebug3d PUBLIC
    ${SMALL3D_LIBRARY}
    ${GLEW_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${PNG_LIBRARIES}
    ${VORBIS_LIBRARIES}
    ${OGG_LIBRARIES}
    ${PORTAUDIO_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    )
endif()

if(UNIX)
  target_include_directories(avoidthebug3d PUBLIC
    ${BZIP2_INCLUDE_DIRS})
  target_link_libraries(avoidthebug3d PUBLIC
    ${BZIP2_LIBRARIES})
endif()

if(WIN32)
  target_link_libraries(avoidthebug3d PUBLIC winmm)
endif()

if(APPLE)
  set_target_properties(avoidthebug3d PROPERTIES LINK_FLAGS "-framework \
AudioUnit -framework AudioToolbox -framework CoreAudio -framework Cocoa \
-framework IOKit -framework CoreVideo")
endif()

if(MSVC)
  set_target_properties(avoidthebug3d PROPERTIES LINK_FLAGS_DEBUG
    "-NODEFAULTLIB:LIBCMTD")
  set_target_properties(avoidthebug3d PROPERTIES LINK_FLAGS_RELEASE
    "-NODEFAULTLIB:LIBCMT")
  
  set_target_properties(avoidthebug3d PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    "${small3d_BINARY_DIR}/bin")
endif()
