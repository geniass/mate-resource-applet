cmake_minimum_required (VERSION 2.6)
project (mate-resource-monitor)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(GTK2 REQUIRED gtk)
INCLUDE_DIRECTORIES(${GTK2_INCLUDE_DIRS})
IF (NOT GTK2_FOUND)
    MESSAGE (FATAL_ERROR "GTK NOT FOUND")
ENDIF (NOT GTK2_FOUND)

find_package(GLIB2 REQUIRED)
INCLUDE_DIRECTORIES(${GLIB2_INCLUDE_DIRS})
#IF (NOT Glib_FOUND)
#    MESSAGE (FATAL_ERROR "Glib NOT FOUND")
#ENDIF (NOT Glib_FOUND)

find_package(Libmatepanelapplet-3.0 REQUIRED)
INCLUDE_DIRECTORIES(${Libmatepanelapplet-3.0_INCLUDE_DIRS})
IF (NOT Libmatepanelapplet-3.0_FOUND)
    MESSAGE(FATAL_ERROR "MATE PANEL not found")
ENDIF (NOT Libmatepanelapplet-3.0_FOUND)

#Version number
set(Applet_VERSION_MAJOR 1)
set(Applet_VERSION_MINOR 0)
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/resource-applet.h.in"
    "${PROJECT_BINARY_DIR}/resource-applet.h")

set(MATE_APPLETS /usr/lib/mate-applets)
# configure mate applet file and dbus. Hardcoded for now
configure_file (
    "${PROJECT_SOURCE_DIR}/org.mate.applets.ResourceApplet.mate-panel-applet.in"
    "${PROJECT_BINARY_DIR}/org.mate.applets.ResourceApplet.mate-panel-applet")
configure_file (
    "${PROJECT_SOURCE_DIR}/org.mate.panel.applet.ResourceAppletFactory.service.in"
    "${PROJECT_BINARY_DIR}/org.mate.panel.applet.ResourceAppletFactory.service")

include_directories("${PROJECT_BINARY_DIR}")

add_library(MateApplet resource-applet.c)

add_executable(mate-resource-monitor main.c)
TARGET_LINK_LIBRARIES(mate-resource-monitor ${GTK2_LIBRARIES})
TARGET_LINK_LIBRARIES(mate-resource-monitor ${GLIB2_LIBRARIES})
TARGET_LINK_LIBRARIES(mate-resource-monitor ${Libmatepanelapplet-3.0_LIBRARIES})
TARGET_LINK_LIBRARIES(mate-resource-monitor MateApplet)

install(TARGETS mate-resource-monitor DESTINATION lib/mate-applets)
install(FILES ${PROJECT_BINARY_DIR}/org.mate.applets.ResourceApplet.mate-panel-applet DESTINATION share/mate-panel/applets)
install(FILES ${PROJECT_BINARY_DIR}/org.mate.panel.applet.ResourceAppletFactory.service DESTINATION share/dbus-1/services)

