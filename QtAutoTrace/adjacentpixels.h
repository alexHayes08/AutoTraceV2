#ifndef ADJACENTPIXELS_H
#define ADJACENTPIXELS_H

#include <cmath>
#include <exception>
#include <limits>

#include <QPoint>

#include "pixeldata.h"

namespace QtAutoTraceV2
{
	enum AdjacentResult
	{
		NOTADJACENT,
		NORTH,
		WEST,
		SOUTH,
		EAST
	};

	enum AdjacentDirection
	{
		NORTH,
		NORTHWEST,
		WEST,
		SOUTHWEST,
		SOUTH,
		SOUTHEAST,
		EAST,
		NORTHEAST
	};

	AdjacentResult ArePixelsAdjacent(PixelData a, PixelData b)
	{
		// Return false if the colors aren't the same.
		if (a.color != b.color)
		{
			return AdjacentResult::NOTADJACENT;
		}

		return ArePointsAdjacent(a.coord, b.coord);
	}

	AdjacentResult ArePointsAdjacent(QPoint a, QPoint b)
	{
		// Check north.
		if (a.x() == b.x()
			&& a.y() == b.y() - 1)
		{
			return AdjacentResult::NORTH;
		}
		// Check west.
		else if (a.x() == b.x() - 1
			&& a.y() == b.y())
		{
			return AdjacentResult::WEST;
		}
		// Check south.
		else if (a.x() == b.x()
			&& a.y() == b.y() + 1)
		{
			return AdjacentResult::SOUTH;
		}
		// Check east.
		else if (a.x() == b.x() - 1
			&& a.y() == b.y())
		{
			return AdjacentResult::EAST;
		}
		// Not adjacent.
		else
		{
			return AdjacentResult::NOTADJACENT;
		}
	}

	/**
	 * Checks to see if point p has another point directly adjacent to it using
	 * the points in otherPoints.
	 * @param p
	 * @param otherPoints
	 */
	bool IsSurrounded(QPoint p, QVector<QPoint> otherPoints)
	{
		bool north = false;
		bool west = false;
		bool south = false;
		bool east = false;

		for (auto it = otherPoints.begin(); it != otherPoints.end(); it++)
		{
			auto otherPoint = *it;
			auto result = ArePointsAdjacent(p, otherPoint);

			switch (result)
			{
			case AdjacentResult::NORTH:
				north = true;
				break;
			case AdjacentResult::WEST:
				west = true;
				break;
			case AdjacentResult::SOUTH:
				south = true;
				break;
			case AdjacentResult::EAST:
				east = true;
				break;
			}

			// Exit loop if done.
			if (north && west && south && east)
			{
				break;
			}
		}

		return north && west && south && east;
	}

	/**
	 * Determines if the outlinePoint has at LEAST two other points adjacent to
	 * it but still has at LEAST one side without any adjacent point.
	 * @param outlinePoint
	 * @param otherOutlinePoints
	 */
	bool IsPointOutliningAndCyclic(QPoint point, QVector<QPoint> otherPoints)
	{
		int adjacentPoints = 0;

		for (auto it = otherPoints.begin(); it != otherPoints.end(); it++)
		{
			auto result = ArePointsAdjacent(point, *it);

			switch (result)
			{
			case AdjacentResult::NORTH:
			case AdjacentResult::WEST:
			case AdjacentResult::SOUTH:
			case AdjacentResult::EAST:
				adjacentPoints++;
				break;
			}

			if (adjacentPoints >= 4)
			{
				break;
			}
		}

		// Return true there are only two or three adjacent sides.
		return adjacentPoints == 2 || adjacentPoints == 3;
	}

	QVector<QPoint*> FindClosestPoints(QPoint point, QVector<QPoint> otherPoints)
	{
		auto result = QVector<QPoint*>();
		double shortestHyp = std::numeric_limits<double>::max();
		auto x1 = point.x();
		auto y1 = point.y();

		for (auto it = otherPoints.begin(); it != otherPoints.end(); it++)
		{
			auto otherPoint = *it;
			auto x2 = otherPoint.x();
			auto y2 = otherPoint.y();
			double hyp = std::hypot(std::abs(x1 - x2), std::abs(y1 - y2));

			if (hyp == shortestHyp)
			{
				result.append(&otherPoint);
			}
			else if (hyp < shortestHyp)
			{
				result.clear();
				result.append(&otherPoint);
			}
		}

		return result;
	}

	AdjacentDirection GetDirectionOfPointRelativeTo(QPoint point, QPoint relativeTo)
	{
		auto dx = point.x() - relativeTo.x();
		auto dy = point.y() - relativeTo.y();

		// Verify the points aren't at the same position.
		if (dx == 0
			&& dy == 0)
		{
			throw std::runtime_error("Points cannot be at identical postions.");
		}

		if (dx == 0 && dy < 0)
			return AdjacentDirection::NORTH;
		else if (dx < 0 && dy < 0)
			return AdjacentDirection::NORTHWEST;
		else if (dx < 0 && dy == 0)
			return AdjacentDirection::WEST;
		else if (dx < 0 && dy > 0)
			return AdjacentDirection::SOUTHWEST;
		else if (dx == 0 && dy > 0)
			return AdjacentDirection::SOUTH;
		else if (dx > 0 && dy > 0)
			return AdjacentDirection::SOUTHEAST;
		else if (dx > 0 && dy == 0)
			return AdjacentDirection::EAST;
		else if (dx > 0 && dy < 0)
			return AdjacentDirection::NORTHEAST;
		else
			throw std::runtime_error("Failed to determine the relative direction of the point.");
	}
}

#endif
