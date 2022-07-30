cmake_minimum_required(VERSION 3.5)

set(APP DanMuShooter)
project(${APP} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network WebSockets ZlibPrivate Multimedia)

set(QRC_SOURCE_FILES 
        src/modules/SummaryWidgetModule/image.qrc
        src/modules/BSdk/BSdkImage.qrc
)
qt6_add_resources(QRC_FILES ${QRC_SOURCE_FILES})
SOURCE_GROUP("Resource Files" FILES ${QRC_FILES})


include_directories(${APP} PRIVATE ./)
include_directories(${APP} PRIVATE ./thirdParty)
include_directories(${APP} PRIVATE ./src)
include_directories(${APP} PRIVATE ./src/Scenes)

include_directories(${APP} PRIVATE ./src/modules/BSdk)
include_directories(${APP} PRIVATE ./src/modules/MusicPlayModule)
include_directories(${APP} PRIVATE ./src/modules/SummaryWidgetModule)

set(THIRD_PARTY_SOURCES
        thirdParty/fmt/os.cc
        thirdParty/fmt/format.cc

        thirdParty/base64.cpp 

        # thirdParty/box2d-lite/Arbiter.cpp
        # thirdParty/box2d-lite/Body.cpp
        # thirdParty/box2d-lite/Collide.cpp
        # thirdParty/box2d-lite/Joint.cpp
        # thirdParty/box2d-lite/World.cpp
)


set(PROJECT_SOURCES
        main.cpp
        GameWidget.cpp
        GameWidget.ui
        src/CxhUtils.cpp
        src/AStarNode.cpp

        src/ai/PlayerStateMachine.cpp

        src/managers/GameManager.cpp
        src/managers/GameSettings.cpp
        src/managers/SystemManager.cpp
        src/managers/PlayerManager.cpp
        src/managers/BulletManager.cpp
        src/managers/DanMuManager.cpp

        src/ecs/components/ComponentAngle.cpp
        src/ecs/components/ComponentBullet.cpp
        src/ecs/components/ComponentHealth.cpp
        src/ecs/components/ComponentFirearmsWeapon.cpp
        src/ecs/components/ComponentMoveStack.cpp

        src/ecs/systems/System.cpp
        src/ecs/systems/SystemCamera.cpp
        src/ecs/systems/SystemPlayer.cpp
        src/ecs/systems/SystemFindRound.cpp
        src/ecs/systems/SystemGamePrepare.cpp
        src/ecs/systems/SystemGameDanMu.cpp
        src/ecs/systems/SystemAngle.cpp
        src/ecs/systems/SystemAnimation.cpp
        src/ecs/systems/SystemBullet.cpp
        src/ecs/systems/SystemMove.cpp
        src/ecs/systems/SystemFirearmsWeapon.cpp

        src/ecs/IntermediateProcess.cpp

        src/res/GameSource.cpp
        src/res/SourceUtils.cpp
        src/res/GameMap.cpp

        src/scenes/GameGui.cpp
        src/scenes/GameScene.cpp

        ${QRC_FILES}
)

set(PROJECT_INCLUDE
        GameWidget.h
        # src/GameStatue.h
        src/Vector2.hpp
        src/Point2.hpp
        src/Size2.hpp
        src/CxhUtils.h
        src/AStarNode.h
        src/GameStack.hpp
        src/Color3.hpp
        src/Color4.hpp
        src/GameStatue.h
        src/DanMuData.h

        src/ai/PlayerStateMachine.h

        src/managers/GameManager.h
        src/managers/GameSettings.h
        src/managers/SystemManager.h
        src/managers/PlayerManager.h
        src/managers/BulletManager.h
        src/managers/DanMuManager.h

        src/ecs/components/ComponentCamera.hpp
        src/ecs/components/ComponentBase.hpp
        src/ecs/components/ComponentAnimation.hpp
        src/ecs/components/ComponentAngle.h
        src/ecs/components/ComponentBullet.h
        src/ecs/components/ComponentHealth.h
        src/ecs/components/ComponentFirearmsWeapon.h
        src/ecs/components/ComponentMoveStack.h

        src/ecs/systems/System.h
        src/ecs/systems/SystemCamera.h
        src/ecs/systems/SystemPlayer.h
        src/ecs/systems/SystemFindRound.h
        src/ecs/systems/SystemGamePrepare.h
        src/ecs/systems/SystemGameDanMu.h
        src/ecs/systems/SystemAngle.h
        src/ecs/systems/SystemAnimation.h
        src/ecs/systems/SystemBullet.h
        src/ecs/systems/SystemMove.h
        src/ecs/systems/SystemFirearmsWeapon.h

        src/ecs/IntermediateProcess.h

        src/res/GameSource.h
        src/res/SourceUtils.h
        src/res/GameMap.h
        src/res/Animation.h

        src/scenes/GameGui.h
        src/scenes/GameScene.h
)

set(PROJECT_MODULES
        src/modules/BSdk/ApiInfo.h
        src/modules/BSdk/ApiInfo.cpp
        src/modules/BSdk/BApi.h
        src/modules/BSdk/BApi.cpp
        src/modules/BSdk/BilibiliGetCodeDialog.h
        src/modules/BSdk/BilibiliGetCodeDialog.cpp
        src/modules/BSdk/BilibiliGetCodeDialog.ui
        src/modules/BSdk/BSdk.h
        src/modules/BSdk/BSdk.cpp
        src/modules/BSdk/DanmaKuWebSocket.h
        src/modules/BSdk/DanmaKuWebSocket.cpp

        src/modules/MusicPlayModule/MusicPlayModule.ui
        src/modules/MusicPlayModule/MusicPlayModule.h
        src/modules/MusicPlayModule/MusicPlayModule.cpp
        src/modules/MusicPlayModule/CommonUtils.h
        src/modules/MusicPlayModule/CommonUtils.cpp

        src/modules/SummaryWidgetModule/InvestWidget.h
        src/modules/SummaryWidgetModule/InvestWidget.cpp
        src/modules/SummaryWidgetModule/ScoreData.hpp
        src/modules/SummaryWidgetModule/ScoreWidget.h
        src/modules/SummaryWidgetModule/ScoreWidget.cpp
        src/modules/SummaryWidgetModule/SummaryWidgetModule.h
        src/modules/SummaryWidgetModule/SummaryWidgetModule.cpp
        src/modules/SummaryWidgetModule/SummaryWidgetModule.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${APP}
        MANUAL_FINALIZATION
        ${THIRD_PARTY_SOURCES}
        ${PROJECT_INCLUDE}
        ${PROJECT_SOURCES}
        ${PROJECT_MODULES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET DanMuShooter APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${APP} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${APP}
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(${APP} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(${APP} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt6::Network Qt6::WebSockets Qt6::ZlibPrivate Qt6::Multimedia)

set_target_properties(${APP} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${APP})
endif()
