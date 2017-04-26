#include "side_bar.h"

SideBar::SideBar(QWidget *parent) :
    QWidget(parent)
{
    v_main = new QVBoxLayout;
    v_main_scroll_area = new QVBoxLayout;
    mainWidget = new QWidget(this);
    mainWidget->setObjectName("mainWidget");
    this->setStyleSheet("#mainWidget {background: rgb(200, 200, 200);}");
    main_scroll_area = new QScrollArea(this);
//    QFont f("Arial", 8, QFont::Normal);
//    this->setFont(f);
    this->setLayout(v_main_scroll_area);
    v_main_scroll_area->addWidget(main_scroll_area);
    main_scroll_area->setWidget(mainWidget);
    main_scroll_area->setWidgetResizable(true);
    mainWidget->setLayout(v_main);

    inputMethod = new InputMethodWidget(mainWidget);
    v_main->addWidget(inputMethod);
    gaussianBlur = new GaussianBlurWidget(mainWidget);
    v_main->addWidget(gaussianBlur);
    lsd = new LSDWidget(mainWidget);
    v_main->addWidget(lsd);
    regionGrow = new RegionGrowWidget(mainWidget);
    v_main->addWidget(regionGrow);
    curveFit = new CurveFitWidget(mainWidget);
    v_main->addWidget(curveFit);
    decisionMake = new DecisionMakeWidget(mainWidget);
    v_main->addWidget(decisionMake);
}

void SideBar::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

SideBar::~SideBar()
{

}
