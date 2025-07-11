# Функции для работы с исходными файлами

function(collect_sources var_name)
    set(source_patterns
        "src/*.cpp"
        "src/*.c"
        "src/**/*.cpp"
        "src/**/*.c"
        "include/controller/*.cpp"
        "include/controller/*.c"
    )
    
    set(header_patterns
        "include/entities/*.h"
        "include/entities/*.hpp"
        "include/utils/*.h"
        "include/utils/*.hpp"
        "src/*.h"
        "src/*.hpp"
        "include/controller/*.h"
        "include/controller/*.hpp"
    )
    
    set(all_sources)
    foreach(pattern ${source_patterns})
        file(GLOB_RECURSE pattern_files "${pattern}")
        list(APPEND all_sources ${pattern_files})
    endforeach()
    
    set(all_headers)
    foreach(pattern ${header_patterns})
        file(GLOB_RECURSE pattern_files "${pattern}")
        list(APPEND all_headers ${pattern_files})
    endforeach()
    
    # Удаляем дубликаты
    list(REMOVE_DUPLICATES all_sources)
    if(all_headers)
        list(REMOVE_DUPLICATES all_headers)
    endif()
    
    # Объединяем источники и заголовки
    set(${var_name} ${all_sources} ${all_headers} PARENT_SCOPE)
    
    # Выводим информацию
    list(LENGTH all_sources sources_count)
    list(LENGTH all_headers headers_count)
    message(STATUS "Found ${sources_count} source files and ${headers_count} header files")
endfunction()

function(setup_include_directories target_name)
    target_include_directories(${target_name} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
endfunction()
