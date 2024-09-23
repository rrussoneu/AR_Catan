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
        AR_Objs/ARObject.h
        AR_Objs/Aruco/ArucoObject.h
        AR_Objs/Aruco/HomographyObj.h
        AR_Objs/Aruco/GamePieces/GamePiece.h
        AR_Objs/Aruco/GamePieces/PlayerPieces/PlayerPiece.h
        AR_Objs/Aruco/GamePieces/NonPlayerPieces/NonPlayerPiece.h
        AR_Objs/Aruco/ResCards/ResourceCard.h
        AR_Objs/Aruco/DevCards/DevelopmentCard.h
        AR_Objs/Aruco/Tiles/HexTile.h
        AR_Objs/Aruco/GamePieces/NonPlayerPieces/Robber.h
        AR_Objs/Aruco/GamePieces/PlayerPieces/City.h
        AR_Objs/Aruco/GamePieces/PlayerPieces/Settlement.h
        AR_Objs/Aruco/GamePieces/PlayerPieces/Road.h
        AR_Objs/ARObjectFactory.h
        Model/GameModel.h
        Model/InitARObjects.cpp
        AR_Objs/Aruco/DevCards/KnightCard.h
        AR_Objs/Aruco/DevCards/MonopolyCard.h
        AR_Objs/Aruco/DevCards/YearOfPlentyCard.h
        AR_Objs/Aruco/DevCards/RoadBuilding.h
        AR_Objs/Aruco/DevCards/VictoryPointCard.h
        AR_Objs/Aruco/ResCards/WoodResCard.h
        AR_Objs/Aruco/ResCards/BrickResCard.h
        AR_Objs/Aruco/ResCards/OreResCard.h
        AR_Objs/Aruco/ResCards/WheatResCard.h
        AR_Objs/Aruco/ResCards/WoolResCard.h
        AR_Objs/Aruco/Tiles/WoodHex.h
        AR_Objs/Aruco/Tiles/BrickHex.h
        AR_Objs/Aruco/Tiles/OreHex.h
        AR_Objs/Aruco/Tiles/WheatHex.h
        AR_Objs/Aruco/Tiles/WoolHex.h
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
