cmake_minimum_required(VERSION 3.10)

set(PACKAGE_REVISION "0.0.0+unknown")
if (DEFINED CONAN_PACKAGE_NAME)
  set(PACKAGE_REVISION "${CONAN_PACKAGE_VERSION}")
endif ()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-empty-body -Wno-deprecated-copy -Wno-unused-parameter -Wno-sign-compare")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPACKAGE_VERSION=\\\"${PACKAGE_REVISION}\\\"")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPACKAGE_NAME=\\\"${PROJECT_NAME}\\\"")

find_package(flatbuffers REQUIRED)
find_package(grpc_internal QUIET)
find_package(sisl REQUIRED)
find_package(liburing REQUIRED)
find_package(Pistache REQUIRED)
find_package(zmarok-semver REQUIRED)

link_directories(${spdk_LIB_DIRS} ${dpdk_LIB_DIRS})

list(APPEND IOMGR_SRC
      iomgr.cpp
      reactor.cpp
      reactor_epoll.cpp
      reactor_spdk.cpp
      iomgr_timer.cpp
      interfaces/drive_interface.cpp
      interfaces/aio_drive_interface.cpp
      interfaces/spdk_drive_interface.cpp
      interfaces/uring_drive_interface.cpp
      interfaces/generic_interface.cpp
      interfaces/spdk_nvmf_interface.cpp
      io_environment.cpp
      http_server.cpp
      )
if (${grpc_internal_FOUND})
list(APPEND IOMGR_SRC
      interfaces/grpc_interface.cpp
      )
list(APPEND IOMGR_LIB_DEPS
      grpc_internal::grpc_internal
    )
endif ()
add_library (iomgr ${IOMGR_SRC})

list(APPEND IOMGR_LIB_DEPS
      liburing::liburing
      Pistache::Pistache
      sisl::sisl
      spdk::spdk
    )

target_link_libraries(iomgr ${IOMGR_LIB_DEPS})

include ("${sisl_INCLUDE_DIRS}/../cmake/settings_gen.cmake")
settings_gen_cpp($<TARGET_FILE:flatbuffers::flatc> ${CMAKE_CURRENT_BINARY_DIR} iomgr iomgr_config.fbs)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
