// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include <QApplication>
#include <QQmlEngine>
#include <QQuickView>

#include "pressureequation.h"
#include "sketchmodel.h"
#include "sketchserializer.h"
#include "sketchview.h"
#include "strokeitem.h"
#include "strokelistitem.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SketchView view;

    QObject::connect(view.engine(), &QQmlEngine::quit, &app, &QGuiApplication::quit);

    qRegisterMetaType<StrokeSample>();
    qRegisterMetaType<Stroke>();
    qRegisterMetaType<Stroke::Type>();
    qRegisterMetaType<Event>();

    qmlRegisterType<PressureEquation>("WashiPad", 1, 0, "PressureEquation");
    qmlRegisterUncreatableType<StrokeSample>("WashiPad", 1, 0, "StrokeSample", QStringLiteral("Use the createSample function on SketchViewHandler instead"));
    qmlRegisterUncreatableType<Stroke>("WashiPad", 1, 0, "Stroke", QStringLiteral("Use the createStroke function on SketchViewHandler instead"));

    qmlRegisterType<SketchViewHandler>("WashiPad", 1, 0, "SketchViewHandler");
    qmlRegisterUncreatableType<Event>("WashiPad", 1, 0, "TabletEvent", QStringLiteral("They are provided by the SketchViewHandler"));

    qmlRegisterType<SketchModel>("WashiPad", 1, 0, "SketchModel");
    qmlRegisterType<SketchSerializer>("WashiPad", 1, 0, "SketchSerializer");
    qmlRegisterType<StrokeItem>("WashiPad", 1, 0, "StrokeItem");
    qmlRegisterType<StrokeListItem>("WashiPad", 1, 0, "StrokeListItem");

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl(QStringLiteral("qrc:/main.qml")));
    view.setCursor(Qt::BlankCursor);
    view.showFullScreen();
    return app.exec();
}
