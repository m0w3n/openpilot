#include "selfdrive/ui/qt/widgets/wifi.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include "selfdrive/ui/qt/util.h"

WiFiPromptWidget::WiFiPromptWidget(QWidget *parent) : QFrame(parent) {
  stack = new QStackedLayout(this);

  // Setup Wi-Fi
  QFrame *setup = new QFrame;
  QVBoxLayout *setup_layout = new QVBoxLayout(setup);
  setup_layout->setContentsMargins(get_d_sm(56), get_d_sm(40), get_d_sm(56), get_d_sm(40));
  setup_layout->setSpacing(get_d_sm(20));
  {
    QHBoxLayout *title_layout = new QHBoxLayout;
    title_layout->setSpacing(get_d_sm(32));
    {
      QLabel *icon = new QLabel;
      QPixmap pixmap("../assets/offroad/icon_wifi_strength_full.svg");
      icon->setPixmap(pixmap.scaledToWidth(get_d_sm(80), Qt::SmoothTransformation));
      title_layout->addWidget(icon);

      QLabel *title = new QLabel(tr("Setup Wi-Fi"));
      title->setStyleSheet("font-size: 25px; font-weight: 240;");
      title_layout->addWidget(title);
      title_layout->addStretch();
    }
    setup_layout->addLayout(title_layout);

    QLabel *desc = new QLabel(tr("Connect to Wi-Fi to upload driving data and help improve openpilot"));
    desc->setStyleSheet("font-size: 16px; font-weight: 160;");
    desc->setWordWrap(true);
    setup_layout->addWidget(desc);

    QPushButton *settings_btn = new QPushButton(tr("Open Settings"));
    connect(settings_btn, &QPushButton::clicked, [=]() { emit openSettings(1); });
    settings_btn->setStyleSheet(R"(
      QPushButton {
        font-size: 19px;
        font-weight: 200;
        border-radius: 4px;
        background-color: #465BEA;
        padding: 12px;
      }
      QPushButton:pressed {
        background-color: #3049F4;
      }
    )");
    setup_layout->addWidget(settings_btn);
  }
  stack->addWidget(setup);

  // Uploading data
  QWidget *uploading = new QWidget;
  QVBoxLayout *uploading_layout = new QVBoxLayout(uploading);
  uploading_layout->setContentsMargins(get_d_sm(64), get_d_sm(56), get_d_sm(64), get_d_sm(56));
  uploading_layout->setSpacing(get_d_sm(36));
  {
    QHBoxLayout *title_layout = new QHBoxLayout;
    {
      QLabel *title = new QLabel(tr("Ready to upload"));
      title->setStyleSheet("font-size: 25px; font-weight: 240;");
      title->setWordWrap(true);
      title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
      title_layout->addWidget(title);
      title_layout->addStretch();

      QLabel *icon = new QLabel;
      QPixmap pixmap("../assets/offroad/icon_wifi_uploading.svg");
      icon->setPixmap(pixmap.scaledToWidth(get_d_sm(120), Qt::SmoothTransformation));
      title_layout->addWidget(icon);
    }
    uploading_layout->addLayout(title_layout);

    QLabel *desc = new QLabel(tr("Training data will be pulled periodically while your device is on Wi-Fi"));
    desc->setStyleSheet("font-size: 19px; font-weight: 160;");
    desc->setWordWrap(true);
    uploading_layout->addWidget(desc);
  }
  stack->addWidget(uploading);

  setStyleSheet(R"(
    WiFiPromptWidget {
      background-color: #333333;
      border-radius: 4px;
    }
  )");

  QObject::connect(uiState(), &UIState::uiUpdate, this, &WiFiPromptWidget::updateState);
}

void WiFiPromptWidget::updateState(const UIState &s) {
  if (!isVisible()) return;

  auto &sm = *(s.sm);

  auto network_type = sm["deviceState"].getDeviceState().getNetworkType();
  auto uploading = network_type == cereal::DeviceState::NetworkType::WIFI ||
      network_type == cereal::DeviceState::NetworkType::ETHERNET;
  stack->setCurrentIndex(uploading ? 1 : 0);
}
