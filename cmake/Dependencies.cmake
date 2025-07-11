# Функции для поиска и настройки Curl и Json библиотек

function(setup_curl target_name)
    find_package(CURL REQUIRED)
    
    target_link_libraries(${target_name} PRIVATE CURL::libcurl)
    
    if(WIN32)
        target_compile_definitions(${target_name} PRIVATE CURL_STATICLIB)
    endif()
    
    message(STATUS "CURL version: ${CURL_VERSION_STRING}")
endfunction()

function(setup_json target_name)
    find_package(nlohmann_json 3.2.0 REQUIRED)
    
    target_link_libraries(${target_name} PRIVATE nlohmann_json::nlohmann_json)
    
    message(STATUS "nlohmann_json found")
endfunction()

function(setup_all_dependencies target_name)
    setup_curl(${target_name})
    setup_json(${target_name})
endfunction()
