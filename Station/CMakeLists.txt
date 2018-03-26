cmake_minimum_required (VERSION 2.8)

project (Demo1)

#set(CMAKE_MODULE_PATH ${ZBARCV_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
find_package (OpenCV)
find_package (ZBar0)

ADD_DEFINITIONS(-DSOURCE_DIR="${CMAKE_COURENT_SOURCE_DIR}")
#AUX_SOURCE_DIRECTORY(${CMAKE_COURENT_SOURCE_DIR}/zbar SUB_DIR_ZBAR)

include_directories("${PROJECT_SOURCE_DIR}/TransportProtocol")
add_subdirectory (TransportProtocol)
set (EXTRA_LIBS ${EXTRA_LIBS} TransportProtocol)

add_executable(Demo 
      test.cpp
)
target_link_libraries (Demo  ${EXTRA_LIBS} ${OpenCV_LIBRARIES} ${ZBAR_LIBRARIES})
