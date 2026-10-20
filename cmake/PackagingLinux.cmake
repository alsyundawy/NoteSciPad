message(STATUS "Configuring Linux packaging (.deb, .rpm, AppImage)")

# Standard installation paths for Linux desktop
install(TARGETS SparkNotePad
    RUNTIME DESTINATION bin
)
install(FILES
    ${PROJECT_SOURCE_DIR}/deploy/linux/SparkNotePad.desktop
    DESTINATION share/applications
)
install(FILES
    ${PROJECT_SOURCE_DIR}/icon/SparkNotePad.svg
    DESTINATION share/icons/hicolor/scalable/apps
)
install(FILES
    ${PROJECT_SOURCE_DIR}/icon/SparkNotePad.svg
    DESTINATION share/icons/hicolor/scalable/mimetypes
)

# ─── CPack Configuration (.deb & .rpm) ─────────────────────────────
set(CPACK_PACKAGE_NAME "sparknotepad")
set(CPACK_PACKAGE_VENDOR "SparkNotePad Contributors")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Lightweight Scintilla-based text and source code editor for macOS and Linux")
set(CPACK_PACKAGE_DESCRIPTION "SparkNotePad is a fast, versatile text and source code editor built on Scintilla and Lexilla.")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_CONTACT "Harry DS Alsyundawy <alsyundawy@gmail.com>")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/alsyundawy/SparkNotePad")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")

# Architecture resolution for Linux packages
if(NOT DEFINED LINUX_PKG_ARCH OR LINUX_PKG_ARCH STREQUAL "")
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|amd64")
        set(DEB_ARCH "amd64")
        set(RPM_ARCH "x86_64")
    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|arm64")
        set(DEB_ARCH "arm64")
        set(RPM_ARCH "aarch64")
    else()
        set(DEB_ARCH "${CMAKE_SYSTEM_PROCESSOR}")
        set(RPM_ARCH "${CMAKE_SYSTEM_PROCESSOR}")
    endif()
endif()

# Debian (.deb)
set(CPACK_DEBIAN_PACKAGE_NAME "sparknotepad")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "${DEB_ARCH}")
set(CPACK_DEBIAN_PACKAGE_SECTION "editors")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "xdg-utils")
set(CPACK_DEBIAN_FILE_NAME "sparknotepad_${PROJECT_VERSION}_${DEB_ARCH}.deb")

# Red Hat (.rpm)
set(CPACK_RPM_PACKAGE_NAME "sparknotepad")
set(CPACK_RPM_PACKAGE_ARCHITECTURE "${RPM_ARCH}")
set(CPACK_RPM_PACKAGE_GROUP "Applications/Editors")
set(CPACK_RPM_PACKAGE_LICENSE "GPL-3.0-or-later")
set(CPACK_RPM_PACKAGE_AUTOREQ ON)
set(CPACK_RPM_PACKAGE_AUTOPROV ON)
set(CPACK_RPM_FILE_NAME "sparknotepad-${PROJECT_VERSION}-1.${RPM_ARCH}.rpm")

# Convenient custom build targets
add_custom_target(deb
    COMMAND ${CMAKE_CPACK_COMMAND} -G DEB
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS SparkNotePad
)

add_custom_target(rpm
    COMMAND ${CMAKE_CPACK_COMMAND} -G RPM
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS SparkNotePad
)

include(CPack)

# ─── AppImage Support (Optional when APP_DISTRIBUTION=AppImage) ────
if(APP_DISTRIBUTION STREQUAL "AppImage")
    set(APPDIR ${CMAKE_BINARY_DIR}/AppDir)
    set(APPDIR_USR ${APPDIR}/usr)
    set(LINUXDEPLOY ${CMAKE_BINARY_DIR}/linuxdeploy-x86_64.AppImage)
    set(LINUXDEPLOY_QT ${CMAKE_BINARY_DIR}/linuxdeploy-plugin-qt-x86_64.AppImage)
    set(APPIMAGE_ENV_VARS LDAI_OUTPUT=SparkNotePad-v${PROJECT_VERSION}-x86_64.AppImage)

    if(DEFINED ENV{QMAKE} AND NOT "$ENV{QMAKE}" STREQUAL "")
        set(APPIMAGE_QMAKE "$ENV{QMAKE}")
    else()
        find_program(APPIMAGE_QMAKE NAMES qmake)
    endif()

    if(APPIMAGE_QMAKE)
        list(APPEND APPIMAGE_ENV_VARS QMAKE=${APPIMAGE_QMAKE})
        add_custom_target(appdir
            COMMAND ${CMAKE_COMMAND} --install . --prefix ${APPDIR_USR}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            DEPENDS SparkNotePad
        )
        add_custom_target(download_linuxdeploy
            COMMAND wget --no-verbose -O ${LINUXDEPLOY} https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
            COMMAND wget --no-verbose -O ${LINUXDEPLOY_QT} https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage
            COMMAND chmod +x ${LINUXDEPLOY} ${LINUXDEPLOY_QT}
        )
        add_custom_target(appimage
            COMMAND ${CMAKE_COMMAND} -E env ${APPIMAGE_ENV_VARS} ${LINUXDEPLOY}
                --appdir ${APPDIR}
                --executable ${APPDIR_USR}/bin/SparkNotePad
                --desktop-file ${APPDIR_USR}/share/applications/SparkNotePad.desktop
                --icon-file ${APPDIR_USR}/share/icons/hicolor/scalable/apps/SparkNotePad.svg
                --plugin qt
                --output appimage
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            DEPENDS appdir download_linuxdeploy
        )
    endif()
endif()
