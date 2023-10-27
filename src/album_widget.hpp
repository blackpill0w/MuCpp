#pragma once

#include <filesystem>
#include <QWidget>
#include <QPixmap>

#include "./ui_album_widget.hpp"

namespace mucpp {

class AlbumWidget : public QWidget {
 public:
  AlbumWidget(const char *img_path, const char *title, QWidget *parent = nullptr,
              Qt::WindowFlags f = Qt::WindowFlags())
      : QWidget(parent, f), m_aw{new Ui::AlbumWidget()}, m_pixmap{new QPixmap(img_path)} {
    m_aw->setupUi(this);
    m_aw->imgL->setPixmap(*m_pixmap);
    m_aw->titleL->setText(title);
    QWidget::connect(m_aw->imgL, &ClickableLabel::clicked,
                     [&]() { qWarning() << "Hello, Mr. K"; });
  }

 private:
  Ui::AlbumWidget *m_aw;
  QPixmap *m_pixmap;
};
}  // namespace mucpp