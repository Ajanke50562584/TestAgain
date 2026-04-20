#include "spaceinvaders.h"
#include <QDebug>

SpaceInvaders::SpaceInvaders(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(1024, 768);

    spaceship = new QLabel(this);
    spaceship->setText("---ooo0ooo---");
    //spaceship->setStyleSheet("background-color: magenta;");
    spaceship->setFixedSize(100,24);
    spaceship->move(0, height()-spaceship->height());

    timer = new QTimer(this);
    timer->setInterval(20); // just an event phoning back, not strictly exact...
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTick()));
    timer->start();

    for (int i = 1; i <= eCount; ++i)
    {
        QProgressBar *e = new QProgressBar(this);
        e->setValue(100);
        e->setFixedSize(100,30);
        e->move(-e->width()*i, 0);
        e->show();
        enemies.append(e);
    }

    movedir = 0;

    setMouseTracking(true);
    //grabMouse();
}

SpaceInvaders::~SpaceInvaders()
{

}

void SpaceInvaders::keyPressEvent(QKeyEvent *e)
{
    qDebug() << e->key();
    switch(e->key())
    {
    case Qt::Key_Left:
    case Qt::Key_A:
        movedir = -1;
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
        movedir = 1;
        break;
    case Qt::Key_Space:
        fire();
        break;
    }
}

void SpaceInvaders::keyReleaseEvent(QKeyEvent *e)
{
    qDebug() << e->key();
    switch(e->key())
    {
    case Qt::Key_Left:
    case Qt::Key_A:
    case Qt::Key_Right:
    case Qt::Key_D:
        movedir = 0;
        break;
    }
}

void SpaceInvaders::mouseMoveEvent(QMouseEvent *e)
{
    qDebug() << e->x() << " " << e->y();
    spaceship->move(e->x(), spaceship->y());
}

void SpaceInvaders::mousePressEvent(QMouseEvent *e)
{
    //if (e->button() == Qt::LeftButton)
        fire();
}

void SpaceInvaders::timerTick()
{
    // Move spaceship
    bool move = false;
    if ((movedir == -1) && (spaceship->x() >= 0))
        move = true;
    else if ((movedir == 1) && (spaceship->x()+spaceship->width() <= width()))
        move = true;

    if (move)
        spaceship->move(spaceship->x()+movedir*ssSpeed, spaceship->y());

    // move enemies
    QListIterator<QProgressBar*> ei(enemies);
    while (ei.hasNext())
    {
        // even tracks right, odd tracks left
        QProgressBar *e = ei.next();
        int dir = e->y()/e->height()%2 ? -1 : 1;
        e->move(e->x()+dir*eSpeed, e->y());
        qDebug() << e->x() << e->y() << e->height() << dir;
        if (((dir == 1) && (e->x()+e->width() >= width())) ||
            ((dir == -1) && (e->x() <= 0)))
            e->move(e->x(), e->y()+e->height());

    }

    // move bullets
    QListIterator<QLabel*> bi(bullets);
    while (bi.hasNext())
    {
        QLabel *b = bi.next();
        b->move(b->x(), b->y()-eSpeed);

        // check enemy collisions
        ei.toFront();
        while (ei.hasNext())
        {
            QProgressBar *e = ei.next();
            if (!((b->x() > e->x()+e->width()) ||
                 (b->x()+b->width() < e->x()) ||
                (b->y() > e->y() + e->height()) ||
                 (b->y()+b->height() < e->y())))
            {
                bullets.removeOne(b);
                b->deleteLater(); // TODO: Slab allocate
                e->setValue(e->value()-eDamage);
                if (e->value() == 0)
                {
                    enemies.removeOne(e);
                    e->deleteLater();
                }
            }
        }

        if (b->y()+b->height() < 0)
        {
            bullets.removeOne(b);

            // TUT TODO: Slab allocate and recycle these plz :)
            b->deleteLater();
        }
    }



    // check win/lose
}


void SpaceInvaders::fire()
{
    qDebug() << "Fire!";
    QLabel *b = new QLabel(this);
    b->setText("$<br>#");
    b->show();
    b->move(spaceship->x()+spaceship->width()/2-b->width()/2, spaceship->y()-b->height());
    bullets.append(b);
}
