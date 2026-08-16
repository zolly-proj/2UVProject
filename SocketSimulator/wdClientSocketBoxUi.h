/********************************************************************************
** Form generated from reading UI file 'wdClientSocketBoxUi.ui'
**
** Created by: Qt User Interface Compiler version 5.15.13
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef WDCLIENTSOCKETBOXUI_H
#define WDCLIENTSOCKETBOXUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_wdClientSocketBoxUi
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QTextEdit *txtSend;
    QTextEdit *txtReceive;
    QLineEdit *txtIP;
    QLineEdit *txtPort;
    QPushButton *btnConnect;
    QPushButton *btnDisconnect;
    QRadioButton *rdbConnected;
    QPushButton *btnSend;

    void setupUi(QWidget *wdClientSocketBoxUi)
    {
        if (wdClientSocketBoxUi->objectName().isEmpty())
            wdClientSocketBoxUi->setObjectName(QString::fromUtf8("wdClientSocketBoxUi"));
        wdClientSocketBoxUi->resize(524, 590);
        label = new QLabel(wdClientSocketBoxUi);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(90, 10, 47, 13));
        label_2 = new QLabel(wdClientSocketBoxUi);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(300, 10, 47, 13));
        label_3 = new QLabel(wdClientSocketBoxUi);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 90, 71, 16));
        label_4 = new QLabel(wdClientSocketBoxUi);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 320, 91, 16));
        txtSend = new QTextEdit(wdClientSocketBoxUi);
        txtSend->setObjectName(QString::fromUtf8("txtSend"));
        txtSend->setGeometry(QRect(10, 110, 501, 211));
        txtReceive = new QTextEdit(wdClientSocketBoxUi);
        txtReceive->setObjectName(QString::fromUtf8("txtReceive"));
        txtReceive->setGeometry(QRect(10, 340, 501, 231));
        txtIP = new QLineEdit(wdClientSocketBoxUi);
        txtIP->setObjectName(QString::fromUtf8("txtIP"));
        txtIP->setGeometry(QRect(160, 10, 113, 20));
        txtPort = new QLineEdit(wdClientSocketBoxUi);
        txtPort->setObjectName(QString::fromUtf8("txtPort"));
        txtPort->setGeometry(QRect(350, 10, 113, 20));
        btnConnect = new QPushButton(wdClientSocketBoxUi);
        btnConnect->setObjectName(QString::fromUtf8("btnConnect"));
        btnConnect->setGeometry(QRect(10, 10, 75, 23));
        btnDisconnect = new QPushButton(wdClientSocketBoxUi);
        btnDisconnect->setObjectName(QString::fromUtf8("btnDisconnect"));
        btnDisconnect->setGeometry(QRect(10, 40, 75, 23));
        rdbConnected = new QRadioButton(wdClientSocketBoxUi);
        rdbConnected->setObjectName(QString::fromUtf8("rdbConnected"));
        rdbConnected->setGeometry(QRect(90, 40, 82, 17));
        btnSend = new QPushButton(wdClientSocketBoxUi);
        btnSend->setObjectName(QString::fromUtf8("btnSend"));
        btnSend->setGeometry(QRect(364, 80, 151, 23));

        retranslateUi(wdClientSocketBoxUi);

        QMetaObject::connectSlotsByName(wdClientSocketBoxUi);
    } // setupUi

    void retranslateUi(QWidget *wdClientSocketBoxUi)
    {
        wdClientSocketBoxUi->setWindowTitle(QCoreApplication::translate("wdClientSocketBoxUi", "Form", nullptr));
        label->setText(QCoreApplication::translate("wdClientSocketBoxUi", "IP adress:", nullptr));
        label_2->setText(QCoreApplication::translate("wdClientSocketBoxUi", "Port:", nullptr));
        label_3->setText(QCoreApplication::translate("wdClientSocketBoxUi", "Send content:", nullptr));
        label_4->setText(QCoreApplication::translate("wdClientSocketBoxUi", "Receive content:", nullptr));
        btnConnect->setText(QCoreApplication::translate("wdClientSocketBoxUi", "Connect", nullptr));
        btnDisconnect->setText(QCoreApplication::translate("wdClientSocketBoxUi", "Disconnect", nullptr));
        rdbConnected->setText(QCoreApplication::translate("wdClientSocketBoxUi", "Connected", nullptr));
        btnSend->setText(QCoreApplication::translate("wdClientSocketBoxUi", "Send", nullptr));
    } // retranslateUi

};

namespace Ui {
    class wdClientSocketBoxUi: public Ui_wdClientSocketBoxUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // WDCLIENTSOCKETBOXUI_H
