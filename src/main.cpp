// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QCommandLineParser>

#include <KLocalizedQmlContext>
#include <KLocalizedString>
#include <KAboutData>

#if __has_include("KCrash")
#include <KCrash>
#endif

#include "washipad-version.h"

int main(int argc, char *argv[])
{
    KLocalizedString::setApplicationDomain(QByteArrayLiteral("washipad"));

    QApplication app(argc, argv);

    KAboutData about(QStringLiteral("washipad"),
                     i18n("WashiPad"),
                     QStringLiteral(WASHIPAD_VERSION_STRING),
                     i18n("Sketching app"),
                     KAboutLicense::GPL_V3,
                     i18n("© 2018-2024 KDE Community"));

    about.addAuthor(i18n("Kevin Ottens"),
                    i18n("Maintainer"),
                    QStringLiteral("ervin@kde.org"));

    about.addAuthor(i18n("Carl Schwan"),
                    i18n("Maintainer"),
                    QStringLiteral("carl@carlschwan.eu"),
                    QStringLiteral("https://carlschwan.eu"),
                    QUrl(QStringLiteral("https://carlschwan.eu/avatar.png")));

    about.setTranslator(i18nc("NAME OF TRANSLATORS", "Your names"), i18nc("EMAIL OF TRANSLATORS", "Your emails"));
    about.setOrganizationDomain("kde.org");

#if __has_include("KCrash")
    KCrash::initialize();
#endif

    KAboutData::setApplicationData(about);

    QCommandLineParser parser;
    about.setupCommandLine(&parser);
    parser.process(app);
    about.processCommandLine(&parser);

    QQmlApplicationEngine engine;
    KLocalization::setupLocalizedContext(&engine);

    engine.loadFromModule("WashiPad", "Main");

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
