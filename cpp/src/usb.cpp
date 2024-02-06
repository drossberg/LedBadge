/*                                   U S B . C P P
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

#include <sstream>

#include "hidapi.h"

#include "usb.h"


static void Log
(
    std::function<void(const char* logString)>* logHandler,
    const char*                                 logString
) {
    if (logHandler != nullptr)
        (*logHandler)(logString);
}


void SendToUsb
(
    std::vector<unsigned char>&                 data,
    std::function<void(const char* logString)>* logHandler
) {
    if (hid_init() == 0) {
        hid_device* ledBadgeDevice = hid_open(0x0416, 0x5020, nullptr);

        if (ledBadgeDevice != nullptr) {
            data.insert(data.begin(), '\x00'); // Report ID

            std::stringstream logstream;
            logstream << "Info: SendToUsb(): Writing " << data.size() << " bytes\n";
            Log(logHandler, logstream.str().c_str());

            int bytesWritten = hid_write(ledBadgeDevice, data.data(), data.size());

            logstream.str().clear();
            logstream << "Info: SendToUsb(): " << bytesWritten << " bytes written\n";
            Log(logHandler, logstream.str().c_str());

            hid_close(ledBadgeDevice);
        }
        else
            Log(logHandler, "Error: SendToUsb(): Cannot open LED Badge device, maybe not connected?\n");

        hid_exit();
    }
    else
        Log(logHandler, "Error: SendToUsb(): Cannot initialize HID\n");
}
