// This file is part of Washi Pad
// SPDX-FileCopyrightText: 2018 Kevin Ottens <ervin@kde.org>
// SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

#include "sketchserializer.h"

#include <functional>
#include <memory>

#include <QImageWriter>
#include <QPainter>
#include <QUrl>
#include <QSvgGenerator>

#include "sketchmodel.h"
#include "strokepainter.h"

class Serializer
{
public:
    Serializer(std::unique_ptr<QPaintDevice> &&device,
               const std::function<void(QPaintDevice*)> writer = {})
        : m_device(std::move(device)),
          m_writer(writer)
    {
    }

    ~Serializer()
    {
        if (m_writer)
            m_writer(m_device.get());
    }

    QPaintDevice *device() const
    {
        return m_device.get();
    }

private:
    std::unique_ptr<QPaintDevice> m_device;
    std::function<void(QPaintDevice *)> m_writer;
};


std::unique_ptr<Serializer> createSerializer(const QSize &size, const QString &fileName)
{
    if (fileName.endsWith(QLatin1String(".svg"))) {
        auto generator = std::make_unique<QSvgGenerator>();
        generator->setFileName(fileName);
        generator->setViewBox(QRect{{0, 0}, size});
        return std::make_unique<Serializer>(std::move(generator));
    } else {
        auto image = std::make_unique<QImage>(size, QImage::Format_RGB32);
        image->fill(Qt::white);

        auto writeImage = [fileName](QPaintDevice *image) {
            QImageWriter writer(fileName);
            writer.write(*static_cast<QImage*>(image));
        };

        return std::make_unique<Serializer>(std::move(image), writeImage);
    }
}

void SketchSerializer::serialize(SketchModel *model, const QSize &size, const QUrl &fileUrl)
{
    Q_ASSERT(fileUrl.isLocalFile());

    auto strokes = model->strokes();
    auto serializer = createSerializer(size, fileUrl.toLocalFile());
    auto painter = std::make_unique<QPainter>(serializer->device());
    painter->setRenderHint(QPainter::Antialiasing);

    for (const auto &stroke : std::as_const(strokes)) {
        StrokePainter::render(stroke, painter.get());
    }
}

#include "moc_sketchserializer.cpp"
