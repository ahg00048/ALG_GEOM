#include "stdafx.h"
#include "PointCloud.h"
#include "RandomUtilities.h"
#include "StringUtilities.h"


PointCloud::PointCloud()
{
}

PointCloud::PointCloud(int size, float max_x, float max_y)
{	
	float min = 0.0f;
	for (int i = 0; i < size; i++) 
	{
		_points.emplace_back(RandomUtilities::getUniformRandom(min, max_x), 
							 RandomUtilities::getUniformRandom(min, max_y));
	}
}

PointCloud::PointCloud(const std::string& filename)
{
	save(filename);
}

PointCloud::~PointCloud()
{
}

void PointCloud::addPoint(const Point& p)
{
	_points.push_back(p);
}

Point PointCloud::centralPoint()
{
	Point* sP = nullptr;
	double minDistance = 1.0;
	for (Point& p : _points) 
	{
		double totalDistance = 0.0;
		for (Point& p2 : _points)
		{
			if (p.equal(p2)) continue;
			totalDistance += p.distance(p2);
		}

		if (minDistance > totalDistance)
		{
			minDistance = totalDistance;
			sP = &p;
		}
	}

	return *sP;
}

void PointCloud::deletePoint(int index)
{
	if (index < _points.size())
	{
		_points.erase(_points.begin() + index);
	}
}

void PointCloud::getEdges(Point& minPoint_x, Point& minPoint_y, Point& maxPoint_x, Point& maxPoint_y)
{
}

Point PointCloud::getPoint(int position)
{
	if ((position >= 0) && (position < _points.size())) 
	{
		return _points[position];
	}

	return Point();
}

PointCloud & PointCloud::operator=(const PointCloud& pointCloud)
{
	if (this != &pointCloud)
	{
		this->_points = pointCloud._points;
	}

	return *this;
}

void PointCloud::save(const std::string& filename)
{
	std::string line = "";
	std::ifstream file(filename);

	if (file.is_open()) {
		while (std::getline(file, line)) 
		{
			const std::vector<std::string_view>& strs = split(line.c_str(), ';');
			_points.emplace_back(std::stod(strs[0].data()), std::stod(strs[1].data(), false));
		}
		file.close();
	}
}