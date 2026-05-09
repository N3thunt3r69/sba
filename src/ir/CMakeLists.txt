add_library(ir)

target_sources(ir
	PUBLIC
		FILE_SET modules TYPE CXX_MODULES
			BASE_DIRS ${CMAKE_SOURCE_DIR}/modules
			FILES
				${CMAKE_SOURCE_DIR}/modules/ir/operation.cppm
)

target_link_libraries(ir PUBLIC arch)
