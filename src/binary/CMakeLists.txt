add_library(binary)

target_sources(binary
	PUBLIC
		FILE_SET modules TYPE CXX_MODULES
			BASE_DIRS ${CMAKE_SOURCE_DIR}/modules
			FILES
				${CMAKE_SOURCE_DIR}/modules/framework/config.cppm
				${CMAKE_SOURCE_DIR}/modules/framework/error.cppm
				${CMAKE_SOURCE_DIR}/modules/framework/framework.cppm
				${CMAKE_SOURCE_DIR}/modules/binary/layout/layout.cppm
)

target_include_directories(binary PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_definitions(binary PRIVATE ${LLVM_DEFINITIONS})
target_link_libraries(binary PRIVATE ${llvm_libs})
