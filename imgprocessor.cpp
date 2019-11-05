#include "imgprocessor.h"
#include "ui_imgprocessor.h"

ImgProcessor::ImgProcessor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ImgProcessor)
{
    ui->setupUi(this);
    //排序
    listLabel =new QLabel(QString::fromLocal8Bit("排序:"));
    listComboBox =new QComboBox;
    listComboBox->addItem("Standard");
    listComboBox->addItem("QTextListFormat::ListDisc");
    listComboBox->addItem("QTextListFormat::ListCircle");
    listComboBox->addItem("QTextListFormat::ListSquare");
    listComboBox->addItem("QTextListFormat::ListDecimal");
    listComboBox->addItem("QTextListFormat::ListLowerAlpha");
    listComboBox->addItem("QTextListFormat::ListUpperAlpha");
    listComboBox->addItem("QTextListFormat::ListLowerRoman");
    listComboBox->addItem("QTextListFormat::ListUpperRoman");
    void (QComboBox::*listComboActivated)(int) = &QComboBox::activated;
    connect(listComboBox,listComboActivated,this,&ImgProcessor::ShowList);
    //字体
    //设置字体
    fontLabel1 =new QLabel(QString::fromLocal8Bit("字体:"));
    fontComboBox =new QFontComboBox;
    fontComboBox->setFontFilters(QFontComboBox::ScalableFonts);
    void (QFontComboBox::*fontComboActivated)(const QString &) = &QFontComboBox::activated;
    connect(fontComboBox,fontComboActivated,this,&ImgProcessor::ShowFontComboBox);

    fontLabel2 =new QLabel(QString::fromLocal8Bit("字号:"));
    sizeComboBox =new QComboBox;
    QFontDatabase db;
    foreach(int size,db.standardSizes()) sizeComboBox->addItem(QString::number(size));
    void (QComboBox::*sizeBoxActivated)(const QString &) = &QComboBox::activated;
    connect(sizeComboBox,sizeBoxActivated,this,&ImgProcessor::ShowSizeSpinBox);

    //加粗
    boldBtn =new QToolButton;
    boldBtn->setIcon(QIcon(":/pics/bold.png"));
    boldBtn->setCheckable(true);
    connect(boldBtn,&QToolButton::clicked,this,&ImgProcessor::ShowBoldBtn);
    //斜体
    italicBtn =new QToolButton;
    italicBtn->setIcon(QIcon(":/pics/italic.png"));
    italicBtn->setCheckable(true);
    connect(italicBtn,&QToolButton::clicked,this,&ImgProcessor::ShowItalicBtn);
    //下划线
    underlineBtn =new QToolButton;
    underlineBtn->setIcon(QIcon(":/pics/underline.png"));
    underlineBtn->setCheckable(true);
    connect(underlineBtn,&QToolButton::clicked,this,&ImgProcessor::ShowUnderlineBtn);
    //设置颜色
    colorBtn =new QToolButton;
    colorBtn->setIcon(QIcon(":/pics/color.png"));
    colorBtn->setCheckable(true);
    connect(colorBtn,&QToolButton::clicked,this,&ImgProcessor::ShowColorBtn);

    createActions();
    createMenus();
    createToolBars();

    //初始化本窗口

    //设置中心部件
    showW = new showWidget(this);
    //设置标题
    setWindowTitle(QString::fromLocal8Bit("文本编辑器"));
    if(img.load(":/pics/image.png"))
    {
        showW->imgLabel->setPixmap(QPixmap::fromImage(img));
        showW->imgLabel->resize(img.size());
    }
    setCentralWidget(showW);
    connect(showW->text,&QTextEdit::currentCharFormatChanged,this,&ImgProcessor::ShowCurrentFormatChanged);
//    connect(showW->text->document(),&QTextDocument::undoAvailable,undoAction,&QAction::setEnabled);
//    connect(showW->text->document(),&QTextDocument::redoAvailable,redoAction,&QAction::setEnabled);
    connect(showW->text,&QTextEdit::cursorPositionChanged,this,&ImgProcessor::ShowCursorPositionChanged);

}

ImgProcessor::~ImgProcessor()
{
    delete ui;
}

void ImgProcessor::createActions()
{
    //菜单栏 -- 文件菜单项
    //打开 新建 打印文字 打印图片
    openFileAction = new QAction(QIcon(":/pics/open.png"),QString::fromLocal8Bit("打开"),this);
    openFileAction->setShortcut(tr("Ctrl+O"));
    openFileAction->setStatusTip(QString::fromLocal8Bit("打开一个文件"));
    connect(openFileAction,&QAction::triggered,this,&ImgProcessor::ShowOpenFile);

    newFileAction = new QAction(QIcon(":/pics/new.png"),QString::fromLocal8Bit("新建"),this);
    newFileAction->setShortcut(tr("Ctrl+N"));
    newFileAction->setStatusTip(QString::fromLocal8Bit("新建一个文件"));
    connect(newFileAction,&QAction::triggered,this,&ImgProcessor::ShowNewFile);

    PrintTextAction = new QAction(QIcon(":/pics/printText.png"),QString::fromLocal8Bit("打印文本"),this);
    PrintTextAction->setStatusTip(QString::fromLocal8Bit("打印文本文件"));
    connect(PrintTextAction,&QAction::triggered,this,&ImgProcessor::ShowPrintText);

    PrintImageAction = new QAction(QIcon(":/pics/printImage.png"),QString::fromLocal8Bit("打印图像"),this);
    PrintImageAction->setStatusTip(QString::fromLocal8Bit("打印图像文件"));
    connect(PrintImageAction,&QAction::triggered,this,&ImgProcessor::ShowPrintImage);

    exitAction = new QAction(QString::fromLocal8Bit("退出"),this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(QString::fromLocal8Bit("退出程序"));
    connect(exitAction,&QAction::triggered,this,[=](){
        this->close();
    });

    //菜单栏 -- 编辑菜单 项
    copyAction = new QAction(QIcon(":/pics/copy.png"),QString::fromLocal8Bit("复制"),this);
    copyAction->setShortcut(tr("Ctrl+C"));
    copyAction->setStatusTip(QString::fromLocal8Bit("复制文件"));
    connect(copyAction,&QAction::triggered,showW->text,&QTextEdit::copy);

    cutAction = new QAction(QIcon(":/pics/cut.png"),QString::fromLocal8Bit("剪切"),this);
    cutAction->setShortcut(tr("Ctrl+X"));
    cutAction->setStatusTip(QString::fromLocal8Bit("剪切文件"));
    connect(cutAction,&QAction::triggered,showW->text,&QTextEdit::cut);

    pasteAction = new QAction(QIcon(":/pics/paste.png"),QString::fromLocal8Bit("粘贴"),this);
    pasteAction->setShortcut(tr("Ctrl+V"));
    pasteAction->setStatusTip(QString::fromLocal8Bit("粘贴文件"));
    connect(pasteAction,&QAction::triggered,showW->text,&QTextEdit::paste);

    aboutAction = new QAction(QString::fromLocal8Bit("关于"),this);
    connect(aboutAction,&QAction::triggered,this,&QApplication::aboutQt);

    zoomInAction = new QAction(QIcon(":/pics/zoomin.png"),QString::fromLocal8Bit("放大"),this);
    zoomInAction->setStatusTip(QString::fromLocal8Bit("放大图像"));
    connect(zoomInAction,&QAction::triggered,this,&ImgProcessor::ShowZoomIn);

    zoomOutAction = new QAction(QIcon(":/pics/zoomout.png"),QString::fromLocal8Bit("缩小"),this);
    zoomOutAction->setStatusTip(QString::fromLocal8Bit("缩小图像"));
    connect(zoomOutAction,&QAction::triggered,this,&ImgProcessor::ShowZoomOut);

    //菜单栏--- 旋转菜单项
    rotate90Action =new QAction(QIcon(":/pics/rotate90.png"),QString::fromLocal8Bit("旋转90°"),this);
    rotate90Action->setStatusTip(QString::fromLocal8Bit("将一幅图旋转90°"));
    connect(rotate90Action,&QAction::triggered,this,&ImgProcessor::ShowRotate90);

    rotate180Action =new QAction(QIcon(":/pics/rotate180.png"),QString::fromLocal8Bit("旋转180°"),this);
    rotate180Action->setStatusTip(QString::fromLocal8Bit("将一幅图旋转180°"));
    connect(rotate180Action,&QAction::triggered,this,&ImgProcessor::ShowRotate180);

    rotate270Action =new QAction(QIcon(":/pics/rotate270.png"),QString::fromLocal8Bit("旋转270°"),this);
    rotate270Action->setStatusTip(QString::fromLocal8Bit("将一幅图旋转270°"));
    connect(rotate270Action,&QAction::triggered,this,&ImgProcessor::ShowRotate270);

    //菜单栏 镜像菜单项
      //纵向镜像 -- 水平翻转
    mirrorVerticalAction =new QAction(QString::fromLocal8Bit("纵向镜像"),this);
    mirrorVerticalAction->setStatusTip(QString::fromLocal8Bit("对一幅图做纵向镜像"));
    connect(mirrorVerticalAction,&QAction::triggered,this,&ImgProcessor::ShowMirrorVertical);
      //横向镜像 -- 垂直翻转
    mirrorHorizontalAction =new QAction( QString::fromLocal8Bit("横向镜像"),this);
    mirrorHorizontalAction->setStatusTip(QString::fromLocal8Bit("对一幅图做横向镜像"));
    connect(mirrorHorizontalAction,&QAction::triggered,this,&ImgProcessor::ShowMirrorHorizontal);

    //撤销和重做
    undoAction =new QAction(QIcon(":/pics/undo.png"),QString::fromLocal8Bit("撤销"),this);
    undoAction->setStatusTip(QString::fromLocal8Bit("撤销"));
    connect(undoAction,&QAction::triggered,showW->text,&QTextEdit::undo);

    redoAction =new QAction(QIcon(":/pics/redo.png"),QString::fromLocal8Bit("重做"),this);
    redoAction->setStatusTip(QString::fromLocal8Bit("重做"));
    connect(redoAction,&QAction::triggered,showW->text,&QTextEdit::redo);

    //排序：左对齐、右对齐、居中和两端对齐
    actGrp =new QActionGroup(this);
    leftAction =new QAction(QIcon(":/pics/left.png"),"左对齐",actGrp);
    leftAction->setCheckable(true);
    rightAction =new QAction(QIcon(":/pics/right.png"),"右对齐",actGrp);
    rightAction->setCheckable(true);
    centerAction =new QAction(QIcon(":/pics/center.png"),"居中",actGrp);
    centerAction->setCheckable(true);
    justifyAction =new QAction(QIcon(":/pics/justify.png"),"两端对齐",actGrp);
    justifyAction->setCheckable(true);
    connect(actGrp,&QActionGroup::triggered,this,&ImgProcessor::ShowAlignment);
}

void ImgProcessor::createMenus()
{
    QMenuBar *menubar = menuBar();
    //文件菜单项
    fileMenu = menubar->addMenu(QString::fromLocal8Bit("文件"));
    fileMenu->addAction(openFileAction);  //打开
    fileMenu->addAction(newFileAction);    //新建
    fileMenu->addAction(PrintTextAction);  //打印文本
    fileMenu->addAction(PrintImageAction); //打印图像
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);       //退出程序

    //编辑菜单项
    zoomMenu = menubar->addMenu(QString::fromLocal8Bit("编辑"));
    zoomMenu->addAction(copyAction);
    zoomMenu->addAction(cutAction);
    zoomMenu->addAction(pasteAction);
    zoomMenu->addAction(zoomInAction);
    zoomMenu->addAction(zoomOutAction);
    zoomMenu->addSeparator();
    zoomMenu->addAction(aboutAction);

    //旋转菜单项
    rotateMenu = menubar->addMenu(QString::fromLocal8Bit("旋转"));
    rotateMenu->addAction(rotate90Action);
    rotateMenu->addAction(rotate180Action);
    rotateMenu->addAction(rotate270Action);

    //镜像菜单项
    mirrorMenu = menubar->addMenu(QString::fromLocal8Bit("镜像"));
    mirrorMenu->addAction(mirrorVerticalAction);
    mirrorMenu->addAction(mirrorHorizontalAction);
}

void ImgProcessor::createToolBars()
{
    //文件工具条
    fileTool = addToolBar("File");
    fileTool->addAction(openFileAction);
    fileTool->addAction(newFileAction);
    fileTool->addAction(PrintTextAction);
    fileTool->addAction(PrintImageAction);
    fileTool->setMovable(false);
    fileTool->setAllowedAreas(Qt::LeftToolBarArea | Qt::TopToolBarArea);
    //编辑工具条
    zoomTool = addToolBar("Zoom");
    zoomTool->addAction(copyAction);
    zoomTool->addAction(cutAction);
    zoomTool->addAction(pasteAction);
    zoomTool->addAction(zoomInAction);
    zoomTool->addAction(zoomOutAction);
    //旋转工具条
    rotateTool = addToolBar("Rotate");
    rotateTool->addAction(rotate90Action);
    rotateTool->addAction(rotate180Action);
    rotateTool->addAction(rotate270Action);
    //撤销和重做
    doToolBar = addToolBar("doEdit");
    doToolBar->addAction(undoAction);
    doToolBar->addAction(redoAction);
    //字体
    fontToolBar = addToolBar("Font");
    fontToolBar->addWidget(fontLabel1);
    fontToolBar->addWidget(fontComboBox);
    fontToolBar->addWidget(fontLabel2);
    fontToolBar->addWidget(sizeComboBox);
    fontToolBar->addWidget(boldBtn);
    fontToolBar->addWidget(italicBtn);
    fontToolBar->addWidget(underlineBtn);
    fontToolBar->addWidget(colorBtn);
    //排序
    listToolBar = addToolBar("List");
    listToolBar->addWidget(listLabel);
    listToolBar->addWidget(listComboBox);
    listToolBar->addSeparator();
    listToolBar->addActions(actGrp->actions());
}

void ImgProcessor::ShowNewFile()  //显示新建文档 对话框
{
    ImgProcessor *imgprocessor = new ImgProcessor;
    imgprocessor->show();
}
void ImgProcessor::ShowOpenFile()
{
    //窗口中已有打开文件 -- 在新窗口打开
    filename = QFileDialog::getOpenFileName(this);
    if(!filename.isEmpty())
    {
        if(showW->text->document()->isEmpty())
        {
            loadFile(filename);

        }
        else{
            ImgProcessor *imgprocessor = new ImgProcessor;
            imgprocessor->show();
            imgprocessor->loadFile(filename);
        }
    }
    //窗口中空白 -- 在当前窗口打开
}

void ImgProcessor::loadFile(QString filename)
{
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QTextStream textStream(&file);
        while(!textStream.atEnd())
        {
            showW->text->append(textStream.readLine());
        }
    }
}

void ImgProcessor::ShowPrintText()
{
    QPrinter printer;
    QPrintDialog printD(&printer,this);
    if(printD.exec()){
        QTextDocument *doc = showW->text->document();
        doc->print(&printer);
    }
}
void ImgProcessor::ShowPrintImage()
{
    QPrinter printer;
    QPrintDialog printD(&printer,this);
    if(printD.exec())
    {
        QPainter painter(&printer);
        QRect rect = painter.viewport();
        QSize size  = img.size();
        size.scale(rect.size(),Qt::KeepAspectRatio);
        painter.setViewport(rect.x(),rect.y(),size.width(),size.height());
        painter.setWindow(img.rect());
        painter.drawImage(0,0,img);

    }
}

void ImgProcessor::ShowZoomIn()
{
    if(img.isNull())
        return;
    QMatrix matrix;
    matrix.scale(2,2);
    img = img.transformed(matrix);
    showW->imgLabel->setPixmap(QPixmap::fromImage(img));

}
void ImgProcessor::ShowZoomOut()
{
    if(img.isNull())
        return;
    QMatrix matrix;
    matrix.scale(0.5,0.5);
    img = img.transformed(matrix);
    showW->imgLabel->setPixmap(QPixmap::fromImage(img));

}

void ImgProcessor::ShowRotate90()
{
    if(img.isNull())
        return;
    QMatrix matrix;
    matrix.rotate(90);
    img = img.transformed(matrix);
    showW->imgLabel->setPixmap(QPixmap::fromImage(img));

}
void ImgProcessor::ShowRotate180()
{
    if(img.isNull())
        return;
    QMatrix matrix;
    matrix.rotate(180);
    img = img.transformed(matrix);
    showW->imgLabel->setPixmap(QPixmap::fromImage(img));

}
void ImgProcessor::ShowRotate270()
{
    if(img.isNull())
        return;
    QMatrix matrix;
    matrix.rotate(270);
    img = img.transformed(matrix);
    showW->imgLabel->setPixmap(QPixmap::fromImage(img));

}

void ImgProcessor::ShowMirrorVertical()
{
    if(img.isNull())
        return;
    img = img.mirrored(false,true);
    showW->imgLabel->setPixmap(QPixmap::fromImage(img));
}
void ImgProcessor::ShowMirrorHorizontal()
{
    if(img.isNull())
        return;
    img = img.mirrored(true,false);
    showW->imgLabel->setPixmap(QPixmap::fromImage(img));
}

void ImgProcessor::mergeFormat(QTextCharFormat &format)
{
    QTextCursor cursor = showW->text->textCursor();
    if(!cursor.hasSelection())
    {
        cursor.select(QTextCursor::WordUnderCursor);
    }
    cursor.mergeCharFormat(format);
    showW->text->mergeCurrentCharFormat(format);
}

void ImgProcessor::ShowFontComboBox(QString comboStr)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(comboStr);
    mergeFormat(fmt);
}


void ImgProcessor::ShowSizeSpinBox(QString spinValue)
{
    QTextCharFormat fmt;
    fmt.setFontPointSize(spinValue.toFloat());
    showW->text->mergeCurrentCharFormat(fmt);
}

void ImgProcessor::ShowCurrentFormatChanged(const QTextCharFormat &fmt)
{
    int fontComboIndex = fontComboBox->findText(fmt.fontFamily());
    int sizeComboIndex = sizeComboBox->findText(QString::number(fmt.fontPointSize()));
    fontComboBox->setCurrentIndex(fontComboIndex);
    sizeComboBox->setCurrentIndex(sizeComboIndex);
    boldBtn->setChecked(fmt.font().bold());
    italicBtn->setChecked(fmt.font().italic());
    underlineBtn->setChecked(fmt.font().underline());
}
void ImgProcessor::ShowBoldBtn()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(boldBtn->isChecked()?QFont::Bold:QFont::Normal);
    showW->text->mergeCurrentCharFormat(fmt);

}
void ImgProcessor::ShowItalicBtn()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(italicBtn->isChecked());
    showW->text->mergeCurrentCharFormat(fmt);
}
void ImgProcessor::ShowUnderlineBtn()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(underlineBtn->isChecked());
    showW->text->mergeCurrentCharFormat(fmt);
}

void ImgProcessor::ShowColorBtn()
{
    QColor color = QColorDialog::getColor(Qt::black,this);
    if(color.isValid())
    {
        QTextCharFormat fmt;
        fmt.setForeground(color);
        showW->text->setCurrentCharFormat(fmt);
    }
}

void ImgProcessor::ShowList(int index)
{
    QTextCursor cursor = showW->text->textCursor();
    if(index!=0)
    {
        QTextListFormat::Style style = QTextListFormat::ListDisc;
        switch(index)
        {
        default:
        case 1:
            style = QTextListFormat::ListDisc;break;
        case 2:
            style = QTextListFormat::ListCircle;break;
        case 3:
            style = QTextListFormat::ListSquare;break;
        case 4:
            style = QTextListFormat::ListDecimal;break;
        case 5:
            style = QTextListFormat::ListLowerAlpha;break;
        case 6:
            style = QTextListFormat::ListUpperAlpha;break;
        case 7:
            style = QTextListFormat::ListLowerRoman;break;
        case 8:
            style = QTextListFormat::ListUpperRoman;break;
        }
        cursor.beginEditBlock();
        QTextBlockFormat blockFmt = cursor.blockFormat();
        QTextListFormat listFmt;
        if(cursor.currentList())
        {
            QTextList *curList= cursor.currentList();
            listFmt = curList->format();
        }
        else{
            listFmt.setIndent(blockFmt.indent()+1);
            blockFmt.setIndent(0);
            cursor.setBlockFormat(blockFmt);

        }
        listFmt.setStyle(style);
        cursor.createList(listFmt);
        cursor.endEditBlock();
    }
    else{
        QTextBlockFormat bfmt;
        bfmt.setObjectIndex(-1);
        cursor.mergeBlockFormat(bfmt);
    }

}
void ImgProcessor::ShowAlignment(QAction *act)
{
    if(act==leftAction)
    {
        showW->text->setAlignment(Qt::AlignLeft);
    }
    if(act==rightAction)
    {
        showW->text->setAlignment(Qt::AlignRight);
    }
    if(act==centerAction)
    {
        showW->text->setAlignment(Qt::AlignCenter);
    }
    if(act==justifyAction)
    {
        showW->text->setAlignment(Qt::AlignJustify);
    }
}
void ImgProcessor::ShowCursorPositionChanged()
{
    int aligment = showW->text->alignment();
    if(aligment==Qt::AlignLeft)
    {
        leftAction->setChecked(true);
    }
    if(aligment==Qt::AlignRight)
    {
        rightAction->setChecked(true);
    }
    if(aligment==Qt::AlignCenter)
    {
        centerAction->setChecked(true);
    }
    if(aligment==Qt::AlignJustify)
    {
        justifyAction->setChecked(true);
    }
}
