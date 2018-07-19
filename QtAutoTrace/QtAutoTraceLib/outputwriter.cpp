#include "outputwriter.h"

namespace QtAutoTraceV2
{

OutputWriter::OutputWriter(QObject *parent) : QObject(parent)
{ }

void OutputWriter::generate(QVector<SvgElement> &elements,
    QRect &size,
    InputOptions &inputoptions)
{
    this->svgGenerator.setFileName(inputoptions.outputFileName);
    this->svgGenerator.setSize(QSize(size.width(), size.height()));
    this->svgGenerator.setDescription("Generated with QtAutoraceV2.");
    this->svgGenerator.setViewBox(QRect(0, 0, size.width(), size.height()));

    QPainter painter;
    if (!painter.begin(&this->svgGenerator))
    {
        auto exc = QException();
        emit this->error(exc);
        return;
    }

    for (SvgElement element : elements)
    {
        QPainterPath path;

        auto firstPoint = element.outlinePoints.first();
        path.moveTo(firstPoint);

        for (int i = 1; i < element.outlinePoints.size() - 1; i++)
        {
            auto point = element.outlinePoints[i];

            // TODO: Handle arcs, rectangles, and circles.
            path.lineTo(point);
        }

        if (element.isClosed)
        {
            path.closeSubpath();
        }

        path.simplified();
        painter.setPen(element.color);
        painter.drawPath(path);
    }

    painter.end();
    emit this->finished();
}

}
