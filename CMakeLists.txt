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
find_package(assimp REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${OpenCV_INCLUDE_DIRS}
        ${Qt6Widgets_INCLUDE_DIRS}
        ${Qt6OpenGLWidgets_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIR}
        ${ASSIMP_INCLUDE_DIRS}
)

add_executable(AR_Settlers main.cpp
        Controller/Video/VideoInput.h
        Controller/Video/CameraInput.h
        Controller/Video/VideoCaptureThread.h
        View/GUIView.cpp
        View/GUIView.h
        Controller/Video/RTSPInput.h
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
        Controller/Controller.cpp
        Controller/Controller.h
        Model/InitARObjects.h
        Controller/Processing/ProcessingThread.cpp
        Controller/Processing/ProcessingThread.h
        Model/Player.cpp
        Model/Player.h
        Model/GameModel.cpp
        Controller/Commands/Command.h
        Controller/Commands/UpdateUsernameCommand.h
        Controller/Commands/IncrementScoreCommand.h
        Controller/Commands/DecrementScoreCommand.h
        Controller/PlayerInfoBuilder.h
        AR_Objs/Rendering/RenderStrategy.h
        AR_Objs/Rendering/OpenCVRenderStrategy.h
        Config.h

        AR_Objs/Rendering/ModelLoader.cpp
        AR_Objs/Rendering/ModelLoader.h
        AR_Objs/Rendering/OpenCVRenderStrategy.h
        AR_Objs/Rendering/OpenGLRenderStrategy.cpp
        AR_Objs/Rendering/ModelData.h

)

target_link_libraries(AR_Settlers
        Qt6::Widgets
        Qt6::OpenGLWidgets
        Qt6::Sql
        ${OpenCV_LIBS}
        ${OPENGL_LIBRARIES}
        assimp::assimp)

# Set RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_MACOSX_RPATH 1)
