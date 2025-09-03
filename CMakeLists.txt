cmake_minimum_required(VERSION 3.12)

project("dbcppp" VERSION 3.8.0)

# CONFIGURATION

set(CMAKE_CXX_STANDARD 17)
option(DBCPPP_build_kcd "Enable support for KCD parsing" OFF)
option(DBCPPP_build_tools "Build dbcppp utility application" OFF)
option(DBCPPP_build_tests "Build tests" OFF)
option(DBCPPP_build_examples "Build examples" OFF)
option(DBCPPP_system_boost "Use system boost" OFF)
option(DBCPPP_BUILD_STATIC "Build as a static library" OFF)
option(DBCPPP_INSTALL "Generate installation target" ON)

# DEPENDENCIES & Requirements
if (system_boost)
  find_package(Boost)
endif()

if(NOT Boost_FOUND)
  message(WARNING "Boost not found. Using libdbcppp boost (third-party/boost)")
  include_directories("third-party/boost")
endif()

# kcd is an xml based can database
message("kcd enabled: ${DBCPPP_build_kcd}")

if(DBCPPP_build_kcd)
  add_compile_definitions(ENABLE_KCD)

  # LibXml2

  find_package(LibXml2)

  if (NOT LibXml2_FOUND)
    message(WARNING "LibXml2 was not found. Using libdbcppp LibXml2 (third-party/libxml2)")
    set(LIBXML2_WITH_ICONV OFF)
    set(LIBXML2_WITH_LZMA OFF)
    set(LIBXML2_WITH_PYTHON OFF)
    set(LIBXML2_WITH_ZLIB OFF)
    set(LIBXML2_WITH_TESTS OFF)
    add_subdirectory(third-party/libxml2)
  endif()

  # libxmlmm (no find package because it is certainly not installed

  add_subdirectory("third-party/libxmlmm")
  target_include_directories(libxmlmm PUBLIC third-party/libxmlmm/libxmlmm ${LIBXML2_INCLUDE_DIR})
endif()


# CREATE LIBRARY

file(GLOB include "include/dbcppp/*.h")
file(GLOB headers "src/*.h")
file(GLOB sources "src/*.cpp")

if (DBCPPP_BUILD_STATIC)
  add_library(${PROJECT_NAME} STATIC ${include} ${headers} ${sources})
  target_compile_definitions(${PROJECT_NAME} PUBLIC DBCPPP_STATIC_DEFINE)
else()
  add_library(${PROJECT_NAME} SHARED ${include} ${headers} ${sources})
  target_compile_definitions(${PROJECT_NAME} PRIVATE DBCPPP_EXPORT)
endif()

# CONFIGURE LIBRARY

if (DBCPPP_build_kcd)
  target_link_libraries(${PROJECT_NAME} PUBLIC libxmlmm)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    SOVERSION ${PROJECT_VERSION}
    PUBLIC_HEADER "${include}"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/dbcppp>
    $<INSTALL_INTERFACE:include/dbcppp>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# INSTALL LIBRARY

if (DBCPPP_INSTALL)
  install(TARGETS ${PROJECT_NAME}
      EXPORT dbcpppTargets
      RUNTIME DESTINATION bin
      LIBRARY DESTINATION lib
      ARCHIVE DESTINATION lib
      PUBLIC_HEADER DESTINATION include/dbcppp
  )

# Export the targets for use with find_package
  install(EXPORT dbcpppTargets
      FILE dbcpppTargets.cmake
      NAMESPACE dbcppp::
      DESTINATION lib/cmake/dbcppp
  )

  include(CMakePackageConfigHelpers)

# Generate a version file for the package configuration
  write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/dbcpppConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
  )

# Configure the package configuration file.
# Create a file called "cmake/dbcpppConfig.cmake.in" in your source tree with the contents shown below.
  configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/dbcpppConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/dbcpppConfig.cmake"
    INSTALL_DESTINATION lib/cmake/dbcppp
  )

# Install the package configuration files
  install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/dbcpppConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/dbcpppConfigVersion.cmake"
    DESTINATION lib/cmake/dbcppp
  )
endif()


# ADDITIONAL: Tools, Tests & Examples

if (DBCPPP_build_tools)
  add_subdirectory(third-party/cxxopts)
  add_subdirectory(tools/dbcppp)
endif()

if (DBCPPP_build_tests)
  add_subdirectory(tests)
endif()

if (DBCPPP_build_examples)
  add_subdirectory(examples)
endif()

# PACKAGE (useful for debugging install, use make package)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY NO)
set(CPACK_VERBATIM_VARIABLES YES)
set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_IGNORE_FILES \\.git/ build/ ".*~$")
include(CPack)
