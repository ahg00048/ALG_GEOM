#pragma once

/* 
 *  File:   DrawSegment.h
 *  Author: lidia
 *
 *  Created on 19 de enero de 2021, 13:10
 */

#include "SegmentLine.h"
#include "SegmentLine3d.h"
#include "Model3D.h"

namespace AlgGeom
{
    class DrawSegment : public Model3D 
    {
    public:
        DrawSegment(SegmentLine& t);
        DrawSegment(SegmentLine3d& t);
        DrawSegment(const DrawSegment& drawSegment) = delete;
        ~DrawSegment() override {};
    };
}