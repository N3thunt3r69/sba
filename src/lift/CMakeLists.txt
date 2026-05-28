add_library(lift)

target_sources(lift
	PUBLIC
		FILE_SET modules TYPE CXX_MODULES
			BASE_DIRS ${CMAKE_SOURCE_DIR}/modules
			FILES
				${CMAKE_SOURCE_DIR}/modules/lift/dedup.cppm
	PRIVATE
		dedup.cpp
)

target_link_libraries(lift
	PUBLIC
		arch
		ir
		binary
)
