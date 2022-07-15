#include "mediacontroller.h"

MediaController::MediaController(QWidget *parent) : QWidget(parent)
{
   QMediaPlayer *dungeonBGM = new QMediaPlayer(this,QMediaPlayer::VideoSurface);
   dungeonBGM->setMedia(QUrl("qrc:/resource/media/dungeonBGM.mp3"));
   dungeonBGM->setObjectName("地牢");
   connect(dungeonBGM,&QMediaPlayer::stateChanged,this,[=](QMediaPlayer::State state){
       qDebug()<<state;
       if(state==QMediaPlayer::StoppedState){
           dungeonBGM->play();
       }
   });
   BGM.push_back(dungeonBGM);

   QMediaPlayer *shot = new QMediaPlayer(this,QMediaPlayer::VideoSurface);
   shot->setMedia(QUrl("resource/media/music/shot.mp3"));
   shot->setObjectName("枪声");
   sounds.push_back(shot);

   QMediaPlayer *monsterShot = new QMediaPlayer(this,QMediaPlayer::VideoSurface);
   monsterShot->setMedia(QUrl("resource/media/music/monsterShot.mp3"));
   monsterShot->setObjectName("怪物枪声");
   sounds.push_back(monsterShot);

   QMediaPlayer *clickButton = new QMediaPlayer(this,QMediaPlayer::VideoSurface);
   clickButton->setMedia(QUrl("resource/media/music/clickButton.mp3"));
   clickButton->setObjectName("点击按钮");
   sounds.push_back(clickButton);

   QMediaPlayer *buy = new QMediaPlayer(this,QMediaPlayer::VideoSurface);
   buy->setMedia(QUrl("resource/media/music/buy.mp3"));
   buy->setObjectName("购买");
   sounds.push_back(buy);

   QMediaPlayer *hit = new QMediaPlayer(this,QMediaPlayer::VideoSurface);
   hit->setMedia(QUrl("resource/media/music/hit.mp3"));
   hit->setObjectName("击中");
   sounds.push_back(hit);

   QMediaPlayer *nextLevel = new QMediaPlayer(this,QMediaPlayer::VideoSurface);
   nextLevel->setMedia(QUrl("resource/media/music/nextLevel.mp3"));
   nextLevel->setObjectName("进入下一关");
   sounds.push_back(nextLevel);

   QMediaPlayer *changeGun = new QMediaPlayer(this,QMediaPlayer::VideoSurface);
   changeGun->setMedia(QUrl("resource/media/music/nextLevel.mp3"));
   changeGun->setObjectName("换枪");
   sounds.push_back(changeGun);

   QMediaPlayer *getIntoHouse = new QMediaPlayer(this,QMediaPlayer::VideoSurface);
   getIntoHouse->setMedia(QUrl("resource/media/music/getIntoHouse.mp3"));
   getIntoHouse->setObjectName("进入房间");
   sounds.push_back(getIntoHouse);

   QMediaPlayer *get = new QMediaPlayer(this,QMediaPlayer::VideoSurface);
   get->setMedia(QUrl("resource/media/music/getCoin.mp3"));
   get->setObjectName("得到");
   sounds.push_back(get);

}


void MediaController::setBGMVolume(int volume)
{
    for (int i = 0;i<BGM.length();i++) {
        BGM[i]->setVolume(volume);
    }
}

void MediaController::setSoundVolume(int volume)
{
    for (int i = 0;i<sounds.length();i++) {
        sounds[i]->setVolume(volume);
    }
}
