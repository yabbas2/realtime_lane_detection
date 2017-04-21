#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)

{
    QMetaObject::connectSlotsByName(this);
    this->setFixedSize(mainWindowWidth, mainWindowHeight);
    this->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));

    grid_main_layout = new QGridLayout;
    cen1 = new QWidget(this);
    cen2 = new QWidget(this);
    cen1->setFixedSize(this->size());
    cen2->setFixedSize(this->size());

    cen1->setLayout(grid_main_layout);

    sidebar = new Side_bar(cen1);
    sidebar->setFixedSize(sideBarWidth, mainWindowHeight);
    grid_main_layout->addWidget(sidebar, 0, 0);

    mulVidWidget = new MultiVideoViewer(cen1);
    mulVidWidget->setFixedSize(multiVideoWidth, mainWindowHeight);
    mulVidWidget->setVideoSize(200, 200, 200, 200);
    grid_main_layout->addWidget(mulVidWidget, 0, 1);

    fsVidWidget = new fullScreenVideoViewer(cen2);
    fsVidWidget->setFixedSize(mainWindowWidth, mainWindowHeight);
    fsVidWidget->setVideoSize(fsVidWidget->size());

    cen1->setVisible(true);
    cen2->setVisible(false);

//    mEffect = new QGraphicsOpacityEffect(cen1);
//    mEffect->setOpacity(1.0);
//    cen1->setGraphicsEffect(mEffect);

//    wEffect = new QGraphicsOpacityEffect(this);
//    wEffect->setOpacity(0.0);
//    cen2->setGraphicsEffect(wEffect);

    connect(mulVidWidget->getVideoWidget(0), SIGNAL(mouseClicked(int)), this, SLOT(switchToFullScreen()));
    connect(mulVidWidget->getVideoWidget(1), SIGNAL(mouseClicked(int)), this, SLOT(switchToFullScreen()));
    connect(mulVidWidget->getVideoWidget(2), SIGNAL(mouseClicked(int)), this, SLOT(switchToFullScreen()));
    connect(mulVidWidget->getVideoWidget(3), SIGNAL(mouseClicked(int)), this, SLOT(switchToFullScreen()));
}

MultiVideoViewer *MainWindow::getMultiVideoViewerWidget()
{
    return mulVidWidget;
}

fullScreenVideoViewer *MainWindow::getFullScreenVideoViewerWidget()
{
    return fsVidWidget;
}

void MainWindow::switchToFullScreen()
{
//    qDebug() << "to full screen";
//    animation = new QPropertyAnimation(mEffect,"opacity");
//    animation->setDuration(2000);
//    animation->setStartValue(1.0);
//    animation->setEndValue(0.0);
//    animation->setEasingCurve(QEasingCurve::InBack);
//    connect(animation,SIGNAL(finished()),this,SLOT(ToFullScreenAnimationFinish()));
//    animation->start(QAbstractAnimation::DeleteWhenStopped);
    cen1->setVisible(false);
    cen2->setVisible(true);
}

void MainWindow::ToFullScreenAnimationFinish()
{
    qDebug() << "on animation finish";
    cen1->setVisible(false);
    animation2 = new QPropertyAnimation(wEffect,"opacity");
    animation2->setDuration(2000);
    animation2->setStartValue(0.0);
    animation2->setEndValue(1.0);
    animation2->setEasingCurve(QEasingCurve::OutBack);
    animation2->start(QAbstractAnimation::DeleteWhenStopped);
    cen2->setVisible(true);
    wEffect->setOpacity(1.0);
}

MainWindow::~MainWindow()
{

}
