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

#include <QtTest>

#include "stroke.h"

class StrokeTest : public QObject
{
    Q_OBJECT
private slots:
    void shouldHaveDefaultState()
    {
        // GIVEN
        const auto stroke = Stroke{};

        // THEN
        QCOMPARE(stroke.type(), Stroke::Type::Outline);
        QCOMPARE(stroke.color(), QColor(Qt::black));
        QVERIFY(stroke.samples().isEmpty());

        // GIVEN
        const auto sample = StrokeSample{};

        // THEN
        QVERIFY(sample.position.isNull());
        QCOMPARE(sample.width, 1.0f);
    }

    void shouldHaveType()
    {
        // GIVEN
        auto stroke = Stroke{};

        // WHEN
        stroke.setType(Stroke::Type::Outline);

        // THEN
        QCOMPARE(stroke.type(), Stroke::Type::Outline);
    }

    void shouldHaveColor()
    {
        // GIVEN
        auto stroke = Stroke{};

        // WHEN
        stroke.setColor(Qt::red);

        // THEN
        QCOMPARE(stroke.color(), QColor(Qt::red));
    }

    void shouldHaveSamples()
    {
        // GIVEN
        auto stroke = Stroke{};

        // WHEN
        const auto sample = StrokeSample{{0.5f, 0.5f}, 0.1f};
        stroke.addSample(sample);

        // THEN
        QCOMPARE(stroke.samples().size(), 1);
        QCOMPARE(stroke.samples().first(), sample);

        // WHEN
        const auto samples = QVector<StrokeSample>{
            {{1.0f, 1.5f}, 0.2f},
            {{1.1f, 1.75f}, 0.4f}
        };
        stroke.addSamples(samples);

        // THEN
        QCOMPARE(stroke.samples().size(), 3);
        QCOMPARE(stroke.samples().at(0), sample);
        QCOMPARE(stroke.samples().at(1), samples.at(0));
        QCOMPARE(stroke.samples().at(2), samples.at(1));
    }

    void shouldEraseAreas_data()
    {
        QTest::addColumn<Stroke>("input");
        QTest::addColumn<QVector2D>("center");
        QTest::addColumn<float>("radius");
        QTest::addColumn<QVector<QVector<StrokeSample>>>("expectedSamples");

        const auto straightLine = [] {
            auto stroke = Stroke{};
            stroke.setType(Stroke::Type::Fill);
            stroke.setColor(Qt::blue);
            stroke.addSample({{0.0f, 0.0f}, 0.5f});
            stroke.addSample({{0.125f, 0.125f}, 0.5f});
            stroke.addSample({{0.25f, 0.25f}, 0.5f});
            stroke.addSample({{0.375f, 0.375f}, 0.5f});
            stroke.addSample({{0.5f, 0.5f}, 0.5f});
            stroke.addSample({{0.625f, 0.625f}, 0.5f});
            stroke.addSample({{0.75f, 0.75f}, 0.5f});
            stroke.addSample({{0.875f, 0.875f}, 0.5f});
            stroke.addSample({{1.0f, 1.0f}, 0.5f});
            return stroke;
        }();

        QTest::newRow("straight line, no hit")
                << straightLine
                << QVector2D{-1.0f, -1.0f} << 0.5f
                << QVector<QVector<StrokeSample>>{straightLine.samples()};

        QTest::newRow("straight line, all hits")
                << straightLine
                << QVector2D{0.5f, 0.5f} << 1.5f
                << QVector<QVector<StrokeSample>>{};

        QTest::newRow("straight line, begin hits with isolated sample")
                << straightLine
                << QVector2D{0.1875f, 0.1875f} << 0.125f
                << QVector<QVector<StrokeSample>>{
                       {{{0.375f, 0.375f}, 0.5f}, {{0.5f, 0.5f}, 0.5f},
                        {{0.625f, 0.625f}, 0.5f}, {{0.75f, 0.75f}, 0.5f},
                        {{0.875f, 0.875f}, 0.5f}, {{1.0f, 1.0f}, 0.5f}}
                   };

        QTest::newRow("straight line, begin hits")
                << straightLine
                << QVector2D{0.0f, 0.0f} << 0.25f
                << QVector<QVector<StrokeSample>>{
                       {{{0.25f, 0.25f}, 0.5f}, {{0.375f, 0.375f}, 0.5f},
                        {{0.5f, 0.5f}, 0.5f}, {{0.625f, 0.625f}, 0.5f},
                        {{0.75f, 0.75f}, 0.5f}, {{0.875f, 0.875f}, 0.5f},
                        {{1.0f, 1.0f}, 0.5f}}
                   };

        QTest::newRow("straight line, middle hits")
                << straightLine
                << QVector2D{0.375f, 0.375f} << 0.2f
                << QVector<QVector<StrokeSample>>{
                       {{{0.0f, 0.0f}, 0.5f}, {{0.125f, 0.125f}, 0.5f}},
                       {{{0.625f, 0.625f}, 0.5f}, {{0.75f, 0.75f}, 0.5f}, {{0.875f, 0.875f}, 0.5f}, {{1.0f, 1.0f}, 0.5f}}
                   };

        QTest::newRow("straight line, end hits")
                << straightLine
                << QVector2D{1.0f, 1.0f} << 0.25f
                << QVector<QVector<StrokeSample>>{
                       {{{0.0f, 0.0f}, 0.5f}, {{0.125f, 0.125f}, 0.5f},
                        {{0.25f, 0.25f}, 0.5f}, {{0.375f, 0.375f}, 0.5f},
                        {{0.5f, 0.5f}, 0.5f}, {{0.625f, 0.625f}, 0.5f},
                        {{0.75f, 0.75f}, 0.5f}}
                   };


        const auto complexLine = [] {
            auto stroke = Stroke{};
            stroke.setType(Stroke::Type::Outline);
            stroke.setColor(Qt::red);
            stroke.addSample({{0.0f, 0.0f}, 0.5f});
            stroke.addSample({{0.125f, 0.125f}, 0.5f});
            stroke.addSample({{0.25f, 0.25f}, 0.5f});
            stroke.addSample({{0.375f, 0.25f}, 0.5f});
            stroke.addSample({{0.5f, 0.125f}, 0.5f});
            stroke.addSample({{0.625f, 0.125f}, 0.5f});
            stroke.addSample({{0.75f, 0.25f}, 0.5f});
            stroke.addSample({{0.875f, 0.375f}, 0.5f});
            stroke.addSample({{1.0f, 0.25f}, 0.5f});
            stroke.addSample({{1.125f, 0.125f}, 0.5f});
            return stroke;
        }();

        QTest::newRow("complex line, multiple hits")
                << complexLine
                << QVector2D{0.5625f, 0.4375f} << 0.27f
                << QVector<QVector<StrokeSample>>{
                       {{{0.0f, 0.0f}, 0.5f}, {{0.125f, 0.125f}, 0.5f}, {{0.25f, 0.25f}, 0.5f}},
                       {{{0.5f, 0.125f}, 0.5f}, {{0.625f, 0.125f}, 0.5f}},
                       {{{0.875f, 0.375f}, 0.5f}, {{1.0f, 0.25f}, 0.5f}, {{1.125f, 0.125f}, 0.5f}}
                   };
    }

    void shouldEraseAreas()
    {
        // GIVEN
        QFETCH(Stroke, input);

        // WHEN
        QFETCH(QVector2D, center);
        QFETCH(float, radius);
        const auto output = input.eraseArea(center, radius);

        // THEN
        QFETCH(QVector<QVector<StrokeSample>>, expectedSamples);
        QCOMPARE(output.size(), expectedSamples.size());
        for (int i = 0; i < output.size(); i++) {
            QCOMPARE(output.at(i).type(), input.type());
            QCOMPARE(output.at(i).color(), input.color());
            QCOMPARE(output.at(i).samples(), expectedSamples.at(i));
        }
    }

    void shouldHaveBoundingRect()
    {
        // GIVEN
        auto stroke = Stroke{};
        stroke.addSample({{0.5f, 0.5f}, 1.0f});
        stroke.addSample({{5.0f, -0.5f}, 1.0f});
        stroke.addSample({{-5.0f, 0.5f}, 1.0f});
        stroke.addSample({{0.25f, 1.5f}, 1.0f});
        stroke.addSample({{0.25f, 5.0f}, 1.0f});
        stroke.addSample({{0.25f, -5.0f}, 1.0f});

        // WHEN
        const auto rect = stroke.boundingRect();

        // THEN
        const auto expected = QRectF{QPointF{-5.0, -5.0}, QPointF{5.0, 5.0}};
        QCOMPARE(rect, expected);
    }
};

QTEST_APPLESS_MAIN(StrokeTest)

#include "stroketest.moc"
