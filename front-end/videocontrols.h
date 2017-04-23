#ifndef VIDEOCONTROLS_H
#define VIDEOCONTROLS_H

#include <QWidget>
#include <QApplication>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QToolButton>
#include <QPixmap>
#include <QIcon>
#include <QGraphicsDropShadowEffect>

class VideoControls : public QWidget
{
    Q_OBJECT

public:
    VideoControls(QWidget *parent = 0);
    ~VideoControls();

    void DisplayVideoControls();

private:
    QToolButton *BackButton;
    QToolButton *PlayButton;
    QToolButton *PauseButton;

    QGroupBox *gb_controls;
    QGraphicsDropShadowEffect *textEffect_controls;
    QHBoxLayout *horizontalLayout_controls;
    QHBoxLayout *mainLayout_controls;
};

#endif // VIDEOCONTROLS_H
