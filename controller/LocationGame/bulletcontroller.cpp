#include "bulletcontroller.h"
#include "model/map.h"
#include <complex>
#include <QtMath>
#include <QDebug>
#include <QTimeLine>


BulletController::BulletController(QWidget* parent) : QWidget(parent)
{

}

void BulletController::updateBullets()
{
	for (int i = 0; i < characterBullets.size(); i++)
	{
		if (!characterBullets[i]->isHidden())
		{
			
			/*QPropertyAnimation* pPosAnimation1 = new QPropertyAnimation(characterBullets[i], "geometry");
			pPosAnimation1->setDuration(40);
			pPosAnimation1->setStartValue(QRect(characterBullets[i]->x(), characterBullets[i]->y(),characterBullets[i]->width(),characterBullets[i]->height()));
			pPosAnimation1->setEndValue(QRect(characterBullets[i]->x() + characterBullets[i]->speed * cos(characterBullets[i]->shootAngle * M_PI / 180)
										, characterBullets[i]->y() + characterBullets[i]->speed * sin(characterBullets[i]->shootAngle * M_PI / 180)
										, characterBullets[i]->width(), characterBullets[i]->height()));
			pPosAnimation1->setEasingCurve(QEasingCurve::InOutQuad);
			emit addAnimation(pPosAnimation1);*/
			/*characterBullets[i]->move(characterBullets[i]->x() + characterBullets[i]->speed * cos(characterBullets[i]->shootAngle * M_PI / 180),
				characterBullets[i]->y() + characterBullets[i]->speed * sin(characterBullets[i]->shootAngle * M_PI / 180));*/
		}
	}
	//deleteBullets();
}

void BulletController::removeBullet(Bullet* bullet)
{
	characterBullets.removeOne(bullet);
}

void BulletController::addCharacterBullet(Bullet* bullet)
{
	connect(bullet, &Bullet::sendPosition, this, &BulletController::judge);
	characterBullets.push_back(bullet);
}

void BulletController::judge(Bullet* bullet)
{
	
}

void BulletController::deleteBullets()
{
	for (QVector<Bullet*>::iterator it = characterBullets.begin(); it != characterBullets.end();)
	{
		if ((*it)->isHidden()) {
			Bullet* bullet = *it;
			it = characterBullets.erase(it);
			delete bullet;
			bullet = NULL;
		}
		else {
			it++;
		}
	}
}

