#pragma once

#include <memory>
#include <vector>

#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>

#include "../deps/Music-Indexer/src/music_indexer.hpp"
#include "./flow_layout.hpp"

namespace mucpp {

using ArtistList = std::vector<MusicIndexer::Artist>;
using AlbumList  = std::vector<MusicIndexer::Album>;
using TrackList  = std::vector<MusicIndexer::Track>;

class MuCpp {
 public:
  MuCpp();
  void run();

 protected:
  /**
     Initialises the database and takes care of creating the configuration directory
     and the database file if they don't already exist.
  */
  void init_database();
  /**
     Initialises pointers that refer to the GUI elements (window, frames, etc).
   */
  void init_gui_ptrs();
  /**
     Makes a given frame visible (the widget of `QScrollArea`) in the `m_content_sa`
     and removes the active one.
   */
  void show_frame(QFrame *f);
  /**
     Gets all the artists, albums and tracks from the database.
   */
  void update_music_data();
  /**
     Displays albums inside `m_albums_frame` and removes whatever content in it.
   */
  void update_albums();

 protected:
  std::unique_ptr<SQLite::Database> m_db = nullptr;
  std::unique_ptr<ArtistList> m_artists  = nullptr;
  std::unique_ptr<AlbumList> m_albums    = nullptr;
  std::unique_ptr<TrackList> m_tracks    = nullptr;

  QMainWindow *m_main_win   = nullptr;
  QScrollArea *m_content_sa = nullptr;

  QPushButton *m_settings_button = nullptr;
  QFrame *m_albums_frame         = nullptr;
  QFrame *m_settings_frame       = nullptr;
};
}  // namespace mucpp
