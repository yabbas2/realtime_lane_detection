#ifndef SIDE_BAR_H
#define SIDE_BAR_H

#include <QWidget>
#include <QDebug>
#include <QVBoxLayout>
#include <QFont>
#include <QScrollArea>
#include <QScrollBar>
#include <QStyle>
#include <QStyleOption>
#include <QPainter>
#include <QPaintEvent>
#include "curvefitwidget.h"
#include "decisionmakewidget.h"
#include "gaussianblurwidget.h"
#include "inputmethodwidget.h"
#include "lsdwidget.h"
#include "regiongrowwidget.h"

class SideBar : public QWidget
{
    Q_OBJECT

public:
    explicit SideBar(QWidget *parent = 0);
    ~SideBar();
    InputMethodWidget *inputMethod;
    CurveFitWidget *curveFit;
    DecisionMakeWidget *decisionMake;
    GaussianBlurWidget *gaussianBlur;
    LSDWidget *lsd;
    RegionGrowWidget *regionGrow;

private:
    QWidget * mainWidget;
    QScrollArea *main_scroll_area;
    QVBoxLayout *v_main;
    QVBoxLayout *v_main_scroll_area;
    void paintEvent(QPaintEvent *);
};

#endif // SIDE_BAR_H
