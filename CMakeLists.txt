cmake_minimum_required(VERSION 3.16)
project(Papyrus LANGUAGES C CXX)

include(FetchContent)

# -----------------------------
# Dependencies
# -----------------------------
FetchContent_Declare(
  box2d
  GIT_REPOSITORY https://github.com/erincatto/box2d.git
  GIT_TAG v3.1.1
)
FetchContent_MakeAvailable(box2d)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.1
)
FetchContent_MakeAvailable(glm)

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.2.22
)
FetchContent_MakeAvailable(SDL3)

add_library(glad STATIC 3rdParty/glad/src/glad.c)
target_include_directories(glad PUBLIC 3rdParty/glad/include)

add_library(stb_image STATIC 3rdParty/stb_image/stb_image.cpp)
target_include_directories(stb_image PUBLIC 3rdParty/stb_image)

# -----------------------------
# Engine (DLL)
# -----------------------------
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/Source/Engine/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Source/Engine/*.h"
)

add_library(PapyrusEngine SHARED ${ENGINE_SOURCES})
target_compile_definitions(PapyrusEngine PRIVATE PAPYRUS_ENGINE_BUILD_DLL)

# Make every engine header directory visible (so includes like "GameObject.h" work)
file(GLOB_RECURSE ENGINE_HEADERS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/Source/Engine/*.h"
)

set(ENGINE_HEADER_DIRS "")
foreach(HDR ${ENGINE_HEADERS})
  get_filename_component(DIR ${HDR} DIRECTORY)
  list(APPEND ENGINE_HEADER_DIRS ${DIR})
endforeach()
list(REMOVE_DUPLICATES ENGINE_HEADER_DIRS)

target_include_directories(PapyrusEngine
  PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Engine"
    ${ENGINE_HEADER_DIRS}  
)  

target_link_libraries(PapyrusEngine  
  PUBLIC
    box2d
    SDL3::SDL3
    glm::glm-header-only
    glad
    stb_image
)

# -----------------------------
# Game (EXE)
# -----------------------------
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/*.h"
)

add_executable(Game ${GAME_SOURCES})

# Make every game header directory visible too (so includes like "AnimationComponent.h" work)
file(GLOB_RECURSE GAME_HEADERS CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/Source/Game/*.h"
)

set(GAME_HEADER_DIRS "")
foreach(HDR ${GAME_HEADERS})
  get_filename_component(DIR ${HDR} DIRECTORY)
  list(APPEND GAME_HEADER_DIRS ${DIR})
endforeach()
list(REMOVE_DUPLICATES GAME_HEADER_DIRS)

target_include_directories(Game
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/Source/Game"
    ${GAME_HEADER_DIRS}
) 

target_link_libraries(Game PRIVATE PapyrusEngine)

# Copy runtime DLLs next to Game.exe (PapyrusEngine.dll, SDL3.dll, etc.)
add_custom_command(TARGET Game POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_RUNTIME_DLLS:Game>
    $<TARGET_FILE_DIR:Game>
  COMMAND_EXPAND_LISTS 
)

# Copy Resources folder next to Game.exe
set(RESOURCE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Resources")
set(RESOURCE_BINARY_DIR "$<TARGET_FILE_DIR:Game>/Resources")

add_custom_command(TARGET Game POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "${RESOURCE_BINARY_DIR}"
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${RESOURCE_SOURCE_DIR}"
          "${RESOURCE_BINARY_DIR}"
)

# C++ standard
set_property(TARGET PapyrusEngine PROPERTY CXX_STANDARD 20)
set_property(TARGET PapyrusEngine PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET Game         PROPERTY CXX_STANDARD 20)
set_property(TARGET Game         PROPERTY CXX_STANDARD_REQUIRED ON)
