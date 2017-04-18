#ifndef SIDE_BAR_H
#define SIDE_BAR_H

#include <QWidget>
#include <QDebug>
#include <QVBoxLayout>
#include <QFont>
#include <QScrollArea>
#include <QScrollBar>
#include "curvefitwidget.h"
#include "decisionmakewidget.h"
#include "gaussianblurwidget.h"
#include "inputmethodwidget.h"
#include "lsdwidget.h"
#include "regiongrowwidget.h"

class Side_bar : public QWidget
{
    Q_OBJECT

public:
    explicit Side_bar(QWidget *parent = 0);
    ~Side_bar();

private:
    QWidget * mainWidget;
    QScrollArea *main_scroll_area;
    QVBoxLayout *v_main;
    QVBoxLayout *v_main_scroll_area;
    CurveFitWidget *curveFit;
    DecisionMakeWidget *decisionMake;
    GaussianBlurWidget *gaussianBlur;
    InputMethodWidget *inputMethod;
    LSDWidget *lsd;
    RegionGrowWidget *regionGrow;
};

#endif // SIDE_BAR_H
