#include "stdafx.h"
#include "SceneContent.h"

#include "ChronoUtilities.h"
#include "InclDraw2D.h"
#include "InclGeom2D.h"
#include "PointCloud.h"
#include "RandomUtilities.h"

#define constexpr INITIAL_CODE false

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

    // Segments
    SegmentLine* sgl1 = new SegmentLine();
    SegmentLine* sgl2 = new SegmentLine();
   
    do 
    {
        sgl1->setA(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
        sgl1->setB(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));

        sgl2->setA(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
        sgl2    ->setB(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
    } while (!sgl1->segmentIntersection(*sgl2));

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

    // Polygon
    Polygon* p = new Polygon();
    for(int i = 0; i < 5; i++)
        p->add(Point());
    int pSize = p->getNumVertices();
    bool pCreated;
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
                SegmentLine psgl2 = p->getEdge(i + 1);
                if(psgl1.segmentIntersection(psgl2)) pCreated = false;
            }
        }

    } while (!pCreated);

    this->addNewModel((new DrawPolygon(*p))->setTriangleColor(vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName());
    delete p;
    p = nullptr;

    // Triangle and its circles

    Vect2d vec1 = Vect2d(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
    Vect2d vec2 = Vect2d(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
    Vect2d vec3 = Vect2d(pc->getPoint(RandomUtilities::getUniformRandomInt(0, cloudSize - 1)));
    Triangle* tr = new Triangle(vec1, vec2, vec3);

    Circle circ = tr->getCirumscribed();
    Circle incs = tr->getInscribed();
    this->addNewModel((new DrawTriangle(*tr))->setLineColor(vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName()->setLineWidth(2.0f));
    this->addNewModel((new DrawCircle(circ, 30))->setLineColor(vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName()->setLineWidth(3.0f));
    this->addNewModel((new DrawCircle(incs, 30))->setLineColor(vec4(RandomUtilities::getUniformRandomColor(), 1.0f))->overrideModelName()->setLineWidth(3.0f));
    delete tr;
    tr = nullptr;

    // Free resources

    delete pc;
    pc = nullptr;

    //PR1b
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
