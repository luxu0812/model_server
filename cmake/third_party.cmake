cmake_minimum_required(VERSION 3.20)
project(third_party)

set(CMAKE_VERBOSE_MAKEFILE on)

## gflags
set(GFLAGS_INSTALL_PATH "/opt/libs/gflags")
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
  set(GFLAGS_INSTALL_PATH "/opt/homebrew/Cellar/gflags/2.2.2")
endif ()
# set(gflags_DIR "${GFLAGS_INSTALL_PATH}/lib/cmake/gflags")
# find_package(gflags REQUIRED)
set(GFLAGS_INCLUDE_PATH "${GFLAGS_INSTALL_PATH}/include")

## glog
set(GLOG_INSTALL_PATH "/opt/libs/glog")
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
  set(GLOG_INSTALL_PATH "/opt/homebrew/Cellar/glog/0.6.0")
endif ()
set(glog_DIR "${GLOG_INSTALL_PATH}/lib/cmake/glog")
find_package(glog REQUIRED)
set(GLOG_INCLUDE_PATH "${GLOG_INSTALL_PATH}/include")

## protobuf
set(PROTOBUF_INSTALL_PATH "/opt/libs/protobuf")
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
  set(PROTOBUF_INSTALL_PATH "/opt/homebrew/Cellar/protobuf/21.7")
endif ()
find_library(protobuf
  NAMES protobuf
  PATHS "${PROTOBUF_INSTALL_PATH}/lib"
)
set(PROTOBUF_INCLUDE_PATH "${PROTOBUF_INSTALL_PATH}/include")

## mysqlpp
set(MYSQLPP_INSTALL_PATH "/opt/libs/mysqlpp")
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
  set(MYSQLPP_INSTALL_PATH "/opt/homebrew/Cellar/mysql++/3.3.0_1")
endif ()
find_library(mysqlpp
  NAMES mysqlpp
  PATHS "${MYSQLPP_INSTALL_PATH}/lib"
)
set(MYSQLPP_INCLUDE_PATH "${MYSQLPP_INSTALL_PATH}/include")
