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

#include "sketchmodel.h"

class SketchModelTest : public QObject
{
    Q_OBJECT
private slots:
    void shouldHaveDefaultState()
    {
        // GIVEN
        const auto model = std::make_unique<SketchModel>();

        // THEN
        QVERIFY(model->strokes(Stroke::Type::Fill).isEmpty());
        QVERIFY(model->strokes(Stroke::Type::Outline).isEmpty());
    }

    void shouldAddStrokes()
    {
        // GIVEN
        const auto model = std::make_unique<SketchModel>();
        const auto fillStroke1 = [] {
            auto stroke = Stroke{};
            stroke.setType(Stroke::Type::Fill);
            stroke.setColor(Qt::green);
            return stroke;
        }();
        const auto fillStroke2 = [] {
            auto stroke = Stroke{};
            stroke.setType(Stroke::Type::Fill);
            stroke.setColor(Qt::blue);
            return stroke;
        }();
        const auto outlineStroke1 = [] {
            auto stroke = Stroke{};
            stroke.setType(Stroke::Type::Outline);
            stroke.setColor(Qt::cyan);
            return stroke;
        }();
        const auto outlineStroke2 = [] {
            auto stroke = Stroke{};
            stroke.setType(Stroke::Type::Outline);
            stroke.setColor(Qt::magenta);
            return stroke;
        }();

        // WHEN
        model->addStroke(fillStroke1);
        model->addStroke(outlineStroke1);
        model->addStroke(outlineStroke2);
        model->addStroke(fillStroke2);

        // THEN
        QCOMPARE(model->strokes(Stroke::Type::Fill).size(), 2);
        QCOMPARE(model->strokes(Stroke::Type::Fill).at(0).type(), Stroke::Type::Fill);
        QCOMPARE(model->strokes(Stroke::Type::Fill).at(0).color(), fillStroke1.color());
        QCOMPARE(model->strokes(Stroke::Type::Fill).at(1).type(), Stroke::Type::Fill);
        QCOMPARE(model->strokes(Stroke::Type::Fill).at(1).color(), fillStroke2.color());

        QCOMPARE(model->strokes(Stroke::Type::Outline).size(), 2);
        QCOMPARE(model->strokes(Stroke::Type::Outline).at(0).type(), Stroke::Type::Outline);
        QCOMPARE(model->strokes(Stroke::Type::Outline).at(0).color(), outlineStroke1.color());
        QCOMPARE(model->strokes(Stroke::Type::Outline).at(1).type(), Stroke::Type::Outline);
        QCOMPARE(model->strokes(Stroke::Type::Outline).at(1).color(), outlineStroke2.color());

        QCOMPARE(model->strokes().size(), 4);
        QCOMPARE(model->strokes().at(0).type(), Stroke::Type::Fill);
        QCOMPARE(model->strokes().at(0).color(), fillStroke1.color());
        QCOMPARE(model->strokes().at(1).type(), Stroke::Type::Fill);
        QCOMPARE(model->strokes().at(1).color(), fillStroke2.color());
        QCOMPARE(model->strokes().at(2).type(), Stroke::Type::Outline);
        QCOMPARE(model->strokes().at(2).color(), outlineStroke1.color());
        QCOMPARE(model->strokes().at(3).type(), Stroke::Type::Outline);
        QCOMPARE(model->strokes().at(3).color(), outlineStroke2.color());
    }


    void shouldEraseAreas_data()
    {
        QTest::addColumn<Stroke>("input");
        QTest::addColumn<Stroke::Type>("inputType");
        QTest::addColumn<Stroke::Type>("otherType");
        QTest::addColumn<QPointF>("center");
        QTest::addColumn<qreal>("radius");
        QTest::addColumn<QVector<QVector<StrokeSample>>>("expectedSamples");

        auto straightLine = [] {
            auto stroke = Stroke{};
            stroke.setType(Stroke::Type::Outline);
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

        QTest::newRow("straight outline, no hit")
                << straightLine << straightLine.type() << Stroke::Type::Fill
                << QPointF{-1.0_r, -1.0_r} << 0.5_r
                << QVector<QVector<StrokeSample>>{straightLine.samples()};

        QTest::newRow("straight outline, all hits")
                << straightLine << straightLine.type() << Stroke::Type::Fill
                << QPointF{0.5_r, 0.5_r} << 1.5_r
                << QVector<QVector<StrokeSample>>{};

        QTest::newRow("straight outline, begin hits with isolated sample")
                << straightLine << straightLine.type() << Stroke::Type::Fill
                << QPointF{0.1875_r, 0.1875_r} << 0.125_r
                << QVector<QVector<StrokeSample>>{
                       {{{0.375_r, 0.375_r}, 0.5_r}, {{0.5_r, 0.5_r}, 0.5_r},
                        {{0.625_r, 0.625_r}, 0.5_r}, {{0.75_r, 0.75_r}, 0.5_r},
                        {{0.875_r, 0.875_r}, 0.5_r}, {{1.0_r, 1.0_r}, 0.5_r}}
                   };

        QTest::newRow("straight outline, begin hits")
                << straightLine << straightLine.type() << Stroke::Type::Fill
                << QPointF{0.0_r, 0.0_r} << 0.25_r
                << QVector<QVector<StrokeSample>>{
                       {{{0.25_r, 0.25_r}, 0.5_r}, {{0.375_r, 0.375_r}, 0.5_r},
                        {{0.5_r, 0.5_r}, 0.5_r}, {{0.625_r, 0.625_r}, 0.5_r},
                        {{0.75_r, 0.75_r}, 0.5_r}, {{0.875_r, 0.875_r}, 0.5_r},
                        {{1.0_r, 1.0_r}, 0.5_r}}
                   };

        QTest::newRow("straight outline, middle hits")
                << straightLine << straightLine.type() << Stroke::Type::Fill
                << QPointF{0.375_r, 0.375_r} << 0.2_r
                << QVector<QVector<StrokeSample>>{
                       {{{0.0_r, 0.0_r}, 0.5_r}, {{0.125_r, 0.125_r}, 0.5_r}},
                       {{{0.625_r, 0.625_r}, 0.5_r}, {{0.75_r, 0.75_r}, 0.5_r}, {{0.875_r, 0.875_r}, 0.5_r}, {{1.0_r, 1.0_r}, 0.5_r}}
                   };

        QTest::newRow("straight outline, end hits")
                << straightLine << straightLine.type() << Stroke::Type::Fill
                << QPointF{1.0_r, 1.0_r} << 0.25_r
                << QVector<QVector<StrokeSample>>{
                       {{{0.0_r, 0.0_r}, 0.5_r}, {{0.125_r, 0.125_r}, 0.5_r},
                        {{0.25_r, 0.25_r}, 0.5_r}, {{0.375_r, 0.375_r}, 0.5_r},
                        {{0.5_r, 0.5_r}, 0.5_r}, {{0.625_r, 0.625_r}, 0.5_r},
                        {{0.75_r, 0.75_r}, 0.5_r}}
                   };


        straightLine.setType(Stroke::Type::Fill);

        QTest::newRow("straight fill, no hit")
                << straightLine << straightLine.type() << Stroke::Type::Outline
                << QPointF{-1.0_r, -1.0_r} << 0.5_r
                << QVector<QVector<StrokeSample>>{straightLine.samples()};

        QTest::newRow("straight fill, all hits")
                << straightLine << straightLine.type() << Stroke::Type::Outline
                << QPointF{0.5_r, 0.5_r} << 1.5_r
                << QVector<QVector<StrokeSample>>{};

        QTest::newRow("straight fill, begin hits with isolated sample")
                << straightLine << straightLine.type() << Stroke::Type::Outline
                << QPointF{0.1875_r, 0.1875_r} << 0.125_r
                << QVector<QVector<StrokeSample>>{
                       {{{0.375_r, 0.375_r}, 0.5_r}, {{0.5_r, 0.5_r}, 0.5_r},
                        {{0.625_r, 0.625_r}, 0.5_r}, {{0.75_r, 0.75_r}, 0.5_r},
                        {{0.875_r, 0.875_r}, 0.5_r}, {{1.0_r, 1.0_r}, 0.5_r}}
                   };

        QTest::newRow("straight fill, begin hits")
                << straightLine << straightLine.type() << Stroke::Type::Outline
                << QPointF{0.0_r, 0.0_r} << 0.25_r
                << QVector<QVector<StrokeSample>>{
                       {{{0.25_r, 0.25_r}, 0.5_r}, {{0.375_r, 0.375_r}, 0.5_r},
                        {{0.5_r, 0.5_r}, 0.5_r}, {{0.625_r, 0.625_r}, 0.5_r},
                        {{0.75_r, 0.75_r}, 0.5_r}, {{0.875_r, 0.875_r}, 0.5_r},
                        {{1.0_r, 1.0_r}, 0.5_r}}
                   };

        QTest::newRow("straight fill, middle hits")
                << straightLine << straightLine.type() << Stroke::Type::Outline
                << QPointF{0.375_r, 0.375_r} << 0.2_r
                << QVector<QVector<StrokeSample>>{
                       {{{0.0_r, 0.0_r}, 0.5_r}, {{0.125_r, 0.125_r}, 0.5_r}},
                       {{{0.625_r, 0.625_r}, 0.5_r}, {{0.75_r, 0.75_r}, 0.5_r}, {{0.875_r, 0.875_r}, 0.5_r}, {{1.0_r, 1.0_r}, 0.5_r}}
                   };

        QTest::newRow("straight fill, end hits")
                << straightLine << straightLine.type() << Stroke::Type::Outline
                << QPointF{1.0_r, 1.0_r} << 0.25_r
                << QVector<QVector<StrokeSample>>{
                       {{{0.0_r, 0.0_r}, 0.5_r}, {{0.125_r, 0.125_r}, 0.5_r},
                        {{0.25_r, 0.25_r}, 0.5_r}, {{0.375_r, 0.375_r}, 0.5_r},
                        {{0.5_r, 0.5_r}, 0.5_r}, {{0.625_r, 0.625_r}, 0.5_r},
                        {{0.75_r, 0.75_r}, 0.5_r}}
                   };


        auto complexLine = [] {
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

        QTest::newRow("complex outline, multiple hits")
                << complexLine << complexLine.type() << Stroke::Type::Fill
                << QPointF{0.5625_r, 0.4375_r} << 0.27_r
                << QVector<QVector<StrokeSample>>{
                       {{{0.0_r, 0.0_r}, 0.5_r}, {{0.125_r, 0.125_r}, 0.5_r}, {{0.25_r, 0.25_r}, 0.5_r}},
                       {{{0.5_r, 0.125_r}, 0.5_r}, {{0.625_r, 0.125_r}, 0.5_r}},
                       {{{0.875_r, 0.375_r}, 0.5_r}, {{1.0_r, 0.25_r}, 0.5_r}, {{1.125_r, 0.125_r}, 0.5_r}}
                   };

        complexLine.setType(Stroke::Type::Fill);

        QTest::newRow("complex fill, multiple hits")
                << complexLine << complexLine.type() << Stroke::Type::Outline
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
        QFETCH(Stroke::Type, inputType);
        QFETCH(Stroke::Type, otherType);
        QFETCH(QPointF, center);
        QFETCH(qreal, radius);

        const auto model = std::make_unique<SketchModel>();
        model->addStroke([=] {
            auto stroke = Stroke{};
            stroke.setType(inputType);
            stroke.addSample({{-10.0_r, -10.0_r}, 0.5_r});
            stroke.addSample({{-11.0_r, -11.0_r}, 0.5_r});
            return stroke;
        }());

        model->addStroke(input);

        model->addStroke([=] {
            auto stroke = Stroke{};
            stroke.setType(inputType);
            stroke.addSample({{10.0_r, 10.0_r}, 0.5_r});
            stroke.addSample({{11.0_r, 11.0_r}, 0.5_r});
            return stroke;
        }());

        model->addStroke([=] {
            auto stroke = Stroke{};
            stroke.setType(otherType);
            stroke.addSample({center, 0.5_r});
            stroke.addSample({{center.x() + radius / 2.0_r, center.y()}, 0.5_r});
            return stroke;
        }());

        // WHEN
        model->eraseArea(inputType, center, radius);

        // THEN
        QCOMPARE(model->strokes(otherType).size(), 1);

        QFETCH(QVector<QVector<StrokeSample>>, expectedSamples);
        QCOMPARE(model->strokes(inputType).size(), expectedSamples.size() + 2);
        for (int i = 0; i < expectedSamples.size(); i++) {
            const auto stroke = model->strokes(inputType).at(i + 1);
            QCOMPARE(stroke.type(), input.type());
            QCOMPARE(stroke.color(), input.color());
            QCOMPARE(stroke.samples(), expectedSamples.at(i));
        }
    }
};

QTEST_APPLESS_MAIN(SketchModelTest)

#include "sketchmodeltest.moc"
