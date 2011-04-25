find_package(KDE4 4.4.80 REQUIRED)
set (QT_MIN_VERSION "4.7.0")

find_package(KDE4Internal REQUIRED)
include(KDE4Defaults)

add_subdirectory(src)
