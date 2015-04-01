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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
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
    QCheckBox *riverButton;
    QSpinBox *fontSizeBox;
    QLabel *label_2;
    QComboBox *alignBox;
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
        plainTextEdit->setGeometry(QRect(10, 330, 321, 211));
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
        riverButton = new QCheckBox(centralWidget);
        riverButton->setObjectName(QStringLiteral("riverButton"));
        riverButton->setGeometry(QRect(30, 150, 82, 17));
        fontSizeBox = new QSpinBox(centralWidget);
        fontSizeBox->setObjectName(QStringLiteral("fontSizeBox"));
        fontSizeBox->setGeometry(QRect(90, 250, 60, 20));
        fontSizeBox->setMaximum(32);
        fontSizeBox->setValue(12);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(30, 250, 50, 20));
        alignBox = new QComboBox(centralWidget);
        alignBox->setObjectName(QStringLiteral("alignBox"));
        alignBox->setGeometry(QRect(30, 280, 120, 20));
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

        alignBox->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(viewerClass);
    } // setupUi

    void retranslateUi(QMainWindow *viewerClass)
    {
        viewerClass->setWindowTitle(QApplication::translate("viewerClass", "Typesetter", 0));
        renderButton->setText(QApplication::translate("viewerClass", "Render", 0));
        plainTextEdit->setPlainText(QApplication::translate("viewerClass", "In olden times, when wishing still did some good, there lived a king whose daughters were all beautiful, but the youngest was so beautiful that the sun itself, who, indeed, has seen so much, marveled every time it shone upon her face. In the vicinity of the king's castle there was a large, dark forest, and in this forest, beneath an old linden tree, there was a well. In the heat of the day the princess would go out into the forest and sit on the edge of the cool well. To pass the time she would take a golden ball, throw it into the air, and then catch it. It was her favorite plaything.\n"
"\n"
"Now one day it happened that the princess's golden ball did not fall into her hands, that she held up high, but instead it fell to the ground and rolled right into the water. The princess followed it with her eyes, but the ball disappeared, and the well was so deep that she could not see its bottom. Then she began to cry. She cried louder and louder, and she could not console herself.\n"
"\n"
"As she was thus lamenting,"
                        " someone called out to her, \"What is the matter with you, princess? Your crying would turn a stone to pity.\"\n"
"\n"
"She looked around to see where the voice was coming from and saw a frog, who had stuck his thick, ugly head out of the water. \"Oh, it's you, old water-splasher,\" she said. \"I am crying because my golden ball has fallen into the well.\"\n"
"\n"
"\"Be still and stop crying,\" answered the frog. I can help you, but what will you give me if I bring back your plaything?\"\n"
"\n"
"\"Whatever you want, dear frog,\" she said, \"my clothes, my pearls and precious stones, and even the golden crown that I am wearing.\"\n"
"\n"
"The frog answered, \"I do not want your clothes, your pearls and precious stones, nor your golden crown, but if you will love me and accept me as a companion and playmate, and let me sit next to you at your table and eat from your golden plate and drink from your cup and sleep in your bed, if you will promise this to me, then I'll dive down and bring your golden ball back to "
                        "you.\"\n"
"\n"
"\"Oh, yes,\" she said, \"I promise all of that to you if you will just bring the ball back to me.\" But she thought, \"What is this stupid frog trying to say? He just sits here in the water with his own kind and croaks. He cannot be a companion to a human.\"\n"
"\n"
"As soon as the frog heard her say \"yes\" he stuck his head under and dove to the bottom. He paddled back up a short time later with the golden ball in his mouth and threw it onto the grass. The princess was filled with joy when she saw her beautiful plaything once again, picked it up, and ran off.\n"
"\n"
"\"Wait, wait,\" called the frog, \"take me along. I cannot run as fast as you.\" But what did it help him, that he croaked out after her as loudly as he could? She paid no attention to him, but instead hurried home and soon forgot the poor frog, who had to return again to his well.\n"
"\n"
"The next day the princess was sitting at the table with the king and all the people of the court, and was eating from her golden plate when "
                        "something came creeping up the marble steps: plip, plop, plip, plop. As soon as it reached the top, there came a knock at the door, and a voice called out, \"Princess, youngest, open the door for me!\"\n"
"\n"
"", 0));
        label->setText(QApplication::translate("viewerClass", "TextLabel", 0));
        spaceBorderButton->setText(QApplication::translate("viewerClass", "Space Border", 0));
        wordBorderButton->setText(QApplication::translate("viewerClass", "Word Border", 0));
        charBorderButton->setText(QApplication::translate("viewerClass", "Char Border", 0));
        pageBorderButton->setText(QApplication::translate("viewerClass", "Page Border", 0));
        lineBorderButton->setText(QApplication::translate("viewerClass", "Line Border", 0));
        riverButton->setText(QApplication::translate("viewerClass", "Show River", 0));
        label_2->setText(QApplication::translate("viewerClass", "Font Size:", 0));
        alignBox->clear();
        alignBox->insertItems(0, QStringList()
         << QApplication::translate("viewerClass", "Ragged Right", 0)
         << QApplication::translate("viewerClass", "First Fit", 0)
         << QApplication::translate("viewerClass", "Best Fit", 0)
         << QApplication::translate("viewerClass", "Optimum Fit", 0)
        );
    } // retranslateUi

};

namespace Ui {
    class viewerClass: public Ui_viewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWER_H
