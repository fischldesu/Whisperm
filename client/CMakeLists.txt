cmake_minimum_required(VERSION 3.20)
set(ProjName Whisperm)
project(${ProjName} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(OPENSSL_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/OpenSSL_v3)
set(OPENSSL_INCLUDE_DIR ${OPENSSL_ROOT_DIR}/include)
set(OPENSSL_CRYPTO_LIBRARY ${OPENSSL_ROOT_DIR}/crypto)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Svg WebSockets Concurrent openssl)
find_package(Qt6 REQUIRED COMPONENTS Widgets Svg WebSockets Concurrent)

include_directories(header)
include_directories(${OPENSSL_INCLUDE_DIR})
set(CMAKE_DISABLE_FIND_PACKAGE_OpenSSL TRUE)
set(Headers
        whisperm.h
        header/Client.h

        header/ui/Window.h
        header/ui/MainWindow.h
        header/ui/LoginDialog.h
        header/ui/PageChats.h
        header/ui/PageContacts.h
        header/ui/PageSettings.h
        header/ui/MessageDialog.h

        header/tools/multiplatform.hpp
        header/tools/Data.h
        header/tools/AppLog.h
        header/tools/Media.h
        header/tools/Style.h
        header/tools/Async.h
        header/tools/Animation.h
        header/tools/Cryptology.h
        header/tools/BigInteger.h

        header/uicomponent/TitleBar.h
        header/uicomponent/MainContainer.h
        header/uicomponent/Button.h
        header/uicomponent/ChatListItem.h
        header/uicomponent/ChatMessage.h
        header/uicomponent/ChatBox.h
        header/uicomponent/Chat.h
        header/uicomponent/SystemTrayMenu.h
)

set(PROJECT_SOURCES
        ${Headers}
        main.cpp
        source/Data.cpp
        source/Client.cpp
        source/Window.cpp
        assets/cr.qrc

        utils/multiplatform.cpp
        utils/AppLog.cpp
        utils/Media.cpp
        utils/Style.cpp
        utils/Animation.cpp
        utils/Cryptology.cpp
        utils/BigInteger.cpp

        ui/component/TitleBar.cpp
        ui/MainWindow.cpp
        ui/MainWindow.ui
        ui/LoginDialog.ui
        ui/LoginDialog.cpp

        ui/page/PageChats.cpp
        ui/page/PageContacts.cpp
        ui/page/PageChats.ui
        ui/page/PageContacts.ui
        ui/page/PageSettings.cpp
        ui/page/PageSettings.ui

        ui/component/Button.cpp
        ui/component/MainContainer.cpp
        ui/component/ChatListItem.cpp
        ui/component/ChatBox.cpp
        ui/component/ChatMessage.cpp
        ui/component/Chat.cpp
        ui/component/MessageDialog.cpp
        ui/component/SystemTrayMenu.cpp

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${ProjName} MANUAL_FINALIZATION ${PROJECT_SOURCES})
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ${ProjName} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${ProjName} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${ProjName}
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(${ProjName} PRIVATE 
    Qt6::Widgets
    Qt6::Svg
    Qt6::WebSockets
        Qt6::Concurrent
    )
if (MSVC)
    target_compile_definitions(${ProjName} PRIVATE NOMINMAX)
endif()
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${ProjName})
endif()
set_target_properties(${ProjName} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${ProjName}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${ProjName})
endif()
