/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "stdint.h"
#include "PluggableUSBHID.h"
#include "PicoGamepad.h"
#include "usb_phy_api.h"

#define REPORT_ID_KEYBOARD 1
#define REPORT_ID_VOLUME 3

using namespace arduino;



PicoGamepad::PicoGamepad(bool connect, uint16_t vendor_id, uint16_t product_id, uint16_t product_release) : USBHID(get_usb_phy(), 0, 0, vendor_id, product_id, product_release)
{

}

PicoGamepad::PicoGamepad(USBPhy *phy, uint16_t vendor_id, uint16_t product_id, uint16_t product_release) : USBHID(phy, 0, 0, vendor_id, product_id, product_release)
{

}

PicoGamepad::~PicoGamepad()
{

}

const uint8_t *PicoGamepad::report_desc()
{
    static const uint8_t reportDescriptor[] = {  // this is the same as the TX12 HID report
    
     0x05,0x01,    // Usage page Generic Desktop controls 
     0x09,0x05,    // Usage Game pad
     0xa1,0x01,    // Collection Application
                
        0xa1,0x00,         // Collection Physical
            0x05,0x09,      // Usage Page (button)
            0x19,0x01,      // Usage minimum (0x01)
            0x29,0x18,      // Usage maximum (0x18)
            0x15,0x00,      // Logical minimum (0)
            0x25,0x01,      // Logical maximum (1)
            0x95,0x18,      // Report count (24)
            0x75,0x01,      // Report size (1)
            0x81,0x02,      // Input (Data,Var,Abs)
            0x05,0x01,      // Usage page (Generic Desktop Controls)
            0x09,0x30,      // Usage (X)
            0x09,0x31,      // Usage (Y)
            0x09,0x32,      // Usage (Z)
            0x09,0x33,      // Usage (Rx)
            0x09,0x34,      // Usage (S1)(Ry)
            0x09,0x35,      // Usage (S2)(Rz)
            0x16,0x01,0x80, //Logical minimum (-32767)
            0x26,0xff,0x7f, //Logical miximum (32767)
            0x75,0x10,      // Report size(16)
            0x95,0x06,      // Report count
            0x81,0x02,      // Input (Data,Var,Abs)
            0x05, 0x01,    // Usage Page (Generic Desktop Controls)
            0x09, 0x36,    // Usage (Slider)
            0x09, 0x36,    // Usage (Slider)
            //0x09, 0x36,    // Usage (Slider)            
            0x16, 0x01, 0x80, // Logical Minimum (-32767)
            0x26, 0xFF, 0x7F, // Logical Maximum (32767)
            0x75, 0x10,    // Report Size (16)
            0x95, 0x02,    // Report Count (2)
            0x81, 0x02,    // Input (Data, Var, Abs)


            



        0xc0,   // End Collection

    0xc0   // End Collection

    };
    reportLength = sizeof(reportDescriptor);
    return reportDescriptor;
}






void PicoGamepad::SetButton(int idx, bool val)
{
    if (idx > 23 || idx < 0)
    {
        return;
    }
    bitWrite(inputArray[(idx / 8)], idx % 8, val);
}

void PicoGamepad::SetX(uint16_t val)
{
    inputArray[X_AXIS_LSB] = LSB(val);
    inputArray[X_AXIS_MSB] = MSB(val);
}

void PicoGamepad::SetY(uint16_t val)
{
    inputArray[Y_AXIS_LSB] = LSB(val);
    inputArray[Y_AXIS_MSB] = MSB(val);
}

void PicoGamepad::SetZ(uint16_t val)
{
    inputArray[Z_AXIS_LSB] = LSB(val);
    inputArray[Z_AXIS_MSB] = MSB(val);
}

void PicoGamepad::SetRx(uint16_t val)
{
    inputArray[Rx_AXIS_LSB] = LSB(val);
    inputArray[Rx_AXIS_MSB] = MSB(val);
}

void PicoGamepad::SetS1(uint16_t val)
{
    inputArray[S1_AXIS_LSB] = LSB(val);
    inputArray[S1_AXIS_MSB] = MSB(val);
}

void PicoGamepad::SetS2(uint16_t val)
{
    inputArray[S2_AXIS_LSB] = LSB(val);
    inputArray[S2_AXIS_MSB] = MSB(val);
}

void PicoGamepad::SetSB(uint16_t val)
{
    inputArray[SB_AXIS_LSB] = LSB(val);
    inputArray[SB_AXIS_MSB] = MSB(val);
}

void PicoGamepad::SetSC(uint16_t val)
{
    inputArray[SC_AXIS_LSB] = LSB(val);
    inputArray[SC_AXIS_MSB] = MSB(val);
}

void PicoGamepad::SetSE(uint16_t val)
{
    inputArray[SE_AXIS_LSB] = LSB(val);
    inputArray[SE_AXIS_MSB] = MSB(val);
}

void PicoGamepad::SetSF(uint16_t val)
{
    inputArray[SF_AXIS_LSB] = LSB(val);
    inputArray[SF_AXIS_MSB] = MSB(val);
}

void PicoGamepad::clearReport(void)
{
    for (int i = 0; i < inputArraySize; i++)
    {
        inputArray[i]=0;

    }

    
}




bool PicoGamepad::send_update()
{
    _mutex.lock();

    HID_REPORT report;
    for (int i = 0; i < inputArraySize; i++)
    {
        report.data[i] = inputArray[i];

    }
    
    report.length = inputArraySize;
 
    if (!send(&report))
    {
        _mutex.unlock();
        return false;
    }

    _mutex.unlock();
    return true;
}



#define DEFAULT_CONFIGURATION (1)
#define TOTAL_DESCRIPTOR_LENGTH ((1 * CONFIGURATION_DESCRIPTOR_LENGTH) + (1 * INTERFACE_DESCRIPTOR_LENGTH) + (1 * HID_DESCRIPTOR_LENGTH) + (2 * ENDPOINT_DESCRIPTOR_LENGTH))

const uint8_t *PicoGamepad::configuration_desc(uint8_t index)
{
    if (index != 0)
    {
        return NULL;
    }
    uint8_t configuration_descriptor_temp[] = {
        CONFIGURATION_DESCRIPTOR_LENGTH, // bLength
        CONFIGURATION_DESCRIPTOR,        // bDescriptorType
        LSB(TOTAL_DESCRIPTOR_LENGTH),    // wTotalLength (LSB)
        MSB(TOTAL_DESCRIPTOR_LENGTH),    // wTotalLength (MSB)
        0x01,                            // bNumInterfaces
        DEFAULT_CONFIGURATION,           // bConfigurationValue
        0x00,                            // iConfiguration
        C_RESERVED | C_SELF_POWERED,     // bmAttributes
        C_POWER(0),                      // bMaxPower

        INTERFACE_DESCRIPTOR_LENGTH, // bLength
        INTERFACE_DESCRIPTOR,        // bDescriptorType
        0x00,                        // bInterfaceNumber
        0x00,                        // bAlternateSetting
        0x02,                        // bNumEndpoints
        HID_CLASS,                   // bInterfaceClass
        HID_SUBCLASS_BOOT,           // bInterfaceSubClass
        HID_PROTOCOL_KEYBOARD,       // bInterfaceProtocol
        0x00,                        // iInterface

        HID_DESCRIPTOR_LENGTH,                // bLength
        HID_DESCRIPTOR,                       // bDescriptorType
        LSB(HID_VERSION_1_11),                // bcdHID (LSB)
        MSB(HID_VERSION_1_11),                // bcdHID (MSB)
        0x00,                                 // bCountryCode
        0x01,                                 // bNumDescriptors
        REPORT_DESCRIPTOR,                    // bDescriptorType
        (uint8_t)(LSB(report_desc_length())), // wDescriptorLength (LSB)
        (uint8_t)(MSB(report_desc_length())), // wDescriptorLength (MSB)

        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _int_in,                    // bEndpointAddress
        E_INTERRUPT,                // bmAttributes
        LSB(MAX_HID_REPORT_SIZE),   // wMaxPacketSize (LSB)
        MSB(MAX_HID_REPORT_SIZE),   // wMaxPacketSize (MSB)
        1,                          // bInterval (milliseconds)

        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _int_out,                   // bEndpointAddress
        E_INTERRUPT,                // bmAttributes
        LSB(MAX_HID_REPORT_SIZE),   // wMaxPacketSize (LSB)
        MSB(MAX_HID_REPORT_SIZE),   // wMaxPacketSize (MSB)
        1,                          // bInterval (milliseconds)
    };
    MBED_ASSERT(sizeof(configuration_descriptor_temp) == sizeof(_configuration_descriptor));
    memcpy(_configuration_descriptor, configuration_descriptor_temp, sizeof(_configuration_descriptor));
    return _configuration_descriptor;
}
