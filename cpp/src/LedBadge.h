/*                         L E D B A D G E . H
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

#include <functional>
#include <vector>

#ifndef LEDBADGE_INCLUDED
#define LEDBADGE_INCLUDED


class LedBadge {
public:
    LedBadge(std::function<void(const char* logString)>* logHandler = nullptr);
    LedBadge(const LedBadge& original);
    ~LedBadge(void);

    LedBadge&  operator=(const LedBadge& original);

    void       SetLogHandler(std::function<void(const char* logString)>* logHandler);

    enum class Brightness {
        Full,
        High,
        Medium,
        Low
    };

    enum class Mode {
        LeftScroll,
        RightScroll,
        UpScroll,
        DownScroll,
        Centered,
        Snowflake,
        DropDown,
        Curtain,
        Laser
    };

    enum class Speed {
        One,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight
    };

    class MemoryBank {
    public:
        MemoryBank(const MemoryBank& original);
        ~MemoryBank(void);

        MemoryBank& operator=(const MemoryBank& original);

        void SetBlinking(bool on);
        void SetAnimatedBorder(bool on);
        void SetMode(Mode value);
        void SetSpeed(Speed value);
        bool SetData(size_t                                         length,
                     const std::function<bool(size_t x, size_t y)>& ledOn);

    private:
        MemoryBank(LedBadge* parent,
                   size_t    index);

        LedBadge* m_parent;
        size_t    m_index;

        friend LedBadge;

        MemoryBank(void); // not implemented
    };

    void       SetBrightness(Brightness value);
    MemoryBank GetMemoryBank(size_t index);
    void       SetYear(unsigned char value);
    void       SetMonth(unsigned char value);
    void       SetDay(unsigned char value);
    void       SetHour(unsigned char value);
    void       SetMinute(unsigned char value);
    void       SetSecond(unsigned char value);

    bool       FetchData(std::vector<unsigned char>& dataCopy) const;

private:
    std::function<void(const char* logString)>* m_logHandler;
    static const size_t                         m_HeaderSize = 64;
    static const size_t                         m_MaxSize    = 8192;
    unsigned char                               m_header[m_HeaderSize];
    std::vector<unsigned char>                  m_bankData[8];

    void Log(const char* logString) const;

    friend MemoryBank;
};

#endif // LEDBADGE_INCLUDED
