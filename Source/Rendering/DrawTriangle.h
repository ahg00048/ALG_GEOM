#pragma once

/* 
 * File:   DrawTriangle.h
 * Author: lidia
 *
 * Created on 5 de enero de 2021, 14:11
 */

#include "Triangle.h"
#include "Triangle3d.h"
#include "Model3D.h"


namespace AlgGeom
{
    class DrawTriangle : public Model3D
    {
    public:
        DrawTriangle(Triangle& t);
        DrawTriangle(Triangle3d& t);
        DrawTriangle(const DrawTriangle& drawTriangle) = delete;
        ~DrawTriangle() override {};
    };
}