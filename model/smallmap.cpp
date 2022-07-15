#include "smallmap.h"
#include <QDebug>
#include <QTime>
#include <QTimer>
#include "model/magicpoint.h"
#include "controller/LocationGame/monstercontroller.h"
#include "controller/LocationGame/charactercontroller.h"
#include "model/coin.h"

SmallMap::SmallMap(QWidget *parent) : QWidget(parent)
{
    db= QSqlDatabase::database("QSQLITE");
    resize(1350,1350);
    for (int i=0;i<45;i++) {
        for(int j=0;j<45;j++){
            elements[i][j] = new Element(this);
            elements[i][j]->setGeometry(j*30,i*30,30,30);
            elements[i][j]->setObjectName("barrier");
        }
    }

}

void SmallMap::init()
{
    QList<MonsterCharacter*> characters = findChildren<MonsterCharacter*>("",Qt::FindDirectChildrenOnly);
    while(!characters.isEmpty())
    {
        MonsterCharacter* character = characters.front();
        characters.pop_front();
        delete character;
    }
    QList<Coin*> coins = findChildren<Coin*>("",Qt::FindDirectChildrenOnly);
    while(!coins.isEmpty())
    {
        Coin* coin = coins.front();
        coins.pop_front();
        delete coin;
    }
    QList<MagicPoint*> magics = findChildren<MagicPoint*>("",Qt::FindDirectChildrenOnly);
    while(!magics.isEmpty())
    {
        MagicPoint* magic = magics.front();
        magics.pop_front();
        delete magic;
    }
    QList<QLabel*> labels = findChildren<QLabel*>("",Qt::FindDirectChildrenOnly);
    while(!labels.isEmpty())
    {
        QLabel* label = labels.front();
        labels.pop_front();
        delete label;
    }
    QLabel* patrol = findChild<QLabel*>("传送门");
    if (patrol) {
        
        delete patrol;
    }
    this->type = SmallMap::NONE;
    for (int i=0;i<45;i++) {
        for(int j=0;j<45;j++){
            elements[i][j]->update(Element::NONE);
        }
    }
}

void SmallMap::update(int type)
{
    //    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    //    width = height = 5;
    this->type = SmallMap::_type(type);
    switch (SmallMap::_type(type)) {
    case SmallMap::NONE:{
        break;
    }
    case SmallMap::START:{
        while((width = qrand()%20)<10);
        while((height = qrand()%20)<10);
        buildWall();
        buildFlat();
        break;
    }
    case SmallMap::FIGHT:{
        while((width = qrand()%21)<15);
        while((height = qrand()%21)<15);
        buildWall();
        buildFlat();
        buildBarriers();
        break;
    }
    case SmallMap::SHOP:{
        width = 10;
        height = 11;
        buildWall();
        buildFlat();
        buildShop();
        break;
    }
    case SmallMap::MAGIC:{
        while((width = qrand()%8)<5);
        while((height = qrand()%8)<5);
        buildWall();
        buildFlat();
        buildMagic();
        break;
    }
    case SmallMap::END:{
        while((width = qrand()%20)<10);
        while((height = qrand()%20)<10);
        buildWall();
        buildFlat();
        buildPortal();
        break;
    }
    case SmallMap::NORMAL:{
        while((width = qrand()%20)<10);
        while((height = qrand()%20)<10);
        buildWall();
        buildFlat();
        buildBox();
        break;
    }
    }
}

void SmallMap::stopGame()
{
    QList<MonsterCharacter*> characters = findChildren<MonsterCharacter*>("",Qt::FindDirectChildrenOnly);
    while(!characters.isEmpty())
    {
        MonsterCharacter* character = characters.front();
        characters.pop_front();
        character->stopGame();
    }
    QList<Coin*> coins = findChildren<Coin*>("",Qt::FindDirectChildrenOnly);
    while(!coins.isEmpty())
    {
        Coin* coin = coins.front();
        coins.pop_front();
        coin->stopGame();
    }
    QList<MagicPoint*> magics = findChildren<MagicPoint*>("",Qt::FindDirectChildrenOnly);
    while(!magics.isEmpty())
    {
        MagicPoint* magic = magics.front();
        magics.pop_front();
        magic->stopGame();
    }
}

void SmallMap::continueGame()
{
    QList<MonsterCharacter*> characters = findChildren<MonsterCharacter*>("",Qt::FindDirectChildrenOnly);
    while(!characters.isEmpty())
    {
        MonsterCharacter* character = characters.front();
        characters.pop_front();
        character->continueGame();
    }
    QList<Coin*> coins = findChildren<Coin*>("",Qt::FindDirectChildrenOnly);
    while(!coins.isEmpty())
    {
        Coin* coin = coins.front();
        coins.pop_front();
        coin->continueGame();
    }
    QList<MagicPoint*> magics = findChildren<MagicPoint*>("",Qt::FindDirectChildrenOnly);
    while(!magics.isEmpty())
    {
        MagicPoint* magic = magics.front();
        magics.pop_front();
        magic->continueGame();
    }
}

void SmallMap::createMonster()
{
    QSqlQuery query = QSqlQuery(db);
    int ID = qrand()% 3;
    ID+=2;
    query.exec(QString("select c.'名称',c.'生命值',c.'能量',c.'护盾',c.'金币',c.'移动速度',c.'身体图片'"
                       ",w.'名称',w.'威力',w.'能量消耗',w.'攻击速度',w.'暴击率',w.'类型',w.URL,w.'子弹图片',w.'子弹速度' "
                       "from '角色' c,'武器' w "
                       "where c.'初始武器ID' = w.ID and c.ID = %1").arg(ID));
    query.next();
    QString name = query.value("名称").toString();
    int HP = query.value("生命值").toInt();
    int MP = query.value("能量").toInt();
    int shield = query.value("护盾").toInt();
    int coin = query.value("金币").toInt();
    float speed = query.value("移动速度").toDouble();
    QString bodyURL = query.value("身体图片").toString();
    QString weaponName = query.value("名称").toString();
    int ATK = query.value("威力").toInt();
    int dMP = query.value("能量消耗").toInt();
    float crit = query.value("暴击率").toFloat();
    float Aspeed = query.value("攻击速度").toFloat();
    int type = query.value("类型").toInt();
    QString weaponURL = query.value("URL").toString();
    QString bulletURL = query.value("子弹图片").toString();
    float bulletSpeed = query.value("子弹速度").toFloat();
    Weapon* weapon = new Weapon(nullptr, weaponName, ATK, dMP, Aspeed, crit, type, weaponURL, bulletURL, bulletSpeed);
    MonsterCharacter* monster = new MonsterCharacter(this, name, HP, MP, shield, coin, bodyURL, weapon, speed);
    monster->move(createPoint());
    weapon->setParent(monster);
    monster->show();
}

bool SmallMap::isEnter(QPoint point)
{
    QPoint topLeft((22-width+1)*30,(22-height+1)*30);
    QPoint rightBottom((22+width-1)*30,(22+height-1)*30);
    if(topLeft.x()<point.x()&&point.x()<rightBottom.x()&&topLeft.y()<point.y()&&point.y()<rightBottom.y())
    {
        return true;
    }
    else
    {
        return false;
    }
}

QPoint SmallMap::getPoint(QPoint point)
{
    return elements[point.y()/30][point.x()/30]->getPoint();
    //    qDebug()<<point.y()/30<<point.x()/30<<Element::_type(type);
}

Element::_type SmallMap::getState_isMove(QPoint point)
{
    QPoint point1(point.y() / 30, point.x() / 30);
    int type = 0;
    if( point1.x()< 45 &&  point1.y()< 45)
        type=elements[point1.x()][point1.y()]->getState_isMove();
    //qDebug()<<point.y()/30<<point.x()/30<<Element::_type(type);
    return Element::_type(type);
}

Element::_type SmallMap::getState_isShoot(QPoint point)
{
    QPoint point1(point.y() / 30, point.x() / 30);
    int type = 0;
    if( point1.x()< 45 &&  point1.y()< 45)
        type=elements[point1.x()][point1.y()]->getState_isShoot();
    //qDebug()<<point.y()/30<<point.x()/30<<Element::_type(type);
    return Element::_type(type);
}

void SmallMap::buildRoad(int way)
{
    int centerX = 45/2,centerY = 45/2;
    switch (way){
    case 0:{
        for (int i = centerX+height;i < 45;i++) {
            elements[i][centerY-3]->update(Element::BARRIER);
            elements[i][centerY+3]->update(Element::BARRIER);
            for(int j = centerY - 3 + 1;j<centerY+3;j++)
            {
                elements[i][j]->update(Element::FLAT);
            }
        }
        for(int j = centerY - 3 + 1;j<centerY+3;j++)
        {
            elements[centerX+height][j]->update(Element::DOOR);
        }
        break;
    }
    case 1:{
        for (int i = centerY+width;i < 45;i++) {
            elements[centerX-3][i]->update(Element::BARRIER);
            elements[centerX+3][i]->update(Element::BARRIER);
            for(int j = centerX - 3 + 1;j<centerX+3;j++)
            {
                elements[j][i]->update(Element::FLAT);
            }
        }
        for(int j = centerX - 3 + 1;j<centerX+3;j++)
        {
            elements[j][centerY+width]->update(Element::DOOR);
        }
        break;
    }
    case 2:{
        for (int i = centerY-width;i >= 0;i--) {
            elements[centerX-3][i]->update(Element::BARRIER);
            elements[centerX+3][i]->update(Element::BARRIER);
            for(int j = centerX - 3 + 1;j<centerX+3;j++)
            {
                elements[j][i]->update(Element::FLAT);
            }
        }
        for(int j = centerX - 3 + 1;j<centerX+3;j++)
        {
            elements[j][centerY-width]->update(Element::DOOR);
        }
        break;
    }
    case 3:{
        for (int i = centerX-height;i >= 0;i--) {
            elements[i][centerY-3]->update(Element::BARRIER);
            elements[i][centerY+3]->update(Element::BARRIER);
            for(int j = centerY - 3 + 1;j<centerY+3;j++)
            {
                elements[i][j]->update(Element::FLAT);
            }
        }
        for(int j = centerY - 3 + 1;j<centerY+3;j++)
        {
            elements[centerX-height][j]->update(Element::DOOR);
        }
        break;
    }
    }
}

void SmallMap::buildWall()
{
    int centerX = 45/2,centerY = 45/2;
    for(int i = centerX-width;i<=centerX+width;i++)elements[centerY-height][i]->update(Element::BARRIER);
    for(int i = centerX-width;i<=centerX+width;i++)elements[centerY+height][i]->update(Element::BARRIER);
    for(int i = centerY-height;i<=centerY+height;i++)elements[i][centerX-width]->update(Element::BARRIER);
    for(int i = centerY-height;i<=centerY+height;i++)elements[i][centerX+width]->update(Element::BARRIER);
}

void SmallMap::buildFlat()
{
    int centerX = 45/2,centerY = 45/2;
    for(int i = centerX-width + 1;i < centerX+width;i++)
    {
        for(int j = centerY-height + 1;j < centerY+height;j++)
        {
            elements[j][i]->update(Element::FLAT);
        }
    }
}

//商店
void SmallMap::buildShop()
{
    int centerX = 45/2,centerY = 45/2;
    for(int i = centerX-3;i<=centerX+4;i++){
        for (int j = centerY-5;j <= centerY+4;j++) {
            elements[i][j]->setType(Element::BARRIER);
        }
    }
    //商人
    QLabel* businessman = new QLabel(this);
    businessman->setGeometry(510,560,300,200);
    businessman->setStyleSheet("border-image: url(resource/image/character/businessman.png);");
    businessman->show();
    //箱子
    QLabel* Box1 = new QLabel(this);
    Box1->setGeometry(540,760,60,60);
    Box1->setStyleSheet("border-image: url(resource/image/map/shopBox.png);");
    Box1->show();
    QLabel* Box2 = new QLabel(this);
    Box2->setGeometry(690,760,60,60);
    Box2->setStyleSheet("border-image: url(resource/image/map/shopBox.png);");
    Box2->show();
    //物品
    Box* HPBigBox = new Box(this,10,4,0);
    HPBigBox->setGeometry(555,765,30,30);
    HPBigBox->setStyleSheet("border-image: url(resource/image/things/HPBox.png);");
    HPBigBox->show();
    Box* MPBigBox = new Box(this,5,0,80);
    MPBigBox->setGeometry(705,765,30,30);
    MPBigBox->setStyleSheet("border-image: url(resource/image/things/MPBox.png);");
    MPBigBox->show();
}

//传送门
void SmallMap::buildPortal()
{
    QLabel* portal = new QLabel(this);
    portal->setGeometry(600,480,150,400);
    portal->setStyleSheet("border-image: url(resource/image/maps/portal1.png);");
    portal->show();
    portal->setObjectName("传送门");
    QTimer *tim = new QTimer(portal);
    tim->setObjectName("timerPortal");
    tim->setInterval(200);
    connect(tim,&QTimer::timeout,this,&SmallMap::onTimeOut);
    tim->start();

}

//蓝矿
void SmallMap::buildMagic()
{
    int centerX = 45/2,centerY = 45/2;
    Weapon* weapon = new Weapon(nullptr, "", 2, 5, 2, 0.3, 0, "resource/image/weapon/gun0/guzsdsn.png", "bulletURL", 2);
    MonsterCharacter *magic = new MonsterCharacter(this, "magic", 50, 150,0, 0, "resource/image/enermy/magic", weapon, 0.0);
    magic->move(centerX*30-magic->width()/2+15,centerY*30-magic->height()/2);
    connect(magic,&MonsterCharacter::dead,this,[=](Character* character){
        parentWidget()->parentWidget()->findChild<MonsterController*>("monsters")->dealMonsterDead(character);
    });

}


//战斗房间的障碍物
void SmallMap::buildBarriers()
{
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    int n = qrand() % 5;
    switch (n) {
    case 0:Barriers0();
        break;
    case 1:Barriers1();
        break;
    case 2:Barriers2();
        break;
    case 3:Barriers3();
        break;
    case 4:Barriers4();
        break;
    }
}

void SmallMap::Barriers0()
{
    QString barrierImage = "border-image: url(resource/image/maps/barrier.png);";
    int centerX = 45/2,centerY = 45/2;
    for (int i = centerX-6;i <= centerX+6;i++) {
        elements[i][centerY-6]->setType(Element::BARRIER);
        QLabel *barrier0 = new QLabel(this);
        barrier0->setGeometry((centerY-6)*30,i*30,30,30);
        barrier0->setStyleSheet(barrierImage);
        barrier0->show();
    }
    for (int i = centerX-6;i <= centerX+6;i++) {
        elements[i][centerY+6]->setType(Element::BARRIER);
        QLabel *barrier0 = new QLabel(this);
        barrier0->setGeometry((centerY+6)*30,i*30,30,30);
        barrier0->setStyleSheet(barrierImage);
        barrier0->show();
    }
    for (int i = centerY-5;i <= centerY+5;i++) {
        elements[centerX][i]->setType(Element::BARRIER);
        QLabel *barrier0 = new QLabel(this);
        barrier0->setGeometry(i*30,(centerX)*30,30,30);
        barrier0->setStyleSheet(barrierImage);
        barrier0->show();
    }
}

void SmallMap::Barriers1()
{
    QString barrierImage = "border-image: url(resource/image/maps/barrier.png);";
    int centerX = 45/2,centerY = 45/2;
    for (int i = centerY-5;i <= centerX+8;i++) {
        elements[centerX-8][i]->setType(Element::BARRIER);
        QLabel *barrier0 = new QLabel(this);
        barrier0->setGeometry(i*30,(centerY-8)*30,30,30);
        barrier0->setStyleSheet(barrierImage);
        barrier0->show();
    }
    for (int i = centerX-5;i <= centerX+8;i++) {
        elements[i][centerY-8]->setType(Element::BARRIER);
        QLabel *barrier0 = new QLabel(this);
        barrier0->setGeometry((centerY-8)*30,i*30,30,30);
        barrier0->setStyleSheet(barrierImage);
        barrier0->show();
    }
    for (int i = centerY-7;i <= centerX+5;i++) {
        elements[centerX+8][i]->setType(Element::BARRIER);
        QLabel *barrier0 = new QLabel(this);
        barrier0->setGeometry(i*30,(centerY+8)*30,30,30);
        barrier0->setStyleSheet(barrierImage);
        barrier0->show();
    }
    for (int i = centerX-7;i <= centerX+5;i++) {
        elements[i][centerY+8]->setType(Element::BARRIER);
        QLabel *barrier0 = new QLabel(this);
        barrier0->setGeometry((centerY+8)*30,i*30,30,30);
        barrier0->setStyleSheet(barrierImage);
        barrier0->show();
    }
}

void SmallMap::Barriers2()
{
    QString barrierImage = "border-image: url(resource/image/maps/barrier.png);";
    int centerX = 45/2,centerY = 45/2;
    for (int i = centerY-8;i <= centerY+8;i++) {
        elements[centerX][i]->setType(Element::BARRIER);
        QLabel *barrier0 = new QLabel(this);
        barrier0->setGeometry(i*30,(centerX)*30,30,30);
        barrier0->setStyleSheet(barrierImage);
        barrier0->show();
    }
    for (int i = centerX-10;i <= centerX+10;i++) {
        if (i==centerX) continue;
        elements[i][centerY]->setType(Element::BARRIER);
        QLabel *barrier0 = new QLabel(this);
        barrier0->setGeometry((centerY)*30,i*30,30,30);
        barrier0->setStyleSheet(barrierImage);
        barrier0->show();
    }
}

void SmallMap::Barriers3()
{
    QString barrierImage = "border-image: url(resource/image/maps/tree.png);";
    int centerX = 45/2,centerY = 45/2;
    for (int i = centerX-1;i <= centerX+1;i++) {
        for (int j = centerY-2;j<=centerY+1;j++) {
            elements[i][j]->setType(Element::BARRIER);
        }
    }
    QLabel *barrier0 = new QLabel(this);
    barrier0->setGeometry((centerX-2)*30,(centerY-1)*30,120,90);
    barrier0->setStyleSheet(barrierImage);
    barrier0->show();
}

void SmallMap::Barriers4()
{
    QString barrierImage = "border-image: url(resource/image/maps/barrier.png);";
    int centerX = 45/2,centerY = 45/2;
    for (int i = centerX-this->height+1;i <= centerX+5;i++) {
        elements[i][centerY-5]->setType(Element::BARRIER);
        QLabel *barrier0 = new QLabel(this);
        barrier0->setGeometry((centerY-5)*30,i*30,30,30);
        barrier0->setStyleSheet(barrierImage);
        barrier0->show();
    }
    for (int i = centerX-5;i <= centerX+this->height-1;i++) {
        elements[i][centerY+5]->setType(Element::BARRIER);
        QLabel *barrier0 = new QLabel(this);
        barrier0->setGeometry((centerY+5)*30,i*30,30,30);
        barrier0->setStyleSheet(barrierImage);
        barrier0->show();
    }
}


//宝箱
void SmallMap::buildBox()
{
    int centerX = 45/2,centerY = 45/2;
    for(int i = centerX-1;i<=centerX;i++){
        for (int j = centerY-1;j <= centerY+1;j++) {
            elements[i][j]->setType(Element::BARRIER);
        }
    }
    QLabel *box = new QLabel(this);
    box->setGeometry(645,630,60,60);
    box->setObjectName("宝箱");
    box->setStyleSheet("border-image: url(resource/image/maps/box.png);");
    box->show();

}

void SmallMap::onTimeOut()
{
    QLabel *portal = this->findChild<QLabel*>("传送门");
    if(tag == 1){
        portal->setStyleSheet("border-image: url(resource/image/maps/portal1.png);");
        tag++;
    }else if(tag == 2){
        portal->setStyleSheet("border-image: url(resource/image/maps/portal2.png);");
        tag++;
    }else if(tag == 3){
        portal->setStyleSheet("border-image: url(resource/image/maps/portal3.png);");
        tag++;
    }else if(tag == 4){
        portal->setStyleSheet("border-image: url(resource/image/maps/portal4.png);");
        tag=1;
    }
}



QPoint SmallMap::createPoint()
{
    int dx,dy;
    do{
        QPoint topLeft((22-width+3)*30,(22-height+3)*30);
        QPoint rightBottom((22+width-3)*30,(22+height-3)*30);
        while((dx = qrand()%((22+width-3)*30))<((22-width+3)*30));
        while((dy = qrand()%((22+height-3)*30))<((22-height+3)*30));
    }while(getState_isShoot(QPoint(dx,dy))==Element::_type::BARRIER);
    return QPoint(dx,dy);
}

void SmallMap::closeDoor()
{
    for (int i=0;i<45;i++)
    {
        for(int j=0;j<45;j++)
        {
            if(elements[i][j]->type == Element::_type::DOOR)
            {
                elements[i][j]->setState(Element::_state::CLOSE);
            }
        }
    }
}

void SmallMap::openDoor()
{
    for (int i=0;i<45;i++)
    {
        for(int j=0;j<45;j++)
        {
            if(elements[i][j]->type == Element::_type::DOOR)
            {
                elements[i][j]->setState(Element::_state::OPEN);
            }
        }
    }
}


