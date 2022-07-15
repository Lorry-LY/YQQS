#ifndef CHARACTERCONTROLLER_H
#define CHARACTERCONTROLLER_H

#include <QWidget>
#include <QPropertyAnimation>

#include "model/character.h"
#include "model/box.h"
#include "controller/mediaController/mediacontroller.h"

class CharacterController : public QWidget
{
    Q_OBJECT
public:
    explicit CharacterController(QWidget *parent = nullptr);

    void createCharacter(Character* character);
    Character* getMyCharacter();
    void move(Character *character,char moveWay);
    void stop(Character *character,char moveWay);
    void shoot(Character *character);
    void changeWeaponAngle(Character *character);
    void changeWeaponAngle(Character *character,float angle);
    void changeWeapon(Character *character);
    void changeWeapon(Character *character,Weapon* weapon);
    void releaseSkill(Character *character);
    void buyBox(Character *character);
    void buyBox(Character *character,Box* box);

    void victory(Character *character);

    void dealChangeWeaponAngle(float angle);
    void dealCharacterDead(Character* character);

    void judgeMove(Character* character,QPoint point,QPoint movePoint);
    void judgeRepel(Character* character,QPoint point);  

    void stopGame();
    void continueGame();

    MediaController *media;
private:
    double distance(QPoint x1, QPoint x2);

protected:
//    void paintEvent(QPaintEvent *event);

signals:
    void sendChangeWeaponAngle();
    void changeWeapon(Weapon* oldWeapon);
    void changeView(QPoint point);
    void addBullet(Character* character, Bullet* bullet);
    void Victory();
    void GameOver();

private slots:
    void run();
    void wait();

private:
    QVector<Character*> characters;
    Character *myCharacter;
    QTimer* timer1;
    QTimer* timer2;
    int skillTime=5;

};

#endif // CHARACTERCONTROLLER_H
