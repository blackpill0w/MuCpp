/********************************************************************************
** Form generated from reading UI file 'album_widgethhXVbT.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef ALBUM_WIDGETHHXVBT_H
#define ALBUM_WIDGETHHXVBT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include "./clickable_label.hpp"

QT_BEGIN_NAMESPACE

class Ui_AlbumWidget {
 public:
  QVBoxLayout *verticalLayout_2;
  ClickableLabel *imgL;
  QLabel *titleL;

  void setupUi(QWidget *AlbumWidget) {
    if (AlbumWidget->objectName().isEmpty())
      AlbumWidget->setObjectName("AlbumWidget");
    AlbumWidget->resize(318, 320);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(AlbumWidget->sizePolicy().hasHeightForWidth());
    AlbumWidget->setSizePolicy(sizePolicy);
    AlbumWidget->setMaximumSize(QSize(318, 320));
    verticalLayout_2 = new QVBoxLayout(AlbumWidget);
    verticalLayout_2->setObjectName("verticalLayout_2");
    imgL = new ClickableLabel(AlbumWidget);
    imgL->setObjectName("imgL");
    QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(imgL->sizePolicy().hasHeightForWidth());
    imgL->setSizePolicy(sizePolicy1);
    imgL->setMaximumSize(QSize(200, 200));
    imgL->setScaledContents(true);

    verticalLayout_2->addWidget(imgL);

    titleL = new QLabel(AlbumWidget);
    titleL->setObjectName("titleL");
    titleL->setMaximumSize(QSize(200, 30));
    titleL->setText(QString::fromUtf8("TITLE"));
    titleL->setTextFormat(Qt::PlainText);
    titleL->setAlignment(Qt::AlignCenter);

    verticalLayout_2->addWidget(titleL);

    retranslateUi(AlbumWidget);

    QMetaObject::connectSlotsByName(AlbumWidget);
  }  // setupUi

  void retranslateUi(QWidget *AlbumWidget) {
    AlbumWidget->setWindowTitle(
        QCoreApplication::translate("AlbumWidget", "Form", nullptr));
    imgL->setText(QCoreApplication::translate(
        "AlbumWidget", "INSERT IMAGE AAAAAAAAAAAAAAAAAAAAAAAAA", nullptr));
  }  // retranslateUi
};

namespace Ui {
class AlbumWidget : public Ui_AlbumWidget {};
}  // namespace Ui

QT_END_NAMESPACE

#endif  // ALBUM_WIDGETHHXVBT_H
