#include "showwidget.h"

showWidget::showWidget(QWidget *parent) : QWidget(parent)
{
    imgLabel = new QLabel;
    imgLabel->setScaledContents(true);
    text = new QTextEdit;
    QHBoxLayout *main = new QHBoxLayout(this);
    main->addWidget(imgLabel);
    main->addWidget(text);
}
