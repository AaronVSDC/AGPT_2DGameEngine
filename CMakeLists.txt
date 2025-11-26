cmake_minimum_required (VERSION 3.16)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()




project(2DGameEngine LANGUAGES CXX)  

include(FetchContent)

# ----------------------------------------------------------------------------------
# BOX2D
# ----------------------------------------------------------------------------------
FetchContent_Declare(
    box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG v3.1.1 
)
 
FetchContent_MakeAvailable(box2d) 

# ----------------------------------------------------------------------------------
# SDL3
# ----------------------------------------------------------------------------------
FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.2.22
) 
FetchContent_MakeAvailable(SDL3)

# ----------------------------------------------------------------------------------
# include all files needed over here (.cpp files or .h files if there are no .cpp )
# ----------------------------------------------------------------------------------
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.h" 
)

add_executable (2DGameEngine ${SOURCES})


# ----------------------------------------------------------------------------------
# Auto–collect all header directories so you can #include without relative paths
# ----------------------------------------------------------------------------------
file(GLOB_RECURSE HEADER_FILES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/*.h"
)

set(HEADER_DIRS "")
foreach(HDR ${HEADER_FILES})
  get_filename_component(HDR_DIR ${HDR} DIRECTORY)
  list(APPEND HEADER_DIRS ${HDR_DIR})
endforeach()
list(REMOVE_DUPLICATES HEADER_DIRS)

target_include_directories(2DGameEngine 
  PRIVATE 
    ${HEADER_DIRS}
)

target_link_libraries(2DGameEngine PRIVATE
    box2d
    SDL3::SDL3
)

#Copy SDL3.dll next to the executable after build
add_custom_command(TARGET 2DGameEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SDL3::SDL3>
    $<TARGET_FILE_DIR:2DGameEngine>)


    if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET 2DGameEngine PROPERTY CXX_STANDARD 20)
endif()