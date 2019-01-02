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
        QTest::addColumn<QVector2D>("center");
        QTest::addColumn<float>("radius");
        QTest::addColumn<QVector<QVector<StrokeSample>>>("expectedSamples");

        auto straightLine = [] {
            auto stroke = Stroke{};
            stroke.setType(Stroke::Type::Outline);
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

        QTest::newRow("straight outline, no hit")
                << straightLine << straightLine.type() << Stroke::Type::Fill
                << QVector2D{-1.0f, -1.0f} << 0.5f
                << QVector<QVector<StrokeSample>>{straightLine.samples()};

        QTest::newRow("straight outline, all hits")
                << straightLine << straightLine.type() << Stroke::Type::Fill
                << QVector2D{0.5f, 0.5f} << 1.5f
                << QVector<QVector<StrokeSample>>{};

        QTest::newRow("straight outline, begin hits with isolated sample")
                << straightLine << straightLine.type() << Stroke::Type::Fill
                << QVector2D{0.1875f, 0.1875f} << 0.125f
                << QVector<QVector<StrokeSample>>{
                       {{{0.375f, 0.375f}, 0.5f}, {{0.5f, 0.5f}, 0.5f},
                        {{0.625f, 0.625f}, 0.5f}, {{0.75f, 0.75f}, 0.5f},
                        {{0.875f, 0.875f}, 0.5f}, {{1.0f, 1.0f}, 0.5f}}
                   };

        QTest::newRow("straight outline, begin hits")
                << straightLine << straightLine.type() << Stroke::Type::Fill
                << QVector2D{0.0f, 0.0f} << 0.25f
                << QVector<QVector<StrokeSample>>{
                       {{{0.25f, 0.25f}, 0.5f}, {{0.375f, 0.375f}, 0.5f},
                        {{0.5f, 0.5f}, 0.5f}, {{0.625f, 0.625f}, 0.5f},
                        {{0.75f, 0.75f}, 0.5f}, {{0.875f, 0.875f}, 0.5f},
                        {{1.0f, 1.0f}, 0.5f}}
                   };

        QTest::newRow("straight outline, middle hits")
                << straightLine << straightLine.type() << Stroke::Type::Fill
                << QVector2D{0.375f, 0.375f} << 0.2f
                << QVector<QVector<StrokeSample>>{
                       {{{0.0f, 0.0f}, 0.5f}, {{0.125f, 0.125f}, 0.5f}},
                       {{{0.625f, 0.625f}, 0.5f}, {{0.75f, 0.75f}, 0.5f}, {{0.875f, 0.875f}, 0.5f}, {{1.0f, 1.0f}, 0.5f}}
                   };

        QTest::newRow("straight outline, end hits")
                << straightLine << straightLine.type() << Stroke::Type::Fill
                << QVector2D{1.0f, 1.0f} << 0.25f
                << QVector<QVector<StrokeSample>>{
                       {{{0.0f, 0.0f}, 0.5f}, {{0.125f, 0.125f}, 0.5f},
                        {{0.25f, 0.25f}, 0.5f}, {{0.375f, 0.375f}, 0.5f},
                        {{0.5f, 0.5f}, 0.5f}, {{0.625f, 0.625f}, 0.5f},
                        {{0.75f, 0.75f}, 0.5f}}
                   };


        straightLine.setType(Stroke::Type::Fill);

        QTest::newRow("straight fill, no hit")
                << straightLine << straightLine.type() << Stroke::Type::Outline
                << QVector2D{-1.0f, -1.0f} << 0.5f
                << QVector<QVector<StrokeSample>>{straightLine.samples()};

        QTest::newRow("straight fill, all hits")
                << straightLine << straightLine.type() << Stroke::Type::Outline
                << QVector2D{0.5f, 0.5f} << 1.5f
                << QVector<QVector<StrokeSample>>{};

        QTest::newRow("straight fill, begin hits with isolated sample")
                << straightLine << straightLine.type() << Stroke::Type::Outline
                << QVector2D{0.1875f, 0.1875f} << 0.125f
                << QVector<QVector<StrokeSample>>{
                       {{{0.375f, 0.375f}, 0.5f}, {{0.5f, 0.5f}, 0.5f},
                        {{0.625f, 0.625f}, 0.5f}, {{0.75f, 0.75f}, 0.5f},
                        {{0.875f, 0.875f}, 0.5f}, {{1.0f, 1.0f}, 0.5f}}
                   };

        QTest::newRow("straight fill, begin hits")
                << straightLine << straightLine.type() << Stroke::Type::Outline
                << QVector2D{0.0f, 0.0f} << 0.25f
                << QVector<QVector<StrokeSample>>{
                       {{{0.25f, 0.25f}, 0.5f}, {{0.375f, 0.375f}, 0.5f},
                        {{0.5f, 0.5f}, 0.5f}, {{0.625f, 0.625f}, 0.5f},
                        {{0.75f, 0.75f}, 0.5f}, {{0.875f, 0.875f}, 0.5f},
                        {{1.0f, 1.0f}, 0.5f}}
                   };

        QTest::newRow("straight fill, middle hits")
                << straightLine << straightLine.type() << Stroke::Type::Outline
                << QVector2D{0.375f, 0.375f} << 0.2f
                << QVector<QVector<StrokeSample>>{
                       {{{0.0f, 0.0f}, 0.5f}, {{0.125f, 0.125f}, 0.5f}},
                       {{{0.625f, 0.625f}, 0.5f}, {{0.75f, 0.75f}, 0.5f}, {{0.875f, 0.875f}, 0.5f}, {{1.0f, 1.0f}, 0.5f}}
                   };

        QTest::newRow("straight fill, end hits")
                << straightLine << straightLine.type() << Stroke::Type::Outline
                << QVector2D{1.0f, 1.0f} << 0.25f
                << QVector<QVector<StrokeSample>>{
                       {{{0.0f, 0.0f}, 0.5f}, {{0.125f, 0.125f}, 0.5f},
                        {{0.25f, 0.25f}, 0.5f}, {{0.375f, 0.375f}, 0.5f},
                        {{0.5f, 0.5f}, 0.5f}, {{0.625f, 0.625f}, 0.5f},
                        {{0.75f, 0.75f}, 0.5f}}
                   };


        auto complexLine = [] {
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

        QTest::newRow("complex outline, multiple hits")
                << complexLine << complexLine.type() << Stroke::Type::Fill
                << QVector2D{0.5625f, 0.4375f} << 0.27f
                << QVector<QVector<StrokeSample>>{
                       {{{0.0f, 0.0f}, 0.5f}, {{0.125f, 0.125f}, 0.5f}, {{0.25f, 0.25f}, 0.5f}},
                       {{{0.5f, 0.125f}, 0.5f}, {{0.625f, 0.125f}, 0.5f}},
                       {{{0.875f, 0.375f}, 0.5f}, {{1.0f, 0.25f}, 0.5f}, {{1.125f, 0.125f}, 0.5f}}
                   };

        complexLine.setType(Stroke::Type::Fill);

        QTest::newRow("complex fill, multiple hits")
                << complexLine << complexLine.type() << Stroke::Type::Outline
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
        QFETCH(Stroke::Type, inputType);
        QFETCH(Stroke::Type, otherType);
        QFETCH(QVector2D, center);
        QFETCH(float, radius);

        const auto model = std::make_unique<SketchModel>();
        model->addStroke([=] {
            auto stroke = Stroke{};
            stroke.setType(inputType);
            stroke.addSample({{-10.0f, -10.0f}, 0.5f});
            stroke.addSample({{-11.0f, -11.0f}, 0.5f});
            return stroke;
        }());

        model->addStroke(input);

        model->addStroke([=] {
            auto stroke = Stroke{};
            stroke.setType(inputType);
            stroke.addSample({{10.0f, 10.0f}, 0.5f});
            stroke.addSample({{11.0f, 11.0f}, 0.5f});
            return stroke;
        }());

        model->addStroke([=] {
            auto stroke = Stroke{};
            stroke.setType(otherType);
            stroke.addSample({center, 0.5f});
            stroke.addSample({{center.x() + radius / 2.0f, center.y()}, 0.5f});
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
