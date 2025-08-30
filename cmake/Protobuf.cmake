set(PROTO_FILES
    proto/connection.proto
)

function(setup_protobuf_files)
    add_library(app_proto ${PROTO_FILES})
    
    get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
    set(GRPC_CPP_PLUGIN_LOCATION ${grpc_cpp_plugin_location} CACHE INTERNAL "gRPC CPP plugin location")
    
    protobuf_generate(TARGET app_proto LANGUAGE cpp)
    protobuf_generate(TARGET app_proto LANGUAGE grpc 
        GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc 
        PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}"
    )
    
    target_link_libraries(app_proto
        PUBLIC
            protobuf::libprotobuf
            gRPC::grpc
            gRPC::grpc++
    )
    
    target_include_directories(app_proto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
    
    message(STATUS "gRPC cpp plugin location: ${grpc_cpp_plugin_location}")
    message(STATUS "Proto files: ${PROTO_FILES}")
    
    set(APP_PROTO_TARGET app_proto PARENT_SCOPE)
endfunction()