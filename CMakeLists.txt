project(peruse)
cmake_minimum_required(VERSION 3.1.0)

set(KF5_DEP_VERSION "5.75.0") # handled by release scripts

if(POLICY CMP0063)
    cmake_policy (SET CMP0063 NEW)
endif(POLICY CMP0063)

option(USE_PERUSE_PDFTHUMBNAILER "Use the internal thumbnail generator for PDF files (defaults to true on windows)" OFF)
if(WIN32)
    set(USE_PERUSE_PDFTHUMBNAILER ON)
endif()

find_package(ECM ${KF5_DEP_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${ECM_MODULE_PATH})

# Android has no DBus, so we can't use that. Sniff things and tell the code
if(CMAKE_SYSTEM_NAME STREQUAL Android)
    message("Building for Android - this means no dbus, and other small details. Work with that")
    add_definitions(-DANDROID)
    SET(QT_QMAKE_EXECUTABLE "$ENV{Qt5_android}/bin/qmake")
elseif(WIN32)
    message("Building for Windows - this means no dbus, and other small details. Work with that")
    add_definitions(-DWINDOWS)
endif()

find_package(Qt5 5.14 REQUIRED NO_MODULE COMPONENTS Qml Quick Gui Widgets OpenGL Sql)
find_package(KF5 ${KF5_DEP_VERSION} REQUIRED COMPONENTS Archive I18n Declarative Config Crash FileMetaData GuiAddons IconThemes KIO NewStuffQuick)

find_package(KF5Baloo ${KF5_DEP_VERSION}) # optional - we'll just let it pass if it's missing

find_package(KF5Kirigami2 ${KF5_DEP_VERSION})
set_package_properties(KF5Kirigami2 PROPERTIES
    PURPOSE "Required at runtime to display the GUI"
    TYPE RUNTIME
)

find_package(ZLIB)
set_package_properties(ZLIB PROPERTIES
    PURPOSE "Required for the unarr based rar support used for reading books in the CBR format"
    TYPE REQUIRED
)

include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(GenerateExportHeader)
include(ECMInstallIcons)
include(ECMAddAppIcon)
include(ECMQtDeclareLoggingCategory)

set(CMAKE_AUTORCC ON)

kde_enable_exceptions()

add_subdirectory(src)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
