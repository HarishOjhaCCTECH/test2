#pragma once
#include <gl/GL.h>
#include <EachPole.h>
class Cylinder
{
public:
	Cylinder();
	Cylinder(GLfloat drillRadius);
	~Cylinder();
	QVector<EachPole> getPoles();
private:
	QVector<EachPole> poles;
};