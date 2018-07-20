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
        QCOMPARE(sample.width, 1.0_r);
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
        const auto sample = StrokeSample{{0.5_r, 0.5_r}, 0.1_r};
        stroke.addSample(sample);

        // THEN
        QCOMPARE(stroke.samples().size(), 1);
        QCOMPARE(stroke.samples().first(), sample);

        // WHEN
        const auto samples = QVector<StrokeSample>{
            {{1.0_r, 1.5_r}, 0.2_r},
            {{1.1_r, 1.75_r}, 0.4_r}
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
        QTest::addColumn<QPointF>("center");
        QTest::addColumn<qreal>("radius");
        QTest::addColumn<QVector<QVector<StrokeSample>>>("expectedSamples");

        const auto straightLine = [] {
            auto stroke = Stroke{};
            stroke.setType(Stroke::Type::Fill);
            stroke.setColor(Qt::blue);
            stroke.addSample({{0.0_r, 0.0_r}, 0.5_r});
            stroke.addSample({{0.125_r, 0.125_r}, 0.5_r});
            stroke.addSample({{0.25_r, 0.25_r}, 0.5_r});
            stroke.addSample({{0.375_r, 0.375_r}, 0.5_r});
            stroke.addSample({{0.5_r, 0.5_r}, 0.5_r});
            stroke.addSample({{0.625_r, 0.625_r}, 0.5_r});
            stroke.addSample({{0.75_r, 0.75_r}, 0.5_r});
            stroke.addSample({{0.875_r, 0.875_r}, 0.5_r});
            stroke.addSample({{1.0_r, 1.0_r}, 0.5_r});
            return stroke;
        }();

        QTest::newRow("straight line, no hit")
                << straightLine
                << QPointF{-1.0_r, -1.0_r} << 0.5_r
                << QVector<QVector<StrokeSample>>{straightLine.samples()};

        QTest::newRow("straight line, all hits")
                << straightLine
                << QPointF{0.5_r, 0.5_r} << 1.5_r
                << QVector<QVector<StrokeSample>>{};

        QTest::newRow("straight line, begin hits with isolated sample")
                << straightLine
                << QPointF{0.1875_r, 0.1875_r} << 0.125_r
                << QVector<QVector<StrokeSample>>{
                       {{{0.375_r, 0.375_r}, 0.5_r}, {{0.5_r, 0.5_r}, 0.5_r},
                        {{0.625_r, 0.625_r}, 0.5_r}, {{0.75_r, 0.75_r}, 0.5_r},
                        {{0.875_r, 0.875_r}, 0.5_r}, {{1.0_r, 1.0_r}, 0.5_r}}
                   };

        QTest::newRow("straight line, begin hits")
                << straightLine
                << QPointF{0.0_r, 0.0_r} << 0.25_r
                << QVector<QVector<StrokeSample>>{
                       {{{0.25_r, 0.25_r}, 0.5_r}, {{0.375_r, 0.375_r}, 0.5_r},
                        {{0.5_r, 0.5_r}, 0.5_r}, {{0.625_r, 0.625_r}, 0.5_r},
                        {{0.75_r, 0.75_r}, 0.5_r}, {{0.875_r, 0.875_r}, 0.5_r},
                        {{1.0_r, 1.0_r}, 0.5_r}}
                   };

        QTest::newRow("straight line, middle hits")
                << straightLine
                << QPointF{0.375_r, 0.375_r} << 0.2_r
                << QVector<QVector<StrokeSample>>{
                       {{{0.0_r, 0.0_r}, 0.5_r}, {{0.125_r, 0.125_r}, 0.5_r}},
                       {{{0.625_r, 0.625_r}, 0.5_r}, {{0.75_r, 0.75_r}, 0.5_r}, {{0.875_r, 0.875_r}, 0.5_r}, {{1.0_r, 1.0_r}, 0.5_r}}
                   };

        QTest::newRow("straight line, end hits")
                << straightLine
                << QPointF{1.0_r, 1.0_r} << 0.25_r
                << QVector<QVector<StrokeSample>>{
                       {{{0.0_r, 0.0_r}, 0.5_r}, {{0.125_r, 0.125_r}, 0.5_r},
                        {{0.25_r, 0.25_r}, 0.5_r}, {{0.375_r, 0.375_r}, 0.5_r},
                        {{0.5_r, 0.5_r}, 0.5_r}, {{0.625_r, 0.625_r}, 0.5_r},
                        {{0.75_r, 0.75_r}, 0.5_r}}
                   };


        const auto complexLine = [] {
            auto stroke = Stroke{};
            stroke.setType(Stroke::Type::Outline);
            stroke.setColor(Qt::red);
            stroke.addSample({{0.0_r, 0.0_r}, 0.5_r});
            stroke.addSample({{0.125_r, 0.125_r}, 0.5_r});
            stroke.addSample({{0.25_r, 0.25_r}, 0.5_r});
            stroke.addSample({{0.375_r, 0.25_r}, 0.5_r});
            stroke.addSample({{0.5_r, 0.125_r}, 0.5_r});
            stroke.addSample({{0.625_r, 0.125_r}, 0.5_r});
            stroke.addSample({{0.75_r, 0.25_r}, 0.5_r});
            stroke.addSample({{0.875_r, 0.375_r}, 0.5_r});
            stroke.addSample({{1.0_r, 0.25_r}, 0.5_r});
            stroke.addSample({{1.125_r, 0.125_r}, 0.5_r});
            return stroke;
        }();

        QTest::newRow("complex line, multiple hits")
                << complexLine
                << QPointF{0.5625_r, 0.4375_r} << 0.27_r
                << QVector<QVector<StrokeSample>>{
                       {{{0.0_r, 0.0_r}, 0.5_r}, {{0.125_r, 0.125_r}, 0.5_r}, {{0.25_r, 0.25_r}, 0.5_r}},
                       {{{0.5_r, 0.125_r}, 0.5_r}, {{0.625_r, 0.125_r}, 0.5_r}},
                       {{{0.875_r, 0.375_r}, 0.5_r}, {{1.0_r, 0.25_r}, 0.5_r}, {{1.125_r, 0.125_r}, 0.5_r}}
                   };
    }

    void shouldEraseAreas()
    {
        // GIVEN
        QFETCH(Stroke, input);

        // WHEN
        QFETCH(QPointF, center);
        QFETCH(qreal, radius);
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
        stroke.addSample({{0.5_r, 0.5_r}, 1.0_r});
        stroke.addSample({{5.0_r, -0.5_r}, 1.0_r});
        stroke.addSample({{-5.0_r, 0.5_r}, 1.0_r});
        stroke.addSample({{0.25_r, 1.5_r}, 1.0_r});
        stroke.addSample({{0.25_r, 5.0_r}, 1.0_r});
        stroke.addSample({{0.25_r, -5.0_r}, 1.0_r});

        // WHEN
        const auto rect = stroke.boundingRect();

        // THEN
        const auto expected = QRectF{QPointF{-5.0_r, -5.0_r}, QPointF{5.0_r, 5.0_r}};
        QCOMPARE(rect, expected);
    }
};

QTEST_APPLESS_MAIN(StrokeTest)

#include "stroketest.moc"
