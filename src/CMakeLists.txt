add_subdirectory(utils)
add_executable(${PROJECT_NAME} main.cpp)
target_sources(${PROJECT_NAME} PRIVATE
	           "${CMAKE_CURRENT_SOURCE_DIR}/engine.h"
			   "${CMAKE_CURRENT_SOURCE_DIR}/engine.cpp"
			   "${CMAKE_CURRENT_SOURCE_DIR}/terrain.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/terrain.cpp"
               "${CMAKE_CURRENT_SOURCE_DIR}/tileset.h"
               "${CMAKE_CURRENT_SOURCE_DIR}/tileset.cpp"
			   "${CMAKE_CURRENT_SOURCE_DIR}/renderer/renderer.h"
			   "${CMAKE_CURRENT_SOURCE_DIR}/renderer/renderer.cpp"
			   "${CMAKE_CURRENT_SOURCE_DIR}/renderer/sdl_renderer.h"
			   "${CMAKE_CURRENT_SOURCE_DIR}/renderer/sdl_renderer.cpp"
			   "${CMAKE_CURRENT_SOURCE_DIR}/platform/platform.h"
			   "${CMAKE_CURRENT_SOURCE_DIR}/platform/platform.cpp"
			   "${CMAKE_CURRENT_SOURCE_DIR}/platform/sdl_platform.h"
			   "${CMAKE_CURRENT_SOURCE_DIR}/platform/sdl_platform.cpp"
			   "${CMAKE_CURRENT_SOURCE_DIR}/loaders/image_loader.h"
			   "${CMAKE_CURRENT_SOURCE_DIR}/loaders/image_loader.cpp")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE utils PNG::PNG SDL2::SDL2)
