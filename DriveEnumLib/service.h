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
        std::wstring readProperty(HDEVINFO devinfo, SP_DEVINFO_DATA data, Property);
    private:
        std::wstring readNormalProperty(HDEVINFO devinfo, SP_DEVINFO_DATA data, Property);
        std::wstring readInf(HDEVINFO devinfo, SP_DEVINFO_DATA data, Property);
        std::vector<std::wstring> m_publicInfs;
        std::vector<std::wstring> m_originalInfs;
        std::vector<FILETIME> m_driverDates;
        std::vector<DWORDLONG> m_driverVersions;
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

