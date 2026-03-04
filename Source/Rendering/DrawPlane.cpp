#include "stdafx.h"
#include "DrawPlane.h"

AlgGeom::DrawPlane::DrawPlane(Plane& plane) : Model3D()
{
    Component* component = new Component;

    float t = 1000.0f;
    std::vector<Vect3d> points = plane.getDrawingPoints(t);

    for (unsigned int i = 0; i < points.size(); i++) {
        Vect3d v = points[i];
        component->_vertices.push_back(VAO::Vertex{ vec3(v.getX(), v.getY(), v.getZ()) });
        component->_indices[VAO::IBO_LINE].insert(component->_indices[VAO::IBO_LINE].end(), { i, (i + 1) % points.size(), RESTART_PRIMITIVE_INDEX });
    }

    this->_components.push_back(std::unique_ptr<Component>(component));

    this->buildVao(component);
}