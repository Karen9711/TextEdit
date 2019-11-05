#ifndef IMGPROCESSOR_H
#define IMGPROCESSOR_H

#include <QMainWindow>
#include<QMenuBar>
#include<QMenu>
#include<QToolBar>
#include<QAction>
#include<QActionGroup>
#include<QList>
#include<QLabel>
#include<QComboBox>
#include<QFontComboBox>
#include<QToolButton>
#include<QFileDialog>
#include<QTextStream>
#include<QtPrintSupport/QPrinter>
#include<QtPrintSupport/QPrintDialog>
#include<QPainter>
#include<QMatrix>
#include<QTextDocument>
#include<QTextCursor>
#include<QTextCharFormat>
#include<QTextBlock>
#include<QTextBlockFormat>
#include<QTextList>
#include<QColor>
#include<QColorDialog>
#include"showwidget.h"
QT_BEGIN_NAMESPACE
namespace Ui { class ImgProcessor; }
QT_END_NAMESPACE

class ImgProcessor : public QMainWindow
{
    Q_OBJECT

public:
    ImgProcessor(QWidget *parent = nullptr);
    ~ImgProcessor();
    void createActions();
    void createMenus();
    void createToolBars();
    void loadFile(QString filename);
    void mergeFormat(QTextCharFormat &format);
private:
    showWidget *showW; //中心部件
    QString filename;
    QImage img;           //放置在中心部件左边的图片

    QMenu *fileMenu;                           		//各项菜单栏 文件 编辑 旋转 镜像
    QMenu *zoomMenu;
    QMenu *rotateMenu;
    QMenu *mirrorMenu;


    //文件菜单项
    QAction *openFileAction; //打开
    QAction *newFileAction; //新建
    QAction *PrintTextAction; //打印文本
    QAction *PrintImageAction; //打印图片
    QAction *exitAction; //退出程序

    //编辑菜单项
    QAction *copyAction; //复制
    QAction *cutAction; //剪切
    QAction *pasteAction; //粘贴
    QAction *aboutAction; //关于
    QAction *zoomInAction; //放大
    QAction *zoomOutAction; //缩小

    //旋转菜单项
    QAction *rotate90Action;
    QAction *rotate180Action;
    QAction *rotate270Action;

    //镜像菜单项
    QAction *mirrorVerticalAction;
    QAction *mirrorHorizontalAction;
    //撤销和重做 动作
    QAction *undoAction;
    QAction *redoAction;

    //工具栏
    QToolBar *fileTool;
    QToolBar *zoomTool;
    QToolBar *rotateTool;
    QToolBar *doToolBar;
    QToolBar *fontToolBar; //字体
    QToolBar *listToolBar; //排序工具条

    //排序设置项
    QLabel *listLabel;
    QComboBox *listComboBox;
    QActionGroup *actGrp;
    QAction *leftAction;   //左对齐
    QAction *rightAction;  //右对齐
    QAction *centerAction; //居中对齐
    QAction *justifyAction;//两边对齐

    //字体设置项
    QLabel *fontLabel1;
    QFontComboBox *fontComboBox;

    QLabel *fontLabel2;
    QComboBox *sizeComboBox;
    QToolButton *boldBtn;
    QToolButton *italicBtn;
    QToolButton *underlineBtn;
    QToolButton *colorBtn;

private:
    Ui::ImgProcessor *ui;
protected slots:
    void ShowNewFile();  //显示新建文档 对话框
    void ShowOpenFile(); //显示打开文档 对话框
    void ShowPrintText(); //显示打印文档对话框
    void ShowPrintImage(); //显示打印图片对话框
    void ShowZoomIn(); //放大
    void ShowZoomOut(); //缩小
    void ShowRotate90();
    void ShowRotate180();
    void ShowRotate270();
    void ShowMirrorVertical();
    void ShowMirrorHorizontal();
    void ShowFontComboBox(QString comboStr);
    void ShowSizeSpinBox(QString spinValue);
    void ShowBoldBtn();
    void ShowItalicBtn();
    void ShowUnderlineBtn();
    void ShowColorBtn();
    void ShowCurrentFormatChanged(const QTextCharFormat &fmt);
    void ShowList(int);
    void ShowAlignment(QAction *act);
    void ShowCursorPositionChanged();
};
#endif // IMGPROCESSOR_H
