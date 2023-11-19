#include "./mucpp.hpp"

#include <filesystem>
#include <format>

#include <QUiLoader>
#include <QFile>
#include <QStandardPaths>
#include <QDir>
#include <QFileDialog>
#include <QFrame>
#include <QLayout>
#include <QAudioOutput>
#include <QShortcut>

#include "./album_widget.hpp"
#include "./flow_layout.hpp"

namespace fs = std::filesystem;

namespace mucpp {

namespace Utils {  // Helper functions

/**
   Removes all items from a layout (and frees the memory)
*/
static void clear_layout(QLayout *layout);

static void toggle_mediaplayer(QMediaPlayer *player);

}  // namespace Utils

MuCpp::MuCpp() {
  this->m_player = new QMediaPlayer();
  m_player->setAudioOutput(new QAudioOutput());
  init_database();
  init_gui_ptrs();
  m_album_frame = new QFrame();
  new FlowLayout(m_album_frame);
  update_music_data();
}

void MuCpp::init_gui_ptrs() {
  QFile f{"../ui/main_win.ui"};
  f.open(QIODevice::ReadOnly);

  QUiLoader loader;

  m_main_win = static_cast<QMainWindow *>(loader.load(&f));
  if (m_main_win == nullptr) {
    qFatal() << "ERROR: couldn't initialise the window.";
    exit(1);
  }

  m_content_sa = m_main_win->findChild<QScrollArea *>("contentSA");
  if (m_content_sa == nullptr) {
    qFatal() << "ERROR: couldn't initialise the `m_content_sa`.";
    exit(1);
  }
  m_content_sa->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  m_content_sa->setWidgetResizable(true);

  m_all_albums_frame = new QFrame();
  new FlowLayout(m_all_albums_frame);
  QWidget::connect(m_main_win->findChild<QPushButton *>("albumsPB"),
                   &QPushButton::pressed, [&] { this->show_frame(m_all_albums_frame); });

  m_content_sa->setWidget(m_all_albums_frame);

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
    Midx::insert_music_dir(*m_db, dir.toStdString());
    Midx::build_music_library(*m_db);
    update_music_data();
    show_frame(m_all_albums_frame);
  });
}

void MuCpp::init_database() {
  QString config_dir{QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation)};
  std::string db_file{std::format("{}/music_library.sqlite", config_dir.toStdString())};
  Midx::data_dir = std::format("{}/album_art", config_dir.toStdString());

  fs::create_directory(config_dir.toStdString());
  fs::create_directory(Midx::data_dir);

  m_db = std::make_unique<SQLite::Database>(db_file,
                                            SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
  Midx::init_database(*m_db);
}

void MuCpp::run() {
  if (m_main_win != nullptr)
    m_main_win->show();
  else {
    qFatal() << "ERROR: couldn't create the window.";
    exit(1);
  }
}

void MuCpp::show_frame(QFrame *f) {
  // We need to take ownership of the current widget otherwise m_content_sa deletes it.
  // We already have the pointer's value so the return value is discarded.
  // You have no idea how much debugging I did to find this bug.
  (void) m_content_sa->takeWidget();
  m_content_sa->setWidget(f);
}

void MuCpp::update_music_data() {
  m_artists = Midx::get_all_artists(*m_db);
  m_albums  = Midx::get_all_albums(*m_db);
  m_tracks  = Midx::get_all_tracks(*m_db);

  this->map_tracks_to_artists_and_albums();
  this->update_albums();
}

void MuCpp::update_albums() {
  auto *layout = m_all_albums_frame->layout();
  Utils::clear_layout(layout);
  for (auto& a : m_albums) {
    std::string album_art{std::format("{}/{}", Midx::data_dir, a.id)};
    if (!fs::exists(album_art)) {
      // TODO: Change this bugs bunny picture as a default album art
      // (or maybe keep it)
      album_art = "../ui/bugs_bunny.jpg";
    }
    auto *w = new AlbumWidget(album_art.c_str(), a.name.c_str(), m_all_albums_frame);
    // We can't pass a function's local variable by reference
    // to a lambda that outlives the function,
    // so we'll pass a pointer to the referenced object by value
    auto *a_ptr = &a;
    QObject::connect(w->get_img_label(), &ClickableLabel::clicked,
                     [this, a_ptr]() { this->display_album(*a_ptr); });
    layout->addWidget(w);
  }
}

void MuCpp::display_album(Midx::Album& a) {
  this->show_frame(m_album_frame);
  auto *layout = m_album_frame->layout();
  Utils::clear_layout(layout);
  for (const Midx::Track *track : m_tracks_album.at(&a)) {
    auto *w = new ClickableLabel();
    w->setText(track->file_path.c_str());
    layout->addWidget(w);
    QObject::connect(w, &ClickableLabel::clicked, [this, track] {
      if (m_player->source() != QUrl::fromLocalFile(track->file_path.c_str())) {
        m_player->setSource(QUrl::fromLocalFile(track->file_path.c_str()));
        m_player->play();
      }
      else {
        Utils::toggle_mediaplayer(this->m_player);
      }
    });
  }
}

void MuCpp::map_tracks_to_artists_and_albums() {
  m_tracks_artist.clear();
  m_tracks_album.clear();
  // TODO: Improve this O(n*m) disaster.
  for (auto& artist : m_artists) {
    m_tracks_artist[&artist] = {};
    for (auto& track : m_tracks) {
      if (track.get_metadata().has_value() &&
          track.get_metadata()->artist_id == artist.id) {
        m_tracks_artist[&artist].push_back(&track);
      }
    }
  }

  for (auto& album : m_albums) {
    m_tracks_album[&album] = {};
    for (auto& track : m_tracks) {
      if (track.get_metadata().has_value() &&
          track.get_metadata()->album_id == album.id) {
        m_tracks_album[&album].push_back(&track);
      }
    }
  }
}

/******************************************************************************/
/************************** --| Static functions |-- **************************/
/******************************************************************************/

void Utils::clear_layout(QLayout *layout) {
  while (!layout->isEmpty()) {
    QLayoutItem *i = layout->takeAt(0);
    delete i->widget();
    delete i;
  }
}

void Utils::toggle_mediaplayer(QMediaPlayer *player) {
  if (player->playbackState() == QMediaPlayer::PlaybackState::PausedState)
    player->play();
  else
    player->pause();
}

}  // namespace mucpp
