/********************************************************************************
** Form generated from reading UI file 'wdChangePropertieText.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef WDCHANGEPROPERTIETEXT_H
#define WDCHANGEPROPERTIETEXT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_wdChangePropertieText
{
public:
    QPushButton *btnOK;
    QPushButton *btnCancel;
    QLabel *lblPropertieName;
    QLineEdit *txtValue;

    void setupUi(QWidget *wdChangePropertieText)
    {
        if (wdChangePropertieText->objectName().isEmpty())
            wdChangePropertieText->setObjectName(QString::fromUtf8("wdChangePropertieText"));
        wdChangePropertieText->resize(680, 77);
        btnOK = new QPushButton(wdChangePropertieText);
        btnOK->setObjectName(QString::fromUtf8("btnOK"));
        btnOK->setGeometry(QRect(510, 50, 75, 23));
        btnCancel = new QPushButton(wdChangePropertieText);
        btnCancel->setObjectName(QString::fromUtf8("btnCancel"));
        btnCancel->setGeometry(QRect(590, 50, 75, 23));
        lblPropertieName = new QLabel(wdChangePropertieText);
        lblPropertieName->setObjectName(QString::fromUtf8("lblPropertieName"));
        lblPropertieName->setGeometry(QRect(10, 20, 46, 13));
        txtValue = new QLineEdit(wdChangePropertieText);
        txtValue->setObjectName(QString::fromUtf8("txtValue"));
        txtValue->setGeometry(QRect(170, 20, 491, 20));

        retranslateUi(wdChangePropertieText);

        QMetaObject::connectSlotsByName(wdChangePropertieText);
    } // setupUi

    void retranslateUi(QWidget *wdChangePropertieText)
    {
        wdChangePropertieText->setWindowTitle(QApplication::translate("wdChangePropertieText", "wdChangePropertieText", nullptr));
        btnOK->setText(QApplication::translate("wdChangePropertieText", "OK", nullptr));
        btnCancel->setText(QApplication::translate("wdChangePropertieText", "Cancel", nullptr));
        lblPropertieName->setText(QApplication::translate("wdChangePropertieText", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class wdChangePropertieText: public Ui_wdChangePropertieText {};
} // namespace Ui

QT_END_NAMESPACE

#endif // WDCHANGEPROPERTIETEXT_H
