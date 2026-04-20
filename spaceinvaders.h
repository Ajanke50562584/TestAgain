#ifndef SPACEINVADERS_H
#define SPACEINVADERS_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QList>
#include <QProgressBar>

class SpaceInvaders : public QWidget
{
    Q_OBJECT

public:
    SpaceInvaders(QWidget *parent = 0);
    ~SpaceInvaders();

private:
    QList<QLabel*> bullets;
    QList<QProgressBar*> enemies;
    QLabel *spaceship;
    QTimer *timer;
    int movedir;

    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void fire();

    static const int ssSpeed = 7;
    static const int bSpeed = 10;
    static const int eSpeed = 5;
    static const int eCount = 40;
    static const int eDamage = 25;

private slots:
    void timerTick();
};

#endif // SPACEINVADERS_H
