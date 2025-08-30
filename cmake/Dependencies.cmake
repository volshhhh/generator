function(setup_curl target_name)
    find_package(CURL REQUIRED)
    target_link_libraries(${target_name} PRIVATE CURL::libcurl)
    message(STATUS "CURL version: ${CURL_VERSION_STRING}")
endfunction()

function(setup_json target_name)
    find_package(nlohmann_json 3.2.0 REQUIRED)
    target_link_libraries(${target_name} PRIVATE nlohmann_json::nlohmann_json)
    message(STATUS "nlohmann_json found")
endfunction()

function(setup_protobuf target_name)
    find_package(Protobuf CONFIG REQUIRED)
    target_link_libraries(${target_name} PRIVATE protobuf::libprotobuf)
    message(STATUS "Protobuf version: ${Protobuf_VERSION}")
endfunction()

function(setup_grpc target_name)
    find_package(gRPC CONFIG REQUIRED)
    target_link_libraries(${target_name} PRIVATE 
        gRPC::grpc 
        gRPC::grpc++
    )
    message(STATUS "gRPC version: ${gRPC_VERSION}")
endfunction()

function(setup_threads target_name)
    find_package(Threads REQUIRED)
    target_link_libraries(${target_name} PRIVATE Threads::Threads)
endfunction()

function(setup_all_dependencies target_name)
    setup_curl(${target_name})
    setup_json(${target_name})
    setup_protobuf(${target_name})
    setup_grpc(${target_name})
    setup_threads(${target_name})
endfunction()