#pragma once

#include <QLabel>
#include <QPushButton>

#include "common/params.h"
#include "selfdrive/ui/qt/util.h"

class ExperimentalModeButton : public QPushButton {
  Q_OBJECT

public:
  explicit ExperimentalModeButton(QWidget* parent = 0);

signals:
  void openSettings(int index = 0, const QString &toggle = "");

private:
  void showEvent(QShowEvent *event) override;

  Params params;
  bool experimental_mode;
  int img_width = get_d_sm(100);
  int horizontal_padding = get_d_sm(30);
  QPixmap experimental_pixmap;
  QPixmap chill_pixmap;
  QLabel *mode_label;
  QLabel *mode_icon;

protected:
  void paintEvent(QPaintEvent *event) override;
};
