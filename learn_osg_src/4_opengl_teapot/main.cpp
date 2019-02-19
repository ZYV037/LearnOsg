#include <iostream>
#include "GL/freeglut.h"
#include "osg/Drawable"
#include "osg/Geode"
#include "osgViewer/Viewer"
#include "gl/gl.h"

using namespace std;

class TeapotDrawable : public osg::Drawable
{
public:
    TeapotDrawable(float size = 0.1f):_size(size){ }
    TeapotDrawable(const TeapotDrawable& copy, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY)   \
                    :osg::Drawable(copy, copyop),_size(copy._size)
    {
    }

    META_Object( osg, TeapotDrawable );

    osg::BoundingBox computeBoundingBox() const override
    {
        osg::Vec3 min( -_size, -_size, -_size), max(_size, _size, _size);
        return osg::BoundingBox(min, max);
    }

    void drawImplementation(osg::RenderInfo&) const override
    {
        glFrontFace(GL_CW);
        glutSolidTeapot(_size);
        glFrontFace(GL_CCW);
    }
private:
    float _size;
};

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);

    glutInitWindowSize(300,300);
    glutInitWindowPosition(100,100);
    glutCreateWindow(nullptr);

    osgViewer::Viewer viewer;
    osg::ref_ptr<osg::Geode> root = new osg::Geode;
    root->addDrawable(new TeapotDrawable(1.0f));

    viewer.setSceneData(root.get());

    return viewer.run();
}
