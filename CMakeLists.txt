cmake_minimum_required(VERSION 3.10)
project(openssl_tls_demo C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

# 添加 OpenSSL 本地路径
include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(openssl_tls_demo
    src/main.c
    src/server.c
)

target_link_libraries(openssl_tls_demo
    ${CMAKE_SOURCE_DIR}/lib/libssl.a
    ${CMAKE_SOURCE_DIR}/lib/libcrypto.a
    -ldl -lpthread
)

