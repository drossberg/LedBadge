/*                         M A I N W I N D O W . H
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

#ifndef MAINWINDOW_INCLUDED
#define MAINWINDOW_INCLUDED

#include <QCheckBox>
#include <QComboBox>
#include <QLabel>
#include <QMainWindow>


class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = 0);

public slots:
    void SelectFont(size_t i);
    void Send(void);

private:
    QComboBox* m_brightnessSelection;
    QCheckBox* m_blinkingSet[8];
    QCheckBox* m_animatedBorderSet[8];
    QComboBox* m_modeSelection[8];
    QComboBox* m_speedSelection[8];
    QLabel*    m_renderedInput[8];
};


#endif // MAINWINDOW_INCLUDED
