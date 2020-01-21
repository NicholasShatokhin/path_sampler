#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMouseEvent>
#include <QPainter>

#include "gaussian.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    path.push_back(QPointF(-350, -250));
    path.push_back(QPointF(350, -250));
    path.push_back(QPointF(350, 250));
    path.push_back(QPointF(-350, 250));

    currPos = QPointF(0, 0);

    speed = 50;

    currentWaypoint = 0;

    samplesCount = 40;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.translate(QPointF(width() / 2, height() / 2));

    for(size_t i =0; i < path.size(); i++)
    {
        painter.drawLine(path[i], path[(i + 1)%path.size()]);

        painter.fillRect(path[i].x(), path[i].y(), 10, 10, QBrush(Qt::red));
    }

    painter.fillRect(currPos.x(), currPos.y(), 10, 10, QBrush(Qt::green));

    QPointF wpDir = path[currentWaypoint] - currPos;

    painter.drawLine(currPos, path[currentWaypoint]);

    double wpDirLength = sqrt(wpDir.x() * wpDir.x() + wpDir.y() * wpDir.y());

    QPointF normalDir = wpDir / wpDirLength;

    QPointF cp = currPos, newCP;
    double sumLength = 0;

    std::vector<float> X, Y;

    for(size_t i = 0; i < samplesCount; i++)
    {
        newCP = normalDir * speed;

        painter.drawLine(cp, cp + newCP);

        cp += newCP;

        painter.fillRect(cp.x(), cp.y(), 10, 10, QBrush(Qt::magenta));

        X.push_back(cp.x());
        Y.push_back(cp.y());

        sumLength += speed;

        if(sumLength >= wpDirLength)
        {
            currentWaypoint = (currentWaypoint + 1) % path.size();
            wpDir = path[currentWaypoint] - cp;
            wpDirLength = sqrt(wpDir.x() * wpDir.x() + wpDir.y() * wpDir.y());
            normalDir = wpDir / wpDirLength;
            sumLength = 0;
        }
    }

    GaussianFilter filter(7, 1.f);
    std::vector<float> Xsmoothed = filter.apply(X);
    std::vector<float> Ysmoothed = filter.apply(Y);

    cp = currPos;
    for(size_t i = 0; i < Xsmoothed.size(); i++)
    {
        painter.drawLine(cp, QPointF(Xsmoothed[i], Ysmoothed[i]));
        painter.fillRect(Xsmoothed[i], Ysmoothed[i], 10, 10, QBrush(Qt::blue));
        cp.setX(Xsmoothed[i]);
        cp.setY(Ysmoothed[i]);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPointF center(width() / 2, height() / 2);

    currPos = event->pos() - center;

    currentWaypoint = 0;

    repaint();
}
