cmake_minimum_required (VERSION 2.6)
project (mate-resource-monitor)

find_package(GTK2 REQUIRED)
INCLUDE_DIRECTORIES(${GTK2_INCLUDE_DIRS})
IF (NOT GTK2_FOUND)
    MESSAGE (FATAL_ERROR "GTK NOT FOUND")
ENDIF (NOT GTK2_FOUND)

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

add_executable(mate-resource-monitor resource-applet.c)
TARGET_LINK_LIBRARIES(mate-resource-monitor ${GTK2_LIBRARIES})

install(TARGETS mate-resource-monitor DESTINATION lib/mate-applets)
install(FILES ${PROJECT_BINARY_DIR}/org.mate.applets.ResourceApplet.mate-panel-applet DESTINATION share/mate-panel/applets)
install(FILES ${PROJECT_BINARY_DIR}/org.mate.panel.applet.ResourceAppletFactory.service DESTINATION share/dbus-1/services)

