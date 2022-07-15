#include "bullet.h"
#include <QDebug>
#include <QPainter>
#include <QtMath>
#include <QTimeLine>
#include <QPropertyAnimation>

Bullet::Bullet(QWidget* parent) : QLabel(parent)
{

}

Bullet::Bullet(QWidget* parent, unsigned ATK, float shootAngle, float speed, QString URL, QPoint point, int offset) : QLabel(parent)
{
	setAttribute(Qt::WA_TranslucentBackground, false);

	this->ATK = ATK;
	this->shootAngle = shootAngle;
	this->speed = speed;
	this->setPixmap(QPixmap(URL));
	this->setScaledContents(true);
	this->resize(20, 20);
	this->move(point.x() - width() / 2 + offset * cos(shootAngle * M_PI / 180), point.y() - height() / 2 + offset * sin(shootAngle * M_PI / 180));
	this->show();
}

QPoint Bullet::getPoint()
{
	return QPoint(this->x() + this->width() / 2, this->y() + this->height() / 2);
}

void Bullet::setPoint(QPoint point)
{
	setGeometry(point.x() - width() / 2, point.y() - height() / 2, width(), height());
}

void Bullet::moveStart()
{
	QTimeLine* timeLine = new QTimeLine(40000, this);
    timeLine->setObjectName("timeLine");
	timeLine->setFrameRange(0, 9000);
	timeLine->setObjectName("timeLine");
	timeLine->setCurveShape(QTimeLine::LinearCurve);
	timeLine->setUpdateInterval(100 / (this->speed / 10.0));
	connect(timeLine, &QTimeLine::frameChanged, this, [=](int frame) {
		this->move(this->x() + 10.0 * cos(this->shootAngle * M_PI / 180),
			this->y() + 10.0 * sin(this->shootAngle * M_PI / 180));
		emit sendPosition(this);
		//qDebug() << frame << "frameChanged" << this->geometry();
	});
	//connect(timeLine, &QTimeLine::finished, timeLine, &QTimeLine::deleteLater);
	timeLine->start();
}

void Bullet::hitBarrier()
{
	QTimeLine* timeLine = this->findChild<QTimeLine*>("timeLine");
	timeLine->stop();
	delete this;
}

void Bullet::hitMonster()
{
	QTimeLine* timeLine = this->findChild<QTimeLine*>("timeLine");
	timeLine->stop();
    delete this;
}

void Bullet::stopGame()
{
    QTimeLine* timeLine = findChild<QTimeLine*>("timeLine");
    timeLine->stop();
}

void Bullet::continueGame()
{
    QTimeLine* timeLine = findChild<QTimeLine*>("timeLine");
    timeLine->resume();
}

void Bullet::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::NoBrush);
	painter.setRenderHint(QPainter::SmoothPixmapTransform);
	painter.translate(this->width() / 2, this->height() / 2);          //设置旋转中心
	painter.rotate(shootAngle);          //旋转
	painter.translate(-(this->width() / 2), -(this->height() / 2));        //将原点复位
	painter.drawPixmap(this->rect(), *pixmap());
	painter.end();
	//    setPixmap(*pixmap());
	//    QWidget::paintEvent(e);
}
