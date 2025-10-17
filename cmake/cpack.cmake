# CPack configuration for creating installers and source distributions

set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VENDOR "Dhruvan Gnanadhandayuthapani")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A lightweight, high-performance charting library")
set(CPACK_PACKAGE_DESCRIPTION "A lightweight, high-performance charting library built with C++ and Qt.")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/dhruvan2006/QTradingView")
set(CPACK_PACKAGE_CONTACT "dhruvan2006@gmail.com")

# License
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

# Source distribution
set(CPACK_SOURCE_GENERATOR "TGZ;ZIP")
set(CPACK_SOURCE_IGNORE_FILES
        "/.git/"
        "/build/"
        "/cmake-build-.*/"
        "/.idea/"
        "/.vscode/"
        "/\\\\..*"
        "/__pycache__/"
        "\\\\.pyc$"
        "\\\\.swp$"
        "\\\\.swo$"
        "*~"
)

# Platform-specific settings
if(WIN32)
    set(CPACK_GENERATOR "ZIP;WIX;NSIS")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    set(CPACK_NSIS_DISPLAY_NAME "${PROJECT_NAME}")
    set(CPACK_NSIS_PACKAGE_NAME "${PROJECT_NAME}")

    # Include Visual C++ redistributables
    include(InstallRequiredSystemLibraries)

elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop;TGZ;ZIP")
    set(CPACK_DMG_FORMAT "UDBZ")
    set(CPACK_DMG_VOLUME_NAME "${PROJECT_NAME}")

elseif(UNIX)
    set(CPACK_GENERATOR "DEB;RPM;TGZ;ZIP")
    set(CPACK_DEB_PACKAGE_SHLIBDEPS ON)
    set(CPACK_DEB_COMPONENT_INSTALL ON)
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt6core6, libqt6gui6, libqt6widgets6")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "${CPACK_PACKAGE_HOMEPAGE_URL}")

    set(CPACK_RPM_PACKAGE_REQUIRES "qt6-qtbase-libs")
    set(CPACK_RPM_PACKAGE_URL "${CPACK_PACKAGE_HOMEPAGE_URL}")
endif()

# Override Darwin label for macOS builds
set(CPACK_SYSTEM_NAME "${CMAKE_SYSTEM_NAME}")
if(${CPACK_SYSTEM_NAME} STREQUAL "Darwin")
    set(CPACK_SYSTEM_NAME "macOS")
endif()

# Common settings
set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_COMPONENT_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-${CPACK_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

include(CPack)
