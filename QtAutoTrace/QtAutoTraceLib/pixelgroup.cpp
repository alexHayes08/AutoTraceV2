#include "pixelgroup.h"

namespace QtAutoTraceV2
{
    PixelGroup::PixelGroup(QVector<QPoint> pixels)
    {
        // Assign pixels.
        this->pixels = pixels;

        // Assign bbox.
        int top = pixels.first().y();
        int left = pixels.first().x();
        int right = top;
        int bottom = left;

        for (auto i = 1; i < pixels.size(); i++)
        {
            auto pixel = pixels[i];
            auto x = pixel.x();
            auto y = pixel.y();

            top = y < top ? y : top;
            left = x < left ? x : left;
            bottom = y > bottom ? y : bottom;
            right = x > right ? x : right;
        }

        int width = right - left;
        int height = bottom - top;

        this->bbox = QRect(left, top, width, height);

        // Assign outline.
        this->outline = QVector<QPoint*>();
        for (auto it = this->getPixels().begin(); it != this->getPixels().end(); it++)
        {
            auto point = *it;
            if (!AdjacentPixels::IsSurrounded(point, this->getPixels()))
            {
                this->outline.append(&point);
            }
        }

        // Assign closed, to determine this each point must have two other points
        // adjacent to it.
        auto outline = this->getOutline();
        this->closed = std::all_of(this->getOutline().begin(),
            this->getOutline().end(),
            [outline](QPoint point)
            {
                return AdjacentPixels::IsPointOutliningAndCyclic(point, outline);
            });

        // Assign filled, determine if each pixel inside the outline is present.
        if (this->isClosed()) {

            // TODO: Check that every pixel inside the outline is present.
            throw std::runtime_error("Not yet implemented.");
        }
        else
        {
            this->filled = false;
        }

        // Next step is to retrieve the outline points that which has a different
        // angle than the previous pair of points.
    }

    bool PixelGroup::isEmpty()
    {
        return this->pixels.isEmpty();
    }

    bool PixelGroup::isClosed()
    {
        return this->closed;
    }

    bool PixelGroup::isFilled()
    {
        return this->filled;
    }

    QVector<QPoint> PixelGroup::getPixels() const
    {
        return this->pixels;
    }

    QRect PixelGroup::getBBox() const
    {
        return this->bbox;
    }

    QVector<QPoint> PixelGroup::getOutline() const
    {
        QVector<QPoint> results;

        for (auto it = this->outline.begin(); it != this->outline.end(); it++)
            results.append(**it);

        return results;
    }

    QVector<QPoint> PixelGroup::getInflectionPoints() const
    {
        QVector<QPoint> inflectionPoints;

        // Rate of change of the inflection angle
        double d_m;

        // If positive the angle acceleration is going to the left, negative to
        // the right.
        double dd_m;
        auto outline = this->getOutline();

        if (outline.size() <= 3)
        {
            return outline;
        }

        do
        {
            auto firstPoint = outline[0];
            auto secondPoint = outline[1];
            auto thirdPoint = outline[2];

            d_m = std::atan2(firstPoint.y() - secondPoint.y(),
                firstPoint.x() - secondPoint.x());

            auto d_m2 = std::atan2(secondPoint.y() - thirdPoint.y(),
                secondPoint.x() - thirdPoint.x());

            dd_m = d_m2 - d_m;
        } while (false);

        for (auto i = 1; i < outline.size(); i++)
        {
            auto previousPoint = i == 0 ? outline.last() : outline[i - 1];
            auto point = outline[i];
            auto nextPoint = i == outline.size() - 1
                ? outline.first()
                : outline[i + 1];

            auto d_prevPoint_point = std::atan2(previousPoint.y() - point.y(),
                previousPoint.x() - point.x());
            auto d_point_nextPoint = std::atan2(point.y() - nextPoint.y(),
                point.x() - nextPoint.x());
        }

        return inflectionPoints;
    }
}
