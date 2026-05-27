add_library(ir)

target_sources(ir
	PUBLIC
		FILE_SET modules TYPE CXX_MODULES
			BASE_DIRS ${CMAKE_SOURCE_DIR}/modules
			FILES
				${CMAKE_SOURCE_DIR}/modules/ir/arch.cppm
				${CMAKE_SOURCE_DIR}/modules/ir/operation.cppm
				${CMAKE_SOURCE_DIR}/modules/ir/context.cppm
				${CMAKE_SOURCE_DIR}/modules/ir/semantics.cppm
)
