add_subdirectory(icons)

add_library(washipadlib STATIC)

ecm_add_qml_module(washipadlib GENERATE_PLUGIN_SOURCE
    URI WashiPad
    QML_FILES
        Main.qml
        PaintingCanvas.qml
        PageMarker.qml
        Sketch.qml
)

target_sources(washipadlib PRIVATE
    pressureequation.cpp
    sketchmodel.cpp
    stroke.cpp
    sketchserializer.cpp
    sketchview.cpp
    strokeitem.cpp
    strokelistitem.cpp
    strokepainter.cpp
    event.cpp
)

target_link_libraries(washipadlib PUBLIC
    Qt::Quick
    Qt::Svg
    Qt::Widgets
    KF6::I18n
    KF6::I18nQml
    KF6::CoreAddons
)

if (TARGET KF6::Crash)
    target_link_libraries(washipadlib PUBLIC KF6::Crash)
endif()

add_executable(washipad main.cpp)
target_link_libraries(washipad PUBLIC washipadlib washipadlibplugin)

install(TARGETS washipad DESTINATION ${KF6_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.washipad.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.washipad.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
