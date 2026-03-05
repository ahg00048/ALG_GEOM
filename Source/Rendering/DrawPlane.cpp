#include "stdafx.h"
#include "DrawPlane.h"

AlgGeom::DrawPlane::DrawPlane(Plane& plane) : Model3D()
{
    Component* component = new Component;

    float t = 1000.0f;
    std::vector<Vect3d> points = plane.getDrawingPoints(t);
    vec3 n = plane.getNormal();

    for (unsigned i = 0; i < points.size(); i++) 
    {
        Vect3d v = points[i];
        component->_vertices.push_back(VAO::Vertex{ v, n });
    }

    for (unsigned i = 0; i < points.size(); i++) 
    {
        Vect3d v = points[i];
        component->_indices[VAO::IBO_TRIANGLE].insert(
            component->_indices[VAO::IBO_TRIANGLE].end(),
            { static_cast<unsigned>(component->_vertices.size()) - 1, i, (i + 1) % points.size(), RESTART_PRIMITIVE_INDEX});
        component->_indices[VAO::IBO_LINE].insert(
            component->_indices[VAO::IBO_LINE].end(), 
            { i, (i + 1) % points.size(), RESTART_PRIMITIVE_INDEX });
    }

    component->completeTopology();
    this->_components.push_back(std::unique_ptr<Component>(component));

    this->buildVao(component);
}