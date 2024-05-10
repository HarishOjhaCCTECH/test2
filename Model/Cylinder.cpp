#include "stdafx.h"
#include "Cylinder.h"
Cylinder::Cylinder() 
{	
	GLfloat cylinder_height = 1.0f;
	GLfloat cylinder_radius = 0.5f;
	GLfloat nr_of_points_cylinder = 360.f;

	for (int i = 0; i < nr_of_points_cylinder; ++i)
	{
		GLfloat u = i / (GLfloat)nr_of_points_cylinder;
		EachPole pole;
		pole.x = 4 + cylinder_radius * cos(2 * M_PI * u); // 4 is center.x

		pole.z = -5 + cylinder_radius * sin(2 * M_PI * u);// -5 iscenter.z

		pole.y_start = 0.0f;
		pole.y_end = cylinder_height;

		poles.push_back(pole); // Add the pole to the vector
	}
}
Cylinder::Cylinder(GLfloat drillRadius) 
{
	GLfloat cylinder_height = 1.0f;
	GLfloat cylinder_radius = drillRadius;
	GLfloat nr_of_points_cylinder = 360.f;

	for (int i = 0; i < nr_of_points_cylinder; ++i)
	{
		GLfloat u = i / (GLfloat)nr_of_points_cylinder;
		EachPole pole;
		pole.x = 4 + cylinder_radius * cos(2 * M_PI * u); // 4 is center.x

		pole.z = -5 + cylinder_radius * sin(2 * M_PI * u);// -5 iscenter.z

		pole.y_start = 0.0f;
		pole.y_end = cylinder_height;

		poles.push_back(pole); // Add the pole to the vector
	}
}
Cylinder::~Cylinder() {}
QVector<EachPole> Cylinder::getPoles()
{
	return poles;
}