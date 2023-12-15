#include "oglwidget.h"

#include <QKeyEvent>
#include <QtOpenGL>
#include <array>
OGLWidget::OGLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

void OGLWidget::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void OGLWidget::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //    glOrtho(-10,10,-10,10,0.5, 5)
  //    glOrtho(-10,10,-10,10,-5, 5);
  //    glFrustum(-2,2,-2,2,-3, 5);
  //    glFrustum(0, this->width(), 0, this->height(), -200, 200);
  //    resize(this->width(), this->height());
  //    glFrustum(-1,1,-1,1,1,2);
}

void OGLWidget::paintGL() {
  //    glVertexPointer(3, GL_FLOAT, 0, &dataObject.matrix_3d.matrix);
  //    glColorPointer(3, GL_FLOAT, 0, &dataObject.matrix_3d.matrix);
  //    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE,
  //    &dataObject.matrix_3d.matrix);

  //    resize(this->width(), this->height());

  //    glClearColor(0.7,1,0.7,0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  //    glScalef(0.0001,0.0001,0.001);
  glFrustum(-5, 5, -5, 5, -5, 5);

  glScalef(scale, scale, scale);
  glLineWidth(1000.0);

  glRotatef(_angleZ, 0, 0, 1);
  glRotatef(_angleY, 0, 1, 0);
  glRotatef(_angleX, 1, 0, 0);
  glTranslatef(coordX, coordY, coordZ);
  //    glFrustum(-2,2,-2,2,0e.5, 5);
  //    glTranslated(0, 0, -2);

  drawObject();
}

void OGLWidget::drawObject() {
  if (dataObject.count_of_vertexes != 0) {
    glColor3d(0.7, 1, 0.7);
    int countVertexes{0};
    QVector<double> vec;
    point **combinedData;

    combinedData = S21_CombineFacetsWithVertexes(&dataObject);
    for (int i = 0; i < dataObject.count_of_facets; ++i) {
      countVertexes += dataObject.polygons[i].numbers_of_vertexes_in_facets;
      for (int j = 0; j < dataObject.polygons[i].numbers_of_vertexes_in_facets;
           ++j) {
        vec.push_back(combinedData[i][j].ox);
        vec.push_back(combinedData[i][j].oy);
        vec.push_back(combinedData[i][j].oz);
      }
    }
    double *data = vec.data();
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_DOUBLE, 0, data);
    glDrawArrays(GL_LINE_STRIP, 0, countVertexes);
    glDisableClientState(GL_VERTEX_ARRAY);
  }
}

double OGLWidget::getScale() const { return scale; }

void OGLWidget::setScale(double newScale) {
  scale = newScale;
  update();
}

void OGLWidget::setCoordZ(int newCoordZ) {
  coordZ = newCoordZ;
  update();
}

void OGLWidget::setCoordY(int newCoordY) {
  coordY = newCoordY;
  update();
}

void OGLWidget::setCoordX(int newCoordX) {
  coordX = newCoordX;
  update();
}

void OGLWidget::setAngleY(int newAngleY) {
  _angleY = newAngleY;
  update();
}

void OGLWidget::setAngleX(int newAngleX) {
  _angleX = newAngleX;
  update();
}

void OGLWidget::setAngleZ(int newAngleZ) {
  _angleZ = newAngleZ;
  update();
}

void OGLWidget::setDataObject(data_t newDataObject) {
  dataObject = newDataObject;
}

void OGLWidget::moveCamera(QString str) {
  glPushMatrix();
  glLoadIdentity();
  if (str == "S") {
    glTranslatef(0, 0, -30);
  } else if (str == "W") {
    _angleZ += 25;
    _angleZ += 25;
    update();
    //        glTranslatef(0,0,-3);
    //        drawObject();
  } else if (str == "A")
    glTranslatef(-30, 0, 0);
  else if (str == "D")
    glTranslatef(30, 0, 0);

  glPopMatrix();
  update();
}
