# - Try to find libmatepanelapplet-3.0 (that's not a version number)
# Once done this will define:
#
# Libmatepanelapplet-3.0_FOUND - system has libmatepanelapplet-3.0
# Libmatepanelapplet-3.0_INCLUDE_DIRS - the include dirs
# Libmatepanelapplet-3.0_LIBRARIES - link to these libraries

include (LibFindMacros)

libfind_pkg_check_modules(Libmatepanelapplet-3.0_PKGCONF libmatepanelapplet-3.0)

find_path(Libmatepanelapplet-3.0_INCLUDE_DIR
    NAMES mate-panel-applet.h
    PATHS ${Libmatepanelapplet-3.0_PKGCONF_INCLUDE_DIRS}
)

find_library(Libmatepanelapplet-3.0_LIBRARY
    NAMES mate-panel-applet-3
    PATHS ${Libmatepanelapplet-3.0_PKGCONF_LIBRARY_DIRS}
)

set(Libmatepanelapplet-3.0_PROCESS_INCLUDES Libmatepanelapplet-3.0_INCLUDE_DIR)
set(Libmatepanelapplet-3.0_PROCESS_LIBS Libmatepanelapplet-3.0_LIBRARY)

libfind_process(Libmatepanelapplet-3.0)
