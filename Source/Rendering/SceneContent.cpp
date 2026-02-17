#include "stdafx.h"
#include "SceneContent.h"

#include "ChronoUtilities.h"
#include "InclDraw2D.h"
#include "InclGeom2D.h"
#include "PointCloud.h"
#include "RandomUtilities.h"

#define constexpr INITIAL_CODE false
#define constexpr PR1A false

// ----------------------------- BUILD YOUR SCENARIO HERE -----------------------------------

void AlgGeom::SceneContent::buildScenario()
{
#if INITIAL_CODE
    vec2 minBoundaries = vec2(-2.0, -.4), maxBoundaries = vec2(-minBoundaries);

    // Random segments
    int numSegments = 8;

    for (int segmentIdx = 0; segmentIdx < numSegments; ++segmentIdx)
    {
        Point a(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x), RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        Point b(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x), RandomUtilities::getUniformRandom(minBoundaries.y, maxBoundaries.y));
        SegmentLine* segment = new SegmentLine(a, b);

        this->addNewModel((new DrawSegment(*segment))->setLineColor(RandomUtilities::getUniformRandomColor())->overrideModelName()->setLineWidth(RandomUtilities::getUniformRandom(1.0f, 3.0f)));
        delete segment;
    }

    // Random points
    int numPoints = 200;

    for (int pointIdx = 0; pointIdx < numPoints; ++pointIdx)
    {
        Point point(RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x), RandomUtilities::getUniformRandom(minBoundaries.x, maxBoundaries.x));
        this->addNewModel((new DrawPoint(point))->setPointColor(RandomUtilities::getUniformRandomColor())->overrideModelName()->setPointSize(RandomUtilities::getUniformRandom(4.0f, 8.0f)));
    }

    // Polygon
    float polygonAngle = .0f;
    constexpr float polygonAlpha = 2.0f * glm::pi<float>() / 5.0f;
    Polygon* polygon = new Polygon;

    while (polygonAngle < 2.0f * glm::pi<float>())
    {
        polygon->add(Point(std::cos(polygonAngle), std::sin(polygonAngle)));
        polygonAngle += polygonAlpha;
    }

    this->addNewModel((new DrawPolygon(*polygon))->setTriangleColor(vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName()->setModelMatrix(glm::rotate(mat4(1.0f), (glm::abs(4 * polygonAlpha - glm::pi<float>() / 2.0f * 3.0f)), vec3(.0f, .0f, 1.0f))));
    delete polygon;

    // Line
    Line* line = new Line(Point(-1.0, -1.0), Point(1.0, 1.0));
    this->addNewModel((new DrawLine(*line))->setLineColor(vec4(.0f, 1.0f, .0f, 1.0f))->overrideModelName()->setLineWidth(2.0f));
#else
    //PR1a
    // PointCloud
    const std::string filePath = "./PointCloud.txt";
    const unsigned int cloudSize = 200;
    
    PointCloud* pc = new PointCloud(cloudSize, 5.0f, 5.0f);
    pc->save(filePath);
    this->addNewModel((new DrawPointCloud(*pc))->setPointColor(vec4(1.0f, 0.0f, 0.0f, 1.0f))->overrideModelName()->setLineWidth(2.0f));

#if PR1A
    // Segments
    SegmentLine* sgl1 = new SegmentLine();
    SegmentLine* sgl2 = new SegmentLine();
    do 
    {
        sgl1->setA(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
        sgl1->setB(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));

        sgl2->setA(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
        sgl2->setB(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
    } 
    while (!sgl1->segmentIntersection(*sgl2));

    this->addNewModel((new DrawSegment(*sgl1))->setTriangleColor(vec4(1.0f, 0.0f, 0.0f, 1.0f))->overrideModelName());
    this->addNewModel((new DrawSegment(*sgl2))->setTriangleColor(vec4(1.0f, 0.0f, 0.0f, 1.0f))->overrideModelName());
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
#endif
    // Polygon
    Polygon* p = new Polygon();
    int pSize = 5;
    for(int i = 0; i < pSize; i++)
        p->add(Point());
    bool pCreated;
#if PR1A
    do
    {
        pCreated = true;

        for(int i = 0; i < 5; i++) 
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
                if(psgl1.segmentIntersection(psgl2)) pCreated = false;
            }
        }

    } 
    while (!pCreated);

    this->addNewModel((new DrawPolygon(*p))->setTriangleColor(vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName());
#endif
    delete p;
    p = nullptr;

#if PR1A
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
    this->addNewModel((new DrawCircle(*circ, 30))->setLineColor(vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName()->setLineWidth(3.0f));
    this->addNewModel((new DrawCircle(*incs, 30))->setLineColor(vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName()->setLineWidth(3.0f));
    delete tr;
    delete circ;
    delete incs;
    tr = nullptr;
    circ = nullptr;
    incs = nullptr;
#else
    //PR1b
    
    // 1 Create a set of lines and a polygon making specific intersections
    vec4 rgbLTypes[4] = { {1.0f, 1.0f, 0.0f, 1.0f},     // Brown - line
                          {1.0f, 0.0f, 1.0f, 1.0f},     // Purple - ray
                          {0.0f, 1.0f, 1.0f, 1.0f},     // Aqua - segment
                          {0.0f, 0.5f, 0.0f, 1.0f} };   // Dark green - polygon
    float interPointSize = 5.0f;

    Line* l1 = new Line(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)),
                        pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
    Line* l2 = new Line(Point(), Point());

    Vect2d auxIntersection;
    do
    {
        l2->setA(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
        l2->setB(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
    }
    while (!l1->intersects(*l2, auxIntersection));

    // Draw l1, l2 and its intersection
    this->addNewModel((new DrawLine(*l1))->setLineColor(rgbLTypes[0])->overrideModelName()->setLineWidth(2.0f));
    this->addNewModel((new DrawLine(*l2))->setLineColor(rgbLTypes[0])->overrideModelName()->setLineWidth(2.0f));
    this->addNewModel((new DrawPoint(auxIntersection))->setPointColor(vec4(1.0f, 1.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(interPointSize));

    SegmentLine* sg1 = new SegmentLine(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)),
                                       pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
    SegmentLine* sg2 = new SegmentLine();
    do
    {
        sg2->setA(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
        sg2->setB(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
    } 
    while (!sg1->intersects(*sg2, auxIntersection));

    // Draw sg1, sg2 and its intersection
    this->addNewModel((new DrawSegment(*sg1))->setLineColor(rgbLTypes[2])->overrideModelName()->setLineWidth(2.0f));
    this->addNewModel((new DrawSegment(*sg2))->setLineColor(rgbLTypes[2])->overrideModelName()->setLineWidth(2.0f));
    this->addNewModel((new DrawPoint(auxIntersection))->setPointColor(vec4(1.0f, 1.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(interPointSize));

    RayLine* r1 = new RayLine(Point(), Point());
    do
    {
        r1->setA(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
        r1->setB(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
    } 
    while (!sg1->intersects(*r1, auxIntersection));

    // Draw r1 and its intersection
    this->addNewModel((new DrawRay(*r1))->setLineColor(rgbLTypes[1])->overrideModelName()->setLineWidth(2.0f));
    this->addNewModel((new DrawPoint(auxIntersection))->setPointColor(vec4(1.0f, 1.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(interPointSize));

    pSize = 4;
    p = new Polygon;
    for (int i = 0; i < pSize; i++) 
        p->add(Point());
    
    pCreated = true;
    bool valid = false;
    do
    {    
        valid = false;
        pCreated = true;

        for (int i = 0; i < pSize; i++)
        {
            Vertex v = pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1));
            p->set(v, i);
        }

        for (int i = 0; i < pSize; i++)
        {
            SegmentLine sgp = p->getEdge(i);
            if (p->getEdge(i).intersects(*l1, auxIntersection))
                valid = true;

            for (int j = 0; j < pSize; j++) 
            {
                if (j == i) continue;

                SegmentLine sgpAux = p->getEdge(j);
                if (sgp.segmentIntersection(sgpAux))
                    pCreated = false;
            }
        }
    } 
    while (!pCreated || !valid);

    this->addNewModel((new DrawPolygon(*p))->setLineColor(rgbLTypes[3])->overrideModelName()->setLineWidth(2.0f));
    this->addNewModel((new DrawPoint(auxIntersection))->setPointColor(vec4(1.0f, 1.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(interPointSize));

    RayLine* r2 = new RayLine(Point(), Point());
    pCreated = false;
    do
    {
        r2->setA(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
        r2->setB(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));

        for (int i = 0; i < pSize; i++)
        {
            if (p->getEdge(i).intersects(*r2, auxIntersection))
                pCreated = true;
        }
    } 
    while (!pCreated);

    this->addNewModel((new DrawRay(*r2))->setLineColor(rgbLTypes[1])->overrideModelName()->setLineWidth(2.0f));
    this->addNewModel((new DrawPoint(auxIntersection))->setPointColor(vec4(1.0f, 1.0f, 1.0f, 1.0f))->overrideModelName()->setPointSize(interPointSize));

    // 2 Distance between each polygon vertex and the lines



    // 3 Create two circles and determine its relationship between them, as well with the lines
    // Each pos of the array corresponds to a color and a circle rel based on the enum value
    
    float rgbCircleRel[6][3] = {{0.0f, 0.0f, 1.0f},
                            {0.0f, 1.0f, 0.0f}, 
                            {0.0f, 1.0f, 1.0f},
                            {1.0f, 0.0f, 0.0f}, 
                            {1.0f, 0.0f, 1.0f}, 
                            {1.0f, 1.0f, 0.0f} };


    // 4 Intersection points will be drawn in white



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
#endif
    delete p;
    p = nullptr;
    delete pc;
    pc = nullptr;
#endif
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
	_sceneAABB.update(model->getAABB());
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
