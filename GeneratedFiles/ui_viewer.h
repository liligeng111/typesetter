/********************************************************************************
** Form generated from reading UI file 'viewer.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
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
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_viewerClass
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSave_As;
    QAction *actionExit;
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionAbout;
    QAction *actionTypeset;
    QAction *actionDemerits;
    QAction *actionR;
    QAction *actionPenalty;
    QAction *actionSpaceR;
    QAction *actionFontR;
    QAction *actionLetterSpaceR;
    QAction *actionAuto_Typeset;
    QAction *actionCommit;
    QAction *actionPrevious;
    QAction *actionNext;
    QAction *actionUse_Magic;
    QAction *actionPick_Shrink_Collor;
    QAction *actionPink_Stretch_Color;
    QAction *actionShow_Only_One_Suggestion;
    QWidget *centralWidget;
    QLabel *label;
    QCheckBox *riverButton;
    QDoubleSpinBox *riverThresholdSpinBox;
    QLabel *label_3;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuFile_2;
    QMenu *menuEdit;
    QMenu *menuCAbout;
    QMenu *menuView;
    QMenu *menuMarkdown;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *viewerClass)
    {
        if (viewerClass->objectName().isEmpty())
            viewerClass->setObjectName(QStringLiteral("viewerClass"));
        viewerClass->resize(1034, 790);
        QIcon icon;
        icon.addFile(QStringLiteral(":/viewer/images/Prorgrams.png"), QSize(), QIcon::Normal, QIcon::Off);
        viewerClass->setWindowIcon(icon);
        actionNew = new QAction(viewerClass);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/viewer/images/TextDocument.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew->setIcon(icon1);
        actionOpen = new QAction(viewerClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/viewer/images/Opened.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon2);
        actionSave = new QAction(viewerClass);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/viewer/images/Floppy.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon3);
        actionSave_As = new QAction(viewerClass);
        actionSave_As->setObjectName(QStringLiteral("actionSave_As"));
        actionSave_As->setIcon(icon3);
        actionExit = new QAction(viewerClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/viewer/images/Close.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon4);
        actionCut = new QAction(viewerClass);
        actionCut->setObjectName(QStringLiteral("actionCut"));
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/viewer/images/CUt.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCut->setIcon(icon5);
        actionCopy = new QAction(viewerClass);
        actionCopy->setObjectName(QStringLiteral("actionCopy"));
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/viewer/images/Scanner.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCopy->setIcon(icon6);
        actionPaste = new QAction(viewerClass);
        actionPaste->setObjectName(QStringLiteral("actionPaste"));
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/viewer/images/Printer.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPaste->setIcon(icon7);
        actionAbout = new QAction(viewerClass);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionAbout->setIcon(icon);
        actionTypeset = new QAction(viewerClass);
        actionTypeset->setObjectName(QStringLiteral("actionTypeset"));
        actionTypeset->setCheckable(false);
        actionTypeset->setChecked(false);
        QIcon icon8;
        icon8.addFile(QStringLiteral(":/viewer/images/Run.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionTypeset->setIcon(icon8);
        actionDemerits = new QAction(viewerClass);
        actionDemerits->setObjectName(QStringLiteral("actionDemerits"));
        actionDemerits->setCheckable(true);
        actionDemerits->setChecked(true);
        actionR = new QAction(viewerClass);
        actionR->setObjectName(QStringLiteral("actionR"));
        actionR->setCheckable(true);
        actionPenalty = new QAction(viewerClass);
        actionPenalty->setObjectName(QStringLiteral("actionPenalty"));
        actionPenalty->setCheckable(true);
        actionSpaceR = new QAction(viewerClass);
        actionSpaceR->setObjectName(QStringLiteral("actionSpaceR"));
        actionSpaceR->setCheckable(true);
        actionFontR = new QAction(viewerClass);
        actionFontR->setObjectName(QStringLiteral("actionFontR"));
        actionFontR->setCheckable(true);
        actionLetterSpaceR = new QAction(viewerClass);
        actionLetterSpaceR->setObjectName(QStringLiteral("actionLetterSpaceR"));
        actionLetterSpaceR->setCheckable(true);
        actionAuto_Typeset = new QAction(viewerClass);
        actionAuto_Typeset->setObjectName(QStringLiteral("actionAuto_Typeset"));
        actionAuto_Typeset->setCheckable(true);
        actionCommit = new QAction(viewerClass);
        actionCommit->setObjectName(QStringLiteral("actionCommit"));
        QIcon icon9;
        icon9.addFile(QStringLiteral(":/viewer/images/Up.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCommit->setIcon(icon9);
        actionPrevious = new QAction(viewerClass);
        actionPrevious->setObjectName(QStringLiteral("actionPrevious"));
        QIcon icon10;
        icon10.addFile(QStringLiteral(":/viewer/images/Backward.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrevious->setIcon(icon10);
        actionNext = new QAction(viewerClass);
        actionNext->setObjectName(QStringLiteral("actionNext"));
        QIcon icon11;
        icon11.addFile(QStringLiteral(":/viewer/images/Forward.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionNext->setIcon(icon11);
        actionUse_Magic = new QAction(viewerClass);
        actionUse_Magic->setObjectName(QStringLiteral("actionUse_Magic"));
        actionUse_Magic->setCheckable(true);
        actionPick_Shrink_Collor = new QAction(viewerClass);
        actionPick_Shrink_Collor->setObjectName(QStringLiteral("actionPick_Shrink_Collor"));
        actionPink_Stretch_Color = new QAction(viewerClass);
        actionPink_Stretch_Color->setObjectName(QStringLiteral("actionPink_Stretch_Color"));
        actionShow_Only_One_Suggestion = new QAction(viewerClass);
        actionShow_Only_One_Suggestion->setObjectName(QStringLiteral("actionShow_Only_One_Suggestion"));
        actionShow_Only_One_Suggestion->setCheckable(true);
        centralWidget = new QWidget(viewerClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(200, 0, 47, 13));
        riverButton = new QCheckBox(centralWidget);
        riverButton->setObjectName(QStringLiteral("riverButton"));
        riverButton->setGeometry(QRect(30, 110, 0, 0));
        riverThresholdSpinBox = new QDoubleSpinBox(centralWidget);
        riverThresholdSpinBox->setObjectName(QStringLiteral("riverThresholdSpinBox"));
        riverThresholdSpinBox->setGeometry(QRect(110, 150, 0, 0));
        riverThresholdSpinBox->setValue(2);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(50, 150, 0, 0));
        viewerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(viewerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1034, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuFile_2 = new QMenu(menuBar);
        menuFile_2->setObjectName(QStringLiteral("menuFile_2"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuCAbout = new QMenu(menuBar);
        menuCAbout->setObjectName(QStringLiteral("menuCAbout"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuMarkdown = new QMenu(menuView);
        menuMarkdown->setObjectName(QStringLiteral("menuMarkdown"));
        viewerClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(viewerClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        viewerClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(viewerClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        viewerClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile_2->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuCAbout->menuAction());
        menuFile->addAction(actionAuto_Typeset);
        menuFile->addAction(actionUse_Magic);
        menuFile->addSeparator();
        menuFile->addAction(actionPick_Shrink_Collor);
        menuFile->addAction(actionPink_Stretch_Color);
        menuFile->addAction(actionShow_Only_One_Suggestion);
        menuFile_2->addAction(actionNew);
        menuFile_2->addAction(actionOpen);
        menuFile_2->addAction(actionSave);
        menuFile_2->addAction(actionSave_As);
        menuFile_2->addSeparator();
        menuFile_2->addAction(actionExit);
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPaste);
        menuCAbout->addAction(actionAbout);
        menuView->addAction(menuMarkdown->menuAction());
        menuMarkdown->addAction(actionDemerits);
        menuMarkdown->addAction(actionR);
        menuMarkdown->addAction(actionPenalty);
        menuMarkdown->addAction(actionSpaceR);
        menuMarkdown->addAction(actionFontR);
        menuMarkdown->addAction(actionLetterSpaceR);
        mainToolBar->addAction(actionNew);
        mainToolBar->addAction(actionOpen);
        mainToolBar->addAction(actionSave);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionTypeset);
        mainToolBar->addAction(actionCommit);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionPrevious);
        mainToolBar->addAction(actionNext);

        retranslateUi(viewerClass);
        QObject::connect(actionNew, SIGNAL(triggered()), viewerClass, SLOT(newFile()));
        QObject::connect(actionExit, SIGNAL(triggered()), viewerClass, SLOT(close()));
        QObject::connect(actionOpen, SIGNAL(triggered()), viewerClass, SLOT(open()));
        QObject::connect(actionSave, SIGNAL(triggered()), viewerClass, SLOT(save()));
        QObject::connect(actionSave_As, SIGNAL(triggered()), viewerClass, SLOT(saveAs()));
        QObject::connect(actionAbout, SIGNAL(triggered()), viewerClass, SLOT(about()));
        QObject::connect(actionTypeset, SIGNAL(triggered()), viewerClass, SLOT(typeset()));
        QObject::connect(actionR, SIGNAL(toggled(bool)), viewerClass, SLOT(setMarkdownR(bool)));
        QObject::connect(actionDemerits, SIGNAL(toggled(bool)), viewerClass, SLOT(setMarkdownDemerits(bool)));
        QObject::connect(actionPenalty, SIGNAL(toggled(bool)), viewerClass, SLOT(setMarkdownPenalty(bool)));
        QObject::connect(actionSpaceR, SIGNAL(toggled(bool)), viewerClass, SLOT(setMarkdownSpaceR(bool)));
        QObject::connect(actionFontR, SIGNAL(toggled(bool)), viewerClass, SLOT(setMarkdownFontR(bool)));
        QObject::connect(actionLetterSpaceR, SIGNAL(toggled(bool)), viewerClass, SLOT(setMarkdownLetterSpaceR(bool)));
        QObject::connect(actionAuto_Typeset, SIGNAL(toggled(bool)), viewerClass, SLOT(auto_typeset(bool)));
        QObject::connect(actionCommit, SIGNAL(triggered()), viewerClass, SLOT(commit()));
        QObject::connect(actionPrevious, SIGNAL(triggered()), viewerClass, SLOT(previous()));
        QObject::connect(actionNext, SIGNAL(triggered()), viewerClass, SLOT(next()));
        QObject::connect(actionUse_Magic, SIGNAL(toggled(bool)), viewerClass, SLOT(use_magic(bool)));
        QObject::connect(actionPick_Shrink_Collor, SIGNAL(triggered()), viewerClass, SLOT(pick_shrink_color()));
        QObject::connect(actionPink_Stretch_Color, SIGNAL(triggered()), viewerClass, SLOT(pick_stretch_color()));
        QObject::connect(actionShow_Only_One_Suggestion, SIGNAL(toggled(bool)), viewerClass, SLOT(show_only_one_suggestion(bool)));

        QMetaObject::connectSlotsByName(viewerClass);
    } // setupUi

    void retranslateUi(QMainWindow *viewerClass)
    {
        viewerClass->setWindowTitle(QApplication::translate("viewerClass", "Typesetter", 0));
#ifndef QT_NO_STATUSTIP
        viewerClass->setStatusTip(QString());
#endif // QT_NO_STATUSTIP
        actionNew->setText(QApplication::translate("viewerClass", "New", 0));
#ifndef QT_NO_TOOLTIP
        actionNew->setToolTip(QApplication::translate("viewerClass", "Create a new file", 0));
#endif // QT_NO_TOOLTIP
        actionNew->setShortcut(QApplication::translate("viewerClass", "Ctrl+N", 0));
        actionOpen->setText(QApplication::translate("viewerClass", "Open", 0));
#ifndef QT_NO_TOOLTIP
        actionOpen->setToolTip(QApplication::translate("viewerClass", "Open an existing file", 0));
#endif // QT_NO_TOOLTIP
        actionOpen->setShortcut(QApplication::translate("viewerClass", "Ctrl+O", 0));
        actionSave->setText(QApplication::translate("viewerClass", "Save", 0));
#ifndef QT_NO_TOOLTIP
        actionSave->setToolTip(QApplication::translate("viewerClass", "Save the document to disk", 0));
#endif // QT_NO_TOOLTIP
        actionSave->setShortcut(QApplication::translate("viewerClass", "Ctrl+S", 0));
        actionSave_As->setText(QApplication::translate("viewerClass", "Save As", 0));
#ifndef QT_NO_TOOLTIP
        actionSave_As->setToolTip(QApplication::translate("viewerClass", "Save the document under a new name", 0));
#endif // QT_NO_TOOLTIP
        actionExit->setText(QApplication::translate("viewerClass", "Exit", 0));
#ifndef QT_NO_TOOLTIP
        actionExit->setToolTip(QApplication::translate("viewerClass", "Exit the application", 0));
#endif // QT_NO_TOOLTIP
        actionExit->setShortcut(QApplication::translate("viewerClass", "Ctrl+Q", 0));
        actionCut->setText(QApplication::translate("viewerClass", "Cut", 0));
#ifndef QT_NO_TOOLTIP
        actionCut->setToolTip(QApplication::translate("viewerClass", "Cut the current selection's contents to the clipboard", 0));
#endif // QT_NO_TOOLTIP
        actionCut->setShortcut(QApplication::translate("viewerClass", "Ctrl+X", 0));
        actionCopy->setText(QApplication::translate("viewerClass", "Copy", 0));
#ifndef QT_NO_TOOLTIP
        actionCopy->setToolTip(QApplication::translate("viewerClass", "Copy the current selection's contents to the clipboard", 0));
#endif // QT_NO_TOOLTIP
        actionCopy->setShortcut(QApplication::translate("viewerClass", "Ctrl+C", 0));
        actionPaste->setText(QApplication::translate("viewerClass", "Paste", 0));
#ifndef QT_NO_TOOLTIP
        actionPaste->setToolTip(QApplication::translate("viewerClass", "Paste the clipboard's contents into the current selection", 0));
#endif // QT_NO_TOOLTIP
        actionPaste->setShortcut(QApplication::translate("viewerClass", "Ctrl+V", 0));
        actionAbout->setText(QApplication::translate("viewerClass", "About", 0));
#ifndef QT_NO_TOOLTIP
        actionAbout->setToolTip(QApplication::translate("viewerClass", "Show the application's About box", 0));
#endif // QT_NO_TOOLTIP
        actionTypeset->setText(QApplication::translate("viewerClass", "Typeset", 0));
#ifndef QT_NO_TOOLTIP
        actionTypeset->setToolTip(QApplication::translate("viewerClass", "Typeset and render text using NVPR.", 0));
#endif // QT_NO_TOOLTIP
        actionTypeset->setShortcut(QApplication::translate("viewerClass", "F5", 0));
        actionDemerits->setText(QApplication::translate("viewerClass", "Demerits", 0));
        actionR->setText(QApplication::translate("viewerClass", "r", 0));
        actionPenalty->setText(QApplication::translate("viewerClass", "Penalty", 0));
        actionSpaceR->setText(QApplication::translate("viewerClass", "Space r", 0));
        actionFontR->setText(QApplication::translate("viewerClass", "Font r", 0));
        actionLetterSpaceR->setText(QApplication::translate("viewerClass", "Letter Space r", 0));
        actionAuto_Typeset->setText(QApplication::translate("viewerClass", "Auto Typeset", 0));
#ifndef QT_NO_TOOLTIP
        actionAuto_Typeset->setToolTip(QApplication::translate("viewerClass", "Check to set auto typeset when text is chenged", 0));
#endif // QT_NO_TOOLTIP
        actionCommit->setText(QApplication::translate("viewerClass", "Commit", 0));
        actionPrevious->setText(QApplication::translate("viewerClass", "Previous", 0));
#ifndef QT_NO_TOOLTIP
        actionPrevious->setToolTip(QApplication::translate("viewerClass", "Jump to previous area", 0));
#endif // QT_NO_TOOLTIP
        actionPrevious->setShortcut(QApplication::translate("viewerClass", "Left", 0));
        actionNext->setText(QApplication::translate("viewerClass", "Next", 0));
#ifndef QT_NO_TOOLTIP
        actionNext->setToolTip(QApplication::translate("viewerClass", "Jump to next area", 0));
#endif // QT_NO_TOOLTIP
        actionNext->setShortcut(QApplication::translate("viewerClass", "Right", 0));
        actionUse_Magic->setText(QApplication::translate("viewerClass", "Use Magic", 0));
        actionPick_Shrink_Collor->setText(QApplication::translate("viewerClass", "Pick Shrink Collor", 0));
        actionPink_Stretch_Color->setText(QApplication::translate("viewerClass", "Pink Stretch Color", 0));
        actionShow_Only_One_Suggestion->setText(QApplication::translate("viewerClass", "Show Only One Suggestion", 0));
        label->setText(QString());
        riverButton->setText(QApplication::translate("viewerClass", "Show River", 0));
        label_3->setText(QApplication::translate("viewerClass", "Threshold:", 0));
        menuFile->setTitle(QApplication::translate("viewerClass", "Settings", 0));
        menuFile_2->setTitle(QApplication::translate("viewerClass", "File", 0));
        menuEdit->setTitle(QApplication::translate("viewerClass", "Edit", 0));
        menuCAbout->setTitle(QApplication::translate("viewerClass", "About", 0));
        menuView->setTitle(QApplication::translate("viewerClass", "View", 0));
        menuMarkdown->setTitle(QApplication::translate("viewerClass", "Markdown", 0));
    } // retranslateUi

};

namespace Ui {
    class viewerClass: public Ui_viewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWER_H
