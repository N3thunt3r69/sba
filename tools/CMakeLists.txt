find_library(OCAML_STR_LIB NAMES str str.a PATHS ${OCAML_LIB_PATH} ${OCAML_LIB_PATH}/str NO_DEFAULT_PATH)
find_library(OCAML_STR_STUB_LIB NAMES camlstr camlstrnat camlstrnat.a libcamlstr.a PATHS ${OCAML_LIB_PATH} ${OCAML_LIB_PATH}/stublibs NO_DEFAULT_PATH)
find_library(OCAML_RUNTIME_LIB NAMES asmrun libasmrun.a PATHS ${OCAML_LIB_PATH} NO_DEFAULT_PATH)

add_subdirectory(jump_table)

add_executable(test_decoder_module /home/nhuhuan/.gemini/antigravity-cli/brain/f1fd56b2-ff78-4843-bda6-5150069bfabe/scratch/test_decoder_module.cpp)
target_link_libraries(test_decoder_module PRIVATE lift binary ir arch ${llvm_libs})

