function(jt_executable file_name)
    add_executable(${file_name}.x ${file_name}.cpp)
    target_link_libraries(${file_name}.x
        PRIVATE
            ${PROJECT_NAME}::${PROJECT_NAME}
        )
    target_compile_features(${file_name}.x PUBLIC cxx_std_26)
endfunction()
