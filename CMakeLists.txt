cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 17)

project(MiaouRollback)

if (EMSCRIPTEN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s ALLOW_MEMORY_GROWTH=1 --no-heap-copy")
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif ()

find_package(raylib REQUIRED)
find_package(ImGui CONFIG REQUIRED)

if (USE_TRACY)
    # Enable Tracy profiling by setting the preprocessor directive
    add_compile_definitions(TRACY_ENABLE)
    # Include Tracy's headers for the entire project
    include_directories(libs/TracyProfiler/tracy)

    # Add the Tracy source code as part of the project
    add_library(tracyClient STATIC libs/TracyProfiler/TracyClient.cpp)
endif()


#Photon
if (NOT EMSCRIPTEN)
    # Create the photon library.
    file(GLOB_RECURSE PHOTON_SRC_FILES libs/LoadBalancing-cpp/inc/*.h libs/LoadBalancing-cpp/src/*.cpp)
    add_library(photon ${PHOTON_SRC_FILES})
    set_target_properties(photon PROPERTIES LINKER_LANGUAGE CXX)
    target_include_directories(photon PUBLIC libs)
    #ReleaseVersion:
    #   target_link_libraries(photon PUBLIC
    #            ${CMAKE_CURRENT_SOURCE_DIR}/externals/photon/Common-cpp/lib/Common-cpp_vc17_release_windows_md_x64.lib
    #            ${CMAKE_CURRENT_SOURCE_DIR}/externals/photon/Photon-cpp/lib/Photon-cpp_vc17_release_windows_md_x64.lib
    #   )
    #DebugVersion
        target_link_libraries(photon PUBLIC
                ${CMAKE_CURRENT_SOURCE_DIR}/libs/Common-cpp/lib/Common-cpp_vc17_debug_windows_md_x64.lib
                ${CMAKE_CURRENT_SOURCE_DIR}/libs/Photon-cpp/lib/Photon-cpp_vc17_debug_windows_md_x64.lib
        )
endif()



set(data_dir "${CMAKE_SOURCE_DIR}/data")

set(data_files)
file(GLOB_RECURSE DATA_FILES
            "data/*.json"
            "data/*.png"
            "data/*.jpg"
            "data/*.wav"
            )
list(APPEND data_files ${DATA_FILES})

# Create the rlImGui library.
file(GLOB_RECURSE RLIMGUI_FILES libs/rlImGui/include/*.h libs/rlImGui/src/*.cpp)
add_library(rl_imgui ${RLIMGUI_FILES})
set_target_properties(rl_imgui PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(rl_imgui PUBLIC libs/rlImGui/include/)
target_link_libraries(rl_imgui PRIVATE raylib)
add_compile_definitions(NO_FONT_AWESOME)

# Create the Math library
file(GLOB_RECURSE MATH_SRC_FILES libs/math/include/*.h libs/math/src/*.cpp)
add_library(math ${MATH_SRC_FILES})
set_target_properties(math PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(math PUBLIC libs/math/include/)

# Create the Common library with Math as a dependency
file(GLOB_RECURSE COMMON_SRC_FILES common/include/*.h common/src/*.cpp)
add_library(common ${COMMON_SRC_FILES})
set_target_properties(common PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(common PUBLIC common/include/)
target_link_libraries(common PRIVATE math)

# Create the physics library with math and common as dependencies.
file(GLOB_RECURSE PHYSICS_SRC_FILES physics/include/*.h physics/src/*.cpp)
add_library(physics ${PHYSICS_SRC_FILES})
set_target_properties(physics PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(physics PUBLIC physics/include/)
target_include_directories(math PUBLIC libs/math/include/)
target_link_libraries(physics PRIVATE math common)
if (USE_TRACY)
    target_compile_definitions(physics PUBLIC TRACY_ENABLE)
    # Link the TracyClient library
    target_link_libraries(physics PRIVATE tracyClient)
endif()

# Create the game library.
file(GLOB_RECURSE GAME_SRC_FILES game/include/*.h game/src/*.cpp)
add_library(game ${GAME_SRC_FILES})
set_target_properties(game PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(game PUBLIC game/include/)
target_link_libraries(game PRIVATE raylib imgui::imgui rl_imgui math common physics photon)

##add_executable(main main/main.cpp)
##target_link_libraries(main PRIVATE raylib imgui::imgui rl_imgui math common physics game photon)

if (EMSCRIPTEN)
        ## The local resources path needs to be mapped to /data virtual path
        string(APPEND data_dir "@data")
        set_target_properties(main PROPERTIES LINK_FLAGS "--preload-file ${data_dir}")
endif ()

if (NOT EMSCRIPTEN)

 add_executable(main main/main.cpp)
    target_link_libraries(main PRIVATE game raylib imgui::imgui rl_imgui math common physics photon)



endif()

# Copy all of the resource files to the destination
file(COPY ${data_files} DESTINATION "data/")



