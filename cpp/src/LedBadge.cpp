/*                         L E D B A D G E . C P P
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

#include <cassert>
#include <cstring>

#include "LedBadge.h"


static inline void SetBit(unsigned char& byte,
                          size_t         index,
                          bool           value) {
    unsigned char bit = value ? '\x01' : '\x00';

    byte = (byte & ~('\x01' << index)) | (bit << index);
}


static inline void SetLowerBits(unsigned char& byte,
                                unsigned char  value) {
    byte = (byte & '\xf0') | value;
}


static inline void SetHigherBits(unsigned char& byte,
                                 unsigned char  value) {
    byte = (byte & '\x0f') | value;
}


LedBadge::LedBadge
(
    std::function<void(const char* logString)>* logHandler
) : m_logHandler(logHandler), m_bankData() {
    static const unsigned char NullHeader[m_HeaderSize] = {
        '\x77', '\x61', '\x6e', '\x67', '\x00', // [0-4]: "wang"
        '\x00',                                 // [5]  : brightness (full), 0x00 = 100% / 0x10 = 75% / 0x20 = 50% / 0x40 = 25%
        '\x00',                                 // [6]  : blinking (no), 1 bit per memory bank
        '\x00',                                 // [7]  : animated border (no), 1 bit per memory bank
        '\x40',                                 // [8]  : lower 4 bit: mode / higher 4 bit: speed of 1st memory bank
        '\x40',                                 // [9]  : lower 4 bit: mode / higher 4 bit: speed of 2nd memory bank
        '\x40',                                 // [10] : lower 4 bit: mode / higher 4 bit: speed of 3rd memory bank
        '\x40',                                 // [11] : lower 4 bit: mode / higher 4 bit: speed of 4th memory bank
        '\x40',                                 // [12] : lower 4 bit: mode / higher 4 bit: speed of 5th memory bank
        '\x40',                                 // [13] : lower 4 bit: mode / higher 4 bit: speed of 6th memory bank
        '\x40',                                 // [14] : lower 4 bit: mode / higher 4 bit: speed of 7th memory bank
        '\x40',                                 // [15] : lower 4 bit: mode / higher 4 bit: speed of 8th memory bank
        '\x00',                                 // [16] : higher byte of the legth of data in 1st memory bank (in multiples of 11 bytes)
        '\x00',                                 // [17] : lower byte of the legth of data in 1st memory bank (in multiples of 11 bytes)
        '\x00',                                 // [18] : higher byte of the legth of data in 2nd memory bank (in multiples of 11 bytes)
        '\x00',                                 // [19] : lower byte of the legth of data in 2nd memory bank (in multiples of 11 bytes)
        '\x00',                                 // [20] : higher byte of the legth of data in 3rd memory bank (in multiples of 11 bytes)
        '\x00',                                 // [21] : lower byte of the legth of data in 3rd memory bank (in multiples of 11 bytes)
        '\x00',                                 // [22] : higher byte of the legth of data in 4th memory bank (in multiples of 11 bytes)
        '\x00',                                 // [23] : lower byte of the legth of data in 4th memory bank (in multiples of 11 bytes)
        '\x00',                                 // [24] : higher byte of the legth of data in 5th memory bank (in multiples of 11 bytes)
        '\x00',                                 // [25] : lower byte of the legth of data in 5th memory bank (in multiples of 11 bytes)
        '\x00',                                 // [26] : higher byte of the legth of data in 6th memory bank (in multiples of 11 bytes)
        '\x00',                                 // [27] : lower byte of the legth of data in 6th memory bank (in multiples of 11 bytes)
        '\x00',                                 // [28] : higher byte of the legth of data in 7th memory bank (in multiples of 11 bytes)
        '\x00',                                 // [29] : lower byte of the legth of data in 7th memory bank (in multiples of 11 bytes)
        '\x00',                                 // [30] : higher byte of the legth of data in 8th memory bank (in multiples of 11 bytes)
        '\x00',                                 // [31] : lower byte of the legth of data in 8th memory bank (in multiples of 11 bytes)
        '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', // [32-37]
        '\x00',                                 // [38] : year (last two digets)
        '\x00',                                 // [39] : month
        '\x00',                                 // [40] : day
        '\x00',                                 // [41] : hour
        '\x00',                                 // [42] : minute
        '\x00',                                 // [43] : second
        '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00' // [44-63]
    };

    memcpy(m_header, NullHeader, m_HeaderSize);
}


LedBadge::LedBadge
(
    const LedBadge& original
) : m_logHandler(original.m_logHandler), m_bankData() {
    memcpy(m_header, original.m_header, m_HeaderSize);

    for (size_t i = 0; i < 8; ++i)
        m_bankData[i] = original.m_bankData[i];
}


LedBadge::~LedBadge(void) {}


LedBadge& LedBadge::operator=
(
    const LedBadge& original
) {
    if (this != &original) {
        memcpy(m_header, original.m_header, m_HeaderSize);

        for (size_t i = 0; i < 8; ++i)
            m_bankData[i] = original.m_bankData[i];
    }

    return *this;
}


LedBadge::MemoryBank::MemoryBank
(
    const MemoryBank& original
) : m_parent(original.m_parent), m_index(original.m_index) {
    assert(m_parent != nullptr);
    assert(m_index < 8);
}


LedBadge::MemoryBank::~MemoryBank(void) {}


LedBadge::MemoryBank& LedBadge::MemoryBank::operator=
(
    const MemoryBank& original
) {
    if (this != &original) {
        m_parent = original.m_parent;
        m_index  = original.m_index;
    }

    assert(m_parent != nullptr);
    assert(m_index < 8);

    return *this;
}


void LedBadge::MemoryBank::SetBlinking
(
    bool on
) {
    if ((m_parent != nullptr) && (m_index < 8))
        SetBit(m_parent->m_header[6], m_index, on);
}


void LedBadge::MemoryBank::SetAnimatedBorder
(
    bool on
) {
    if ((m_parent != nullptr) && (m_index < 8))
        SetBit(m_parent->m_header[7], m_index, on);
}


void LedBadge::MemoryBank::SetMode
(
    Mode value
) {
    if ((m_parent != nullptr) && (m_index < 8)) {
        switch (value) {
            case Mode::LeftScroll:
                SetLowerBits(m_parent->m_header[8 + m_index], '\x00');
                break;

            case Mode::RightScroll:
                SetLowerBits(m_parent->m_header[8 + m_index], '\x01');
                break;

            case Mode::UpScroll:
                SetLowerBits(m_parent->m_header[8 + m_index], '\x02');
                break;

            case Mode::DownScroll:
                SetLowerBits(m_parent->m_header[8 + m_index], '\x03');
                break;

            case Mode::Centered:
                SetLowerBits(m_parent->m_header[8 + m_index], '\x04');
                break;

            case Mode::Snowflake:
                SetLowerBits(m_parent->m_header[8 + m_index], '\x05');
                break;

            case Mode::DropDown:
                SetLowerBits(m_parent->m_header[8 + m_index], '\x06');
                break;

            case Mode::Curtain:
                SetLowerBits(m_parent->m_header[8 + m_index], '\x07');
                break;

            case Mode::Laser:
                SetLowerBits(m_parent->m_header[8 + m_index], '\x08');
        }
    }
}


void LedBadge::MemoryBank::SetSpeed
(
    Speed value
) {
    if ((m_parent != nullptr) && (m_index < 8)) {
        switch (value) {
            case Speed::One:
                SetHigherBits(m_parent->m_header[8 + m_index], '\x00');
                break;

            case Speed::Two:
                SetHigherBits(m_parent->m_header[8 + m_index], '\x10');
                break;

            case Speed::Three:
                SetHigherBits(m_parent->m_header[8 + m_index], '\x20');
                break;

            case Speed::Four:
                SetHigherBits(m_parent->m_header[8 + m_index], '\x30');
                break;

            case Speed::Five:
                SetHigherBits(m_parent->m_header[8 + m_index], '\x40');
                break;

            case Speed::Six:
                SetHigherBits(m_parent->m_header[8 + m_index], '\x50');
                break;

            case Speed::Seven:
                SetHigherBits(m_parent->m_header[8 + m_index], '\x60');
                break;

            case Speed::Eight:
                SetHigherBits(m_parent->m_header[8 + m_index], '\x70');
        }
    }
}


bool LedBadge::MemoryBank::SetData
(
    size_t                                         length,
    const std::function<bool(size_t x, size_t y)>& ledOn
) {
    bool ret = true;

    if ((m_parent != nullptr) && (m_index < 8)) {
        // get real length
        for (; length > 0; --length) {
            bool rowEmpty = true;

            for (size_t i = 0; i < 11; ++i) {
                if (ledOn(length - 1, i)) {
                    rowEmpty = false;
                    break;
                }
            }

            if (!rowEmpty)
                break;
        }

        std::vector<unsigned char>& bankData = m_parent->m_bankData[m_index];

        if (length > 0) {
            size_t lengthInBytes = (length - 1) / 8 + 1;

            if (lengthInBytes <= ((m_MaxSize - m_HeaderSize) / 11)) {
                bankData.resize(11 * lengthInBytes);

                for (size_t byteColumn = 0; byteColumn < lengthInBytes; ++byteColumn) {
                    for (size_t byteRow = 0; byteRow < 11; ++byteRow) {
                        unsigned char byte = '\x00';

                        for (size_t byteDigit = 0; byteDigit < 8; ++byteDigit) {
                            bool   pixel = false;
                            size_t x     = byteColumn * 8 + byteDigit;

                            if (x < length)
                                pixel = ledOn(x, byteRow);

                            SetBit(byte, 7 - byteDigit, pixel);
                        }

                        bankData[11 * byteColumn + byteRow] = byte;
                    }
                }

                m_parent->m_header[16 + 2 * m_index]     = lengthInBytes / 256;
                m_parent->m_header[16 + 2 * m_index + 1] = lengthInBytes % 256;
            }
            else {
                m_parent->Log("Error: LedBadge::MemoryBank::SetData(): Data size to hight, max length for all banks together is 5904 pixel\n");
                ret = false;
            }
        }
        else  if (bankData.size() > 0) {
            bankData.clear();

            m_parent->m_header[16 + 2 * m_index]     = '\x00';
            m_parent->m_header[16 + 2 * m_index + 1] = '\x00';
        }
    }

    return ret;
}


LedBadge::MemoryBank::MemoryBank
(
    LedBadge* parent,
    size_t    index
) : m_parent(parent), m_index(index) {
    assert(m_parent != nullptr);
    assert(m_index < 8);
}


void LedBadge::SetBrightness
(
    Brightness value
) {
    switch (value) {
        case Brightness::Full:
            m_header[5] = '\x00';
            break;

        case Brightness::High:
            m_header[5] = '\x10';
            break;

        case Brightness::Medium:
            m_header[5] = '\x20';
            break;

        case Brightness::Low:
            m_header[5] = '\x40';
    }
}


LedBadge::MemoryBank LedBadge::GetMemoryBank
(
    size_t index
) {
    assert(index < 8);

    return MemoryBank(this, index);
}

void LedBadge::SetYear
(
    unsigned char value
) {
    m_header[38] = value;
}


void LedBadge::SetMonth
(
    unsigned char value
) {
    m_header[39] = value;
}


void LedBadge::SetDay
(
    unsigned char value
) {
    m_header[40] = value;
}


void LedBadge::SetHour
(
    unsigned char value
) {
    m_header[41] = value;
}


void LedBadge::SetMinute
(
    unsigned char value
) {
    m_header[42] = value;
}


void LedBadge::SetSecond
(
    unsigned char value
) {
    m_header[43] = value;
}


bool LedBadge::FetchData
(
    std::vector<unsigned char>& dataCopy
) const {
    bool   ret      = false;
    size_t dataSize = m_HeaderSize;

    for (size_t i = 0; i < 8; ++i)
        dataSize += m_bankData[i].size();

    if (dataSize <= m_MaxSize) {
        dataCopy.clear();
        dataCopy.reserve(dataSize);

        for (size_t i = 0; i < m_HeaderSize; ++i)
            dataCopy.push_back(m_header[i]);

        for (size_t i = 0; i < 8; ++i)
            dataCopy.insert(dataCopy.end(), m_bankData[i].begin(), m_bankData[i].end());

        ret = true;
    }
    else
        Log("Error: LedBadge::FetchData(): Data size to hight, max is 8192 bytes, try to reduce the bank data\n");

    return ret;
}


void LedBadge::Log
(
    const char* logString
) const {
    if (m_logHandler != nullptr)
        (*m_logHandler)(logString);
}
