#include "pixelgroup.h"

namespace QtAutoTraceV2
{
	PixelGroup::PixelGroup(QVector<QPoint> pixels)
	{
		// Assign pixels.
		this->pixels = pixels;

		// Assign bbox.
		qint64 top = pixels.first().y;
		qint64 left = pixels.first().x;
		qint64 right = top;
		qint64 bottom = left;

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

		auto width = right - left;
		auto height = bottom - top;

		this->bbox = QRect(left, top, width, height);

		// Assign outline.
		this->outline = QVector<QPoint*>();
		for (auto it = this->getPixels().begin(); it != this->getPixels().end(); it++)
		{
			auto point = *it;
			if (!IsSurrounded(point, this->getPixels()))
			{
				this->outline.append(&point);
			}
		}

		// Assign closed, to determine this each point must have two other points
		// adjacent to it.
		auto outline = this->getOutline();
		bool isClosed = std::all_of(this->getOutline().begin(),
			this->getOutline().end(),
			[outline](QPoint point)
			{
				return IsPointOutliningAndCyclic(point, outline);
			});

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
}
