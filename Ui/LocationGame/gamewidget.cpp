#include "gamewidget.h"
#include "ui_gamewidget.h"
#include <QDir>
#include <QStackedLayout>
#include <QTimer>
#include <QtMath>
#include <QDebug>
#include <QMovie>
#include <QParallelAnimationGroup>
#include "tools/myanimation.h"
#include "model/message.h"
#include "model/magicpoint.h"

GameWidget::GameWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::GameWidget)
{


    db = QSqlDatabase::database("QSQLITE");
    map = new Map(this);
    map->setObjectName("map");
    stop = new StopWidget(this);
    stop->hide();
    setting = new SettingWidget(this);
    setting->hide();

    media = this->parentWidget()->parentWidget()->findChild<MediaController*>("media");

    characters = new CharacterController(this);
    bullets = new BulletController(this);
    monsters = new MonsterController(this);
    monsters->setObjectName("monsters");
    props = new PropController(this);

    connect(this, SIGNAL(StopGame()), this, SLOT(dealStopGame()));
    connect(stop, SIGNAL(Setting()), this, SLOT(dealSetting()));
    connect(stop, SIGNAL(Continue()), this, SLOT(dealContinueGame()));
    connect(stop,&StopWidget::BackToHall,this,[=](){
        media->findChild<QMediaPlayer*>("地牢")->pause();
    });
    connect(setting, SIGNAL(finished()), this, SLOT(dealFinished()));
    connect(setting, &SettingWidget::changeBGMvolume, this, &GameWidget::dealChangeBGMvolume);
    connect(setting, &SettingWidget::changeSoundVolume, this, &GameWidget::dealChangeSoundVolume);
    setting->init();
    connect(map,&Map::fightting,monsters,&MonsterController::controller);
    connect(map,&Map::unfightting,monsters,&MonsterController::uncontroller);
    connect(map,&Map::fightting,this,[=](){
        QMediaPlayer *getIntoHouse = new QMediaPlayer(this);
        getIntoHouse->setMedia(media->findChild<QMediaPlayer*>("进入房间")->media());
        getIntoHouse->setVolume(media->findChild<QMediaPlayer*>("进入房间")->volume());
        getIntoHouse->play();
        connect(getIntoHouse,&QMediaPlayer::stateChanged,getIntoHouse,&QMediaPlayer::deleteLater);
    });
    connect(map,&Map::unfightting,this,[=](){
        QMediaPlayer *getIntoHouse = new QMediaPlayer(this);
        getIntoHouse->setMedia(media->findChild<QMediaPlayer*>("进入房间")->media());
        getIntoHouse->setVolume(media->findChild<QMediaPlayer*>("进入房间")->volume());
        getIntoHouse->play();
        connect(getIntoHouse,&QMediaPlayer::stateChanged,getIntoHouse,&QMediaPlayer::deleteLater);
    });
    connect(map,&Map::finished,this,[=](Character* character){
        changeView(character->getPoint());
    });



    characterBulletsThread = new QThread(this);
    hitMonster = new HitMonsterThread(map);
    connect(hitMonster, &HitMonsterThread::hit, this, [=](Bullet* bullet, MonsterCharacter* character) {
        Message* message = new Message(character->parentWidget(),character->getPoint(),QString("-%1").arg(bullet->ATK),QColor("#f00"));
        message->disappear();
        character->hitted(bullet->ATK,bullet->shootAngle);
        QMediaPlayer *shot = new QMediaPlayer(this);
        shot->setMedia(media->findChild<QMediaPlayer*>("击中")->media());
        shot->setVolume(media->findChild<QMediaPlayer*>("击中")->volume());
        shot->play();
        connect(shot,&QMediaPlayer::stateChanged,shot,&QMediaPlayer::deleteLater);
        bullet->hitMonster();
    });

    hitCharacter = new HitCharacterThread(map);
    connect(hitCharacter, &HitCharacterThread::hit, this, [=](Bullet* bullet, Character* character) {
        Message* message = new Message(character->parentWidget(),character->getPoint(),QString("-%1").arg(bullet->ATK),QColor("#f00"));
        message->disappear();
        character->hitted(bullet->ATK,bullet->shootAngle);
        setHP(character->getHP(),character->MaxHP);
        bullet->hitMonster();
    });

    /*hitMonster->moveToThread(characterBulletsThread);
    connect(characterBulletsThread, &QThread::finished, hitMonster, &QObject::deleteLater);*/
    hitBarrier = new HitBarrierThread(map);
    connect(hitBarrier, &HitBarrierThread::hitBarrier, this , [=](Bullet* bullet) {
        bullet->hitBarrier();
    });

    pickThings = new PickThingsThread(map);
    connect(pickThings,&PickThingsThread::pickMagic,this,[=](Character* character,MagicPoint* magic){
        magic->Picked(character);
    });
    connect(pickThings,&PickThingsThread::pickCoin,this,[=](Character* character,Coin* coin){
        coin->Picked(character);
    });


    connect(characters,&CharacterController::Victory,this,[=](){
        if(map->number!=5)
        {
            transitionAnimation();
            map->continueNew(characters->getMyCharacter());
            media->findChild<QMediaPlayer*>("地牢")->pause();
            QTimer::singleShot(2000,media->findChild<QMediaPlayer*>("地牢"),&QMediaPlayer::play);
            QMediaPlayer *shot = new QMediaPlayer(this);
            shot->setMedia(media->findChild<QMediaPlayer*>("进入下一关")->media());
            shot->setVolume(media->findChild<QMediaPlayer*>("进入下一关")->volume());
            shot->play();
            connect(shot,&QMediaPlayer::stateChanged,shot,&QMediaPlayer::deleteLater);
            emit saveGame();
        }else{
            emit die(gameTime,*characters->getMyCharacter()->getWeapon()->pixmap(),characters->getMyCharacter()->getCoin(),map->number,"你赢了！",
                     characters->getMyCharacter()->name,characters->getMyCharacter()->getWeapon()->name,characters->getMyCharacter()->getCoin());
            media->findChild<QMediaPlayer*>("地牢")->pause();
            delete this;
        }
    });
    connect(characters,&CharacterController::GameOver,this,[=](){
        emit die(gameTime,*characters->getMyCharacter()->getWeapon()->pixmap(),characters->getMyCharacter()->getCoin(),map->number,"你输了！",
                 characters->getMyCharacter()->name,characters->getMyCharacter()->getWeapon()->name,characters->getMyCharacter()->getCoin());
        media->findChild<QMediaPlayer*>("地牢")->pause();
        delete this;
    });
    connect(characters, &CharacterController::changeView, this, &GameWidget::changeView);
    connect(characters, &CharacterController::addBullet, this, &GameWidget::dealCharacterShoot);
    connect(monsters,&MonsterController::addBulletMonster,this,&GameWidget::dealMonsterShoot);
    //characterBulletsThread->start();

    ui->setupUi(this);
    ui->PushButton_Parse->move(940,0);
}

GameWidget::~GameWidget()
{
    characterBulletsThread->quit();
    characterBulletsThread->wait();
    delete ui;
}

void GameWidget::init(int ID)
{
    this->isRunning = true;
    this->gameTime = 0;
    QSqlQuery query = QSqlQuery(db);
    query.exec(QString("select c.'名称' characterName,c.'生命值',c.'能量',c.'护盾',c.'移动速度',c.'身体图片'"
                       ",w.'名称' weaponName,w.'威力',w.'能量消耗',w.'攻击速度',w.'暴击率',w.'类型',w.URL,w.'子弹图片',w.'子弹速度' "
                       "from '角色' c,'武器' w "
                       "where c.'初始武器ID' = w.ID and c.ID = %1").arg(ID));
    query.next();
    QString name = query.value("characterName").toString();
    int HP = query.value("生命值").toInt();
    int MP = query.value("能量").toInt();
    int shield = query.value("护盾").toInt();
    float speed = query.value("移动速度").toDouble();
    QString bodyURL = query.value("身体图片").toString();
    QString weaponName = query.value("weaponName").toString();
    int ATK = query.value("威力").toInt();
    int dMP = query.value("能量消耗").toInt();
    float crit = query.value("暴击率").toFloat();
    float Aspeed = query.value("攻击速度").toFloat();
    int type = query.value("类型").toInt();
    QString weaponURL = query.value("URL").toString();
    QString bulletURL = query.value("子弹图片").toString();
    float bulletSpeed = query.value("子弹速度").toFloat();
    Weapon* weapon = new Weapon(nullptr, weaponName, ATK, dMP, Aspeed, crit, type, weaponURL, bulletURL, bulletSpeed);
    Character* character = new Character(nullptr, name, HP, MP, shield, bodyURL, weapon, speed);
    weapon->setParent(character);
    //    qDebug()<<character->geometry();
    characters->createCharacter(character);
    connect(character,&Character::sendPosition,characters,&CharacterController::judgeMove);
    connect(character,&Character::sendRepelPosition,characters,&CharacterController::judgeRepel);
    connect(character,&Character::sendPosition,pickThings,&PickThingsThread::judge);
    connect(character,&Character::sendPosition,map,&Map::judge);
    connect(character,&Character::changedHP,this,[=](int number,int maxNumber){
        setHP(number,maxNumber);
    });
    connect(character,&Character::changedMP,this,[=](int number,int maxNumber){
        setMP(number,maxNumber);
    });
    connect(character,&Character::changedShied,this,[=](int number,int maxNumber){
        setArmor(number,maxNumber);
    });
    connect(character,&Character::changedCoin,this,[=](int number){
        ui->money->setNum(number);
    });
    connect(character,&Character::dead,characters,&CharacterController::dealCharacterDead);

    QTimer* updateTimer = new QTimer(this);
    updateTimer->setInterval(40);
    connect(updateTimer, &QTimer::timeout, this, &GameWidget::updateGame);
    updateTimer->setObjectName("updateTimer");
    updateTimer->start();

    QTimer* gameTime = new QTimer(this);
    gameTime->setInterval(1000);
    connect(gameTime, &QTimer::timeout, this,[=](){this->gameTime++;});
    gameTime->setObjectName("gameTime");
    gameTime->start();

    //左上角面板属性初始化
    ui->HP_progressBar->setMaximum(character->MaxHP);
    setHP(character->MaxHP,character->MaxHP);
    ui->HP_progressBar->setStyleSheet("QProgressBar{border: none;font-weight: bold;color: white;text-align: center;background: rgb(68, 69, 73);}"
                                      "QProgressBar::chunk {border: none;background: rgb(255, 0, 0);}");
    ui->MP_progressBar->setMaximum(character->MaxMP);
    setMP(character->MaxMP,character->MaxMP);
    ui->MP_progressBar->setStyleSheet("QProgressBar{border: none;font-weight: bold;color: white;text-align: center;background: rgb(68, 69, 73);}"
                                      "QProgressBar::chunk {border: none;background: rgb(0, 160, 230);}");
    ui->Armor_progressBar->setMaximum(character->MaxShield);
    setArmor(character->MaxShield,character->MaxShield);
    ui->Armor_progressBar->setStyleSheet("QProgressBar{border: none;font-weight: bold;color: white;text-align: center;background: rgb(68, 69, 73);}"
                                         "QProgressBar::chunk {border: none;background: rgb(156,170,179);}");

    transitionAnimation();
    map->startNew(character);
    QTimer::singleShot(2000,media->findChild<QMediaPlayer*>("地牢"),&QMediaPlayer::play);



}

void GameWidget::continueGame(int number,int gameTime, int characterID, int HP, int MP, int coin, int weaponID)
{
    QSqlQuery query = QSqlQuery(db);
    query.exec(QString("select c.'名称',c.'生命值',c.'能量',c.'护盾',c.'移动速度',c.'身体图片' "
                       "from '角色' c "
                       "where c.ID = %1").arg(characterID));
    query.next();
    QString name = query.value("名称").toString();
    int maxHP = query.value("生命值").toInt();
    int maxMP = query.value("能量").toInt();
    int maxShield = query.value("护盾").toInt();
    float speed = query.value("移动速度").toDouble();
    QString bodyURL = query.value("身体图片").toString();
    query.exec(QString("select w.'名称',w.'威力',w.'能量消耗',w.'攻击速度',w.'暴击率',w.'类型',w.'URL',w.'子弹图片',w.'子弹速度' "
                       "from '武器' w "
                       "where w.ID = %1").arg(weaponID));
    query.next();
    QString weaponName = query.value("名称").toString();
    int ATK = query.value("威力").toInt();
    int dMP = query.value("能量消耗").toInt();
    float Aspeed = query.value("攻击速度").toFloat();
    float crit = query.value("暴击率").toFloat();
    int type = query.value("类型").toInt();
    QString weaponURL = query.value("URL").toString();
    QString bulletURL = query.value("子弹图片").toString();
    float bulletSpeed = query.value("子弹速度").toFloat();
    Weapon* weapon = new Weapon(nullptr,weaponName,ATK,dMP,Aspeed,crit,type,weaponURL,bulletURL,bulletSpeed);
    Character* character = new Character(map,name,maxHP,maxMP,maxShield,bodyURL,weapon,speed);
    weapon->setParent(character);
    character->changeHP(HP-character->MaxHP);
    character->changeMP(MP-character->MaxMP);
    character->changeCoin(coin);
    this->isRunning = true;
    this->gameTime = gameTime;
    characters->createCharacter(character);
    connect(character,&Character::sendPosition,characters,&CharacterController::judgeMove);
    connect(character,&Character::sendRepelPosition,characters,&CharacterController::judgeRepel);
    connect(character,&Character::sendPosition,pickThings,&PickThingsThread::judge);
    connect(character,&Character::sendPosition,map,&Map::judge);
    connect(character,&Character::changedHP,this,[=](int number,int maxNumber){
        setHP(number,maxNumber);
    });
    connect(character,&Character::changedMP,this,[=](int number,int maxNumber){
        setMP(number,maxNumber);
    });
    connect(character,&Character::changedShied,this,[=](int number,int maxNumber){
        setArmor(number,maxNumber);
    });
    connect(character,&Character::changedCoin,this,[=](int number){
        ui->money->setNum(number);
    });
    connect(character,&Character::dead,characters,&CharacterController::dealCharacterDead);

    QTimer* updateTimer = new QTimer(this);
    updateTimer->setInterval(40);
    connect(updateTimer, &QTimer::timeout, this, &GameWidget::updateGame);
    updateTimer->setObjectName("updateTimer");
    updateTimer->start();

    QTimer* gameTimer = new QTimer(this);
    gameTimer->setInterval(1000);
    connect(gameTimer, &QTimer::timeout, this,[=](){this->gameTime++;});
    gameTimer->setObjectName("gameTime");
    gameTimer->start();

    //左上角面板属性初始化
    ui->HP_progressBar->setMaximum(character->MaxHP);
    setHP(character->MaxHP,character->MaxHP);
    ui->HP_progressBar->setStyleSheet("QProgressBar{border: none;font-weight: bold;color: white;text-align: center;background: rgb(68, 69, 73);}"
                                      "QProgressBar::chunk {border: none;background: rgb(255, 0, 0);}");
    ui->MP_progressBar->setMaximum(character->MaxMP);
    setMP(character->MaxMP,character->MaxMP);
    ui->MP_progressBar->setStyleSheet("QProgressBar{border: none;font-weight: bold;color: white;text-align: center;background: rgb(68, 69, 73);}"
                                      "QProgressBar::chunk {border: none;background: rgb(0, 160, 230);}");
    ui->Armor_progressBar->setMaximum(character->MaxShield);
    setArmor(character->MaxShield,character->MaxShield);
    ui->Armor_progressBar->setStyleSheet("QProgressBar{border: none;font-weight: bold;color: white;text-align: center;background: rgb(68, 69, 73);}"
                                         "QProgressBar::chunk {border: none;background: rgb(156,170,179);}");

    setHP(character->getHP(),character->MaxHP);
    setMP(character->getMP(),character->MaxMP);
    setArmor(character->getShield(),character->MaxShield);
    ui->money->setNum(coin);
    update();

    transitionAnimation();
    map->continueGame(number,character);
    QTimer::singleShot(2000,media->findChild<QMediaPlayer*>("地牢"),&QMediaPlayer::play);

}

void GameWidget::updateGame()
{
    characters->changeWeaponAngle(characters->getMyCharacter());
    monsters->changeWeaponAngle();
    //bullets->updateBullets();
}

void GameWidget::dealMonsterShoot(MonsterBullet *bullet)
{
    connect(bullet, &Bullet::sendPosition, hitBarrier, &HitBarrierThread::judge);
    connect(bullet, &Bullet::sendPosition, hitCharacter, &HitCharacterThread::judge);
    bullet->moveStart();
}

void GameWidget::dealCharacterShoot(Character *character, Bullet* bullet)
{
    setMP(character->getMP(),character->MaxMP);
    connect(bullet, &Bullet::sendPosition, hitBarrier, &HitBarrierThread::judge);
    connect(bullet, &Bullet::sendPosition, hitMonster, &HitMonsterThread::judge);
    bullet->moveStart();
}

void GameWidget::setHP(int hp, int maxHp)
{
    ui->HP_progressBar->setValue(hp);
    ui->HP_progressBar->setFormat(QString::fromLocal8Bit("%1 / %2").arg(QString::number(hp)).arg(QString::number(maxHp)));
}

void GameWidget::setMP(int mp, int maxMp)
{
    ui->MP_progressBar->setValue(mp);
    ui->MP_progressBar->setFormat(QString::fromLocal8Bit("%1 / %2").arg(QString::number(mp)).arg(QString::number(maxMp)));
}

void GameWidget::setArmor(int armor, int maxArmor)
{
    ui->Armor_progressBar->setValue(armor);
    ui->Armor_progressBar->setFormat(QString::fromLocal8Bit("%1 / %2").arg(QString::number(armor)).arg(QString::number(maxArmor)));
}

QVector<QString> GameWidget::getInformation()
{
    QVector<QString> gameInformation =   QVector<QString>();
    int ID,weaponID;
    QString character,level,HP,MP,money,weapon;
    character = characters->getMyCharacter()->name;
    if(character == "战士") ID = 1;
    else ID = 0;
    level = QString::number(map->number);
    HP = QString::number(characters->getMyCharacter()->getHP());
    MP = QString::number(characters->getMyCharacter()->getMP());
    money = QString::number(characters->getMyCharacter()->getCoin());
    weapon = characters->getMyCharacter()->getWeapon()->name;
    if(weapon == "枪") weaponID = 1;
    else weaponID = 0;
    gameInformation.append(QString::number(ID));
    gameInformation.append(level);
    gameInformation.append(QString::number(this->gameTime));
    gameInformation.append(HP);
    gameInformation.append(MP);
    gameInformation.append(money);
    gameInformation.append(QString::number(weaponID));
    return  gameInformation;
}



QStringList GameWidget::getFileNames(const QString& path)
{
    QDir dir(path);
    QStringList nameFilters;
    nameFilters << "*.jpg" << "*.png";
    QStringList files = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
    return files;
}


//暂停界面
void GameWidget::dealStopGame()
{
    stop->show();
}

void GameWidget::dealContinueGame()
{
    QTimer* timer = findChild<QTimer*>("updateTimer");
    timer->start();
    QTimer* timer2 = findChild<QTimer*>("gameTime");
    timer2->start();
    characters->continueGame();
    map->continueGame();
    this->isRunning = true;
}

//返回设置界面
void GameWidget::dealSetting() {
    ui->PushButton_Parse->close();
    ui->Charactor->close();
    ui->HP_label->close();
    ui->MP_label->close();
    ui->Armor_label->close();
    ui->HP_progressBar->close();
    ui->MP_progressBar->close();
    ui->Armor_progressBar->close();
    setting->init();
    setting->show();
}

//
void GameWidget::dealFinished() {
    setting->close();
    ui->PushButton_Parse->show();
    ui->Charactor->show();
    ui->HP_label->show();
    ui->MP_label->show();
    ui->Armor_label->show();
    ui->HP_progressBar->show();
    ui->MP_progressBar->show();
    ui->Armor_progressBar->show();
}

void GameWidget::dealChangeBGMvolume(int volume)
{
    media->setBGMVolume(volume);
}

void GameWidget::dealChangeSoundVolume(int volume)
{
    media->setSoundVolume(volume);
}

void GameWidget::dealChangeWeapon(Weapon* oldWeapon)
{

}

void GameWidget::changeView(QPoint center)
{
    QPropertyAnimation* pPosAnimation1 = new QPropertyAnimation(map, "geometry");
    connect(pPosAnimation1, &QPropertyAnimation::finished, pPosAnimation1, &QPropertyAnimation::deleteLater);
    pPosAnimation1->setDuration(500);
    pPosAnimation1->setStartValue(QRect(map->x(), map->y(), map->width(), map->height()));
    pPosAnimation1->setEndValue(QRect(-center.x() + width() / 2, -center.y() + height() / 2,
                                      map->width(), map->height()));
    pPosAnimation1->setEasingCurve(QEasingCurve::OutQuad);
    pPosAnimation1->start();
}

StopWidget *GameWidget::getStop()
{
    return stop;
}

void GameWidget::keyPressEvent(QKeyEvent* e)
{
    if(isRunning)
    {
        char ch = e->text().at(0).toUpper().toLatin1();
        char W = setting->keys["上移"].at(0).toUpper().toLatin1();
        char S = setting->keys["下移"].at(0).toUpper().toLatin1();
        char A = setting->keys["左移"].at(0).toUpper().toLatin1();
        char D = setting->keys["右移"].at(0).toUpper().toLatin1();
        char E = setting->keys["拾取"].at(0).toUpper().toLatin1();
        char P = setting->keys["购买"].at(0).toUpper().toLatin1();
        char V = setting->keys["进入"].at(0).toUpper().toLatin1();
        char F = setting->keys["技能"].at(0).toUpper().toLatin1();
        if(ch==W){
            characters->move(characters->getMyCharacter(), 'W');
        }
        else if(ch==S){
            characters->move(characters->getMyCharacter(), 'S');
        }
        else if(ch==A){
            characters->move(characters->getMyCharacter(), 'A');
        }
        else if(ch==D){
            characters->move(characters->getMyCharacter(), 'D');
        }
        else if(ch==E){
            characters->changeWeapon(characters->getMyCharacter());
        }
        else if(ch==P){
            characters->buyBox(characters->getMyCharacter());
        }
        else if(ch==V){
            characters->victory(characters->getMyCharacter());
        }
        else if(ch==F){
            characters->releaseSkill(characters->getMyCharacter());
        }
    }
}

void GameWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(isRunning)
    {
        char ch = event->text().at(0).toUpper().toLatin1();
        char W = setting->keys["上移"].at(0).toUpper().toLatin1();
        char S = setting->keys["下移"].at(0).toUpper().toLatin1();
        char A = setting->keys["左移"].at(0).toUpper().toLatin1();
        char D = setting->keys["右移"].at(0).toUpper().toLatin1();
        if(ch==W){
            characters->stop(characters->getMyCharacter(), 'W');
        }
        else if(ch==S){
            characters->stop(characters->getMyCharacter(), 'S');
        }
        else if(ch==A){
            characters->stop(characters->getMyCharacter(), 'A');
        }
        else if(ch==D){
            characters->stop(characters->getMyCharacter(), 'D');
        }

    }
}

void GameWidget::mousePressEvent(QMouseEvent* e)
{
    if(isRunning)
    {
        if (e->button() == Qt::LeftButton)
        {
            characters->shoot(characters->getMyCharacter());
            //bullets->updateBullets();
        }
    }
}



void GameWidget::on_PushButton_Parse_released()
{
    QTimer* timer = findChild<QTimer*>("updateTimer");
    timer->stop();
    QTimer* timer2 = findChild<QTimer*>("gameTime");
    timer2->stop();
    characters->stopGame();
    map->stopGame();
    this->isRunning=false;
    emit StopGame();
}

void GameWidget::setMoney(int money)
{
    ui->money->setText(QString::number(money));
}

void GameWidget::transitionAnimation()
{
    QWidget* widget = new QWidget(this);
    widget->setObjectName("widget");
    widget->resize(this->width(),this->height());
    QLabel* label = new QLabel(widget);
    label->resize(widget->width(),widget->height());
    QMovie* gif = new QMovie(QString("resource\\image\\map\\transition.gif"));
    gif->setParent(label);
    //    connect(gif,&QMovie::stateChanged,this,[=](QMovie::MovieState state){
    //        if(state==QMovie::NotRunning){
    //            gif->start();
    //        }
    //    });
    label->setMovie(gif);
    label->setScaledContents(true);
    gif->start();
    widget->show();
    QTimer::singleShot(2000,this,[=](){
        gif->stop();
        delete gif;
        delete widget;
    });
}
