#include "stdafx.h"
#include "PointCloud3d.h"


PointCloud3d::PointCloud3d() : _maxPoint(INFINITY, -INFINITY, -INFINITY), _minPoint(INFINITY, INFINITY, INFINITY)
{
}

PointCloud3d::PointCloud3d(std::vector<Vect3d>& pointCloud) :_points(pointCloud), _maxPoint(INFINITY, -INFINITY, -INFINITY), _minPoint(INFINITY, INFINITY, INFINITY)
{
}

PointCloud3d::PointCloud3d(const std::string& filename) : _maxPoint(-INFINITY, -INFINITY, -INFINITY), _minPoint(INFINITY, INFINITY, INFINITY)
{
	auto splitByComma = [this](std::string& string) -> std::vector<std::string>
		{
			std::stringstream ss(string);
			std::vector<std::string> result;

			while (ss.good())
			{
				std::string substr;
				getline(ss, substr, ',');
				result.push_back(substr);
			}

			return result;
		};

	std::string currentLine; 				// Línea actual del fichero.
	std::ifstream inputStream;				// Flujo de entrada.
	inputStream.open(filename.c_str());

	while (std::getline(inputStream, currentLine))
	{
		std::vector<std::string> coord = splitByComma(currentLine);

		if (coord.size() == 3)
		{
			try {
				Vect3d point(std::stof(coord[0].c_str(), nullptr), std::stof(coord[1].c_str(), nullptr), (std::stof(coord[2].c_str(), nullptr)));
				this->addPoint(point);
			}
			catch (const std::exception& excep)
			{
				inputStream.close();

				throw excep;
			}
		}
	}
	inputStream.close();					// Cerramos fichero.
}

PointCloud3d::PointCloud3d(int size, float max_x, float max_y, float max_z) : _maxPoint(-INFINITY, -INFINITY, -INFINITY), _minPoint(INFINITY, INFINITY, INFINITY)
{
	_points = std::vector<Vect3d>();

	while (size > 0)
	{
		float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max_x * 2.0f))) - max_x;
		float y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max_y * 2.0f))) - max_y;
		float z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max_z * 2.0f))) - max_z;
		Vect3d val(x, y, z);
		this->addPoint(val);

		--size;
	}
}

PointCloud3d::PointCloud3d(int size, float radius) : _maxPoint(-INFINITY, -INFINITY, -INFINITY), _minPoint(INFINITY, INFINITY, INFINITY)
{
	_points = std::vector<Vect3d>();

	while (size > 0)
	{
		float theta = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)) * 2.0f * glm::pi<float>();
		float phi = std::acos(1.0f - 2.0f * static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)));
		double x = std::sin(phi) * std::cos(theta);
		double y = std::sin(phi) * std::sin(theta);
		double z = std::cos(phi);

		float r = radius * std::sqrt(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX)));
		Vect3d point(r * x, r * y, r * z);
		this->addPoint(point);

		--size;
	}
}

PointCloud3d::PointCloud3d(const PointCloud3d& pointCloud)
	: _points(pointCloud._points), _maxPoint(pointCloud._maxPoint), _minPoint(pointCloud._minPoint)
{
}

PointCloud3d::~PointCloud3d()
{

}

void PointCloud3d::addPoint(Vect3d& p)
{
	_points.push_back(p);
	this->updateMaxMin(_points.size() - 1);
}

AABB PointCloud3d::getAABB()
{
	return AABB(_minPoint, _maxPoint);
}

Vect3d PointCloud3d::getPoint(int pos)
{
	if ((pos >= 0) && (pos < _points.size())) {
		return _points[pos];
	}
	return Vect3d();
}

PointCloud3d& PointCloud3d::operator=(const PointCloud3d& pointCloud)
{
	if (this != &pointCloud)
	{
		_points = pointCloud._points;
		_maxPoint = pointCloud._maxPoint;
		_minPoint = pointCloud._minPoint;
	}

	return *this;
}

void PointCloud3d::save(const std::string& filename)
{
	std::ofstream file(filename);

	for (int i = 0; i < _points.size(); ++i)
	{
		file << _points[i].getX() << ", " << _points[i].getY() << ", " << _points[i].getZ() << std::endl;
	}

	file.close();
}


/// PROTECTED METHODS

void PointCloud3d::updateMaxMin(int index)
{
	Vect3d point = _points[index];

	if (point.getX() < _minPoint.getX()) { _minPoint.setX(point.getX()); _minPointIndex.setX(index); }
	if (point.getY() < _minPoint.getY()) { _minPoint.setY(point.getY()); _minPointIndex.setY(index); }
	if (point.getZ() < _minPoint.getZ()) { _minPoint.setZ(point.getZ()); _minPointIndex.setZ(index); }

	if (point.getX() > _maxPoint.getX()) { _maxPoint.setX(point.getX()); _maxPointIndex.setX(index); }
	if (point.getY() > _maxPoint.getY()) { _maxPoint.setY(point.getY()); _maxPointIndex.setY(index); }
	if (point.getZ() > _maxPoint.getZ()) { _maxPoint.setZ(point.getZ()); _maxPointIndex.setZ(index); }
}

void PointCloud3d::getMostDistanced(int& a, int& b)
{
	int candidates[2] = { 0, 0 };
	double candidatesDist[2] = { 0.0f, 0.0f };
	int currentMin = 0;

	for (unsigned int i = 0; i < _points.size(); i++)
	{
		// Calculamos distancia
		double iDist = 0.0f;
		for (unsigned int j = 0; j < _points.size(); j++)
		{
			if (j == i)
				continue;

			iDist += _points[i].distance(_points[j]);
		}

		if (candidatesDist[currentMin] < iDist)
		{
			candidatesDist[currentMin] = iDist;
			candidates[currentMin] = i;

			currentMin = candidatesDist[currentMin] < candidates[(currentMin + 1) % 2] ? currentMin : ((currentMin + 1) % 2);
		}
	}

	a = candidates[0]; 
	b = candidates[1];
}


std::vector<Triangle3d> PointCloud3d::giftWrapping()
{
	std::vector<Triangle3d> triangles;
	std::list<SegmentLine3d> boundaryCH;
	std::set<Vect3d> pointsInCH;

	std::vector<Vect3d> projectedPC;

	// lowest y point
	Vect3d pointA = _points.back();
	pointA.setZ(0.0);
	Vect3d pointB;
	Vect3d pointC;

	Vect3d truePointA;
	Vect3d truePointB;
	Vect3d truePointC;

	// Projecting PC in XY plane
	for (int i = 0; i < _points.size(); i++)
	{
		Vect3d v = _points[i];
		v.setZ(0.0);
		projectedPC.push_back(v);

		if (v.getY() < pointA.getY())
		{
			pointA = v;
			truePointA = _points[i];
		}
		else
		{
			pointB = v;
			truePointB = _points[i];
		}
	}

	Vect3d aux(1.0, 0.0, 0.0);
	aux = aux.add(pointA);
	SegmentLine3d segmentX(pointA, aux);
	SegmentLine3d segmentAB(pointA, pointB);

	// We find segmentAB
	float minAngleFound = segmentX.angle(segmentAB);
	for (int i = 0; i < projectedPC.size(); i++)
	{
		Vect3d v = projectedPC[i];

		if (v == pointA) continue;

		segmentAB.setDestination(v);
		float auxAngle = segmentX.angle(segmentAB);

		if (auxAngle < minAngleFound)
		{
			minAngleFound = auxAngle;
			pointB = v;
			truePointB = _points[i];
		}
		else
		{
			pointC = v;
			truePointC = _points[i];
		}
	}

	segmentAB.setOrigin(truePointA);
	segmentAB.setDestination(truePointB);

	Triangle3d triABC(truePointA, truePointB, truePointC);

	// Find point c
	for (Vect3d& v1 : _points)
	{
		if (v1 == truePointA || v1 == truePointB) continue;

		triABC.setC(v1);
		bool valid = true;

		for (Vect3d& v2 : _points)
		{
			if (v2 == truePointA || v2 == truePointB || v2 == v1) continue;

			Triangle3d::PointPosition classi = triABC.classify(v2);

			if (classi == Triangle3d::PointPosition::POSITIVE) 
			{
				valid = false;
				break;
			}
		}

		if (valid) truePointC = v1;
	}

	SegmentLine3d segmentBC(truePointB, truePointC);
	SegmentLine3d segmentCA(truePointC, truePointA);

	boundaryCH.push_back(segmentAB);
	boundaryCH.push_back(segmentBC);
	boundaryCH.push_back(segmentCA);

	pointsInCH.insert(truePointA);
	pointsInCH.insert(truePointB);
	pointsInCH.insert(truePointC);

	triangles.push_back(triABC);

	while (!boundaryCH.empty())
	{
		SegmentLine3d segmentAux = boundaryCH.front();
		boundaryCH.pop_front();

		Vect3d segOrig = segmentAux.getOrigin();
		Vect3d segDest = segmentAux.getDestination();

		Triangle3d triAux(segOrig, segDest, _points.back());
		
		for (Vect3d& v1 : _points)
		{
			triAux.setC(v1);
			
			if (v1 == segOrig || v1 == segDest || std::find(triangles.cbegin(), triangles.cend(), triAux) != triangles.cend()) continue;
			
			bool valid = true;

			for (Vect3d& v2 : _points)
			{
				if (v2 == segOrig || v2 == segDest || v2 == v1) continue;

				Triangle3d::PointPosition classi = triABC.classify(v2);

				if (classi == Triangle3d::PointPosition::POSITIVE) valid = false;
			}

			if (valid) truePointC = v1;
		}

		triAux.setC(truePointC);
		triangles.push_back(triAux);

		// Edges D-V and E-V
		segmentAB.setOrigin(segOrig);
		segmentAB.setDestination(truePointC);

		segmentBC.setOrigin(segDest);
		segmentBC.setDestination(truePointC);

		if (!pointsInCH.contains(truePointC))
		{
			pointsInCH.insert(truePointC);
			boundaryCH.push_back(segmentAB);
			boundaryCH.push_back(segmentBC);
		}
		else
		{
			std::list<SegmentLine3d>::iterator it1 = std::find(boundaryCH.begin(), boundaryCH.end(), segmentAB);
			std::list<SegmentLine3d>::iterator it2 = std::find(boundaryCH.begin(), boundaryCH.end(), segmentBC);

			if (it1 != boundaryCH.end() && it2 == boundaryCH.end())
			{
				boundaryCH.erase(it1);
				boundaryCH.push_back(segmentBC);
			}
			else if (it1 == boundaryCH.end() && it2 != boundaryCH.end())
			{
				boundaryCH.erase(it2);
				boundaryCH.push_back(segmentAB);
			}
			else if (it1 != boundaryCH.end() && it2 != boundaryCH.end())
			{
				boundaryCH.erase(it1);
				boundaryCH.erase(it2);
			}
			else
			{
				boundaryCH.push_back(segmentAB);
				boundaryCH.push_back(segmentBC);
			}
		}
	}

	return triangles;
}