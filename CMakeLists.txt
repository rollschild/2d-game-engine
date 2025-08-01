cmake_minimum_required(VERSION 3.29)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(
    FATAL_ERROR
      "Do not build in-source. Please remove CMakeCache.txt and the CMakeFiles/ directory. Then build out-of-source."
  )
endif()

project(
  2d-game-engine
  VERSION 0.1.0
  LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)
find_package(sol2 REQUIRED)

set(CMAKE_C_COMPILER gcc)
if(MSVC)
  # warning level 4
  add_compile_options(/W4)
else()
  # additional warnings
  add_compile_options(-Wall -Wfatal-errors -Wextra -Werror -g -O1)
endif()

if(APPLE)
  add_compile_options(-mmacosx-version-min=10.15)
endif()
# ##############################################################################
# Modules and scripts   ##
# ##############################################################################

# Standard CMake modules

include(CTest) # Must be called before adding tests but after calling project().
               # This automatically calls enable_testing() and configures ctest
               # targets when using Make/Ninja
include(CMakeDependentOption) # This is a really useful scripts that creates
                              # options that depends on other options. It can
                              # even be used with generator expressions !
include(GNUInstallDirs) # This will define the default values for installation
                        # directories (all platforms even if named GNU)
include(InstallRequiredSystemLibraries) # Tell CMake that the `install` target
                                        # needs to install required system
                                        # libraries (eg: Windows SDK)
include(CMakePackageConfigHelpers) # Helper to create relocatable packages

# Custom modules and scripts

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake") # Make our
                                                                 # cmake scripts
                                                                 # available

# include(BuildHelpers) # Helpers for writing cmake files

# ##############################################################################
# Options  ##
# ##############################################################################

option(
  ENABLE_INSTALL
  "Should ${PROJECT_NAME} be added to the install list? Useful if included using add_subdirectory."
  ON)
option(ENABLE_TESTING "Should unit tests be compiled." ON)

set(${PROJECT_NAME}_INSTALL_CMAKEDIR
    "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    CACHE STRING "Path to install ${PROJECT_NAME} Config*.cmake files to.")

# ##############################################################################
# Dependencies  ##
# ##############################################################################

if(ENABLE_TESTING)
  find_package(GTest REQUIRED)
  include(GoogleTest)
  enable_testing()
endif()

# ##############################################################################
# Targets  ##
# ##############################################################################

# Use the project root to find includes
include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(src)
# add_subdirectory(tests)

# ##############################################################################
# Packaging ##
# ##############################################################################

# if(ENABLE_INSTALL) # Use version checking helper provided by CMake so that
# users can # safely use a version number in their find_package calls
# write_basic_package_version_file( ${PROJECT_NAME}ConfigVersion.cmake VERSION
# ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion)
#
# configure_package_config_file( ${PROJECT_SOURCE_DIR}/cmake/Config.cmake.in
# ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake INSTALL_DESTINATION
# ${${PROJECT_NAME}_INSTALL_CMAKEDIR} NO_SET_AND_CHECK_MACRO
# NO_CHECK_REQUIRED_COMPONENTS_MACRO)
#
# install( TARGETS what_time simple EXPORT ${PROJECT_NAME}_Targets INCLUDES
# DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
#
# install(DIRECTORY src DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING
# PATTERN *.h)
#
# # This time, install all the exported targets under the #
# ${PROJECT_NAME}_Targets name. install( EXPORT ${PROJECT_NAME}_Targets
# NAMESPACE ${PROJECT_NAME}:: FILE ${PROJECT_NAME}Targets.cmake DESTINATION
# ${${PROJECT_NAME}_INSTALL_CMAKEDIR})
#
# # So far we only installed the exported targets, now install the package
# config files. # # If you do not list headers in the PUBLIC_HEADER property,
# you will need to copy them using # `install(FILES)` or `install(DIRECTORY)`
# too. # # In that case, you can use CMAKE_INSTALL_INCLUDEDIR as the base
# destination path. install(FILES
# ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
# ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake DESTINATION
# ${${PROJECT_NAME}_INSTALL_CMAKEDIR}) endif()
