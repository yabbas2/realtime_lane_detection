#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QToolButton>
#include <QPixmap>
#include <QIcon>
#include <QGraphicsDropShadowEffect>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void VideoControls();

private:
    Ui::Widget *ui;

    QToolButton *BackButton;
    QToolButton *PlayButton;
    QToolButton *PauseButton;

    QGroupBox *gb_controls;
    QGraphicsDropShadowEffect *textEffect_controls;
    QHBoxLayout *horizontalLayout_controls;
    QHBoxLayout *mainLayout_controls;
};

#endif // WIDGET_H
