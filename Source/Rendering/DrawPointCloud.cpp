#include "stdafx.h"
#include "DrawPointCloud.h"

// Public methods

AlgGeom::DrawPointCloud::DrawPointCloud(PointCloud& pointCloud) : Model3D()
{
    size_t numPoints = pointCloud.size();
    Component* component = new Component;

    for (unsigned vertexIdx = 0; vertexIdx < numPoints; vertexIdx++) {

        Point point = pointCloud.getPoint(vertexIdx);

        component->_vertices.push_back(VAO::Vertex{ vec3(point.getX(), point.getY(), .0f) });
        component->_indices[VAO::IBO_POINT].push_back(vertexIdx);
    }

    this->_components.push_back(std::unique_ptr<Component>(component));

    this->buildVao(component);
}

AlgGeom::DrawPointCloud::DrawPointCloud(PointCloud3d& pointCloud) : Model3D()
{
    size_t numPoints = pointCloud.size();
    Component* component = new Component;

    for (unsigned vertexIdx = 0; vertexIdx < numPoints; vertexIdx++) {

        Vect3d point = pointCloud.getPoint(vertexIdx);

        component->_vertices.push_back(VAO::Vertex{ vec3(point.getX(), point.getY(), point.getZ()) });
        component->_indices[VAO::IBO_POINT].push_back(vertexIdx);
    }

    this->_components.push_back(std::unique_ptr<Component>(component));

    this->buildVao(component);
}
