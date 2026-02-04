#include "NdefRecord.h"
#include "esp_log.h"
#include <cstdlib>
#include <cstring>

static const char *TAG = "NdefRecord";

NdefRecord::NdefRecord()
{
    _tnf = NdefRecord::TNF_EMPTY;
    _typeLength = 0;
    _payloadLength = 0;
    _idLength = 0;
    _type = (byte *)NULL;
    _payload = (byte *)NULL;
    _id = (byte *)NULL;
}

NdefRecord::NdefRecord(const NdefRecord& rhs)
{
    _tnf = rhs._tnf;
    _typeLength = rhs._typeLength;
    _payloadLength = rhs._payloadLength;
    _idLength = rhs._idLength;
    _type = (byte *)NULL;
    _payload = (byte *)NULL;
    _id = (byte *)NULL;

    if (_typeLength)
    {
        _type = (byte*)malloc(_typeLength);
        memcpy(_type, rhs._type, _typeLength);
    }

    if (_payloadLength)
    {
        _payload = (byte*)malloc(_payloadLength);
        memcpy(_payload, rhs._payload, _payloadLength);
    }

    if (_idLength)
    {
        _id = (byte*)malloc(_idLength);
        memcpy(_id, rhs._id, _idLength);
    }
}

NdefRecord::~NdefRecord()
{
    free(_type);
    free(_payload);
    free(_id);
}

NdefRecord& NdefRecord::operator=(const NdefRecord& rhs)
{
    if (this != &rhs)
    {
        // free existing
        if (_typeLength)
        {
            free(_type);
        }

        if (_payloadLength)
        {
            free(_payload);
        }

        if (_idLength)
        {
            free(_id);
        }

        _tnf = rhs._tnf;
        _typeLength = rhs._typeLength;
        _payloadLength = rhs._payloadLength;
        _idLength = rhs._idLength;

        if (_typeLength)
        {
            _type = (byte*)malloc(_typeLength);
            memcpy(_type, rhs._type, _typeLength);
        }

        if (_payloadLength)
        {
            _payload = (byte*)malloc(_payloadLength);
            memcpy(_payload, rhs._payload, _payloadLength);
        }

        if (_idLength)
        {
            _id = (byte*)malloc(_idLength);
            memcpy(_id, rhs._id, _idLength);
        }
    }
    return *this;
}

unsigned int NdefRecord::getEncodedSize()
{
    unsigned int size = 2; // tnf + typeLength
    if (_payloadLength > 0xFF)
    {
        size += 4;
    }
    else
    {
        size += 1;
    }

    if (_idLength)
    {
        size += 1;
    }

    size += (_typeLength + _payloadLength + _idLength);

    return size;
}

void NdefRecord::encode(byte *data, bool firstRecord, bool lastRecord)
{
    uint8_t* data_ptr = &data[0];

    *data_ptr = _getTnfByte(firstRecord, lastRecord);
    data_ptr += 1;

    *data_ptr = _typeLength;
    data_ptr += 1;

    if (_payloadLength <= 0xFF) {  // short record
        *data_ptr = _payloadLength;
        data_ptr += 1;
    } else { // long format
        data_ptr[0] = 0x0; // (_payloadLength >> 24) & 0xFF;
        data_ptr[1] = 0x0; // (_payloadLength >> 16) & 0xFF;
        data_ptr[2] = (_payloadLength >> 8) & 0xFF;
        data_ptr[3] = _payloadLength & 0xFF;
        data_ptr += 4;
    }

    if (_idLength)
    {
        *data_ptr = _idLength;
        data_ptr += 1;
    }

    memcpy(data_ptr, _type, _typeLength);
    data_ptr += _typeLength;

    if (_idLength)
    {
        memcpy(data_ptr, _id, _idLength);
        data_ptr += _idLength;
    }
    
    memcpy(data_ptr, _payload, _payloadLength);
    data_ptr += _payloadLength;
}

byte NdefRecord::_getTnfByte(bool firstRecord, bool lastRecord)
{
    int value = _tnf;

    if (firstRecord) { // mb
        value = value | 0x80;
    }

    if (lastRecord) { //
        value = value | 0x40;
    }

    if (_payloadLength <= 0xFF) {
        value = value | 0x10;
    }

    if (_idLength) {
        value = value | 0x8;
    }

    return value;
}

NdefRecord::TNF NdefRecord::getTnf()
{
    return _tnf;
}

void NdefRecord::setTnf(NdefRecord::TNF tnf)
{
    _tnf = tnf;
}

unsigned int NdefRecord::getTypeLength()
{
    return _typeLength;
}

unsigned int NdefRecord::getPayloadLength()
{
    return _payloadLength;
}

unsigned int NdefRecord::getIdLength()
{
    return _idLength;
}

const byte* NdefRecord::getType()
{
    return _type;
}

void NdefRecord::setType(const byte *type, const unsigned int numBytes)
{
    free(_type);

    _type = (uint8_t*)malloc(numBytes);
    memcpy(_type, type, numBytes);
    _typeLength = numBytes;
}

const byte* NdefRecord::getPayload()
{
    return _payload;
}

void NdefRecord::setPayload(const byte *payload, const int numBytes)
{
    free(_payload);

    _payload = (byte*)malloc(numBytes);
    memcpy(_payload, payload, numBytes);
    _payloadLength = numBytes;
}

void NdefRecord::setPayload(const byte *header, const int headerLength, const byte *payload, const int payloadLength)
{
    free(_payload);

    _payload = (byte*)malloc(headerLength+payloadLength);
    memcpy(_payload, header, headerLength);
    memcpy(_payload+headerLength, payload, payloadLength);
    _payloadLength = headerLength+payloadLength;
}

const byte* NdefRecord::getId()
{
    return _id;
}

void NdefRecord::setId(const byte *id, const unsigned int numBytes)
{
    free(_id);

    _id = (byte*)malloc(numBytes);
    memcpy(_id, id, numBytes);
    _idLength = numBytes;
}

void NdefRecord::print()
{
    ESP_LOGI(TAG, "  NDEF Record");
    ESP_LOGI(TAG, "    TNF 0x%x ", _tnf);
    switch (_tnf) {
    case TNF_EMPTY:
        ESP_LOGI(TAG, "Empty");
        break;
    case TNF_WELL_KNOWN:
        ESP_LOGI(TAG, "Well Known");
        break;
    case TNF_MIME_MEDIA:
        ESP_LOGI(TAG, "Mime Media");
        break;
    case TNF_ABSOLUTE_URI:
        ESP_LOGI(TAG, "Absolute URI");
        break;
    case TNF_EXTERNAL_TYPE:
        ESP_LOGI(TAG, "External");
        break;
    case TNF_UNKNOWN:
        ESP_LOGI(TAG, "Unknown");
        break;
    case TNF_UNCHANGED:
        ESP_LOGI(TAG, "Unchanged");
        break;
    case TNF_RESERVED:
        ESP_LOGI(TAG, "Reserved");
        break;
    }
    ESP_LOGI(TAG, "    Type Length 0x%x %d", _typeLength, _typeLength);
    ESP_LOGI(TAG, "    Payload Length 0x%x %d", _payloadLength, _payloadLength);
    if (_idLength)
    {
        ESP_LOGI(TAG, "    Id Length 0x%x", _idLength);
    }
    ESP_LOGI(TAG, "    Type ");
    PrintHexChar(_type, _typeLength);
    ESP_LOGI(TAG, "    Payload ");
    PrintHexChar(_payload, _payloadLength);
    if (_idLength)
    {
        ESP_LOGI(TAG, "    Id ");
        PrintHexChar(_id, _idLength);
    }
    ESP_LOGI(TAG, "    Record is %d bytes", getEncodedSize());
}