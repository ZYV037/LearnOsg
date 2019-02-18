#include <iostream>
#include "osgDB/ReadFile"
#include "osgViewer/Viewer"

using namespace std;

int main()
{
    osgViewer::Viewer viewer;
    viewer.setSceneData(osgDB::readNodeFile("../../OpenSceneGraph-Data/cessna.osg"));
    return viewer.run();
}
