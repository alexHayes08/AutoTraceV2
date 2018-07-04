#ifndef PIXELGROUP_H
#define PIXELGROUP_H

#include <algorithm>
#include <exception>

#include <QPoint>
#include <QRect>
#include <QVector>

#include "adjacentpixels.h"

namespace QtAutoTraceV2
{

class PixelGroup
{
public:
	/**
	 * @briefDefault constructor.
	 */
	PixelGroup() : PixelGroup(QVector<QPoint>()) {}

	/**
	 * @param pixels All points should be connected (there should be one other
	 * point one unit away from it).
	 */
	PixelGroup(QVector<QPoint> pixels);

	/**
	 * @brief Returns true if there are no pixels.
	 */
	bool isEmpty();

	/**
	 * Returns true if the outline (or part of the outline) can fully contain
	 * an ellipse.
	 */
	bool isClosed();
	QVector<QPoint> getPixels() const;
	QRect getBBox() const;
	QVector<QPoint> getOutline() const;
    bool isFilled();

private:
	QRect bbox;
	QVector<QPoint> pixels;
	QVector<QPoint*> outline;
	bool closed;
    bool filled;
};

}

#endif // PIXELGROUP_H
