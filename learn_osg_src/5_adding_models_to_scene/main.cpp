#include <iostream>
#include "osg/Group"
#include "osgDB/ReadFile"
#include "osgViewer/Viewer"

#include "osg/MatrixTransform"
#include "osg/PositionAttitudeTransform"
#include "osg/Switch"

#include "osg/LOD"
#include "osgUtil/Simplifier"

#include "osg/ProxyNode"

#include "osg/ArgumentParser"

using namespace std;

osg::Node* createLODNode( osg::ref_ptr<osg::PagedLOD>* pagedLOD, const osg::Vec3& pos)
{
    osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
    mt->setMatrix(osg::Matrix::translate(pos));
    mt->addChild(pagedLOD->get());
    return mt.release();
}

class AnimatingSwitch : public osg::Switch
{
public:
    AnimatingSwitch():osg::Switch(), _count(0)
    {
    }

    AnimatingSwitch(const AnimatingSwitch& copy, const osg::CopyOp copyop = osg::CopyOp::SHALLOW_COPY)\
                    :osg::Switch(copy, copyop), _count(copy._count)
    {

    }

    void traverse(osg::NodeVisitor& nv) override
    {
        if( !(++_count % 60))
        {
            setValue(0, !getValue(0));
            setValue(1, !getValue(1));
        }

        osg::Switch::traverse(nv);
    }

private:
    unsigned int _count;
};

class InfoVisitor : public osg::NodeVisitor
{
public:
    InfoVisitor():_level(0)
    {
        setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
    }

    string spaces()
    {
        return string(_level*2, ' ');
    }

    void apply(osg::Node& node) override
    {
        cout << spaces() << node.libraryName() << "::" << node.className() << endl;
        _level ++;
        traverse(node);
        _level --;
    }

    void apply(osg::Geode& geode) override
    {
        cout << spaces() << geode.libraryName() << "::" << geode.className() << endl;
        _level ++;
        for(unsigned int i = 0; i < geode.getNumDrawables(); ++i)
        {
            osg::Drawable* drawable = geode.getDrawable(i);
            cout << spaces() << drawable->libraryName() << "::" << drawable->className() << endl;
        }
        traverse(geode);
        --_level;
    }
private:
    unsigned int _level;
};

int main(int argc, char* argv[])
{
    osg::ref_ptr<osg::Node> model_cessna = osgDB::readNodeFile("cessna.osg");
    osg::ref_ptr<osg::Node> model2 = osgDB::readNodeFile("cow.osg");
    osg::ref_ptr<osg::Node> model_cessna_fire = osgDB::readNodeFile("cessnafire.osg");

    osg::ref_ptr<osg::MatrixTransform> transformation1 = new osg::MatrixTransform;
    transformation1->setMatrix(osg::Matrix::translate(-25.0f, 0.0f, 0.0f));
    transformation1->addChild(model_cessna.get());

    osg::ref_ptr<osg::MatrixTransform> tranformation2 = new osg::MatrixTransform;
    tranformation2->setMatrix(osg::Matrix::translate(25.0f, 0.0f, 0.0f));
    tranformation2->addChild(model2);

    osg::ref_ptr<osg::PositionAttitudeTransform> positionAttitudeTranformation = new osg::PositionAttitudeTransform;
    positionAttitudeTranformation->setPosition(osg::Vec3(0.0f, 25.0f, 0.0f));
    osg::Quat quat( 10.0f, osg::X_AXIS,
                    45.0f, osg::Y_AXIS,
                    90.0f, osg::Z_AXIS);
    positionAttitudeTranformation->setAttitude(quat);
    positionAttitudeTranformation->addChild(model_cessna.get());

    osg::ref_ptr<osg::Node> modelL3 = osgDB::readNodeFile("cessna.osg");
    osg::ref_ptr<osg::Node> modelL2 = dynamic_cast<osg::Node*>(modelL3->clone(osg::CopyOp::DEEP_COPY_ALL));
    osg::ref_ptr<osg::Node> modelL1 = dynamic_cast<osg::Node*>(modelL3->clone(osg::CopyOp::DEEP_COPY_ALL));

    osgUtil::Simplifier simplifier;
    simplifier.setSampleRatio(0.5);
    modelL2->accept(simplifier);

    simplifier.setSampleRatio(0.1);
    modelL3->accept(simplifier);

    /* 1. transform
    osg::ref_ptr<osg::Group> root = new osg::Group;
    root->addChild(transformation1.get());
    root->addChild(tranformation2.get());
    root->addChild(positionAttitudeTranformation.get());
    */

    /* 2. osg::Switch
    osg::ref_ptr<osg::Switch> root = new osg::Switch;
    root->addChild(model1.get(), false);
    root->addChild(model_cessna_fire.get(), true);
    */

    // 3. osg::LOD
    osg::ref_ptr<osg::LOD> rootLod = new osg::LOD;
    rootLod->addChild(modelL1.get(), 200.0f, FLT_MAX);
    rootLod->addChild(modelL2.get(), 50.0f, 200.0f);
    rootLod->addChild(modelL3.get(), 0.0f, 50.0f);

    //4. Loading a model at runtime proxyNode&PageNode
    osg::ref_ptr<osg::ProxyNode> rootProxyNode = new osg::ProxyNode;
    rootProxyNode->setFileName(0, "cow.osg");

    osg::ref_ptr<osg::PagedLOD> rootPagedLOD = new osg::PagedLOD;
    rootPagedLOD->addChild(modelL1, 200.0f, FLT_MAX);
    rootPagedLOD->setFileName(1, "cessna.osg");
    rootPagedLOD->setRange(1, 0.0f, 200.0f);

    // 5. animalting
    osg::ref_ptr<osg::Node> model_cessna1 = osgDB::readNodeFile("cessna.osg");
    osg::ref_ptr<osg::Node> model_cessna_fire1 = osgDB::readNodeFile("cessnafire.osg");
    osg::ref_ptr<osg::MatrixTransform> cessna_file_rotate = new osg::MatrixTransform;
    cessna_file_rotate->setMatrix( osg::Matrix::rotate(osg::DegreesToRadians(-48.0f), osg::X_AXIS,
                                                       osg::DegreesToRadians(17.9f), osg::Y_AXIS,
                                                       osg::DegreesToRadians(-46.0f), osg::Z_AXIS) );
    cessna_file_rotate->addChild(model_cessna_fire1);

    osg::ref_ptr<AnimatingSwitch> rootAnimatingSwitch = new AnimatingSwitch;
    rootAnimatingSwitch->addChild(model_cessna1, true);
    rootAnimatingSwitch->addChild(cessna_file_rotate, false);

    //6. analyzing the Cessa structure
    osg::ArgumentParser parser(&argc, argv);

    osg::ref_ptr<osg::Node> root = osgDB::readNodeFiles(parser);

    if(nullptr == root)
    {
        OSG_FATAL << parser.getApplicationName() << ": No data loaded." << endl;
        return -1;
    }

    InfoVisitor infoVisitor;
    root->accept(infoVisitor);

    osgViewer::Viewer viewer;
    viewer.setSceneData(root.get());

    return viewer.run();
}
