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
    QAction *actionOpen_File;
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
    QComboBox *paperBox;
    QDoubleSpinBox *riverThresholdSpinBox;
    QLabel *label_3;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *viewerClass)
    {
        if (viewerClass->objectName().isEmpty())
            viewerClass->setObjectName(QStringLiteral("viewerClass"));
        viewerClass->resize(897, 642);
        actionOpen_File = new QAction(viewerClass);
        actionOpen_File->setObjectName(QStringLiteral("actionOpen_File"));
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
        paperBox = new QComboBox(centralWidget);
        paperBox->setObjectName(QStringLiteral("paperBox"));
        paperBox->setGeometry(QRect(30, 210, 69, 22));
        riverThresholdSpinBox = new QDoubleSpinBox(centralWidget);
        riverThresholdSpinBox->setObjectName(QStringLiteral("riverThresholdSpinBox"));
        riverThresholdSpinBox->setGeometry(QRect(90, 180, 60, 20));
        riverThresholdSpinBox->setValue(2);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(30, 180, 50, 20));
        viewerClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(viewerClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 897, 21));
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


        QMetaObject::connectSlotsByName(viewerClass);
    } // setupUi

    void retranslateUi(QMainWindow *viewerClass)
    {
        viewerClass->setWindowTitle(QApplication::translate("viewerClass", "Typesetter", 0));
        actionOpen_File->setText(QApplication::translate("viewerClass", "Open File", 0));
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
"She ran to see who was outside. She opened the door, and the frog was sitting there. Frightened, she slammed the door shut and returned to the table. The king saw that her heart was pounding and asked, \"My child, why are you afraid? Is there a giant outside the door who wants to get you?\"\n"
"\n"
"\"Oh, no,\" she answered. \"it is a disgusting frog.\"\n"
"\n"
"\"What does the frog want from you?\"\n"
"\n"
"\"Oh, father dear, yesterday when I was sitting near the well in the forest and playing, my golden ball fell into the water. And because I was crying so much, the frog brought it back, and because he insisted, I promised him that he could be my companion, but I didn't think that he could leave his water. But now he is just outside the door and wants to come in.\"\n"
"\n"
"Just then there came a seco"
                        "nd knock at the door, and a voice called out:\n"
"\n"
"Youngest daughter of the king,\n"
"Open up the door for me,\n"
"Don't you know what yesterday,\n"
"You said to me down by the well?\n"
"Youngest daughter of the king,\n"
"Open up the door for me.\n"
"The king said, \"What you have promised, you must keep. Go and let the frog in.\"\n"
"\n"
"She went and opened the door, and the frog hopped in, then followed her up to her chair. He sat there and called out, \"Lift me up next to you.\"\n"
"\n"
"She hesitated, until finally the king commanded her to do it. When the frog was seated next to her he said, \"Now push your golden plate closer, so we can eat together.\"\n"
"\n"
"She did it, but one could see that she did not want to. The frog enjoyed his meal, but for her every bite stuck in her throat. Finally he said, \"I have eaten all I want and am tired. Now carry me to your room and make your bed so that we can go to sleep.\"\n"
"\n"
"The princess began to cry and was afraid of the cold frog and did not dare to"
                        " even touch him, and yet he was supposed to sleep in her beautiful, clean bed.\n"
"\n"
"The king became angry and said, \"You should not despise someone who has helped you in time of need.\"\n"
"\n"
"She picked him up with two fingers, carried him upstairs, and set him in a corner. As she was lying in bed, he came creeping up to her and said, \"I am tired, and I want to sleep as well as you do. Pick me up or I'll tell your father.\"\n"
"\n"
"With that she became bitterly angry and threw him against the wall with all her might. \"Now you will have your peace, you disgusting frog!\"\n"
"\n"
"But when he fell down, he was not a frog, but a prince with beautiful friendly eyes. And he was now, according to her father's will, her dear companion and husband. He told her how he had been enchanted by a wicked witch, and that she alone could have rescued him from the well, and that tomorrow they would go together to his kingdom. Then they fell asleep.\n"
"\n"
"The next morning, just as the sun was waking them, a carriag"
                        "e pulled up, drawn by eight horses. They had white ostrich feathers on their heads and were outfitted with chains of gold. At the rear stood the young king's servant, faithful Heinrich. Faithful Heinrich had been so saddened by his master's transformation into a frog that he had had to place three iron bands around his heart to keep it from bursting in grief and sorrow. The carriage was to take the king back to his kingdom. Faithful Heinrich lifted them both inside and took his place at the rear. He was filled with joy over the redemption. After they had gone a short distance, the prince heard a crack from behind, as though something had broken.\n"
"\n"
"He turned around and said, \"Heinrich, the carriage is breaking apart.\"\n"
"\n"
"No, my lord, the carriage it's not,\n"
"But one of the bands surrounding my heart,\n"
"That suffered such great pain,\n"
"When you were sitting in the well,\n"
"When you were a frog.\n"
"Once again, and then once again the prince heard a cracking sound and thought that the carria"
                        "ge was breaking apart, but it was the bands springing from faithful Heinrich's heart because his master was now redeemed and happy.", 0));
        label->setText(QApplication::translate("viewerClass", "TextLabel", 0));
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
        menuFile->setTitle(QApplication::translate("viewerClass", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class viewerClass: public Ui_viewerClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWER_H
