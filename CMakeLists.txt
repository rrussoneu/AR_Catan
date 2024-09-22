cmake_minimum_required(VERSION 3.26)
project(AR_Settlers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets OpenGLWidgets REQUIRED)
find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SQLite3 REQUIRED)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${OpenCV_INCLUDE_DIRS}
        ${Qt6Widgets_INCLUDE_DIRS}
        ${Qt6OpenGLWidgets_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIR}
        ${SQLite3_INCLUDE_DIRS}
)

add_executable(AR_Settlers main.cpp
        Video/VideoInput.h
        Video/CameraInput.h
        Video/VideoCaptureThread.h
        View/GUIView.cpp
        View/GUIView.h
        Video/RTSPInput.h
)

target_link_libraries(AR_Settlers
        Qt6::Widgets
        Qt6::OpenGLWidgets
        ${OpenCV_LIBS}
        ${OPENGL_LIBRARIES}
        ${SQLite3_LIBRARIES})

# Set RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_MACOSX_RPATH 1)
