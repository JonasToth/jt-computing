function(jt_executable file_name)
    add_executable(${file_name}.x ${file_name}.cpp)
    target_link_libraries(${file_name}.x
        PRIVATE
            ${PROJECT_NAME}::${PROJECT_NAME}
        )
    jt_compile_setup(${file_name}.x)
endfunction()

function(jt_compile_setup target)
    set_target_properties(${target}
        PROPERTIES
            CMAKE_CXX_STANDARD cxx_std_26
            CMAKE_CXX_EXTENSIONS OFF
            CMAKE_CXX_MODULE_STD ON
            CMAKE_CXX_SCAN_FOR_MODULES ON
    )
endfunction()
