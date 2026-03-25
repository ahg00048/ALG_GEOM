#include "stdafx.h"
#include "DrawTriangle.h"

// Public methods

AlgGeom::DrawTriangle::DrawTriangle (Triangle &triangle): Model3D()
{  
    Component* component = new Component;
    component->_vertices.insert(component->_vertices.end(), {
            VAO::Vertex { vec3(triangle.getA().getX(), triangle.getA().getY(), .0f), vec3(.0f, .0f, 1.0f) },
            VAO::Vertex { vec3(triangle.getB().getX(), triangle.getB().getY(), .0f), vec3(.0f, .0f, 1.0f) },
            VAO::Vertex { vec3(triangle.getC().getX(), triangle.getC().getY(), .0f), vec3(.0f, .0f, 1.0f) }
        });
    component->_indices[VAO::IBO_TRIANGLE].insert(component->_indices[VAO::IBO_TRIANGLE].end(), { 0, 1, 2, RESTART_PRIMITIVE_INDEX });
    component->completeTopology();
    this->_components.push_back(std::unique_ptr<Component>(component));

    this->buildVao(component);
}

AlgGeom::DrawTriangle::DrawTriangle(Triangle3d& triangle) : Model3D()
{
    Vect3d triN = triangle.normal();
    vec3 n(triN.getX(), triN.getY(), triN.getZ());

    Component* component = new Component;
    component->_vertices.insert(component->_vertices.end(), {
            VAO::Vertex { vec3(triangle.getA().getX(), triangle.getA().getY(), triangle.getA().getZ()), n },
            VAO::Vertex { vec3(triangle.getB().getX(), triangle.getB().getY(), triangle.getB().getZ()), n },
            VAO::Vertex { vec3(triangle.getC().getX(), triangle.getC().getY(), triangle.getC().getZ()), n }
        });
    component->_indices[VAO::IBO_TRIANGLE].insert(component->_indices[VAO::IBO_TRIANGLE].end(), { 0, 1, 2, RESTART_PRIMITIVE_INDEX });
    component->completeTopology();
    
    this->_components.push_back(std::unique_ptr<Component>(component));
    this->buildVao(component);
}
