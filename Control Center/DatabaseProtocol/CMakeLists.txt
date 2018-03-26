add_library(DatabaseProtocol DatabaseProtocol.cpp mysqlpp_connection.cpp mysqlpp_result.cpp mysqlpp_statement.cpp mysqlpp.hpp)

#aux_source_directory(. DIR_WORLD_SRCS)
#add_library(DatabaseProtocol ${DIR_WORLD_SRCS})

#MySQLpp Infor
include_directories (/usr/include /usr/local/include)
find_library (MySQLClient_LIBRARIES NAMES mysqlclient PATHS /usr/lib/mysql /usr/local/lib/mysql)
if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
	set (CMAKE_COMPILER_IS_CLANGCXX 1)
endif()
if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGCXX)
	set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -g -fexceptions -O2 -DNDEBUG")
endif()

#aux_source_directory (. Source_DIRECTORIES)
