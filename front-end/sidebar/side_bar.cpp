#include "side_bar.h"

Side_bar::Side_bar(QWidget *parent) :
    QWidget(parent)
{
    v_main = new QVBoxLayout;
    v_main_scroll_area = new QVBoxLayout;
    mainWidget = new QWidget(this);
    main_scroll_area = new QScrollArea(this);
    QFont f( "Arial", 8, QFont::Normal);
    this->setFont(f);
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

Side_bar::~Side_bar()
{

}
