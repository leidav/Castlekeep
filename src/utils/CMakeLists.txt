add_library(utils INTERFACE)
target_sources(utils INTERFACE
	           "${CMAKE_CURRENT_SOURCE_DIR}/utils.h"
			   "${CMAKE_CURRENT_SOURCE_DIR}/container/array.h"
			   "${CMAKE_CURRENT_SOURCE_DIR}/memory/allocator.h"
			   "${CMAKE_CURRENT_SOURCE_DIR}/memory/linear_allocator.h"
			   "${CMAKE_CURRENT_SOURCE_DIR}/memory/block_allocator.h"
			   "${CMAKE_CURRENT_SOURCE_DIR}/memory/freelist_allocator.h"
			   "${CMAKE_CURRENT_SOURCE_DIR}/memory/freelist_allocator.cpp"
			   "${CMAKE_CURRENT_SOURCE_DIR}/memory/polymorphic_allocator_wrappers.h"
			   "${CMAKE_CURRENT_SOURCE_DIR}/memory/polymorphic_allocator_wrappers.cpp")

target_include_directories(utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(utils INTERFACE cxx_std_17)
