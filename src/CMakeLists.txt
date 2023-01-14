set(LIBRARIES
    ${CONAN_LIBS}
)

file(GLOB_RECURSE SRC_FILES "*.cpp")

foreach(src_file ${SRC_FILES})
    get_filename_component(target_name ${src_file} NAME_WE)
    add_executable(${target_name} ${src_file})
    target_link_libraries(${target_name} PRIVATE ${LIBRARIES})
endforeach()
