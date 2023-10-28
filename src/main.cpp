#include <QApplication>

#include "./mucpp.hpp"

int main(int argc, char **argv) {
  QApplication app{argc, argv};

  mucpp::MuCpp m{};
  m.run();

  app.exec();
}
