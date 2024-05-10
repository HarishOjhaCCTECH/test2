#include "stdafx.h"
#include "OpenGLWindow.h"

#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QOpenGLShaderProgram>
#include <QPainter>
#include <iostream>
#include <qopenglvertexarrayobject.h>



OpenGLWindow::OpenGLWindow(const QColor& background, QMainWindow* parent) :
    mBackground(background)
{
    setParent(parent);
    setMinimumSize(300, 250);
}
OpenGLWindow::~OpenGLWindow()
{
    reset();
}

void OpenGLWindow::reset()
{
    makeCurrent();
    delete mProgram;
    mProgram = nullptr;
    delete mVshader;
    mVshader = nullptr;
    delete mFshader;
    mFshader = nullptr;
    mVbo.destroy();
    doneCurrent();
    QObject::disconnect(mContextWatchConnection);
}

void OpenGLWindow::paintGL()
{
	glClearColor(0.5f, 0.75f, 1.75f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mProgram->bind();
	glLineWidth(2.5f);

	QMatrix4x4 modelMatrix;
	QMatrix4x4 translationMatrix;
	QMatrix4x4 scaleMatrix;
	QMatrix4x4 rotationMatrix;
	QMatrix4x4 rotationMatrix1;
	QMatrix4x4 rotationMatrix2;
	QMatrix4x4 viewMatrix;
	QMatrix4x4 projectionMatrix;
	QMatrix3x3 normalMatrix;
	QVector3D lightPos;

	lightPos = QVector3D(0.0f, 0.0f, 20.0f);
	projectionMatrix.ortho(-100.0f * scaleFactor, 100.0f * scaleFactor, -100.0f * scaleFactor, 100.0f * scaleFactor, 0.00000000001f, 1000000.0f);

	translationMatrix.translate(0, 200, -400);
	scaleMatrix.scale(30.0);
	rotationMatrix1.rotate(rotationAngle);
	rotationMatrix2.rotate(180.0, 0.0, 1.0, 0.0);
	rotationMatrix = rotationMatrix1 * rotationMatrix2;

	modelMatrix = translationMatrix * scaleMatrix * rotationMatrix;
	viewMatrix.setToIdentity();
	normalMatrix = (modelMatrix.normalMatrix()).transposed();
	normalMatrix = (modelMatrix.normalMatrix());

	mProgram->setUniformValue(m_modelMatrixUniform, modelMatrix);
	mProgram->setUniformValue(m_viewMatrixUniform, viewMatrix);
	mProgram->setUniformValue(m_projectionMatrixUniform, projectionMatrix);
	mProgram->setUniformValue(m_normalMatrixUniform, normalMatrix);
	mProgram->setUniformValue(m_lightPosUniform, lightPos);

	glEnableVertexAttribArray(m_posAttr);
	glEnableVertexAttribArray(m_colAttr);

	int boxes = 1;
	for (int i = 0; i < boxes; i++)
	{
		int startIndex = (i * 15 );
		QVector<GLfloat> vertices1 = vx1->getVertices1().mid(startIndex, 15);
		QVector<GLfloat> colors = vx1->getColors();
		glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices1.data());
		glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors.data());
		glDrawArrays(GL_LINE_STRIP, 0, vertices1.size() / 3);

		QVector<GLfloat> vertices2 = vx1->getVertices2().mid(startIndex, 15);
		glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices2.data());
		glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors.data());
		glDrawArrays(GL_LINE_STRIP, 0, vertices2.size() / 3);

		startIndex = (i * 24);
		QVector<GLfloat> vertices3 = vx1->getVertices3().mid(startIndex, 24);
		glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices3.data());
		glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors.data());
		glDrawArrays(GL_LINES, 0, vertices3.size() / 3);

		
		vertices1.clear();
		vertices2.clear();
		vertices3.clear();
		colors.clear();
	}
	QVector<GLfloat> points;
	points << 0 << 0 << 0;
	QVector<GLfloat> colors;
	colors << 1 << 0 << 0;
	glEnable(GL_PROGRAM_POINT_SIZE);
	glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, points.data());
	glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors.data());
	glDrawArrays(GL_POINTS, 0, points.size() / 3);
	glDisable(GL_PROGRAM_POINT_SIZE);
	for (int i = 0; i < 360; i++)
	{
		QVector<GLfloat> cylinderVertices;
		QVector<EachPole> polesVector = cyn1->getPoles();
		cylinderVertices << polesVector[i].x << polesVector[i].y_start << polesVector[i].z;
		cylinderVertices << polesVector[i].x << polesVector[i].y_end << polesVector[i].z;
		QVector<GLfloat> colors;
		colors << 0 << 1 << 0;
		colors << 0 << 1 << 0;
		glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, cylinderVertices.data());
		glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors.data());
		glDrawArrays(GL_LINES, 0, cylinderVertices.size() / 3);

	}

	points.clear();
	colors.clear();
	points << -2 << -2 << -2;
	colors << 0 << 1 << 0;
	glEnable(GL_PROGRAM_POINT_SIZE);
	glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, points.data());
	glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors.data());

	glDrawArrays(GL_POINTS, 0, points.size() / 3);
	glDisable(GL_PROGRAM_POINT_SIZE);

	//check point is inside or not
	/*
	* point is (2,2,2)
	* points of box?
	* got the points into a QVector
	* now checking statement
	* 
	*/

	if ((points.at(0) > vx1->boxMinAndMax.at(0) && points.at(0) <vx1->boxMinAndMax.at(3) ) && (points.at(1) > vx1->boxMinAndMax.at(1) && points.at(1) < vx1->boxMinAndMax.at(4)) && (points.at(2) > vx1->boxMinAndMax.at(2) && points.at(2) < vx1->boxMinAndMax.at(5)))
	{
		points.clear();
		colors.clear();
		points << -1 << -1 << -1;
		colors << 0 << 0 << 1;
		glEnable(GL_PROGRAM_POINT_SIZE);
		glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, points.data());
		glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors.data());

		glDrawArrays(GL_POINTS, 0, points.size() / 3);
		glDisable(GL_PROGRAM_POINT_SIZE);
	}


	glDisableVertexAttribArray(m_colAttr);
	glDisableVertexAttribArray(m_posAttr);


}


void OpenGLWindow::initializeGL()
{
	static const char* vertexShaderSource =
		"attribute highp vec4 posAttr; \n"
		"attribute lowp vec4 colAttr;\n"
		"attribute lowp vec3 norAttr;\n"
		"varying lowp vec4 col;\n"
		"varying vec3 vert;\n"
		"varying vec3 vertNormal;\n"
		"uniform highp mat4 projMatrix;\n"
		"uniform highp mat4 viewMatrix;\n"
		"uniform highp mat4 modelMatrix;\n"
		"uniform mat3 normalMatrix;\n"
		"void main() {\n"
		"   col = colAttr;\n"
		"   vert = posAttr.xyz;\n"
		"   gl_PointSize = 10.0;\n"
		"   vertNormal = normalMatrix * norAttr;\n"
		"   gl_Position = projMatrix * viewMatrix * modelMatrix * posAttr;\n"
		"}\n";

	static const char* fragmentShaderSource =
		"varying lowp vec4 col;\n"
		"varying highp vec3 vert;\n"
		"varying highp vec3 vertNormal;\n"
		"uniform highp vec3 lightPos;\n"
		"void main() {\n"
		"  highp vec3 L = normalize(lightPos - vert);\n"
		"   highp float NL = max(dot(normalize(vertNormal), L), 0.0);\n"
		"   highp vec3 color = vec3(col);\n"
		"   highp vec3 col1 = clamp(color * 0.2 + color * 0.8 * NL, 0.0, 1.0);\n"
		"   gl_FragColor = vec4(col1, 1.0);\n"
		"}\n";



	rotationAngle = QQuaternion::fromAxisAndAngle(180.0f, 0.0f, 1.0f, 0.0f);

	initializeOpenGLFunctions();
	glEnable(GL_DEPTH_TEST);
	mProgram = new QOpenGLShaderProgram(this);
	mProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
	mProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
	mProgram->link();
	m_posAttr = mProgram->attributeLocation("posAttr");
	Q_ASSERT(m_posAttr != -1);
	m_colAttr = mProgram->attributeLocation("colAttr");
	Q_ASSERT(m_colAttr != -1);
	m_normAttr = mProgram->attributeLocation("norAttr");
	Q_ASSERT(m_normAttr != -1);
	m_modelMatrixUniform = mProgram->uniformLocation("modelMatrix");
	Q_ASSERT(m_modelMatrixUniform != -1);
	m_viewMatrixUniform = mProgram->uniformLocation("viewMatrix");
	Q_ASSERT(m_viewMatrixUniform != -1);
	m_projectionMatrixUniform = mProgram->uniformLocation("projMatrix");
	Q_ASSERT(m_projectionMatrixUniform != -1);
	m_normalMatrixUniform = mProgram->uniformLocation("normalMatrix");
	Q_ASSERT(m_normalMatrixUniform != -1);
	m_lightPosUniform = mProgram->uniformLocation("lightPos");
	Q_ASSERT(m_lightPosUniform != -1);

}


void OpenGLWindow::mouseMoveEvent(QMouseEvent* event)
{
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();
	if (event->buttons() & Qt::LeftButton)
	{
		QQuaternion rotX = QQuaternion::fromAxisAndAngle(0.0f, 1.0f, 0.0f, 0.5f * dx);
		QQuaternion rotY = QQuaternion::fromAxisAndAngle(1.0f, 0.0f, 0.0f, 0.5f * dy);

		rotationAngle = rotX * rotY * rotationAngle;
		update();
	}
	lastPos = event->pos();
}

void OpenGLWindow::wheelEvent(QWheelEvent* event)
{
	if (event->angleDelta().y() > 0)
	{
		zoomOut();
	}
	else
	{
		zoomIn();
	}
}

void OpenGLWindow::zoomIn()
{
	scaleFactor *= 1.1f;
	update();
}

void OpenGLWindow::zoomOut()
{
	scaleFactor /= 1.1f;
	update();
}



/*
QVector3D cylinderColor(0.0f, 0.0f, 0.0f); // Black color

	// Calculate vertices around the perimeter of the rectangle to form the sides of the cylinder
	QVector<GLfloat> vertices;
	QVector<GLuint> indices;

	// Vertices for the rectangle
	QVector3D rectVertices[] = {
		QVector3D(0, 0, 0),
		QVector3D(5, 0, 0),
		QVector3D(5, 5, 0),
		QVector3D(0, 5, 0)
	};

	// Number of vertices in the rectangle
	int rectVertexCount = sizeof(rectVertices) / sizeof(rectVertices[0]);

	// Number of vertices to draw the sides of the cylinder
	int cylinderVertexCount = rectVertexCount * 2; // Two vertices for each rectangle vertex to create the sides of the cylinder

	// Add vertices around the perimeter of the rectangle
	for (int i = 0; i < rectVertexCount; ++i) {
		vertices << rectVertices[i].x() << rectVertices[i].y() << rectVertices[i].z(); // Vertex on the bottom of the cylinder
		vertices << rectVertices[i].x() << rectVertices[i].y() << rectVertices[i].z() + 5; // Vertex on the top of the cylinder
	}

	// Add indices to connect vertices to form triangles that create the sides of the cylinder
	for (int i = 0; i < rectVertexCount; ++i) {
		int nextIndex = (i + 1) % rectVertexCount;
		// Indices for the bottom triangle
		indices << i * 2 << nextIndex * 2 << i * 2 + 1;
		// Indices for the top triangle
		indices << i * 2 + 1 << nextIndex * 2 << nextIndex * 2 + 1;
	}

	// Draw the cylinder
	mProgram->setUniformValue("color", cylinderColor);

	QOpenGLVertexArrayObject vao;
	vao.create();
	vao.bind();

	QOpenGLBuffer vbo(QOpenGLBuffer::VertexBuffer);
	vbo.create();
	vbo.bind();
	vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo.allocate(vertices.constData(), vertices.size() * sizeof(GLfloat));

	glEnableVertexAttribArray(m_posAttr);
	glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	QOpenGLBuffer ebo(QOpenGLBuffer::IndexBuffer);
	ebo.create();
	ebo.bind();
	ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	ebo.allocate(indices.constData(), indices.size() * sizeof(GLuint));

	// Draw the cylinder
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

	// Clean up
	vao.release();
	mProgram->release();
*/

////////////////////////////////////////////////////////////////////////////
//RECTANGLE
/*
QVector3D rectColor(0.0f, 1.0f, 0.0f); // Red color
	mProgram->setUniformValue("color", rectColor);

	// Draw the rectangle using glDrawElements()
	QVector<GLfloat> vertices;
	vertices << 0 << 0 << 0;
	vertices << 5 << 0 << 0;
	vertices << 5 << 5 << 0;
	vertices << 0 << 5 << 0;

	QVector<GLuint> indices;
	indices << 0 << 1 << 2 << 3 << 0;

	QOpenGLVertexArrayObject vao;
	vao.create();
	vao.bind();

	QOpenGLBuffer vbo(QOpenGLBuffer::VertexBuffer);
	vbo.create();
	vbo.bind();
	vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo.allocate(vertices.constData(), vertices.size() * sizeof(GLfloat));

	glEnableVertexAttribArray(m_posAttr);
	glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	QOpenGLBuffer ebo(QOpenGLBuffer::IndexBuffer);
	ebo.create();
	ebo.bind();
	ebo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	ebo.allocate(indices.constData(), indices.size() * sizeof(GLuint));

	// Draw the rectangle
	glDrawElements(GL_TRIANGLE_FAN, indices.size(), GL_UNSIGNED_INT, nullptr);

	vao.release();
	mProgram->release();
*/