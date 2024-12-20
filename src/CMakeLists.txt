add_subdirectory(icons)

set(lib_SRCS
    pressureequation.cpp
    sketchmodel.cpp
    stroke.cpp
)

add_library(washipadlib STATIC ${lib_SRCS})
target_link_libraries(washipadlib
    Qt::Quick
    Qt::Svg
    Qt::Widgets
    KF6::I18n
    KF6::I18nQml
)

set(exec_SRCS
    main.cpp
    sketchserializer.cpp
    sketchview.cpp
    strokeitem.cpp
    strokelistitem.cpp
    strokepainter.cpp
    event.cpp
)

add_executable(washipad ${exec_SRCS})
ecm_add_qml_module(washipad
    URI WashiPad
    QML_FILES
        Main.qml
        PaintingCanvas.qml
        PageMarker.qml
        Sketch.qml
)
target_link_libraries(washipad PUBLIC washipadlib)

install(TARGETS washipad DESTINATION ${KF6_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.washipad.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.washipad.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
