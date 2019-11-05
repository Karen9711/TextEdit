#ifndef SHOWWIDGET_H
#define SHOWWIDGET_H

#include <QWidget>
#include<QLabel>
#include<QTextEdit>
#include<QImage>
#include<QLayout>
class showWidget : public QWidget
{
    Q_OBJECT

public:
    explicit showWidget(QWidget *parent = nullptr);
    QImage img;
    QLabel *imgLabel;
    QTextEdit *text;

signals:

public slots:
};

#endif // SHOWWIDGET_H
