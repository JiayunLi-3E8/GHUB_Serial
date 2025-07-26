cmake_minimum_required(VERSION 3.16)

project(GHUBSerial VERSION 1.3.1 LANGUAGES CXX)

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

add_compile_definitions(APP_VERSION="${PROJECT_VERSION}")

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
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(IMPORTED_RUNTIME_ARTIFACTS
    kdsingleapplication
)

qt_generate_deploy_qml_app_script(
    TARGET GHUBSerial
    OUTPUT_SCRIPT deploy_script
    NO_TRANSLATIONS
)
install(SCRIPT ${deploy_script})

include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "Joker")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Ghub Serial")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_NAME}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

set(CPACK_GENERATOR "NSIS")
set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_NAME}")
set(CPACK_NSIS_PACKAGE_NAME "${CPACK_PACKAGE_NAME}")
set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/res/TrayIcon.ico")
set(CPACK_NSIS_MUI_UNIICON "${CMAKE_SOURCE_DIR}/res/TrayIcon.ico")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
set(CPACK_NSIS_MENU_LINKS
    "${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}.exe" "${PROJECT_NAME}"
)
set(CPACK_NSIS_CREATE_ICONS_EXTRA "
    CreateShortCut '$DESKTOP\\\\${PROJECT_NAME}.lnk' '$INSTDIR\\\\${CMAKE_INSTALL_BINDIR}\\\\${PROJECT_NAME}.exe'
")
set(CPACK_NSIS_DELETE_ICONS_EXTRA "
    Delete '$DESKTOP\\\\${PROJECT_NAME}.lnk'
")
set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
    WriteRegStr HKLM \\\"Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Uninstall\\\\${PROJECT_NAME}\\\" \\\"DisplayIcon\\\" \\\"$INSTDIR\\\\${CMAKE_INSTALL_BINDIR}\\\\${PROJECT_NAME}.exe,0\\\"
")

include(CPack)
