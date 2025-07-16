cmake_minimum_required(VERSION 3.16)

project(GHUBSerial VERSION 1.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Quick
    SerialPort
    Widgets
    LabsPlatform
)

qt_standard_project_setup(REQUIRES 6.8)

add_subdirectory(3rdparty/kdsingleapplication)

qt_add_executable(GHUBSerial
    main.cpp
    crc32.h
    crc32.cpp
    resources.rc
)

qt_add_qml_module(GHUBSerial
    URI GHUBSerial
    VERSION 1.0
    QML_FILES
        Main.qml
    RESOURCES
        res/TrayIcon.ico
    SOURCES
        serialportinfomodel.h
        serialportinfomodel.cpp
        globalkeyboardhook.h
        globalkeyboardhook.cpp
        serialportmanager.h
        serialportmanager.cpp
        autostartmanager.h
        autostartmanager.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(GHUBSerial PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appGHUB_Serial
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_link_libraries(GHUBSerial PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::SerialPort
    Qt6::Widgets
    Qt6::LabsPlatform
    kdsingleapplication
)

include(GNUInstallDirs)
install(TARGETS GHUBSerial
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT MainApp
)

install(IMPORTED_RUNTIME_ARTIFACTS
    kdsingleapplication
    COMPONENT MainApp
)

qt_generate_deploy_qml_app_script(
    TARGET GHUBSerial
    OUTPUT_SCRIPT deploy_script
    NO_TRANSLATIONS
)
install(SCRIPT ${deploy_script} COMPONENT MainApp)

include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "Joker")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Ghub Serial")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})

set(CPACK_GENERATOR "IFW")
set(CPACK_IFW_ROOT "C:/Qt/Tools/QtInstallerFramework/4.10")
set(CPACK_IFW_PACKAGE_TITLE "${PROJECT_NAME} Installer")
set(CPACK_IFW_PACKAGE_PUBLISHER "Joker")
set(CPACK_IFW_PACKAGE_WIZARD_STYLE "Modern")
set(CPACK_IFW_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/res/TrayIcon.ico")
set(CPACK_IFW_PACKAGE_LOGO "${CMAKE_SOURCE_DIR}/res/TrayIcon.ico")

include(CPack)
include(CPackIFW)

cpack_add_component(MainApp
    DISPLAY_NAME "${PROJECT_NAME}"
    DESCRIPTION "Main application"
    REQUIRED
)

cpack_ifw_configure_component(MainApp
    SCRIPT "${CMAKE_SOURCE_DIR}/installscript.qs"
    VERSION "${CPACK_PACKAGE_VERSION}"
)
