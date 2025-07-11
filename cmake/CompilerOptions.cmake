# Опции компилятора для разных конфигураций

function(set_compiler_options target_name)
    # Общие флаги для всех компиляторов
    target_compile_features(${target_name} PRIVATE cxx_std_17)
    
    # Флаги для GCC и Clang
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
    
    # Флаги для MSVC
    target_compile_options(${target_name} PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:
            /W4
            /permissive-
            $<$<CONFIG:Debug>:/Od /Zi>
            $<$<CONFIG:Release>:/O2 /DNDEBUG>
        >
    )
    
    # Дополнительные определения для разных платформ
    if(WIN32)
        target_compile_definitions(${target_name} PRIVATE
            WIN32_LEAN_AND_MEAN
            NOMINMAX
        )
    endif()
endfunction()
