#pragma once

#include <memory>
#include <QtMultimedia/QAudioOutput>
#include <QtMultimedia/QMediaPlayer>

/**
 * A basic music player for use with Qt6 applications (it needs QCoreApplication or
 * QApplication to be initialised before use).
 */
namespace mucpp {

class MusicPlayer {
 public:
  enum class State { None, Stopped, Playing, Paused };
  State state                                = State::None;
  std::unique_ptr<QMediaPlayer> player       = nullptr;
  std::unique_ptr<QAudioOutput> audio_output = nullptr;

 public:
  MusicPlayer() : player{new QMediaPlayer()}, audio_output{new QAudioOutput()} {
    audio_output->setVolume(50);
  };
  void set_track(const std::string& file_path) {
    player->setSource(QUrl::fromLocalFile(file_path.c_str()));
    state = State::Stopped;
  }
  /**
   * Start or resume playing the current source.
   */
  void play() {
    player->play();
    state = State::Playing;
  }
  /**
   * Pause playing the current source.
   */
  void pause() {
    player->pause();
    state = State::Paused;
  }
  /**
   * Stops playing and resets the play position to the beginning.
   */
  void stop() {
    player->stop();
    state = State::Stopped;
  }
  /**
   * Mute or unmute the audio output.
   */
  void mute() { audio_output->setMuted(not audio_output->isMuted()); }
  bool is_muted() const { return audio_output->isMuted(); }
  void set_volume(int v) { audio_output->setVolume(v); }
};

}  // namespace mucpp
