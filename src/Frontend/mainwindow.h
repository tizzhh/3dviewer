#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void on_screenButton_clicked();

    void on_loadButton_clicked();

    void on_SliderZ_sliderMoved(int position);

    void on_SliderY_sliderMoved(int position);

    void on_sliderX_sliderMoved(int position);

    void on_radioButton_2_clicked();

    void on_radioButton_clicked();

    void on_sliderMoveX_sliderMoved(int position);

    void on_sliderMoveY_sliderMoved(int position);

    void on_sliderMoveZ_sliderMoved(int position);

    void on_verticalSlider_sliderMoved(int position);

    void on_gifButton_clicked();

private:
    Ui::MainWindow *ui;
    int screenshots;
};
#endif // MAINWINDOW_H
