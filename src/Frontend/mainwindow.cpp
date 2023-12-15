#include "mainwindow.h"

#include "./ui_mainwindow.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "../c_files/3d_viewer.h"
#ifdef __cplusplus
}
#endif

#include <QDir>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QTimer>

#include "../giflib/dgif_lib.c"
#include "../giflib/egif_lib.c"
#include "../giflib/gif_err.c"
#include "../giflib/gif_hash.c"
#include "../giflib/gifalloc.c"
#include "../giflib/qgifimage.h"
#include "oglwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  this->screenshots = 0;
  this->gifs = 0;
  gif_list.clear();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_W) {
    double scale = ui->oglwidget->getScale();
    scale += 0.1;
    ui->verticalSlider->setValue(scale * 10);
    ui->oglwidget->setScale(scale);
  } else if (event->key() == Qt::Key_A) {
    ui->oglwidget->moveCamera("A");
  } else if (event->key() == Qt::Key_S) {
    double scale = ui->oglwidget->getScale();
    scale -= 0.1;
    ui->verticalSlider->setValue(scale * 10);
    ui->oglwidget->setScale(scale);
  } else if (event->key() == Qt::Key_D) {
    ui->oglwidget->moveCamera("D");
  } else if (event->key() == Qt::Key_Q) {
  } else if (event->key() == Qt::Key_E) {
  }
}

void MainWindow::on_screenButton_clicked() {
  QDir parentDir(QCoreApplication::applicationDirPath());
  parentDir.cdUp();

  QDir screenshotsDir(parentDir.absolutePath() + "/screenshots");
  if (!screenshotsDir.exists()) {
    screenshotsDir.mkpath(".");
  }
  auto geom = ui->oglwidget->grab();
  auto image = geom.toImage();
  image = image.scaled(WIDTH, HEIGHT, Qt::KeepAspectRatio,
                       Qt::SmoothTransformation);

  QStringList screens;

  int highest_num = 0;
  if (this->screenshots == 0) {
    screens = screenshotsDir.entryList(QStringList() << "*.bmp"
                                                     << "*.jpeg",
                                       QDir::Files);
    for (auto &screen : screens) {
      screen.replace("screenshot_", "");
      int name_num = screen.at(0).digitValue();
      if (name_num > highest_num) {
        highest_num = name_num;
      }
    }
    this->screenshots = highest_num + 1;
  }

  QString file_name;
  if (ui->radioButton_bmp->isChecked()) {
    file_name = screenshotsDir.filePath("screenshot_") +
                QString::number(this->screenshots++) + ".bmp";
  } else {
    file_name = screenshotsDir.filePath("screenshot_") +
                QString::number(this->screenshots++) + ".jpeg";
  }

  image.save(file_name);
  QMessageBox::information(this, "Screenshot Captured",
                           "Screenshot saved as:\n" + file_name);
}

void MainWindow::on_loadButton_clicked() {
  QString file =
      QFileDialog::getOpenFileName(this, "Open", "../test_objs", "*.obj");
  if (!file.isEmpty()) {
    struct data dataObject = {0};
    QByteArray ba = file.toLocal8Bit();
    char *path = ba.data();
    output res = S21_PrepareData(path, &dataObject);
    if (res == ERROR) {
      QMessageBox::critical(this, "Error", "Failed parse file");
    } else {
      ui->oglwidget->setDataObject(dataObject);
      ui->oglwidget->update();
    }
  }
}

void MainWindow::on_SliderZ_sliderMoved(int position) {
  ui->oglwidget->setAngleZ(position);
}

void MainWindow::on_SliderY_sliderMoved(int position) {
  ui->oglwidget->setAngleY(position);
}

void MainWindow::on_sliderX_sliderMoved(int position) {
  ui->oglwidget->setAngleX(position);
}

void MainWindow::on_radioButton_2_clicked() {
  ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_radioButton_clicked() {
  ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_sliderMoveX_sliderMoved(int position) {
  ui->oglwidget->setCoordX(position);
}

void MainWindow::on_sliderMoveY_sliderMoved(int position) {
  ui->oglwidget->setCoordY(position);
}

void MainWindow::on_sliderMoveZ_sliderMoved(int position) {
  ui->oglwidget->setCoordZ(position);
}

void MainWindow::on_verticalSlider_sliderMoved(int position) {
  ui->oglwidget->setScale(position / 10.0);
}

void MainWindow::on_gifButton_clicked() {
  connect(&timer, SIGNAL(timeout()), this, SLOT(CaptureFrames()));
  timer.start(DELAY_BETWEEN_FRAMES);
}

void MainWindow::CaptureFrames() {
  qDebug() << gif_list.size();
  auto geom = ui->oglwidget->grab();
  auto image = geom.toImage();
  gif_list.append(image.scaled(WIDTH, HEIGHT, Qt::KeepAspectRatio,
                               Qt::SmoothTransformation));
  if (gif_list.size() >= GIF_DURATION * FPS) {
    timer.stop();
    disconnect(&timer, SIGNAL(timeout()), this, SLOT(CaptureFrames()));
    SaveGif();
  }
}

void MainWindow::SaveGif() {
  QDir parentDir(QCoreApplication::applicationDirPath());
  parentDir.cdUp();

  QDir gifDir(parentDir.absolutePath() + "/gifs");
  if (!gifDir.exists()) {
    gifDir.mkpath(".");
  }

  QStringList gifs;

  int highest_num = 0;
  if (this->gifs == 0) {
    gifs = gifDir.entryList(QStringList() << "*.gif", QDir::Files);
    for (auto &screen : gifs) {
      screen.replace("gif_", "");
      int name_num = screen.at(0).digitValue();
      if (name_num > highest_num) {
        highest_num = name_num;
      }
    }
    this->gifs = highest_num + 1;
  }

  QGifImage gif_recorder;
  gif_recorder.setDefaultDelay(DELAY_BETWEEN_FRAMES);
  for (const auto frame : gif_list) {
    gif_recorder.addFrame(frame);
  }

  QString file_name;
  file_name = gifDir.filePath("gif_") + QString::number(this->gifs++) + ".gif";

  gif_recorder.save(file_name);
  gif_list.clear();

  QMessageBox::information(this, "Gif Captured", "Gif saved as:\n" + file_name);
}