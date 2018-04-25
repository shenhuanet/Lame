cmake_minimum_required(VERSION 3.4.1)
set(SRC_DIR src/main/cpp/lamemp3)
include_directories(src/main/cpp/lamemp3)
aux_source_directory(src/main/cpp/lamemp3 SRC_LIST)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/jniLibs/${ANDROID_ABI})

add_library(lame-mp3 SHARED src/main/cpp/lame-mp3.cpp ${SRC_LIST})

find_library(log-lib log)

target_link_libraries(lame-mp3 ${log-lib})