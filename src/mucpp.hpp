#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QMediaPlayer>

#include "midx.hpp"
#include "./flow_layout.hpp"

namespace mucpp {

using ArtistList = std::vector<Midx::Artist>;
using AlbumList  = std::vector<Midx::Album>;
using TrackList  = std::vector<Midx::Track>;

/**
   An `unordered_map` that maps tracks to artists.
*/
using ArtistToTrackMap = std::unordered_map<Midx::Artist *, std::vector<Midx::Track *>>;
/**
   An `unordered_map` that maps tracks to albums.
*/
using AlbumToTrackMap = std::unordered_map<Midx::Album *, std::vector<Midx::Track *>>;

class MuCpp {
 public:
  MuCpp();
  void run();
  ~MuCpp() {
    delete m_album_frame;
    delete m_all_albums_frame;
    delete m_settings_frame;
  }

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
     @note: This function makes a call to `map_tracks_to_artists_and_albums()` and
     `update_albums()`.
  */
  void update_music_data();
  /**
     Displays albums inside `m_albums_frame` and removes whatever content in it.
  */
  void update_albums();

  /**
     Display album and its tracks.
  */
  void display_album(Midx::Album& a);

  /**
     Updates the `m_tracks_artists` and `m_tracks_albums`.
  */
  void map_tracks_to_artists_and_albums();

 protected:
  QMediaPlayer *m_player = nullptr;

  std::unique_ptr<SQLite::Database> m_db = nullptr;
  ArtistList m_artists;
  AlbumList m_albums;
  TrackList m_tracks;
  ArtistToTrackMap m_tracks_artist;
  AlbumToTrackMap m_tracks_album;

  QMainWindow *m_main_win   = nullptr;
  QScrollArea *m_content_sa = nullptr;

  QPushButton *m_settings_button = nullptr;
  // TODO: frames that are not active are not freed when the program finishes.
  QFrame *m_all_albums_frame = nullptr;
  QFrame *m_settings_frame   = nullptr;
  QFrame *m_album_frame      = nullptr;
};
}  // namespace mucpp
