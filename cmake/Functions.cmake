function(jt_executable file_name)
    add_executable(${file_name}.x ${file_name}.cpp)
    target_link_libraries(${file_name}.x
        PRIVATE
            ${PROJECT_NAME}::${PROJECT_NAME}
        )
    target_compile_features(${file_name}.x PUBLIC cxx_std_26)
endfunction()

function(jt_compile_setup target)
    set_target_properties(${target}
        PROPERTIES
            CMAKE_CXX_STANDARD cxx_std_26
            CMAKE_CXX_MODULE_STD OFF
            CMAKE_CXX_EXTENSIONS OFF
            CMAKE_CXX_SCAN_FOR_MODULES OFF
    )
endfunction()
