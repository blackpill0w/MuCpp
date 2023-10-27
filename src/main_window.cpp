#include "./main_window.hpp"

#include <filesystem>
#include <format>

#include <QUiLoader>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QFileDialog>
#include <QFrame>
#include <QLayout>

#include "./album_widget.hpp"
#include "./flow_layout.hpp"

namespace fs = std::filesystem;

namespace mucpp {
MainWindow::MainWindow() {
  QString config_dir{QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)};
  std::string db_file{std::format("{}/music_library.sqlite", config_dir.toStdString())};
  MusicIndexer::data_dir = std::format("{}/album_art", config_dir.toStdString());

  fs::create_directory(config_dir.toStdString());

  m_db = std::make_unique<SQLite::Database>(db_file,
                                            SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
  MusicIndexer::init_database(*m_db);

  QFile f{"../ui/main_win.ui"};
  f.open(QIODevice::ReadOnly);

  QUiLoader loader;

  m_main_win = static_cast<QMainWindow *>(loader.load(&f));
  assert((m_main_win != nullptr) && "1");

  m_content_sa = m_main_win->findChild<QScrollArea *>("contentSA");
  assert((m_content_sa != nullptr) && "2");
  m_content_sa->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  m_content_sa->setWidgetResizable(true);

  m_albums_frame = new QFrame();
  new FlowLayout(m_albums_frame);
  QWidget::connect(m_main_win->findChild<QPushButton *>("albumsPB"),
                   &QPushButton::pressed, [&] { this->show_frame(m_albums_frame); });

  m_content_sa->setWidget(m_albums_frame);
  update_albums();

  f.close();
  f.setFileName("../ui/settings_frame.ui");
  f.open(QIODevice::ReadOnly);

  m_settings_button = m_main_win->findChild<QPushButton *>("settingsPB");
  m_settings_frame  = loader.load(&f)->findChild<QFrame *>("settingsF");
  QWidget::connect(m_settings_button, &QPushButton::pressed,
                   [&] { this->show_frame(m_settings_frame); });

  auto *add_mdir_pb = m_settings_frame->findChild<QPushButton *>("addMDirPB");
  QWidget::connect(add_mdir_pb, &QPushButton::pressed, [&] {
    QString dir = QFileDialog::getExistingDirectory(m_settings_frame, "Add a directory",
                                                    "", QFileDialog::ShowDirsOnly);
    MusicIndexer::insert<MusicIndexer::MusicDir>(*m_db, dir.toStdString());
    MusicIndexer::build_music_library(*m_db);
    update_albums();
    show_frame(m_albums_frame);
  });
}

void MainWindow::run() {
  if (m_main_win != nullptr)
    m_main_win->show();
  else
    exit(1);
}

void MainWindow::show_frame(QFrame *f) {
  // We need to take ownership of the current widget otherwise m_content_sa deletes it.
  // We already have the pointer's value so the return value is discarded.
  // You have no idea how much debugging I did to find this bug.
  (void) m_content_sa->takeWidget();
  m_content_sa->setWidget(f);
}

void MainWindow::update_albums() {
  auto* layout = m_albums_frame->layout();
  while (!m_albums_frame->layout()->isEmpty()) {
    layout->removeItem(layout->itemAt(0));
  }
  const auto albums{MusicIndexer::get_all<MusicIndexer::Album>(*m_db)};

  for (const auto &a : albums) {
    std::string album_art{std::format("{}/{}", MusicIndexer::data_dir, a.id)};
    if (! fs::exists(album_art)) {
      // TODO: Change this bug bunny picture as a default album art
      // (or maybe keep it)
      album_art = "../ui/bugs_bunny.jpg";
    }
    auto *w = new AlbumWidget(album_art.c_str(), a.name.c_str(), m_albums_frame);
    layout->addWidget(w);
  }
}
}  // namespace mucpp
