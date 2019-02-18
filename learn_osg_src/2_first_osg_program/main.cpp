#include <iostream>

#include "osgDB/ReadFile"
#include "osgViewer/Viewer"

#include "osg/ref_ptr"
#include "osg/Referenced"

#include <fstream>

using namespace std;

class MonitoringTarget : public osg::Referenced
{
public:
	MonitoringTarget(int id): _id(id)
	{
		cout << "Constructing target " << _id << endl;
	}
	
	static MonitoringTarget* createMonitoringTarget( int id )
	{
		osg::ref_ptr<MonitoringTarget> target = new MonitoringTarget(id);
		return target.release();
	}
	
protected:
	virtual ~ MonitoringTarget()
	{
		cout << "Destorying target " << _id << std::endl;
	}
	
	int _id;
};

class LogFileHandler : public osg::NotifyHandler
{
public:
	LogFileHandler( const std::string& file )
	{
		_log.open( file.c_str() );
	}
	
	virtual ~LogFileHandler()
	{
		_log.close();
	}
	
	virtual void notify(osg::NotifySeverity serverity, const char* msg)
	{
		_log << msg << endl;
	}
	
protected:
	std::ofstream _log;
};

int main( int argc, char* argv[] )
{
	// 4. osg::notify
	
	osg::setNotifyLevel( osg::INFO );
	osg::setNotifyHandler( new LogFileHandler("output.txt") );
	osg::notify(osg::WARN) << "Some warn message. " << endl;
	
	// 3. osg::ArgumentParser
	osg::ArgumentParser arguments(&argc, argv);
	std::string filename;
	arguments.read("--model", filename);
	
	float size;
	arguments.read("--size", size);	
	cout << "osg::ArgumentParser read --size = " << size << endl;
	
	// 2. ref_ptr
	osg::ref_ptr<MonitoringTarget> target = new MonitoringTarget(0);
	cout << "Referenced count befor referring : " << target->referenceCount() << endl;
	
	osg::ref_ptr<MonitoringTarget> anotherTarget = target;
	cout << "Referenced count after referring : " << target->referenceCount() << endl;
	
	for( unsigned int i = 1; i < 5; ++i)
	{
		osg::ref_ptr<MonitoringTarget> subTarget = new MonitoringTarget(i);
	}
	
	// 1. osg::Node
	osg::ref_ptr<osg::Node> root = osgDB::readNodeFile(filename);
	
    osgViewer::Viewer viewer;
    viewer.setSceneData(root.get());
    return viewer.run();
}
