// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include <QApplication>
#include <QQmlApplicationEngine>

#include <KLocalizedQmlContext>
#include <KLocalizedString>

int main(int argc, char *argv[])
{
    KLocalizedString::setApplicationDomain(QByteArrayLiteral("washipad"));

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    KLocalization::setupLocalizedContext(&engine);

    engine.loadFromModule("WashiPad", "Main");

    return app.exec();
}
