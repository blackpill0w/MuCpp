#pragma once

#include <filesystem>
#include <QWidget>
#include <QPixmap>

#include "./ui_album_widget.hpp"

namespace mucpp {

class AlbumWidget : public QWidget {
 public:
  AlbumWidget(const char *img_path, const char *title, QWidget *parent = nullptr,
              Qt::WindowFlags f = Qt::WindowFlags());
  QLabel *get_title_label();
  ClickableLabel *get_img_label();

 private:
  Ui::AlbumWidget *m_aw;
  QPixmap *m_pixmap;
};
}  // namespace mucpp
