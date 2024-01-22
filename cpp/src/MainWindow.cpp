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
    QLineEdit*   input         = new QLineEdit();

    mainLayout->addWidget(input, 0, 0);

    m_renderedInput = new QLabel();

    m_renderedInput->setMinimumHeight(11);
    m_renderedInput->setMaximumHeight(11);
    m_renderedInput->setStyleSheet("background-color: white; color: black;");

    QFont renderedInputFont;
    renderedInputFont.setStyleStrategy(QFont::NoAntialias);

    m_renderedInput->setFont(renderedInputFont);

    mainLayout->addWidget(m_renderedInput, 0, 1);

    QPushButton* sendButton = new QPushButton(tr("Send"));

    mainLayout->addWidget(sendButton, 1, 0);

    connect(input, &QLineEdit::textChanged, m_renderedInput, &QLabel::setText);
    connect(sendButton, &QPushButton::clicked, this, &MainWindow::Send);

    setCentralWidget(centralWidget);
}


void MainWindow::Send(void) {
    QPixmap pixmap = m_renderedInput->grab();
    QImage  image  = pixmap.toImage();

    assert(image.height() == 11);

    LedBadge             ledBadge;
    LedBadge::MemoryBank memoryBank = ledBadge.GetMemoryBank(0);

    if (memoryBank.SetData(image.width(), [&image](size_t x, size_t y) {return (image.pixelColor(x, y).valueF() > 0.7) ? false : true;})) {
        memoryBank.SetMode(LedBadge::Mode::Centered);

        std::vector<unsigned char> data;

        if (ledBadge.FetchData(data))
            SendToUsb(data);
    }
}
