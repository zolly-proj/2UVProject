/********************************************************************************
** Form generated from reading UI file 'wdParameterView.ui'
**
** Created by: Qt User Interface Compiler version 5.12.12
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef WDPARAMETERVIEW_H
#define WDPARAMETERVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_wdParameterView
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QTreeWidget *tvwObjects;
    QListWidget *lstProperties;
    QTreeWidget *tvwActions;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btnRefresh;
    QPushButton *btnRead;
    QPushButton *btnWrite;
    QPushButton *btnMethod;

    void setupUi(QWidget *wdParameterView)
    {
        if (wdParameterView->objectName().isEmpty())
            wdParameterView->setObjectName(QString::fromUtf8("wdParameterView"));
        wdParameterView->resize(882, 628);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(100);
        sizePolicy.setVerticalStretch(100);
        sizePolicy.setHeightForWidth(wdParameterView->sizePolicy().hasHeightForWidth());
        wdParameterView->setSizePolicy(sizePolicy);
        wdParameterView->setMaximumSize(QSize(16777215, 16777215));
        verticalLayout = new QVBoxLayout(wdParameterView);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMaximumSize);
        tvwObjects = new QTreeWidget(wdParameterView);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        tvwObjects->setHeaderItem(__qtreewidgetitem);
        tvwObjects->setObjectName(QString::fromUtf8("tvwObjects"));

        horizontalLayout->addWidget(tvwObjects);

        lstProperties = new QListWidget(wdParameterView);
        lstProperties->setObjectName(QString::fromUtf8("lstProperties"));

        horizontalLayout->addWidget(lstProperties);

        tvwActions = new QTreeWidget(wdParameterView);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setText(0, QString::fromUtf8("1"));
        tvwActions->setHeaderItem(__qtreewidgetitem1);
        tvwActions->setObjectName(QString::fromUtf8("tvwActions"));

        horizontalLayout->addWidget(tvwActions);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout_3->setSizeConstraint(QLayout::SetMaximumSize);
        btnRefresh = new QPushButton(wdParameterView);
        btnRefresh->setObjectName(QString::fromUtf8("btnRefresh"));

        horizontalLayout_3->addWidget(btnRefresh);

        btnRead = new QPushButton(wdParameterView);
        btnRead->setObjectName(QString::fromUtf8("btnRead"));

        horizontalLayout_3->addWidget(btnRead);

        btnWrite = new QPushButton(wdParameterView);
        btnWrite->setObjectName(QString::fromUtf8("btnWrite"));

        horizontalLayout_3->addWidget(btnWrite);

        btnMethod = new QPushButton(wdParameterView);
        btnMethod->setObjectName(QString::fromUtf8("btnMethod"));

        horizontalLayout_3->addWidget(btnMethod);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(wdParameterView);

        QMetaObject::connectSlotsByName(wdParameterView);
    } // setupUi

    void retranslateUi(QWidget *wdParameterView)
    {
        wdParameterView->setWindowTitle(QApplication::translate("wdParameterView", "wdParameterView", nullptr));
        btnRefresh->setText(QApplication::translate("wdParameterView", "Refresh", nullptr));
        btnRead->setText(QApplication::translate("wdParameterView", "Read", nullptr));
        btnWrite->setText(QApplication::translate("wdParameterView", "Write", nullptr));
        btnMethod->setText(QApplication::translate("wdParameterView", "Method", nullptr));
    } // retranslateUi

};

namespace Ui {
    class wdParameterView: public Ui_wdParameterView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // WDPARAMETERVIEW_H
