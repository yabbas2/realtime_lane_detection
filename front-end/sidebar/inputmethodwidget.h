#ifndef INPUTMETHOD_H
#define INPUTMETHOD_H

#include <QWidget>
#include <QSlider>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QStringList>
#include <QComboBox>
#include <QPushButton>
#include <QSpacerItem>
#include <QString>
#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QIcon>

#define InputBoxFixedWidth  240
#define InputBoxFixedHeight  110

class InputMethodWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InputMethodWidget(QWidget *parent = 0);

private slots:
    void comboBox(QString item);
    void pause();
    void play();
    void browseVideo();

signals:
    void pauseStreaming();
    void startStreaming();
    void changeVideoSource(QString source);
    void reInit();

private:
    QGroupBox *input_gb;
    QComboBox *input;
    QStringList *input_list;
    QHBoxLayout *h_input;
    QHBoxLayout *h_input_extra_buttons;
    QVBoxLayout *v_input;
    QPushButton *browse_video;
    QPushButton *start_video;
    QPushButton *pause_video;
    QSpacerItem *buttons_spacer;
    QString fileName;
};

#endif // INPUTMETHOD_H
