#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    std::vector<QPointF> path;
    QPointF currPos;
    size_t currentWaypoint;
    double speed;
    size_t samplesCount;

private:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
};
#endif // MAINWINDOW_H
