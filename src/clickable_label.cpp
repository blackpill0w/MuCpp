#include "clickable_label.hpp"

ClickableLabel::ClickableLabel(QWidget *parent, Qt::WindowFlags f) : QLabel(parent, f) {}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent([[maybe_unused]] QMouseEvent *event) {
  emit clicked();
}
