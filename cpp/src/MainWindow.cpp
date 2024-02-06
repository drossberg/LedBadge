/*                       M A I N W I N D O W . C P P
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <QDate>
#include <QFontDialog>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>

#include "LedBadge.h"
#include "usb.h"
#include "MainWindow.h"


MainWindow::MainWindow
(
    QWidget* parent
) : QMainWindow(parent) {
    setWindowTitle(tr("LED Badge Designer"));

    QWidget*     centralWidget = new QWidget(this);
    QGridLayout* mainLayout    = new QGridLayout(centralWidget);

    QFont        renderedInputFont;
    renderedInputFont.setStyleStrategy(QFont::NoAntialias);

    for (size_t i = 0; i < 8; ++i) {
        QLabel*      title      = new QLabel(tr("Bank %1:").arg(i));
        QPushButton* fontButton = new QPushButton(tr("Font"));
        QLineEdit*   input      = new QLineEdit();

        m_renderedInput[i] = new QLabel();

        m_renderedInput[i]->setMinimumHeight(11);
        m_renderedInput[i]->setMaximumHeight(11);
        m_renderedInput[i]->setStyleSheet("background-color: white; color: black;");
        m_renderedInput[i]->setFont(renderedInputFont);

        m_blinkingSet[i]       = new QCheckBox(tr("blinking"));
        m_animatedBorderSet[i] = new QCheckBox(tr("animated border"));

        QLabel* modeLabel = new QLabel(tr("Mode:"));

        m_modeSelection[i] = new QComboBox();

        m_modeSelection[i]->addItem(tr("left scroll"), static_cast<int>(LedBadge::Mode::LeftScroll));
        m_modeSelection[i]->addItem(tr("right scroll"), static_cast<int>(LedBadge::Mode::RightScroll));
        m_modeSelection[i]->addItem(tr("up scroll"), static_cast<int>(LedBadge::Mode::UpScroll));
        m_modeSelection[i]->addItem(tr("down scroll"), static_cast<int>(LedBadge::Mode::DownScroll));
        m_modeSelection[i]->addItem(tr("centered"), static_cast<int>(LedBadge::Mode::Centered));
        m_modeSelection[i]->addItem(tr("snowlake"), static_cast<int>(LedBadge::Mode::Snowflake));
        m_modeSelection[i]->addItem(tr("drop down"), static_cast<int>(LedBadge::Mode::DropDown));
        m_modeSelection[i]->addItem(tr("curtain"), static_cast<int>(LedBadge::Mode::Curtain));
        m_modeSelection[i]->addItem(tr("laser"), static_cast<int>(LedBadge::Mode::Laser));

        QLabel* speedLabel = new QLabel(tr("Speed:"));

        m_speedSelection[i] = new QComboBox();

        m_speedSelection[i]->addItem(tr("one"), static_cast<int>(LedBadge::Speed::One));
        m_speedSelection[i]->addItem(tr("two"), static_cast<int>(LedBadge::Speed::Two));
        m_speedSelection[i]->addItem(tr("three"), static_cast<int>(LedBadge::Speed::Three));
        m_speedSelection[i]->addItem(tr("four"), static_cast<int>(LedBadge::Speed::Four));
        m_speedSelection[i]->addItem(tr("five"), static_cast<int>(LedBadge::Speed::Five));
        m_speedSelection[i]->addItem(tr("six"), static_cast<int>(LedBadge::Speed::Six));
        m_speedSelection[i]->addItem(tr("seven"), static_cast<int>(LedBadge::Speed::Seven));
        m_speedSelection[i]->addItem(tr("eight"), static_cast<int>(LedBadge::Speed::Eight));
        m_speedSelection[i]->setCurrentIndex(4);

        mainLayout->addWidget(title, i * 4, 0);

        mainLayout->addWidget(fontButton, i * 4 + 1, 0);
        mainLayout->addWidget(input, i * 4 + 1, 1);
        mainLayout->addWidget(m_renderedInput[i], i * 4 + 1, 2, 1, 4);

        mainLayout->addWidget(m_blinkingSet[i], i * 4 + 2, 0);
        mainLayout->addWidget(m_animatedBorderSet[i], i * 4 + 2, 1);
        mainLayout->addWidget(modeLabel, i * 4 + 2, 2);
        mainLayout->addWidget(m_modeSelection[i], i * 4 + 2, 3);
        mainLayout->addWidget(speedLabel, i * 4 + 2, 4);
        mainLayout->addWidget(m_speedSelection[i], i * 4 + 2, 5);

        mainLayout->addItem(new QSpacerItem(10, 10), i * 4 + 3, 0);

        connect(fontButton, &QPushButton::clicked, this, [this, i](){MainWindow::SelectFont(i);});
        connect(input, &QLineEdit::textChanged, m_renderedInput[i], &QLabel::setText);
    }

    QLabel*      brightnessLabel = new QLabel(tr("Brightness:"));
    QPushButton* sendButton      = new QPushButton(tr("Send"));

    m_brightnessSelection = new QComboBox();

    m_brightnessSelection->addItem(tr("full"), static_cast<int>(LedBadge::Brightness::Full));
    m_brightnessSelection->addItem(tr("hight"), static_cast<int>(LedBadge::Brightness::High));
    m_brightnessSelection->addItem(tr("medium"), static_cast<int>(LedBadge::Brightness::Medium));
    m_brightnessSelection->addItem(tr("low"), static_cast<int>(LedBadge::Brightness::Low));

    mainLayout->addWidget(brightnessLabel, 32, 0);
    mainLayout->addWidget(m_brightnessSelection, 32, 1);
    mainLayout->addWidget(sendButton, 32, 5);

    connect(sendButton, &QPushButton::clicked, this, &MainWindow::Send);

    m_logWidget = new LogWidget();

    mainLayout->addItem(new QSpacerItem(10, 10), 33, 0);
    mainLayout->addWidget(m_logWidget, 34, 0, 1, 6);

    setCentralWidget(centralWidget);
}


void MainWindow::SelectFont
(
    size_t i
) {
    assert(i < 8);

    bool  ok                = true;
    QFont renderedInputFont = QFontDialog::getFont(&ok, m_renderedInput[i]->font(), this);

    renderedInputFont.setStyleStrategy(QFont::NoAntialias);

    if (ok)
        m_renderedInput[i]->setFont(renderedInputFont);
}


void MainWindow::Send(void) {
    std::function<void(const char* logString)> logHandler = [this](const char* logString){(*m_logWidget)(logString);};
    LedBadge ledBadge(&logHandler);
    bool     ok = true;

    ledBadge.SetBrightness(static_cast<LedBadge::Brightness>(m_brightnessSelection->currentData().toInt()));

    for (size_t i = 0; i < 8; ++i) {
        QPixmap pixmap = m_renderedInput[i]->grab();
        QImage  image  = pixmap.toImage();

        assert(image.height() == 11);

        LedBadge::MemoryBank memoryBank = ledBadge.GetMemoryBank(i);

        memoryBank.SetBlinking(m_blinkingSet[i]->checkState() == Qt::Checked);
        memoryBank.SetAnimatedBorder(m_animatedBorderSet[i]->checkState() == Qt::Checked);
        memoryBank.SetMode(static_cast<LedBadge::Mode>(m_modeSelection[i]->currentData().toInt()));
        memoryBank.SetSpeed(static_cast<LedBadge::Speed>(m_speedSelection[i]->currentData().toInt()));

        ok &= memoryBank.SetData(image.width(), [&image](size_t x, size_t y) {return (image.pixelColor(x, y).valueF() > 0.7) ? false : true;});
    }

    if (ok) {
        QDate date = QDate::currentDate();

        ledBadge.SetYear(date.year() % 100);
        ledBadge.SetMonth(date.month());
        ledBadge.SetDay(date.day());

        QTime time = QTime::currentTime();

        ledBadge.SetHour(time.hour());
        ledBadge.SetMinute(time.minute());
        ledBadge.SetSecond(time.second());

        std::vector<unsigned char> data;

        if (ledBadge.FetchData(data))
            SendToUsb(data, &logHandler);
    }
}
