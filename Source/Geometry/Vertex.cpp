#include "stdafx.h"
#include "Polygon.h"

#include "Vertex.h"

// Public methods

Vertex::Vertex() : Point()
{
	_position = INVALID_POSITION;
	_polygon = nullptr;
}

Vertex::Vertex(const Point& point) : Point(point)
{
	_position = INVALID_POSITION;
	_polygon = nullptr;
}

Vertex::Vertex(const Point & point, Polygon* polygon, int pos) : Point(point)
{
	_position = pos;
	_polygon = polygon;
}

Vertex::~Vertex()
{
}

bool Vertex::convex()
{
	Vertex imas1 = Vertex(next());
	Vertex imenos1 = Vertex(previous());
	return (imas1.left(imenos1, *this));
}

bool Vertex::concave()
{
	return !convex();
}

Vertex Vertex::next()
{
	if (!_polygon)
		return Vertex();

	return _polygon->getVertexAt((_position + 1) % _polygon->getNumVertices());
}

SegmentLine Vertex::nextEdge()
{
	return SegmentLine(*this, next());
}

Vertex & Vertex::operator=(const Vertex & vertex)
{
	if (this != &vertex)
	{
		Point::operator=(vertex);
		this->_polygon = vertex._polygon;
		this->_position = vertex._position;
	}

	return *this;
}

std::ostream& operator<<(std::ostream& os, const Vertex& vertex)
{
	os << "Position: " << std::to_string(vertex._position);

	return os;
}

Vertex Vertex::previous()
{
	if (!_polygon)
		return Vertex();

	return _polygon->getVertexAt((_position - 1) % _polygon->getNumVertices());
}

SegmentLine Vertex::previousEdge()
{
	return SegmentLine(previous(), *this);
}