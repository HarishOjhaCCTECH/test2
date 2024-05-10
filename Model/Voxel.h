#pragma once
class Voxel
{
public:
	Voxel();
	Voxel(float xLen, float yLen, float zLen);
	~Voxel();

	QVector<GLfloat> getVertices1();
	QVector<GLfloat> getVertices2();
	QVector<GLfloat> getVertices3();
	QVector<GLfloat> getColors();
	QVector<GLfloat> boxMinAndMax;

private:
	bool existence[3][3][3];
	QVector<GLfloat> vertices1;
	QVector<GLfloat> vertices2;
	QVector<GLfloat> vertices3;
	QVector<GLfloat> colors;
	float startPoint[3] = { 1,1,1 };
	
};
