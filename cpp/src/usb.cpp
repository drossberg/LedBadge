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

#include "hidapi.h"

#include "usb.h"


void SendToUsb
(
    std::vector<unsigned char>& data
) {
    if (hid_init() == 0) {
        hid_device* ledBadgeDevice = hid_open(0x0416, 0x5020, nullptr);

        if (ledBadgeDevice != nullptr) {
            data.insert(data.begin(), '\x00'); // Report ID
            hid_write(ledBadgeDevice, data.data(), data.size());
            hid_close(ledBadgeDevice);
        }

        hid_exit();
    }
}
