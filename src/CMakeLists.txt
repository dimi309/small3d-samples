add_executable(groom main.cpp Game.cpp Map.cpp Enemy.cpp KeyInput.hpp Game.hpp Map.hpp Enemy.hpp)

target_include_directories(groom PUBLIC
  ${SMALL3D_INCLUDE_DIR}
  ${GLFW_INCLUDE_DIRS}
  ${Vulkan_INCLUDE_DIR}
  ${PNG_INCLUDE_DIRS}
  ${GLM_INCLUDE_DIRS}
  ${OGG_INCLUDE_DIRS}
  ${VORBIS_INCLUDE_DIR}
  ${PORTAUDIO_INCLUDE_DIRS}
  ${FREETYPE_INCLUDE_DIRS}
  )

target_link_libraries(groom PUBLIC
  ${SMALL3D_LIBRARY}
  ${GLFW_LIBRARIES}
  ${Vulkan_LIBRARIES}
  ${PNG_LIBRARIES}
  ${VORBIS_LIBRARIES}
  ${OGG_LIBRARIES}
  ${PORTAUDIO_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  )

if(UNIX)
  target_include_directories(groom PUBLIC
    ${BZIP2_INCLUDE_DIRS})
  target_link_libraries(groom PUBLIC
    ${BZIP2_LIBRARIES})
endif(UNIX)

if(WIN32)
  target_link_libraries(groom PUBLIC winmm)
endif(WIN32)

if(APPLE)
  set_target_properties(groom PROPERTIES LINK_FLAGS "-framework \
AudioUnit -framework AudioToolbox -framework CoreAudio -framework Cocoa \
-framework IOKit -framework CoreVideo")
endif(APPLE)

if(MSVC)
  set_target_properties(groom PROPERTIES LINK_FLAGS_DEBUG
    "-NODEFAULTLIB:LIBCMTD")
  set_target_properties(groom PROPERTIES LINK_FLAGS_RELEASE
    "-NODEFAULTLIB:LIBCMT")
  
  set_target_properties(groom PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY
    "${small3d_BINARY_DIR}/bin")
endif(MSVC)
