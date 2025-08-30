function(set_compiler_options target_name)
    target_compile_features(${target_name} PRIVATE cxx_std_17)
    
    target_compile_options(${target_name} PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
            -Wall
            -Wextra
            -Wpedantic
            -Wshadow
            -Wconversion
            -Wsign-conversion
            $<$<CONFIG:Debug>:-g -O0>
            $<$<CONFIG:Release>:-O3 -DNDEBUG>
            $<$<CONFIG:RelWithDebInfo>:-g -O2 -DNDEBUG>
        >
    )
endfunction()
