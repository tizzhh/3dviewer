#include "mainwindow.h"
#include "./ui_mainwindow.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "../c_files/3d_viewer.h"
#ifdef __cplusplus
}
#endif

#include <QMessageBox>
#include <QFileDialog>
#include <QKeyEvent>
#include "oglwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W) {
        ui->oglwidget->moveCamera("W");
    }
    else if(event->key() == Qt::Key_A) {
        ui->oglwidget->moveCamera("A");
    }
    else if(event->key() == Qt::Key_S) {
        ui->oglwidget->moveCamera("S");
    }
    else if(event->key() == Qt::Key_D) {
        ui->oglwidget->moveCamera("D");
    }
    else if(event->key() == Qt::Key_Q) {

    }
    else if(event->key() == Qt::Key_E) {

    }
}
void MainWindow::on_loadButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Open", "../test_objs", "*.obj");
    if(!file.isEmpty()) {
        struct data dataObject = {0};
        QByteArray ba = file.toLocal8Bit();
        char *path = ba.data();
        output res = S21_PrepareData(path, &dataObject);
        if(res == ERROR) {
            QMessageBox::critical(this, "Error", "Failed parse file");
        }
        else {
            ui->oglwidget->setDataObject(dataObject);
            ui->oglwidget->update();
        }
    }
}


void MainWindow::on_SliderZ_sliderMoved(int position)
{
    ui->oglwidget->setAngleZ(position);
}

void MainWindow::on_SliderY_sliderMoved(int position)
{
    ui->oglwidget->setAngleY(position);
}



void MainWindow::on_sliderX_sliderMoved(int position)
{
    ui->oglwidget->setAngleX(position);
}


void MainWindow::on_radioButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_radioButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_sliderMoveX_sliderMoved(int position)
{
    ui->oglwidget->setCoordX(position);
}


void MainWindow::on_sliderMoveY_sliderMoved(int position)
{
    ui->oglwidget->setCoordY(position);
}


void MainWindow::on_sliderMoveZ_sliderMoved(int position)
{
    ui->oglwidget->setCoordZ(position);
}

