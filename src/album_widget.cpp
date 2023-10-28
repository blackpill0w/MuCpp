#include "./album_widget.hpp"

namespace mucpp {
AlbumWidget::AlbumWidget(const char *img_path, const char *title,
                         QWidget *parent, Qt::WindowFlags f)
    : QWidget{parent, f}, m_aw{new Ui::AlbumWidget()}, m_pixmap{new QPixmap(img_path)} {
  m_aw->setupUi(this);
  m_aw->imgL->setPixmap(*m_pixmap);
  m_aw->titleL->setText(title);
  QWidget::connect(m_aw->imgL, &ClickableLabel::clicked,
                   [&] { qWarning() << "Hello, Mr. K"; });
}

}  // namespace mucpp
