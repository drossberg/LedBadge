cmake_minimum_required(VERSION 3.20)

project(LedBadge)

#find_package(hidapi REQUIRED)
INCLUDE_DIRECTORIES(/usr/include/hidapi)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(CMAKE_AUTOMOC ON)

set(DesignerSources
    src/LedBadge.cpp
    src/main.cpp
    src/MainWindow.cpp
    src/usb.cpp
)

add_executable(designer ${DesignerSources})
#target_link_libraries(designer hidapi-hidraw)
target_link_libraries(designer PRIVATE Qt6::Widgets hidapi-libusb)
