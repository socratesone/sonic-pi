//--
// This file is part of Sonic Pi: http://sonic-pi.net
// Full project source: https://github.com/samaaron/sonic-pi
// License: https://github.com/samaaron/sonic-pi/blob/main/LICENSE.md
//
// Copyright 2022 by Sam Aaron (http://sam.aaron.name).
// All rights reserved.
//
// Permission is granted for use, copying, modification, and
// distribution of modified versions of this work as long as this
// notice is included.
//++

#include "sonicpimetro.h"
#include <QVBoxLayout>
#include "qt_api_client.h"

SonicPiMetro::SonicPiMetro(std::shared_ptr<SonicPi::QtAPIClient> spClient, SonicPiTheme *theme, QWidget* parent)
  : QWidget(parent)
  , m_spClient(spClient)
{
  this->theme = theme;
  mutex = new QMutex;
  enableLinkButton = new QPushButton(tr("Enable Link"));
  enableLinkButton->setAutoFillBackground(true);
  enableLinkButton->setObjectName("enableLinkButton");
  enableLinkButton->setFlat(true);
  QHBoxLayout* metro_layout  = new QHBoxLayout;
  metro_layout->addWidget(enableLinkButton);
  setLayout(metro_layout);

  connect(enableLinkButton, &QPushButton::clicked, [=]() {
    this->toggleLink();
  });

  connect(m_spClient.get(), &SonicPi::QtAPIClient::UpdateNumActiveLinks, this, &SonicPiMetro::updateActiveLinkCount);

  updateLinkButtonDisplay();
}

void SonicPiMetro::toggleLink()
{
  linkEnabled = !linkEnabled;
  updateLinkButtonDisplay();
}

void SonicPiMetro::updateActiveLinkCount(int count)
{
  numActiveLinks = count;
  updateLinkButtonDisplay();
}

void SonicPiMetro::updateActiveLinkText()
{
  if(numActiveLinks == 1) {
    enableLinkButton->setText("1 Link");
  } else {
    enableLinkButton->setText(QString("%1 Links").arg(numActiveLinks));
  }
}

void SonicPiMetro::updateLinkButtonDisplay()
{
  QPalette palette = enableLinkButton->palette();
  QString qss;
  if(linkEnabled) {
    updateActiveLinkText();
    qss = QString("\nQPushButton {\nbackground-color: %1;}\nQPushButton::hover:!pressed {\nbackground-color: %2}\n").arg(theme->color("PressedButton").name()).arg(theme->color("PressedButton").name());
  } else {
    enableLinkButton->setText("Enable Link");
    qss = QString("\nQPushButton {\nbackground-color: %1;}\nQPushButton::hover:!pressed {\nbackground-color: %2}\n").arg(theme->color("Button").name()).arg(theme->color("HoverButton").name());
  }


  enableLinkButton->setStyleSheet(theme->getAppStylesheet() + qss);
}
