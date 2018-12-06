#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

//start**//
#include <QFileDialog>
#include <QString>
#include <QLabel>
#include <QImageReader>
#include <QScreen>
#include <QGuiApplication>
#include <QColor>
#include <math.h>
//****end//

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Widget *ui;

    //start**//
    //定义一些类内参数
    QImage image_main;
    //****end//
};

#endif // WIDGET_H
