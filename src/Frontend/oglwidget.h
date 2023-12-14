#ifndef OGLWINDOW_H
#define OGLWINDOW_H

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QObject>

#ifdef __cplusplus
extern "C" {
#endif
#include "../c_files/3d_viewer.h"
typedef struct data data_t;
#ifdef __cplusplus
}
#endif


class OGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    OGLWidget(QWidget *parent = nullptr);

    void setDataObject(data_t newDataObject);
    void moveCamera(QString str);
    void setAngleZ(int newAngleZ);

    void setAngleX(int newAngleX);

    void setAngleY(int newAngleY);

    void setCoordX(int newCoordX);

    void setCoordY(int newCoordY);

    void setCoordZ(int newCoordZ);

    void setScale(double newScale);

    double getScale() const;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
private:
    void drawObject();
    data_t dataObject = {0};
    int _angleZ = 0;
    int _angleX = 0;
    int _angleY = 0;
    int coordX {0};
    int coordY {0};
    int coordZ {0};
    double scale{0.5};

};

#endif // OGLWINDOW_H
