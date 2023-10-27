#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>

class ClickableLabel : public QLabel {
  Q_OBJECT

 public:
  explicit ClickableLabel(QWidget *parent   = Q_NULLPTR,
                          Qt::WindowFlags f = Qt::WindowFlags());
  ~ClickableLabel();

 signals:
  void clicked();

 protected:
  void mousePressEvent(QMouseEvent *event) override;
  void enterEvent([[maybe_unused]] QEnterEvent *ev) override {
    setStyleSheet("QLabel{ border: 2px solid red;}");
  }
  void leaveEvent([[maybe_unused]] QEvent *ev) override {
    setStyleSheet("QLabel{border: 0px;}");
  }
};

#endif  // CLICKABLELABEL_H
