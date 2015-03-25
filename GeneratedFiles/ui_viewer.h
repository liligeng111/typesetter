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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_viewerClass
{
public:
    QWidget *centralWidget;
    QPushButton *renderButton;
    QPlainTextEdit *plainTextEdit;
    QLabel *label;
    QCheckBox *spaceBorderButton;
    QCheckBox *wordBorderButton;
    QCheckBox *charBorderButton;
    QCheckBox *pageBorderButton;
    QCheckBox *lineBorderButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *viewerClass)
    {
        if (viewerClass->objectName().isEmpty())
            viewerClass->setObjectName(QStringLiteral("viewerClass"));
        viewerClass->resize(897, 642);
        centralWidget = new QWidget(viewerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        renderButton = new QPushButton(centralWidget);
        renderButton->setObjectName(QStringLiteral("renderButton"));
        renderButton->setGeometry(QRect(70, 560, 75, 23));
        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(20, 410, 211, 121));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(200, 0, 47, 13));
        spaceBorderButton = new QCheckBox(centralWidget);
        spaceBorderButton->setObjectName(QStringLiteral("spaceBorderButton"));
        spaceBorderButton->setGeometry(QRect(30, 30, 82, 17));
        wordBorderButton = new QCheckBox(centralWidget);
        wordBorderButton->setObjectName(QStringLiteral("wordBorderButton"));
        wordBorderButton->setGeometry(QRect(30, 70, 82, 17));
        charBorderButton = new QCheckBox(centralWidget);
        charBorderButton->setObjectName(QStringLiteral("charBorderButton"));
        charBorderButton->setGeometry(QRect(30, 50, 82, 17));
        pageBorderButton = new QCheckBox(centralWidget);
        pageBorderButton->setObjectName(QStringLiteral("pageBorderButton"));
        pageBorderButton->setGeometry(QRect(30, 110, 82, 17));
        lineBorderButton = new QCheckBox(centralWidget);
        lineBorderButton->setObjectName(QStringLiteral("lineBorderButton"));
        lineBorderButton->setGeometry(QRect(30, 90, 82, 17));
        viewerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(viewerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 897, 21));
        viewerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(viewerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        viewerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(viewerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        viewerClass->setStatusBar(statusBar);

        retranslateUi(viewerClass);

        QMetaObject::connectSlotsByName(viewerClass);
    } // setupUi

    void retranslateUi(QMainWindow *viewerClass)
    {
        viewerClass->setWindowTitle(QApplication::translate("viewerClass", "Typesetter", 0));
        renderButton->setText(QApplication::translate("viewerClass", "Render", 0));
        plainTextEdit->setPlainText(QApplication::translate("viewerClass", "The quick brown fox jumps over the lazy dog.", 0));
        label->setText(QApplication::translate("viewerClass", "TextLabel", 0));
        spaceBorderButton->setText(QApplication::translate("viewerClass", "Space Border", 0));
        wordBorderButton->setText(QApplication::translate("viewerClass", "Word Border", 0));
        charBorderButton->setText(QApplication::translate("viewerClass", "Char Border", 0));
        pageBorderButton->setText(QApplication::translate("viewerClass", "Page Border", 0));
        lineBorderButton->setText(QApplication::translate("viewerClass", "Line Border", 0));
    } // retranslateUi

};

namespace Ui {
    class viewerClass: public Ui_viewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWER_H
