cmake_minimum_required(VERSION 3.10)

project(webrtc-vad)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if(UNIX)
    add_compile_options(-DWEBRTC_LINUX -DWEBRTC_POSIX)
elseif(WIN32)
    add_compile_options(-DWIN32 -DWEBRTC_WIN)
endif(UNIX)

include_directories(thirdparty/webrtc)
file(GLOB WEBRTC_VAD_SRCS thirdparty/webrtc/common_audio/*/*.c
                          thirdparty/webrtc/rtc_base/*.c*
                          thirdparty/webrtc/common_audio/vad/*.c
                          ffi_vad.c
                          thirdparty/webrtc/common_audio/signal_processing/spl_inl.c)

add_library(vad SHARED ${WEBRTC_VAD_SRCS})

add_executable(sample_buffer_cut ${CMAKE_CURRENT_SOURCE_DIR}/sample_buffer_cut.c)

if(UNIX)
    message(STATUS "==>> is unix")
    target_link_libraries(vad PRIVATE pthread)
    target_link_libraries(sample_buffer_cut pthread)
else()
    message(STATUS "==>> is other os")
endif(UNIX)

target_link_libraries(sample_buffer_cut vad)
