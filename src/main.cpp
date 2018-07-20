/* This file is part of Washi Pad

   Copyright 2018 Kevin Ottens <ervin@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License or (at your option) version 3 or any later version
   accepted by the membership of KDE e.V. (or its successor approved
   by the membership of KDE e.V.), which shall act as a proxy
   defined in Section 14 of version 3 of the license.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,
   USA.
*/

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
    qRegisterMetaType<TabletEvent>();

    qmlRegisterType<PressureEquation>("WashiPad", 1, 0, "PressureEquation");
    qmlRegisterUncreatableType<StrokeSample>("WashiPad", 1, 0, "StrokeSample", "Use the createSample function on SketchViewHandler instead");
    qmlRegisterUncreatableType<Stroke>("WashiPad", 1, 0, "Stroke", "Use the createStroke function on SketchViewHandler instead");

    qmlRegisterType<SketchViewHandler>("WashiPad", 1, 0, "SketchViewHandler");
    qmlRegisterUncreatableType<TabletEvent>("WashiPad", 1, 0, "TabletEvent", "They are provided by the SketchViewHandler");

    qmlRegisterType<SketchModel>("WashiPad", 1, 0, "SketchModel");
    qmlRegisterType<SketchSerializer>("WashiPad", 1, 0, "SketchSerializer");
    qmlRegisterType<StrokeItem>("WashiPad", 1, 0, "StrokeItem");
    qmlRegisterType<StrokeListItem>("WashiPad", 1, 0, "StrokeListItem");

    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:/main.qml"));
    view.setCursor(Qt::BlankCursor);
    view.showFullScreen();
    return app.exec();
}
