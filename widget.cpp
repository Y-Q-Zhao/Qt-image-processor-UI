#include "widget.h"
#include "ui_widget.h"

//start**//
#include <QImageReader>
#include <QDebug>
//****end//

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //start**//
    ui->label->setScaledContents(true);//使图像填充满label
    //****end//
}

Widget::~Widget()
{
    delete ui;
}

//读取按钮
void Widget::on_pushButton_clicked()
{
    //start**//
    //打开文件对话窗口，获取图片路径
    QString filename=QFileDialog::getOpenFileName(this,"open file name","./","*.bmp *.png");
    //读取图片
    QImageReader reader(filename);
    reader.setAutoTransform(true);
    const QImage newImage=reader.read();
    image_main=newImage;//用来存储图像供其他处理图像的函数使用

    //在ui界面label窗口中显示图片
    ui->label->setPixmap(QPixmap::fromImage(newImage));
    ui->label_2->setText("读取图像成功");
    //****end//
}

//保存按钮
void Widget::on_pushButton_2_clicked()
{
    //start**//
    QString filename=QFileDialog::getSaveFileName(this,"Save Image","./","*.png");//该函数有点问题，无法自动添加后缀
    if(filename!=NULL)
    {
        filename.append(".png");
        image_main.save(filename);
        ui->label_2->setText("保存图像成功");
    }
    else
    {
        ui->label_2->setText("请选择文件保存地址");
    }
    //****end//
}

//图像均衡化
void Widget::on_pushButton_3_clicked()
{
    //start**//
    int nWidth=image_main.width();
    int nHeight=image_main.height();
    QImage* grayImg=new QImage(nWidth,nHeight,QImage::Format_Grayscale8);
    QRgb rgbVal=0;
    int grayVal=0;
    int img_mat[nHeight][nWidth];

    //获得灰度矩阵//
    for(int x=0;x<nWidth;++x)
    {
        for(int y=0;y<nHeight;++y)
        {
            rgbVal=image_main.pixel(x,y);
            grayVal=qGray(rgbVal);//此时grayVal的值为int型，能进行像素操作
            img_mat[y][x]=grayVal;//讲图像灰度值提取到了二维数组img_mat中，便于处理
        }
    }
    //图像均衡化操作//
    double nk[256]={0};
    double cdf[256]={0};
    int equaled_mat[nHeight][nWidth];
    for(int x=0;x<nWidth;++x)//灰度直方图
    {
        for(int y=0;y<nHeight;++y)
        {
            int index=img_mat[y][x];
            nk[index]+=1;
        }
    }
    double count=0;
    for(int i=0;i<256;i++)//累积直方图
    {
        count=count+nk[i];
        double n=nWidth*nHeight;
        cdf[i]=(double)count/n;
    }
    for(int x=0;x<nWidth;++x)//灰度直方图
    {
        for(int y=0;y<nHeight;++y)
        {
            int index=img_mat[y][x];
            equaled_mat[y][x]=cdf[index]*255;
        }
    }
    //显示图片//
    for(int x=0;x<nWidth;++x)
    {
        for(int y=0;y<nHeight;++y)
        {
            grayVal=equaled_mat[y][x];
            grayImg->setPixel(x,y,QColor(grayVal,grayVal,grayVal).rgb());//将数值转化为rgb图片
        }
    }
    ui->label->setPixmap(QPixmap::fromImage(*grayImg));
    ui->label_2->setText("图像已均衡化处理");

    image_main=*grayImg;//讲均衡化后的图像传递给类参数image_main便于进行保存操作
    //****end//
}

//对数变换
void Widget::on_pushButton_4_clicked()
{
    //start**//
    int nWidth=image_main.width();
    int nHeight=image_main.height();
    QImage rgb_image(nWidth,nHeight,QImage::Format_RGB888);
    QRgb rgbVal=0;
    float log_r[nHeight][nWidth];
    float log_g[nHeight][nWidth];
    float log_b[nHeight][nWidth];
    float max_r=0,max_g=0,max_b=0;

    //建立映射表
    float pixels[256];
    for(int i=0;i<256;i++)
    {
        pixels[i]=log(1+i);
    }

    QString nString=ui->lineEdit->text();
    if(nString==NULL)
    {
        ui->label_2->setText("请输入参数");
    }
    else
    {
        qDebug()<<nString;
        //映射
        for(int x=0;x<nWidth;++x)
        {
            for(int y=0;y<nHeight;++y)
            {
                rgbVal=image_main.pixel(x,y);
                int img_r=qRed(rgbVal);
                int img_g=qGreen(rgbVal);
                int img_b=qBlue(rgbVal);
                log_r[y][x]=pixels[img_r];
                log_g[y][x]=pixels[img_g];
                log_b[y][x]=pixels[img_b];

                if(log_r[y][x]>=max_r)
                {
                    max_r=log_r[y][x];
                }
                if(log_g[y][x]>=max_g)
                {
                    max_g=log_g[y][x];
                }
                if(log_b[y][x]>=max_b)
                {
                    max_b=log_b[y][x];
                }
            }
        }
        //归一化
        for(int x=0;x<nWidth;++x)
        {
            for(int y=0;y<nHeight;++y)
            {
                log_r[y][x]=(int)(log_r[y][x]*255/max_r);
                log_g[y][x]=(int)(log_g[y][x]*255/max_g);
                log_b[y][x]=(int)(log_b[y][x]*255/max_b);
                rgb_image.setPixel(x,y,QColor(log_r[y][x],log_g[y][x],log_b[y][x]).rgb());
            }
        }
        ui->label->setPixmap(QPixmap::fromImage(rgb_image));
        ui->label_2->setText("图像已对数变换处理");
        image_main=rgb_image;
    }
    //****end//
}
