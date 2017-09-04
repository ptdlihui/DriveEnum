#pragma once

#include <windows.h>
#include <setupapi.h>
#include <string>
#include <SetupAPI.h>
#include <unordered_map>

typedef LONG_PTR DriverHandle;

namespace DeviceEnum
{
    enum Property
    {
        eDescription = SPDRP_DEVICEDESC,

    };

    typedef std::unordered_map<Property, std::wstring> Properties;

    struct DeviceID
    {
        unsigned short VID = 0;
        unsigned short PID = 0;
    };
    class DeviceImp;

    class Device
    {
    public:
        Device();
        ~Device();

        std::wstring propertyAsWString(const Property) const;
        GUID propertyAsGUID(const Property) const;
        DeviceID propertyAsID(const Property) const;
    protected:
        DeviceImp* m_pImp;
    };
};

