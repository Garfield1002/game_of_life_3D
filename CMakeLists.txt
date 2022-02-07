cmake_minimum_required (VERSION 3.8)

# Replace by your project name ------------------------------------------------
project ("game_of_life_3D")

# Source files
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(LIB_DIR "${CMAKE_SOURCE_DIR}/libraries")

set(SOURCES 
	"${SRC_DIR}/shader/shader.h"
	"${SRC_DIR}/shader/shader.cpp"
	"${SRC_DIR}/main.cpp"
	
	"${SRC_DIR}/camera/camera.h"
	"${SRC_DIR}/camera/camera.cpp"
	"${SRC_DIR}/util/raycasting.h"
	"${SRC_DIR}/render/line.h"
	"${SRC_DIR}/render/line.cpp"
	"${SRC_DIR}/geometry/box.h"
	"${SRC_DIR}/geometry/box.cpp"
	"${SRC_DIR}/render/renderer.h"
	"${SRC_DIR}/render/box_renderer.h"
	"${SRC_DIR}/render/box_renderer.cpp"
	"${SRC_DIR}/geometry/rbox.h"
	 "src/render/frame_renderer.h"
	 "src/render/frame_renderer.cpp"
	 "src/render/font_renderer.h"
	 "src/render/font_renderer.cpp"
	 "src/render/crosshairs.h"
	 "src/render/crosshairs.cpp"
	 "src/render/debug_crosshairs.h"
	 "src/render/debug_crosshairs.cpp"
	  "src/util/raycasting.cpp" "src/geometry/rbox.cpp" "src/window.h" "src/window.cpp" "src/world.h" "src/world.cpp" "src/player.h" "src/player.cpp" "src/settings.h" "src/ui/terminal.h" "src/ui/terminal.cpp")

# Add source to this project's executable.
add_executable (${PROJECT_NAME} ${SOURCES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

# Copy ressource files
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)

# GLFW
set(GLFW_DIR "${LIB_DIR}/glfw")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLFW_DIR}")
target_link_libraries(${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

# GLAD
set(GLAD_DIR "${LIB_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

# GLM
set(GLM_DIR "${LIB_DIR}/glm")
add_subdirectory("${GLM_DIR}")
target_link_libraries(${PROJECT_NAME} "glm" "${GLM_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLM_DIR}/glm")

## FreeType
set(FREETYPE_DIR "${LIB_DIR}/freetype")
add_subdirectory("${FREETYPE_DIR}")
target_link_libraries(${PROJECT_NAME} "freetype" "${FREETYPE_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${FREETYPE_DIR}/include")

# JHR
set(JHR_DIR "${LIB_DIR}/jhr")
add_library("jhr" INTERFACE)
target_include_directories("jhr" INTERFACE "${JHR_DIR}")
target_sources("jhr" INTERFACE "${JHR_DIR}/jhr_skip_list.h")
target_link_libraries(${PROJECT_NAME} "jhr")


