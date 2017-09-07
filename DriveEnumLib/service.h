#pragma once
#include "deviceenum.h"

namespace DriveEnum
{

    class EnumCallback
    {
    public:
        virtual ~EnumCallback() {}
        virtual void Process(HDEVINFO& devInfo, SP_DEVINFO_DATA& data, DWORD index) = 0;
    protected:
        Value readProperty(HDEVINFO devinfo, SP_DEVINFO_DATA data, Property);
    private:
        std::wstring readNormalProperty(HDEVINFO devinfo, SP_DEVINFO_DATA data, Property);
        Value readInf(HDEVINFO devinfo, SP_DEVINFO_DATA data, Property);
        std::vector<Value> m_publicInfs;
        std::vector<Value> m_originalInfs;
        std::vector<Value> m_driverDates;
        std::vector<Value> m_driverVersions;
    };

    
    class EnumService
    {
    public:
        EnumService(EnumStyle style) : m_style(style) {}
        bool EnumBody(EnumCallback* );
    protected:
        EnumStyle m_style;
    };
};

