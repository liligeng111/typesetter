/********************************************************************************
** Form generated from reading UI file 'viewer.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEWER_H
#define UI_VIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_viewerClass
{
public:
    QAction *actionOpen_File;
    QWidget *centralWidget;
    QPushButton *renderButton;
    QLabel *label;
    QCheckBox *spaceBorderButton;
    QCheckBox *wordBorderButton;
    QCheckBox *charBorderButton;
    QCheckBox *pageBorderButton;
    QCheckBox *lineBorderButton;
    QCheckBox *riverButton;
    QSpinBox *fontSizeBox;
    QLabel *label_2;
    QComboBox *paperBox;
    QDoubleSpinBox *riverThresholdSpinBox;
    QLabel *label_3;
    QSlider *pageSlider;
    QComboBox *expansionBox;
    QDoubleSpinBox *expansionSpinBox;
    QLabel *label_4;
    QLabel *label_5;
    QDoubleSpinBox *magicGainSpinBox;
    QCheckBox *useMagicBox;
    QDoubleSpinBox *magicSpinBox;
    QLabel *label_6;
    QComboBox *markdownBox;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *viewerClass)
    {
        if (viewerClass->objectName().isEmpty())
            viewerClass->setObjectName(QStringLiteral("viewerClass"));
        viewerClass->resize(258, 595);
        actionOpen_File = new QAction(viewerClass);
        actionOpen_File->setObjectName(QStringLiteral("actionOpen_File"));
        centralWidget = new QWidget(viewerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        renderButton = new QPushButton(centralWidget);
        renderButton->setObjectName(QStringLiteral("renderButton"));
        renderButton->setGeometry(QRect(20, 520, 75, 23));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(200, 0, 47, 13));
        spaceBorderButton = new QCheckBox(centralWidget);
        spaceBorderButton->setObjectName(QStringLiteral("spaceBorderButton"));
        spaceBorderButton->setGeometry(QRect(30, 10, 82, 17));
        spaceBorderButton->setChecked(true);
        wordBorderButton = new QCheckBox(centralWidget);
        wordBorderButton->setObjectName(QStringLiteral("wordBorderButton"));
        wordBorderButton->setGeometry(QRect(30, 50, 82, 17));
        wordBorderButton->setChecked(true);
        charBorderButton = new QCheckBox(centralWidget);
        charBorderButton->setObjectName(QStringLiteral("charBorderButton"));
        charBorderButton->setGeometry(QRect(30, 30, 82, 17));
        charBorderButton->setChecked(true);
        pageBorderButton = new QCheckBox(centralWidget);
        pageBorderButton->setObjectName(QStringLiteral("pageBorderButton"));
        pageBorderButton->setGeometry(QRect(30, 90, 82, 17));
        pageBorderButton->setChecked(true);
        lineBorderButton = new QCheckBox(centralWidget);
        lineBorderButton->setObjectName(QStringLiteral("lineBorderButton"));
        lineBorderButton->setGeometry(QRect(30, 70, 82, 17));
        lineBorderButton->setChecked(true);
        riverButton = new QCheckBox(centralWidget);
        riverButton->setObjectName(QStringLiteral("riverButton"));
        riverButton->setGeometry(QRect(30, 110, 0, 0));
        fontSizeBox = new QSpinBox(centralWidget);
        fontSizeBox->setObjectName(QStringLiteral("fontSizeBox"));
        fontSizeBox->setGeometry(QRect(90, 190, 60, 20));
        fontSizeBox->setMaximum(1000);
        fontSizeBox->setValue(12);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 190, 50, 20));
        paperBox = new QComboBox(centralWidget);
        paperBox->setObjectName(QStringLiteral("paperBox"));
        paperBox->setGeometry(QRect(30, 150, 69, 22));
        riverThresholdSpinBox = new QDoubleSpinBox(centralWidget);
        riverThresholdSpinBox->setObjectName(QStringLiteral("riverThresholdSpinBox"));
        riverThresholdSpinBox->setGeometry(QRect(110, 150, 0, 0));
        riverThresholdSpinBox->setValue(2);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(50, 150, 0, 0));
        pageSlider = new QSlider(centralWidget);
        pageSlider->setObjectName(QStringLiteral("pageSlider"));
        pageSlider->setGeometry(QRect(180, 10, 30, 391));
        pageSlider->setPageStep(10);
        pageSlider->setValue(0);
        pageSlider->setOrientation(Qt::Vertical);
        pageSlider->setInvertedAppearance(true);
        expansionBox = new QComboBox(centralWidget);
        expansionBox->setObjectName(QStringLiteral("expansionBox"));
        expansionBox->setGeometry(QRect(30, 260, 69, 22));
        expansionSpinBox = new QDoubleSpinBox(centralWidget);
        expansionSpinBox->setObjectName(QStringLiteral("expansionSpinBox"));
        expansionSpinBox->setGeometry(QRect(90, 310, 62, 22));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(30, 290, 100, 16));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 370, 191, 21));
        magicGainSpinBox = new QDoubleSpinBox(centralWidget);
        magicGainSpinBox->setObjectName(QStringLiteral("magicGainSpinBox"));
        magicGainSpinBox->setGeometry(QRect(90, 400, 62, 22));
        magicGainSpinBox->setMaximum(10000);
        magicGainSpinBox->setSingleStep(10);
        magicGainSpinBox->setValue(0);
        useMagicBox = new QCheckBox(centralWidget);
        useMagicBox->setObjectName(QStringLiteral("useMagicBox"));
        useMagicBox->setGeometry(QRect(20, 490, 70, 17));
        magicSpinBox = new QDoubleSpinBox(centralWidget);
        magicSpinBox->setObjectName(QStringLiteral("magicSpinBox"));
        magicSpinBox->setGeometry(QRect(90, 460, 62, 22));
        magicSpinBox->setMaximum(20);
        magicSpinBox->setSingleStep(0.1);
        magicSpinBox->setValue(0);
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 430, 191, 21));
        markdownBox = new QComboBox(centralWidget);
        markdownBox->setObjectName(QStringLiteral("markdownBox"));
        markdownBox->setGeometry(QRect(30, 110, 69, 22));
        viewerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(viewerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 258, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        viewerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(viewerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        viewerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(viewerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        viewerClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionOpen_File);

        retranslateUi(viewerClass);

        paperBox->setCurrentIndex(2);
        expansionBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(viewerClass);
    } // setupUi

    void retranslateUi(QMainWindow *viewerClass)
    {
        viewerClass->setWindowTitle(QApplication::translate("viewerClass", "Typesetter", 0));
        actionOpen_File->setText(QApplication::translate("viewerClass", "Open File", 0));
        renderButton->setText(QApplication::translate("viewerClass", "Render", 0));
        label->setText(QString());
        spaceBorderButton->setText(QApplication::translate("viewerClass", "Space Border", 0));
        wordBorderButton->setText(QApplication::translate("viewerClass", "Word Border", 0));
        charBorderButton->setText(QApplication::translate("viewerClass", "Char Border", 0));
        pageBorderButton->setText(QApplication::translate("viewerClass", "Page Border", 0));
        lineBorderButton->setText(QApplication::translate("viewerClass", "Line Border", 0));
        riverButton->setText(QApplication::translate("viewerClass", "Show River", 0));
        label_2->setText(QApplication::translate("viewerClass", "Font Size:", 0));
        paperBox->clear();
        paperBox->insertItems(0, QStringList()
         << QApplication::translate("viewerClass", "A2", 0)
         << QApplication::translate("viewerClass", "A3", 0)
         << QApplication::translate("viewerClass", "A4", 0)
         << QApplication::translate("viewerClass", "A5", 0)
         << QApplication::translate("viewerClass", "A6", 0)
         << QApplication::translate("viewerClass", "A7", 0)
         << QApplication::translate("viewerClass", "B2", 0)
         << QApplication::translate("viewerClass", "B3", 0)
         << QApplication::translate("viewerClass", "B4", 0)
         << QApplication::translate("viewerClass", "B5", 0)
         << QApplication::translate("viewerClass", "B6", 0)
         << QApplication::translate("viewerClass", "B7", 0)
        );
        label_3->setText(QApplication::translate("viewerClass", "Threshold:", 0));
        expansionBox->clear();
        expansionBox->insertItems(0, QStringList()
         << QApplication::translate("viewerClass", "Font First", 0)
         << QApplication::translate("viewerClass", "Uniform", 0)
        );
        label_4->setText(QApplication::translate("viewerClass", "Max Expansion:", 0));
        label_5->setText(QApplication::translate("viewerClass", "show Magic Edge with gain >", 0));
        useMagicBox->setText(QApplication::translate("viewerClass", "Use Magic!", 0));
        label_6->setText(QApplication::translate("viewerClass", "Max amount magic allowed:", 0));
        markdownBox->clear();
        markdownBox->insertItems(0, QStringList()
         << QApplication::translate("viewerClass", "Demerits", 0)
         << QApplication::translate("viewerClass", "r", 0)
        );
        menuFile->setTitle(QApplication::translate("viewerClass", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class viewerClass: public Ui_viewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWER_H
