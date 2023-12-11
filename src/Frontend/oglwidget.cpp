#include "oglwidget.h"
#include <QtOpenGL>
#include <QKeyEvent>
OGLWidget::OGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

void OGLWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
//    glFrustum(-1,1,-1,1,0,8);
//    glFrustum(-1,1,-1,1, 2, 8);
//    glEnable(GL_DEPTH_TEST);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void OGLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
//    glOrtho(-10,10,-10,10,0.5, 5);
    glFrustum(-10,10,-10,10,-0.5, 5);
//    glFrustum(0, this->width(), 0, this->height(), -100, 100);
//    resize(this->width(), this->height());
//    glFrustum(-1,1,-1,1,1,2);
}

void OGLWidget::paintGL()
{

//    glVertexPointer(3, GL_FLOAT, 0, &dataObject.matrix_3d.matrix);
//    glColorPointer(3, GL_FLOAT, 0, &dataObject.matrix_3d.matrix);
//    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, &dataObject.matrix_3d.matrix);

//    resize(this->width(), this->height());
    glClearColor(0.7,1,0.7,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glRotatef(_angleZ,0,0,1);
    glRotatef(_angleX,1,0,0);
    glRotatef(_angleY,0,1,0);
    glTranslatef(coordX, coordY, coordZ);
//    glFrustum(-2,2,-2,2,0.5, 5);
//    glTranslated(0, 0, -2);


    drawObject();
//    glScalef(0.3,0.3,0.3);


//    glOrtho(0, this->width(), 0, this->height(), -100, 100);
//    glTranslated(2, 0, -10);


}

void OGLWidget::drawObject()
{
    if(dataObject.count_of_vertexes != 0) {
        float vertex[] = {0,0,0,    0,1,0,  0,1,1,
                         0,0,1,     1,0,1,  1,1,1,
                         1,1,0,     1,0,0};
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &dataObject.matrix_3d.matrix);
//        glVertexPointer(3, GL_FLOAT, 0, &vertex);
        glDrawArrays(GL_LINE_STRIP, 0, dataObject.count_of_vertexes);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
}

void OGLWidget::setCoordZ(int newCoordZ)
{
    coordZ = newCoordZ;
    update();
}

void OGLWidget::setCoordY(int newCoordY)
{
    coordY = newCoordY;
    update();
}

void OGLWidget::setCoordX(int newCoordX)
{
    coordX = newCoordX;
    update();
}

void OGLWidget::setAngleY(int newAngleY)
{
    _angleY = newAngleY;
    update();
}

void OGLWidget::setAngleX(int newAngleX)
{
    _angleX = newAngleX;
    update();
}

void OGLWidget::setAngleZ(int newAngleZ)
{
    _angleZ = newAngleZ;
    update();
}

void OGLWidget::setDataObject(data_t newDataObject)
{
    dataObject = newDataObject;
}

void OGLWidget::moveCamera(QString str)
{
    glPushMatrix();
    glLoadIdentity();
    if(str == "S") {
        glTranslatef(0,0,-30);
    }
    else if(str == "W") {
        _angleZ +=25;
        _angleZ +=25;
        update();
//        glTranslatef(0,0,-3);
//        drawObject();
    }
    else if(str == "A")
        glTranslatef(-30,0,0);
    else if(str == "D")
        glTranslatef(30,0,0);

    glPopMatrix();
    update();
}

