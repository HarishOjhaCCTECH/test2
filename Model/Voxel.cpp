#include "stdafx.h"
#include "Voxel.h"

Voxel::Voxel() {}

Voxel::Voxel(float xLen, float yLen, float zLen)
{
	int boxLimit = 1;
	for (int k = 0; k < boxLimit; k++)
	{
		for (int j = 0; j < boxLimit; j++)
		{
			for (int i = 0; i < boxLimit; i++)
			{
				float x_min = startPoint[0] + (i * xLen);
				float x_max = x_min + (xLen);

				float y_min = startPoint[1] + (j * yLen);
				float y_max = y_min + (yLen);

				float z_min = startPoint[2] + (k * zLen);
				float z_max = z_min + (zLen);
				boxMinAndMax << x_min << y_min << z_min;
				boxMinAndMax << x_max << y_max << z_max;
				//front
				vertices1 << x_min << y_min << z_max;
				vertices1 << x_max << y_min << z_max;
				vertices1 << x_max << y_max << z_max;
				vertices1 << x_min << y_max << z_max;
				vertices1 << x_min << y_min << z_max;
				




				vertices2 << x_min << y_min << z_min;
				vertices2 << x_max << y_min << z_min;
				vertices2 << x_max << y_max << z_min;
				vertices2 << x_min << y_max << z_min;
				vertices2 << x_min << y_min << z_min;



				vertices3 << x_min << y_min << z_max;
				vertices3 << x_min << y_min << z_min;

				vertices3 << x_max << y_min << z_max;
				vertices3 << x_max << y_min << z_min;

				vertices3 << x_max << y_max << z_max;
				vertices3 << x_max << y_max << z_min;

				vertices3 << x_min << y_max << z_max;
				vertices3 << x_min << y_max << z_min;
			}
		}

	}
	
	for (int i = 0; i < 24; i++)
	{
		colors << 1 << 1 << 1;
	}

	
}

Voxel::~Voxel() {}

QVector<GLfloat> Voxel::getVertices1()
{
	return vertices1;
}
QVector<GLfloat> Voxel::getVertices2()
{
	return vertices2;
}
QVector<GLfloat> Voxel::getVertices3()
{
	return vertices3;
}
QVector<GLfloat> Voxel::getColors()
{
	return colors;
}