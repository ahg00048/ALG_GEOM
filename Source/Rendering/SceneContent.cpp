#include "stdafx.h"
#include "SceneContent.h"

#include "InclDraw2D.h"
#include "InclGeom2D.h"
#include "InclDraw3D.h"
#include "InclGeom3D.h"
#include "PointCloud.h"
#include "RandomUtilities.h"
#include <format>
#include <string>

// ----------------------------- BUILD YOUR SCENARIO HERE -----------------------------------

void AlgGeom::SceneContent::buildScenario()
{
    pr2A_B_C();
    //pr2D();
}


// ------------------------------------------------------------------------------------------


AlgGeom::SceneContent::SceneContent()
{
}

AlgGeom::SceneContent::~SceneContent()
{
	_camera.clear();
	_model.clear();
}

void AlgGeom::SceneContent::addNewCamera(ApplicationState* appState)
{
	_camera.push_back(std::make_unique<Camera>(appState->_viewportSize.x, appState->_viewportSize.y, true));
}

void AlgGeom::SceneContent::addNewModel(Model3D* model)
{	
	_model.push_back(std::unique_ptr<Model3D>(model));
}

AlgGeom::Model3D* AlgGeom::SceneContent::getModel(Model3D::Component* component)
{
	for (auto& model : _model)
	{
		if (model->belongsModel(component))
			return model.get();
	}

	return nullptr;
}

//---------------------------------------------------------------------------------------------------------------

void AlgGeom::SceneContent::pr1A()
{
    //PR1a
 
    // PointCloud
    const std::string filePath = "./PointCloud.txt";
    const unsigned int cloudSize = 200;

    PointCloud* pc = new PointCloud(cloudSize, 5.0f, 5.0f);
    pc->save(filePath);
    this->addNewModel((new DrawPointCloud(*pc))->setPointColor(vec4(1.0f, 0.0f, 0.0f, 1.0f))->overrideModelName()->setLineWidth(2.0f));

    // Segments
    SegmentLine* sgl1 = new SegmentLine();
    SegmentLine* sgl2 = new SegmentLine();
    do
    {
        sgl1->setA(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
        sgl1->setB(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));

        sgl2->setA(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
        sgl2->setB(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
    } while (!sgl1->segmentIntersection(*sgl2));

    this->addNewModel((new DrawSegment(*sgl1))->setLineColor(vec4(1.0f, 0.0f, 0.0f, 1.0f))->overrideModelName());
    this->addNewModel((new DrawSegment(*sgl2))->setLineColor(vec4(1.0f, 0.0f, 0.0f, 1.0f))->overrideModelName());
    delete sgl1;
    delete sgl2;
    sgl1 = nullptr;
    sgl2 = nullptr;

    // Ray and line
    Point a(RandomUtilities::getUniformRandom(-5.0f, 5.0f), RandomUtilities::getUniformRandom(-5.0f, 5.0f));
    Point b(RandomUtilities::getUniformRandom(-5.0f, 5.0f), RandomUtilities::getUniformRandom(-5.0f, 5.0f));

    Line* l = new Line(a, b);

    a.set(RandomUtilities::getUniformRandom(-5.0f, 5.0f), RandomUtilities::getUniformRandom(-5.0f, 5.0f));
    b.set(RandomUtilities::getUniformRandom(-5.0f, 5.0f), RandomUtilities::getUniformRandom(-5.0f, 5.0f));

    RayLine* rl = new RayLine(a, b);

    this->addNewModel((new DrawLine(*l))->setLineColor(vec4(0.0f, 0.0f, 1.0f, 1.0f))->overrideModelName()->setLineWidth(5.0f));
    this->addNewModel((new DrawRay(*rl))->setLineColor(vec4(0.0f, 1.0f, 0.0f, 1.0f))->overrideModelName()->setLineWidth(5.0f));

    delete l;
    delete rl;
    l = nullptr;
    rl = nullptr;

    // Polygon
    Polygon* p = new Polygon();
    int pSize = 5;
    for (int i = 0; i < pSize; i++)
        p->add(Point());
    bool pCreated;

    do
    {
        pCreated = true;

        for (int i = 0; i < 5; i++)
        {
            Vertex v = pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1));
            p->set(v, i);
        }

        for (int i = 0; i < pSize; i++)
        {
            SegmentLine psgl1 = p->getEdge(i);
            for (int j = 0; j < pSize; j++)
            {
                if (j == i) continue;
                SegmentLine psgl2 = p->getEdge(j);
                if (psgl1.segmentIntersection(psgl2)) pCreated = false;
            }
        }

    } while (!pCreated);

    this->addNewModel((new DrawPolygon(*p))->setTriangleColor(vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName());

    delete p;
    p = nullptr;

    // Triangle and its circles

    Vect2d* vec1 = new Vect2d(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
    Vect2d* vec2 = new Vect2d(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
    Vect2d* vec3 = new Vect2d(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
    Triangle* tr = new Triangle(*vec1, *vec2, *vec3);

    delete vec1;
    delete vec2;
    delete vec3;
    vec1 = nullptr;
    vec2 = nullptr;
    vec3 = nullptr;

    Circle* circ = new Circle(tr->getCirumscribed());
    Circle* incs = new Circle(tr->getInscribed());
    this->addNewModel((new DrawTriangle(*tr))->setLineColor(vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName()->setLineWidth(2.0f));
    this->addNewModel((new DrawCircle(*circ))->setLineColor(vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName()->setLineWidth(3.0f));
    this->addNewModel((new DrawCircle(*incs))->setLineColor(vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName()->setLineWidth(3.0f));
    delete tr;
    delete circ;
    delete incs;
    tr = nullptr;
    circ = nullptr;
    incs = nullptr;

    delete p;
    p = nullptr;
    delete pc;
    pc = nullptr;
}

void AlgGeom::SceneContent::pr1B() 
{
    //PR1b    
    float axisDepth = 1.0f;

    // 1 Create a set of lines and a polygon making specific intersections
    vec4 rgbLTypes[4] = { {1.0f, 1.0f, 0.0f, 1.0f},     // Brown - line
                          {1.0f, 0.0f, 1.0f, 1.0f},     // Purple - ray
                          {0.0f, 1.0f, 1.0f, 1.0f},     // Aqua - segment
                          {0.0f, 0.5f, 0.0f, 1.0f} };   // Dark green - polygon
    float interPointSize = 10.0f;


    Line* l1 = new Line(randomPointInUnitDisk(axisDepth), 
                        randomPointInUnitDisk(axisDepth));
    Line* l2 = new Line(Point(), Point());

    Vect2d intersectionPoint;
    Vect2d auxIntersection;
    do
    {
        l2->setA(randomPointInUnitDisk(axisDepth));
        l2->setB(randomPointInUnitDisk(axisDepth));
    } while (!l1->intersects(*l2, intersectionPoint));

    // Draw l1, l2 and its intersection
    this->addNewModel((new DrawLine(*l1))->setLineColor(rgbLTypes[0])->overrideModelName()->setLineWidth(2.0f));
    this->addNewModel((new DrawLine(*l2))->setLineColor(rgbLTypes[0])->overrideModelName()->setLineWidth(2.0f));
    this->addNewModel((new DrawPoint(intersectionPoint))->setPointColor(vec4(1.0f, 1.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(interPointSize));

    SegmentLine* sg1 = new SegmentLine();
    do
    {
        sg1->setA(randomPointInUnitDisk(axisDepth));
        sg1->setB(randomPointInUnitDisk(axisDepth));
    } 
    while (sg1->intersects(*l1, intersectionPoint) ||
           sg1->intersects(*l2, intersectionPoint));
   
    SegmentLine* sg2 = new SegmentLine();
    do
    {
        sg2->setA(randomPointInUnitDisk(axisDepth));
        sg2->setB(randomPointInUnitDisk(axisDepth));
    } while (!sg2->intersects(*sg1, intersectionPoint) ||
              sg2->intersects(*l1, auxIntersection) ||
              sg2->intersects(*l2, auxIntersection));

    // Draw sg1, sg2 and its intersection
    this->addNewModel((new DrawSegment(*sg1))->setLineColor(rgbLTypes[2])->overrideModelName()->setLineWidth(2.0f));
    this->addNewModel((new DrawSegment(*sg2))->setLineColor(rgbLTypes[2])->overrideModelName()->setLineWidth(2.0f));
    this->addNewModel((new DrawPoint(intersectionPoint))->setPointColor(vec4(1.0f, 1.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(interPointSize));

    RayLine* r1 = new RayLine(Point(), Point());
    do
    {
        r1->setA(randomPointInUnitDisk(axisDepth));
        r1->setB(randomPointInUnitDisk(axisDepth));
    } while (!r1->intersects(*sg1, intersectionPoint) ||
              r1->intersects(*l1, auxIntersection) || 
              r1->intersects(*l2, auxIntersection) || 
              r1->intersects(*sg2, auxIntersection));

    // Draw r1 and its intersection
    this->addNewModel((new DrawRay(*r1))->setLineColor(rgbLTypes[1])->overrideModelName()->setLineWidth(2.0f));
    this->addNewModel((new DrawPoint(intersectionPoint))->setPointColor(vec4(1.0f, 1.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(interPointSize));

    int pSize = 4;
    Polygon* p = new Polygon;
    for (int i = 0; i < pSize; i++)
        p->add(Point());

    bool pCreated = true;
    bool valid = false;
    do
    {
        valid = false;
        pCreated = true;

        for (int i = 0; i < pSize; i++)
        {
            Vertex v = randomPointInUnitDisk(axisDepth);
            p->set(v, i);
        }

        for (int i = 0; i < pSize; i++)
        {
            SegmentLine sgp = p->getEdge(i);
            if (p->getEdge(i).intersects(*l1, intersectionPoint) && 
                !(sgp.intersects(*l2, auxIntersection) ||
                  sgp.intersects(*sg1, auxIntersection) ||
                  sgp.intersects(*sg2, auxIntersection) ||
                  sgp.intersects(*r1, auxIntersection)))
                valid = true;

            for (int j = 0; j < pSize; j++)
            {
                if (j == i) continue;

                SegmentLine sgpAux = p->getEdge(j);
                if (sgp.segmentIntersection(sgpAux) ||
                    sgp.intersects(*l2, auxIntersection) || 
                    sgp.intersects(*sg1, auxIntersection) ||
                    sgp.intersects(*sg2, auxIntersection) ||
                    sgp.intersects(*r1, auxIntersection))
                    
                    pCreated = false;
            }
        }
    } while (!pCreated || !valid);

    this->addNewModel((new DrawPolygon(*p))->setLineColor(rgbLTypes[3])->overrideModelName()->setLineWidth(2.0f));
    this->addNewModel((new DrawPoint(intersectionPoint))->setPointColor(vec4(1.0f, 1.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(interPointSize));

    RayLine* r2 = new RayLine(Point(), Point());
    do
    {
        pCreated = false;
        r2->setA(randomPointInUnitDisk(axisDepth));
        r2->setB(randomPointInUnitDisk(axisDepth));

        for (int i = 0; i < pSize; i++)
        {
            if (p->getEdge(i).intersects(*r2, intersectionPoint))
                pCreated = true;
        }
    } while (!pCreated ||
        r2->intersects(*l1, auxIntersection) ||
        r2->intersects(*l2, auxIntersection) ||
        r2->intersects(*sg1, auxIntersection) ||
        r2->intersects(*sg2, auxIntersection) ||
        r2->intersects(*r1, auxIntersection));

    this->addNewModel((new DrawRay(*r2))->setLineColor(rgbLTypes[1])->overrideModelName()->setLineWidth(2.0f));
    this->addNewModel((new DrawPoint(intersectionPoint))->setPointColor(vec4(1.0f, 1.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(interPointSize));

    // 2 Distance between each polygon vertex and the lines

    vec4 rgbPoint[4] = { {0.0f, 0.0f, 1.0f, 1.0f},     // 1st vertex blue
                            {0.0f, 1.0f, 0.0f, 1.0f},       // 2st vertex green
                            {1.0f, 0.0f, 0.0f, 1.0f},       // 3st vertex red
                            {0.0f, 0.0f, 0.0f, 1.0f} };     // 4st vertex black

    constexpr const char* fmt = " {} -> {} {}\n";

    std::cout << "\nEJERCICIO 2:\n"
        " 0: Azul\n 1: Verde\n 2: Rojo\n 3: Negro\n\n";

    for (int i = 0; i < p->getNumVertices(); i++)
    {
        Vect2d pPoint = p->getVertexAt(i);

        std::cout << std::format(fmt, i, "l1", l1->distToPoint(pPoint));
        std::cout << std::format(fmt, i, "l2", l2->distToPoint(pPoint));      
        std::cout << std::format(fmt, i, "sg1", sg1->distToPointSeg(pPoint));
        std::cout << std::format(fmt, i, "sg2", sg2->distToPointSeg(pPoint));        
        std::cout << std::format(fmt, i, "r1", r1->distToPoint(pPoint));
        std::cout << std::format(fmt, i, "r2", r2->distToPoint(pPoint)) << std::endl;

        this->addNewModel((new DrawPoint(pPoint))->overrideModelName()->setPointColor(rgbPoint[i])->setPointSize(interPointSize));
    }
    
    // 3 Create two circles and determine its relationship between them, as well with the lines
    // Each pos of the array corresponds to a color and a circle rel based on the enum value

    vec4 rgbCircleLine[3] = { {0.0f, 0.0f, 1.0f, 1.0f},
                            {0.0f, 1.0f, 0.0f, 1.0f},
                            {0.0f, 1.0f, 1.0f, 1.0f} };

    Circle* c1 = new Circle(randomPointInUnitDisk(axisDepth), RandomUtilities::getUniformRandom(0.01f, 1.0f));
    Circle* c2 = new Circle(randomPointInUnitDisk(axisDepth), RandomUtilities::getUniformRandom(0.01f, 1.0f));

    Circle::RelationCircles c1c2 = c1->relationCircle(*c2);

    Circle::RelationCircleLine c1l1 = c1->relationLine(*l1);
    Circle::RelationCircleLine c1l2 = c1->relationLine(*l2);

    Circle::RelationCircleLine c2l1 = c2->relationLine(*l1);
    Circle::RelationCircleLine c2l2 = c2->relationLine(*l2);

    std::cout << "\nEJERCICIO 3:\n";
    std::cout << " C1 - C2: " << Circle::circleRelToString(c1c2) << std::endl;
    std::cout << " C1 -> l1: " << Circle::lineRelToString(c1l1) << std::endl;
    std::cout << " C1 -> l2: " << Circle::lineRelToString(c1l2) << std::endl;
    std::cout << " C2 -> l1: " << Circle::lineRelToString(c2l1) << std::endl;
    std::cout << " C2 -> l2: " << Circle::lineRelToString(c2l2) << std::endl;

    this->addNewModel((new DrawCircle(*c1))->setLineColor(rgbCircleLine[0])->overrideModelName()->setLineWidth(3.0f));
    this->addNewModel((new DrawCircle(*c2))->setLineColor(rgbCircleLine[1])->overrideModelName()->setLineWidth(3.0f));

    // 4 Intersection points will be drawn in white
    
    // c1 and c2 with l1
    
    Circle::RelationCircleLine cRel = c1->intersect(*l1, intersectionPoint, auxIntersection);
    if(cRel != Circle::RelationCircleLine::NO_INTERSECT)
        this->addNewModel((new DrawPoint(intersectionPoint))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));
    if(cRel == Circle::RelationCircleLine::INTERSECT)
        this->addNewModel((new DrawPoint(auxIntersection))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));

    cRel = c2->intersect(*l1, intersectionPoint, auxIntersection);
    if (cRel != Circle::RelationCircleLine::NO_INTERSECT)
        this->addNewModel((new DrawPoint(intersectionPoint))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));
    if (cRel == Circle::RelationCircleLine::INTERSECT)
        this->addNewModel((new DrawPoint(auxIntersection))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));

    // c1 and c2 with l2

    cRel = c1->intersect(*l2, intersectionPoint, auxIntersection);
    if (cRel != Circle::RelationCircleLine::NO_INTERSECT)
        this->addNewModel((new DrawPoint(intersectionPoint))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));
    if (cRel == Circle::RelationCircleLine::INTERSECT)
        this->addNewModel((new DrawPoint(auxIntersection))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));

    cRel = c2->intersect(*l2, intersectionPoint, auxIntersection);
    if (cRel != Circle::RelationCircleLine::NO_INTERSECT)
        this->addNewModel((new DrawPoint(intersectionPoint))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));
    if (cRel == Circle::RelationCircleLine::INTERSECT)
        this->addNewModel((new DrawPoint(auxIntersection))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));

    // c1 and c2 with r1

    Point testValue(INFINITY, INFINITY);

    auxIntersection.set(INFINITY, INFINITY);
    cRel = c1->intersect(*r1, intersectionPoint, auxIntersection);
    if (cRel != Circle::RelationCircleLine::NO_INTERSECT)
        this->addNewModel((new DrawPoint(intersectionPoint))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));
    if (cRel == Circle::RelationCircleLine::INTERSECT && 
        !auxIntersection.equal(testValue))
        this->addNewModel((new DrawPoint(auxIntersection))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));

    auxIntersection.set(INFINITY, INFINITY);
    cRel = c2->intersect(*r1, intersectionPoint, auxIntersection);
    if (cRel != Circle::RelationCircleLine::NO_INTERSECT)
        this->addNewModel((new DrawPoint(intersectionPoint))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));
    if (cRel == Circle::RelationCircleLine::INTERSECT && 
        !auxIntersection.equal(testValue))
        this->addNewModel((new DrawPoint(auxIntersection))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));

    // c1 and c2 with r2

    auxIntersection.set(INFINITY, INFINITY);
    cRel = c1->intersect(*r2, intersectionPoint, auxIntersection);
    if (cRel != Circle::RelationCircleLine::NO_INTERSECT)
        this->addNewModel((new DrawPoint(intersectionPoint))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));
    if (cRel == Circle::RelationCircleLine::INTERSECT && 
        !auxIntersection.equal(testValue))
        this->addNewModel((new DrawPoint(auxIntersection))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));

    auxIntersection.set(INFINITY, INFINITY);
    cRel = c2->intersect(*r2, intersectionPoint, auxIntersection);
    if (cRel != Circle::RelationCircleLine::NO_INTERSECT)
        this->addNewModel((new DrawPoint(intersectionPoint))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));
    if (cRel == Circle::RelationCircleLine::INTERSECT &&
        !auxIntersection.equal(testValue))
        this->addNewModel((new DrawPoint(auxIntersection))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));

    // c1 and c2 with sg1

    auxIntersection.set(INFINITY, INFINITY);
    cRel = c1->intersect(*sg1, intersectionPoint, auxIntersection);
    if (cRel != Circle::RelationCircleLine::NO_INTERSECT)
        this->addNewModel((new DrawPoint(intersectionPoint))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));
    if (cRel == Circle::RelationCircleLine::INTERSECT &&
        !auxIntersection.equal(testValue))
        this->addNewModel((new DrawPoint(auxIntersection))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));

    auxIntersection.set(INFINITY, INFINITY);
    cRel = c2->intersect(*sg1, intersectionPoint, auxIntersection);
    if (cRel != Circle::RelationCircleLine::NO_INTERSECT)
        this->addNewModel((new DrawPoint(intersectionPoint))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));
    if (cRel == Circle::RelationCircleLine::INTERSECT &&
        !auxIntersection.equal(testValue))
        this->addNewModel((new DrawPoint(auxIntersection))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));

    // c1 and c2 with sg2

    auxIntersection.set(INFINITY, INFINITY);
    cRel = c1->intersect(*sg2, intersectionPoint, auxIntersection);
    if (cRel != Circle::RelationCircleLine::NO_INTERSECT)
        this->addNewModel((new DrawPoint(intersectionPoint))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));
    if (cRel == Circle::RelationCircleLine::INTERSECT &&
        !auxIntersection.equal(testValue))
        this->addNewModel((new DrawPoint(auxIntersection))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));

    auxIntersection.set(INFINITY, INFINITY);
    cRel = c2->intersect(*sg2, intersectionPoint, auxIntersection);
    if (cRel != Circle::RelationCircleLine::NO_INTERSECT)
        this->addNewModel((new DrawPoint(intersectionPoint))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));
    if (cRel == Circle::RelationCircleLine::INTERSECT &&
        !auxIntersection.equal(testValue))
        this->addNewModel((new DrawPoint(auxIntersection))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));

    // c1 and c2 with p

    for (int i = 0; i < pSize; i++)
    {
        SegmentLine sgp = p->getEdge(i);

        auxIntersection.set(INFINITY, INFINITY);
        cRel = c1->intersect(sgp, intersectionPoint, auxIntersection);
        if (cRel != Circle::RelationCircleLine::NO_INTERSECT)
            this->addNewModel((new DrawPoint(intersectionPoint))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));
        if (cRel == Circle::RelationCircleLine::INTERSECT &&
            !auxIntersection.equal(testValue))
            this->addNewModel((new DrawPoint(auxIntersection))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));

        auxIntersection.set(INFINITY, INFINITY);
        cRel = c2->intersect(sgp, intersectionPoint, auxIntersection);
        if (cRel != Circle::RelationCircleLine::NO_INTERSECT)
            this->addNewModel((new DrawPoint(intersectionPoint))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));
        if (cRel == Circle::RelationCircleLine::INTERSECT &&
            !auxIntersection.equal(testValue))
            this->addNewModel((new DrawPoint(auxIntersection))->setPointColor(rgbCircleLine[2])->overrideModelName()->setPointSize(interPointSize));
    }

    // Free resources

    delete l1;
    l1 = nullptr;
    delete l2;
    l2 = nullptr;
    delete r1;
    r1 = nullptr;
    delete r2;
    r2 = nullptr;
    delete sg1;
    sg1 = nullptr;
    delete sg2;
    sg2 = nullptr;
    delete p;
    p = nullptr;
    delete c1;
    c1 = nullptr;
    delete c2;
    c2 = nullptr;
}

void AlgGeom::SceneContent::pr2A_B_C()
{
    // PR2.A --------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // 1 - Una nube de puntos aleatoria de tamaño 50. Fijar un color para que todos los puntos de la nube tengan el mismo. -- HECHO
    int pcSize = 50;
    float pointPCSize = 3.0f;
    float pointSize = 5.0f;
    float lineWidth = 3.0f;
    PointCloud3d* pc = new PointCloud3d(pcSize, 2.0f);
    
    std::vector<Vect3d> pcPoints = pc->getPoints();
    DrawPointCloud* drPc = new DrawPointCloud(*pc);
    AABB pcAABB = drPc->getAABB();
    this->addNewModel(drPc->setPointColor(vec3(1.0f, 0.0f, 0.0f))->setPointSize(pointPCSize)->overrideModelName()); // red

    // 2 - Usar puntos de la nube de forma aleatoria para crear una recta L1, un rayo R1 y un segmento S1. Dibujar las cuatro primitivas. -- HECHO
    Line3d* l1 = new Line3d(pcPoints[RandomUtilities::getUniformRandomInt(0, pcSize)], pcPoints[RandomUtilities::getUniformRandomInt(0, pcSize)]);
    RayLine3d* r1 = new RayLine3d(pcPoints[RandomUtilities::getUniformRandomInt(0, pcSize)], pcPoints[RandomUtilities::getUniformRandomInt(0, pcSize)]);
    SegmentLine3d* s1 = new SegmentLine3d(pcPoints[RandomUtilities::getUniformRandomInt(0, pcSize)], pcPoints[RandomUtilities::getUniformRandomInt(0, pcSize)]);
    
    this->addNewModel((new DrawLine(*l1))->setLineColor(vec3(1.0f, 0.0f, 1.0f))->setLineWidth(lineWidth)->overrideModelName());     // purple
    this->addNewModel((new DrawSegment(*s1))->setLineColor(vec3(0.0f, 1.0f, 1.0f))->setLineWidth(lineWidth)->overrideModelName());  // yellow
    this->addNewModel((new DrawRay(*r1))->setLineColor(vec3(0.0f, 1.0f, 0.0f))->setLineWidth(lineWidth)->overrideModelName());      // green

    // 3 - Calcular las distancias entre la pareja que corresponda a vuestro ejercicio de book_dee. -- HECHO
    constexpr const char* fmt1 = "  Distance between {} and {} is: {} \n";
    constexpr const char* fmt2 = "\nEXERCISE {}: \n\n";
    double dist1 = l1->distance(*s1);
    std::cout << std::format(fmt2, "2.a.3");
    std::cout << std::format(fmt1, "l1", "s1", dist1);

    // 4 - Elegir otro punto de la nube V1 y calcular la distancia de dicho punto a L1. -- HECHO
    Vect3d pointV1 = pc->getPoint(RandomUtilities::getUniformRandomInt(0, pcSize - 1));
    this->addNewModel((new DrawPoint(pointV1))->setPointColor(vec3(0.0f, 1.0f, 0.0f))->setPointSize(pointSize * 2.0f)->overrideModelName());
    double dist2 = l1->distance(pointV1);
    std::cout << std::format(fmt2, "2.a.4");
    std::cout << std::format(fmt1, "l1", "v1", dist2);

    // 5 - Elegir otro punto de la nube V2 y calcular la recta normal a L1 a través de ese punto V2. -- HECHO
    Vect3d pointV2 = pc->getPoint(RandomUtilities::getUniformRandomInt(0, pcSize - 1));
    this->addNewModel((new DrawPoint(pointV2))->setPointColor(vec3(0.0f, 0.0f, 1.0f))->setPointSize(pointSize * 2.0f)->overrideModelName());
    Line3d l2 = l1->normalLine(pointV2);
    this->addNewModel((new DrawLine(l2))->setLineColor(vec3(0.0f, 0.0f, 1.0f))->setLineWidth(lineWidth)->overrideModelName());

    // 6 - Calcular la distancia entre V1 con el segmento S1 y la de V2 con R1. -- HECHO
    double dist3 = s1->distance(pointV1);
    double dist4 = r1->distance(pointV2);
    std::cout << std::format(fmt2, "2.a.6");
    std::cout << std::format(fmt1, "s1", "v1", dist3);
    std::cout << std::format(fmt1, "r1", "v2", dist4);

    // 7 - Calcula y dibuja la caja AABB de la nube de puntos. -- HECHO
    this->addNewModel((new DrawAABB(pcAABB))->setLineColor(vec3(1.0f, 1.0f, 1.0f))->setLineWidth(lineWidth)->overrideModelName());
    Vect3d aabbMin = pcAABB.getMin();
    Vect3d aabbMax = pcAABB.getMax();

    Vect3d debugVect;
    // DIBUJAMOS LOS PUNTOS EN LOS CUALES SE BASA LA AABB DE COLOR NEGRO
    for (Vect3d& v : pcPoints)
    {
        if (BasicGeometry::equal(aabbMax.getX(), static_cast<float>(v.getX())) || // al ser obtenido el aabb a traves del ya procesado VAO el cual utiliza floats en vez de double, es necesario el cast a float para conservar precision
            BasicGeometry::equal(aabbMax.getY(), static_cast<float>(v.getY())) || 
            BasicGeometry::equal(aabbMax.getZ(), static_cast<float>(v.getZ())))
        {
            this->addNewModel((new DrawPoint(v))->setPointColor(vec3(0.0f, 0.0f, 0.0f))->setPointSize(pointSize * 3.0f)->overrideModelName());

            if (BasicGeometry::equal(aabbMax.getY(), static_cast<float>(v.getY())))
                debugVect = v;
        }
        else if (   BasicGeometry::equal(aabbMin.getX(), static_cast<float>(v.getX())) ||
                    BasicGeometry::equal(aabbMin.getY(), static_cast<float>(v.getY())) ||
                    BasicGeometry::equal(aabbMin.getZ(), static_cast<float>(v.getZ())))
        {
            this->addNewModel((new DrawPoint(v))->setPointColor(vec3(0.0f, 0.0f, 0.0f))->setPointSize(pointSize * 3.0f)->overrideModelName());
        }
    }

    // PR2.B --------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // 1 - Obtener el plano P, correspondiente a la tapa superior de la caja y pintarlo. Obtener y dibujar el punto de la nube V3, el más cercano a P. -- HECHO
    Vect3d pl1P1(aabbMax);
    Vect3d pl1P2(aabbMin.getX(), aabbMax.getY(), aabbMax.getZ());
    Vect3d pl1P3(aabbMin.getX(), aabbMax.getY(), aabbMin.getZ());
    Plane* pl1 = new Plane(pl1P1, pl1P2, pl1P3, true);
    
    Vect3d pointV3;
    double minDist = std::numeric_limits<double>::max();
    for (Vect3d& v : pcPoints)
    {
        if (v == debugVect)
            std::cout << "";
        double pl1Dist = pl1->distance(v);
        if (pl1Dist < minDist)
        {
            minDist = pl1Dist;
            pointV3 = v;
        }
    }

    vec4 pl1Color = vec4(0.0f, 0.5f, 0.0f, 1.0f);
    this->addNewModel((new DrawPlane(*pl1))->setTriangleColor(pl1Color)->setLineColor(pl1Color)->overrideModelName());              // dim green
    this->addNewModel((new DrawPoint(pointV3))->setPointColor(pl1Color)->setPointSize(pointSize * 5.0f)->overrideModelName());      // dim green

    // 2 - Crear el plano A con tres puntos aleatorios y que intersecta con P.Dibujar la línea de intersección. -- HECHO
    Plane* pl2 = new Plane();
    Line3d* interLine = new Line3d();
    Vect3d randPoint1;
    Vect3d randPoint2;
    Vect3d randPoint3;

    do
    {
        randPoint1 = pcPoints[RandomUtilities::getUniformRandomInt(0, pcSize - 1)];
        randPoint2 = pcPoints[RandomUtilities::getUniformRandomInt(0, pcSize - 1)];
        randPoint3 = pcPoints[RandomUtilities::getUniformRandomInt(0, pcSize - 1)];

        pl2->set(randPoint1, randPoint2, randPoint3, true);
    } 
    while (!pl2->intersect(*pl1, *interLine));

    vec4 pl2Color = vec4(0.0f, 0.0f, 0.5f, 1.0f);
    this->addNewModel((new DrawPlane(*pl2))->setTriangleColor(pl2Color)->setLineColor(pl2Color)->overrideModelName());              // dim blue
    this->addNewModel((new DrawLine(*interLine))->setLineColor(pl2Color)->setLineWidth(lineWidth * 2.0f)->overrideModelName());     // dim blue

    // 3 - Crear una recta L3 para que interseque con A y pintar el punto de intersección. -- HECHO
    Vect3d l3Dir = pl2->getNormal();
    Vect3d l3Orig(0.0f, -3.0f, 0.5f);
    Vect3d l3Dest = l3Orig.add(l3Dir);
    Line3d* l3 = new Line3d(l3Orig, l3Dest);
    Vect3d interPoint;
    pl2->intersect(*l3, interPoint);

    vec3 l3Color = vec3(0.5f, 0.5f, 0.0f);
    this->addNewModel((new DrawLine(*l3))->setLineColor(l3Color)->setLineWidth(lineWidth * 2.0f)->overrideModelName());                        // gold like color
    this->addNewModel((new DrawPoint(interPoint))->setPointColor(l3Color)->setPointSize(pointSize * 3.0f)->overrideModelName());                // gold like color

    // 4 - Reflejar el punto V3 en el plano P y pintarlo de otro color. 5 -- HECHO
    Vect3d reflectedPoint = pl2->reflectedPoint(pointV3);
    
    vec3 reflPColor(1.0f);
    this->addNewModel((new DrawPoint(reflectedPoint))->setPointColor(reflPColor)->setPointSize(pointSize * 3.0f)->overrideModelName());        // gold like color

    // 5 - Obtener los puntos más alejados de la nube de puntos y pintar el plano equidistante entre ambos. --
    Vect3d pcMax1;
    Vect3d pcMax2;
    Vect3d pl3N;
    float pl3D = 0.0;
    pcMax1.getPlane(pcMax2, pl3N, pl3D);
    Plane* pl3 = new Plane();

    // PR2.C --------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    // 1 - Dibujar la nube de puntos de diferente color según estén en la parte positiva o negativa del plano A. 

    // free resources
    delete pl1;
    delete pl2;
    delete pl3;
    delete l3;

    delete pc;
    delete l1;
    delete r1;
    delete s1;
}

void AlgGeom::SceneContent::pr2D() 
{
    // 1 - Probar y dibujar la funcionalidad asignada por sorteo ( 3. distancia segmento-linea 3D ) -- HECHO
    const double lineWidth = 3.0f;
    
    Vect3d orig1(-1.0f, -1.0f, 0.0f);
    Vect3d dest1(-1.0f, 1.0f, 0.0f);

    Vect3d orig2(1.0f, 1.0f, 0.0f);
    Vect3d dest2(3.0f, 1.0f, 0.0f);

    Vect3d orig3(-3.0f, 0.0f, 0.0f);
    Vect3d dest3(0.0f, 0.0f, 0.0f);

    Vect3d orig4(-2.0f, 1.0f, 0.0f);
    Vect3d dest4(-4.0f, -1.0f, 0.0f);

    Line3d* l1 = new Line3d(orig1, dest1);
    SegmentLine3d* s1 = new SegmentLine3d(orig2, dest2);
    SegmentLine3d* s2 = new SegmentLine3d(orig3, dest3);
    SegmentLine3d* s3 = new SegmentLine3d(orig4, dest4);

    this->addNewModel((new DrawLine(*l1))->setLineColor(vec3(1.0f, 0.0f, 1.0f))->setLineWidth(lineWidth)->overrideModelName());     // purple
    this->addNewModel((new DrawSegment(*s1))->setLineColor(vec3(0.0f, 1.0f, 1.0f))->setLineWidth(lineWidth)->overrideModelName());  // celeste  -- dist: 2
    this->addNewModel((new DrawSegment(*s2))->setLineColor(vec3(1.0f, 0.0f, 0.0f))->setLineWidth(lineWidth)->overrideModelName());  // red      -- dist: 0
    this->addNewModel((new DrawSegment(*s3))->setLineColor(vec3(0.0f, 0.0f, 1.0f))->setLineWidth(lineWidth)->overrideModelName());  // blue     -- dist: 1

    constexpr const char* fmt2 = "\nEXERCISE {}: \n\n";
    constexpr const char* fmt1 = "  Distance between {} and {} is: {} \n";
    double dist1 = l1->distance(*s1);
    double dist2 = l1->distance(*s2);
    double dist3 = l1->distance(*s3);
    std::cout << std::format(fmt2, "2.d");
    std::cout << std::format(fmt1, "l1", "s1 (celeste)", dist1);
    std::cout << std::format(fmt1, "l1", "s2 (red)", dist2);
    std::cout << std::format(fmt1, "l1", "s3 (blue)", dist3) << std::endl;

    delete l1;
    delete s1;
    delete s2;
    delete s3;
}

Point AlgGeom::SceneContent::randomPointInUnitDisk(float diskR)
{
    vec3 vect = RandomUtilities::getUniformRandomInUnitDisk();

    return Point(vect.x * diskR, vect.y * diskR);
}