#include <iostream>
#include "osgViewer/Viewer"
#include "osgDB/ReadFile"
#include "osg/Geode"
#include "osg/ShapeDrawable"

#include "osg/Geometry"
#include "osgUtil/SmoothingVisitor"
#include "osgUtil/Tessellator"
#include "osg/TriangleFunctor"

#include "osg/BoundingBox"

using namespace std;

ostream& operator<<(ostream& stream, const osg::Vec3& v1)
{
    return  stream << v1.x() << " " << v1.y() << " " << v1.z();
}

struct FaceCollector
{
    void operator()(const osg::Vec3& v1, const osg::Vec3& v2, const osg::Vec3& v3 )
    {
        cout << "Face vertices : v1 = " << v1 << ", v2 = " << v2 << ", v3 = " << v3 << endl;
    }
};

int main()
{
    osg::ref_ptr<osg::Geode> root = new osg::Geode;

if( 0 )//1.
{
    osg::ref_ptr<osg::ShapeDrawable> shape1 = new osg::ShapeDrawable;
    shape1->setShape(new osg::Box( osg::Vec3(-3.0f, 0.0f, 0.0f), 2.0f, 2.0f, 1.0f));

    osg::ref_ptr<osg::ShapeDrawable> shape2 = new osg::ShapeDrawable;
    shape2->setShape(new osg::Sphere(osg::Vec3(3.0f, 0.0f, 0.0f), 1.0f));

    osg::ref_ptr<osg::ShapeDrawable> shape3 = new osg::ShapeDrawable;
    shape3->setShape( new osg::Capsule(osg::Vec3(0.0f, 0.0f, 0.0f), 1.0f, 3.0f));
    shape3->setColor(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));

    osg::ref_ptr<osg::ShapeDrawable> shape4 = new osg::ShapeDrawable;
    shape4->setShape( new osg::Cone(osg::Vec3(-6.0f, 0.0f, 0.0f), 1.0f, 1.0f));

    osg::ref_ptr<osg::Geode> root = new osg::Geode;
    root->addDrawable(shape1.get());
    root->addDrawable(shape2.get());
    root->addDrawable(shape3.get());
    root->addDrawable(shape4.get());
}

if( 0 )//2.
{
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    vertices->push_back( osg::Vec3(0.0f, 0.0f, 0.0f));
    vertices->push_back( osg::Vec3(1.0f, 0.0f, 0.0f));
    vertices->push_back( osg::Vec3(1.0f, 0.0f, 1.0f));
    vertices->push_back( osg::Vec3(0.0f, 0.0f, 1.0f));

    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0.0f,  -1.0f, 0.0f ));

    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1.0f, 0.0f, 0.f, 1.0f));
    colors->push_back(osg::Vec4(0.0f, 1.0f, 0.f, 1.0f));
    colors->push_back(osg::Vec4(0.0f, 0.0f, 1.f, 1.0f));
    colors->push_back(osg::Vec4(1.0f, 1.0f, 1.f, 1.0f));

    osg::ref_ptr<osg::Geometry> quad = new osg::Geometry;
    quad->setVertexArray(vertices.get());
    quad->setNormalArray(normals.get());
    quad->setNormalBinding(osg::Geometry::BIND_OVERALL);
    quad->setColorArray(colors.get());
    quad->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

    quad->addPrimitiveSet( new osg::DrawArrays(GL_QUADS, 0, 4));

    root->addDrawable(quad.get());
}

if( 0 )// 3. octahedron
{
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array(6);
//    vertices->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));
//    vertices->push_back(osg::Vec3(-0.5f, -0.5f, 0.0f));
//    vertices->push_back(osg::Vec3(0.5f, -0.5f, 0.0f));
//    vertices->push_back(osg::Vec3(0.5f, 0.5f, 0.0f));
//    vertices->push_back(osg::Vec3(-0.5f, 0.5f, 0.0f));
//    vertices->push_back(osg::Vec3(0.0f, 0.0f, -1.0f));
    (*vertices)[0].set(0.0, 0.0f, 1.0f);
    (*vertices)[1].set(-0.5f, -0.5f, 0.0f);
    (*vertices)[2].set(0.5f, -0.5f, 0.0f);
    (*vertices)[3].set(0.5f, 0.5f, 0.0f);
    (*vertices)[4].set(-0.5f, 0.5f, 0.0f);
    (*vertices)[5].set(0.0f, 0.0f, -1.0f);

    osg::ref_ptr<osg::DrawElementsUInt> indices = new osg::DrawElementsUInt(GL_TRIANGLES, 24);
    indices->push_back(0);indices->push_back(1);indices->push_back(2);
    indices->push_back(0);indices->push_back(2);indices->push_back(3);
    indices->push_back(0);indices->push_back(3);indices->push_back(4);
    indices->push_back(0);indices->push_back(4);indices->push_back(1);
    indices->push_back(5);indices->push_back(2);indices->push_back(1);
    indices->push_back(5);indices->push_back(3);indices->push_back(2);
    indices->push_back(5);indices->push_back(4);indices->push_back(3);
    indices->push_back(5);indices->push_back(1);indices->push_back(4);

    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
    geom->setVertexArray( vertices.get() );
    geom->addPrimitiveSet( indices.get() );

    osgUtil::SmoothingVisitor::smooth( *geom );
    root->addDrawable(geom.get());
}

if( 0 )// 4.concave
{
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    vertices->push_back( osg::Vec3(0.0f, 0.0f, 0.0f) );
    vertices->push_back( osg::Vec3(2.0f, 0.0f, 0.0f) );
    vertices->push_back( osg::Vec3(2.0f, 0.0f, 1.0f) );
    vertices->push_back( osg::Vec3(1.0f, 0.0f, 1.0f) );
    vertices->push_back( osg::Vec3(1.0f, 0.0f, 2.0f) );
    vertices->push_back( osg::Vec3(2.0f, 0.0f, 2.0f) );
    vertices->push_back( osg::Vec3(2.0f, 0.0f, 3.0f) );
    vertices->push_back( osg::Vec3(0.0f, 0.0f, 3.0f) );

    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));

    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
    geom->setVertexArray(( vertices.get()));
    geom->setNormalArray(normals.get());
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
    geom->addPrimitiveSet((new osg::DrawArrays(GL_POLYGON, 0, 8)));

    osgUtil::Tessellator tessellator;
    tessellator.retessellatePolygons(*geom);

    root->addDrawable(geom.get());
}

if( 1 )// 5. functors
{
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    vertices->push_back( osg::Vec3(0.0f, 0.0f, 0.0f) );
    vertices->push_back( osg::Vec3(0.0f, 0.0f, 1.0f) );
    vertices->push_back( osg::Vec3(1.0f, 0.0f, 0.0f) );
    vertices->push_back( osg::Vec3(1.0f, 0.0f, 1.5f) );
    vertices->push_back( osg::Vec3(2.0f, 0.0f, 0.0f) );
    vertices->push_back( osg::Vec3(2.0f, 0.0f, 1.0f) );
    vertices->push_back( osg::Vec3(3.0f, 0.0f, 0.0f) );
    vertices->push_back( osg::Vec3(3.0f, 0.0f, 1.5f) );
    vertices->push_back( osg::Vec3(4.0f, 0.0f, 0.0f) );
    vertices->push_back( osg::Vec3(4.0f, 0.0f, 1.0f) );

    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));

    osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
    geom->setVertexArray(vertices.get());
    geom->setNormalArray(normals.get());
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
    geom->addPrimitiveSet(new osg::DrawArrays(GL_QUAD_STRIP, 0, 10));

    osg::TriangleFunctor<FaceCollector> functor;
    geom->accept(functor);

    root->addDrawable(geom.get());
}

    osgViewer::Viewer viewer;
    viewer.setSceneData(root.get());
    return viewer.run();
}
