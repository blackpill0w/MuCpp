#pragma once

#include <memory>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include "../deps/Music-Indexer/src/music_indexer.hpp"
#include "./flow_layout.hpp"
#include <QSizePolicy>

namespace mucpp {
class MainWindow {
 public:
  MainWindow();
  void run();

 protected:
  void show_frame(QFrame *);
  void update_albums();

 protected:
  std::unique_ptr<SQLite::Database> m_db = nullptr;

  QMainWindow *m_main_win   = nullptr;
  QScrollArea *m_content_sa = nullptr;

  QPushButton *m_settings_button = nullptr;
  QFrame *m_albums_frame         = nullptr;
  QFrame *m_settings_frame       = nullptr;
};
}  // namespace mucpp