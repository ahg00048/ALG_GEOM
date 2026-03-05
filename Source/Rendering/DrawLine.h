#pragma once

/* 
 *  File:   DrawLine.h
 *  Author: lidia
 *
 *  Created on 20 de enero de 2021, 12:06
 */

#include "Line.h"
#include "Line3d.h"
#include "Model3D.h"

namespace AlgGeom
{
    class DrawLine : public Model3D 
    {
    public:
        DrawLine(Line& t);
        DrawLine(Line3d& t);
        DrawLine(const DrawLine& drawLine) = delete;
        ~DrawLine() override {};
    };
}

