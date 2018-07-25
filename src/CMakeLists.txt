add_subdirectory(icons)

set(lib_SRCS
    pressureequation.cpp
    sketchmodel.cpp
    stroke.cpp
)

add_library(washipadlib STATIC ${lib_SRCS})
target_link_libraries(washipadlib
    Qt5::Quick
    Qt5::Svg
    Qt5::Widgets
)

set(exec_SRCS
    main.cpp
    sketchserializer.cpp
    sketchview.cpp
    strokeitem.cpp
    strokelistitem.cpp
    strokepainter.cpp
    tabletevent.cpp
)

qt5_add_resources(exec_SRCS
    main.qrc
)

add_executable(washipad ${exec_SRCS})
target_link_libraries(washipad washipadlib)

install(TARGETS washipad DESTINATION ${KDE_INSTALL_BINDIR})
install(PROGRAMS org.kde.washipad.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.washipad.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
