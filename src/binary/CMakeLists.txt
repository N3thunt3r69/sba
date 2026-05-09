add_library(binary)

target_sources(binary
	PUBLIC
		FILE_SET modules TYPE CXX_MODULES
			BASE_DIRS ${CMAKE_SOURCE_DIR}/modules
			FILES
				${CMAKE_SOURCE_DIR}/modules/binary/loader/types.cppm
				${CMAKE_SOURCE_DIR}/modules/binary/loader/object.cppm
				${CMAKE_SOURCE_DIR}/modules/binary/loader/elf.cppm
	PRIVATE
		loader/types.cpp
		loader/object.cpp
		loader/elf.cpp
)

target_include_directories(binary PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_definitions(binary PRIVATE ${LLVM_DEFINITIONS})
target_link_libraries(binary
	PUBLIC
		framework
		arch
		ir
	PRIVATE
		${llvm_libs}
)
