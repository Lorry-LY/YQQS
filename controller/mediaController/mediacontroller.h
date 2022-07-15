#ifndef MEDIACONTROLLER_H
#define MEDIACONTROLLER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QSound>

class MediaController : public QWidget
{
    Q_OBJECT
public:
    explicit MediaController(QWidget *parent = nullptr);


private:
    QVector<QMediaPlayer*> BGM;
    QVector<QMediaPlayer*> sounds;

public:
    void setBGMVolume(int volume);
    void setSoundVolume(int volume);

};

#endif // MEDIACONTROLLER_H
