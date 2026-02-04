#include "Ndef.h"
#include "esp_log.h"
#include <cstdio>

static const char *TAG = "Ndef";

// Borrowed from Adafruit_NFCShield_I2C
void PrintHex(const byte *data, const long numBytes)
{
    int32_t szPos;
    char buffer[3 * numBytes + 1]; // Each byte takes up to 3 characters ("0x " or "0x\0")
    char *ptr = buffer;

    for (szPos = 0; szPos < numBytes; szPos++)
    {
        ptr += sprintf(ptr, "0x%02X ", data[szPos]);
    }

    ESP_LOGI(TAG, "%s", buffer);
}

// Borrowed from Adafruit_NFCShield_I2C
void PrintHexChar(const byte *data, const long numBytes)
{
    int32_t szPos;
    char hexBuffer[3 * numBytes + 1]; // Each byte takes up to 3 characters ("0x " or "0x\0")
    char charBuffer[numBytes + 1];    // Each byte takes 1 character
    char *hexPtr = hexBuffer;
    char *charPtr = charBuffer;

    for (szPos = 0; szPos < numBytes; szPos++)
    {
        hexPtr += sprintf(hexPtr, "%02X ", data[szPos]);
        if (data[szPos] <= 0x1F)
        {
            *charPtr++ = '.';
        }
        else
        {
            *charPtr++ = (char)data[szPos];
        }
    }

    *hexPtr = '\0';
    *charPtr = '\0';

    ESP_LOGI(TAG, "%s  %s", hexBuffer, charBuffer);
}

// Note if buffer % blockSize != 0, last block will not be written
void DumpHex(const byte *data, const unsigned long numBytes, const unsigned int blockSize)
{
    unsigned int i;
    for (i = 0; i < (numBytes / blockSize); i++)
    {
        PrintHexChar(data, blockSize);
        data += blockSize;
    }
}