#pragma once

#include <windows.h>
#include <setupapi.h>
#include <string>
#include <SetupAPI.h>
#include <vector>


typedef LONG_PTR DriverHandle;

#define DeclearExtraEnum(CUR, PRE, STEP) \
ePublicInf##CUR = ePublicInf##PRE + STEP, \
eOriginalInf##CUR = eOriginalInf##PRE + STEP, \
eDriverDate##CUR = eDriverDate##PRE + STEP, \
eDriverVersion##CUR = eDriverVersion##PRE + STEP,

namespace DriveEnum
{
    enum Property : unsigned int
    {
        eDescription = SPDRP_DEVICEDESC,
        eHardwareID = SPDRP_HARDWAREID,
        eCompatibleIDs = SPDRP_COMPATIBLEIDS,
        eService = SPDRP_SERVICE,
        eClass = SPDRP_CLASS,
        eClassGUID = SPDRP_CLASSGUID,
        eDriver = SPDRP_DRIVER,
        eConfigFlag = SPDRP_CONFIGFLAGS,
        eMFG = SPDRP_MFG,
        eFriendlyName = SPDRP_FRIENDLYNAME,
        eLocationInformation = SPDRP_LOCATION_INFORMATION,
        ePhysicalDeviceObjectName = SPDRP_PHYSICAL_DEVICE_OBJECT_NAME,
        eCapabilities = SPDRP_CAPABILITIES,
        eUINumber = SPDRP_UI_NUMBER,
        eUpperFilters = SPDRP_UPPERFILTERS,
        eLowerFilters = SPDRP_LOWERFILTERS,
        eBusTypeGUID = SPDRP_BUSTYPEGUID,
        eLegacyBusType = SPDRP_LEGACYBUSTYPE,
        eBusNumber = SPDRP_BUSNUMBER,
        eEnumeratorName = SPDRP_ENUMERATOR_NAME,
        eSecurityBinary = SPDRP_SECURITY,
        eSecuritySDS = SPDRP_SECURITY_SDS,
        eDevType = SPDRP_DEVTYPE,
        eExclusive = SPDRP_EXCLUSIVE,
        eCharacteristics = SPDRP_CHARACTERISTICS,
        eAddress = SPDRP_ADDRESS,
        eUiNumberDescFormat = SPDRP_UI_NUMBER_DESC_FORMAT,
        ePowerData = SPDRP_DEVICE_POWER_DATA,
        eInstallState = SPDRP_INSTALL_STATE,
        eLocationPaths = SPDRP_LOCATION_PATHS,
        eBaseContainerID = SPDRP_BASE_CONTAINERID,
        ePropertyCount = SPDRP_MAXIMUM_PROPERTY,
        ePublicInf0 = SPDRP_MAXIMUM_PROPERTY + 1,
        eOriginalInf0 = SPDRP_MAXIMUM_PROPERTY + 2,
        eDriverDate0 = SPDRP_MAXIMUM_PROPERTY + 3,
        eDriverVersion0 = SPDRP_MAXIMUM_PROPERTY + 4,
        DeclearExtraEnum(1, 0, 4) 
        DeclearExtraEnum(2, 1, 4)
        DeclearExtraEnum(3, 2, 4)
        DeclearExtraEnum(4, 3, 4)
    };

    enum EnumStyle
    {
        eAll = 0,
        eCurrent = 1,
        eUnknownStyle = 2
    };

    
    struct Value
    {
        Value()
        {
            ver.fileversion = 0;
        }
        Value(std::wstring v) : string(v)
        {
            if (v.size() > 0)
                ver.fileversion = 1;
            else
                ver.fileversion = 0;
        };
        Value(FILETIME time)
        {
            ver.filetime = time;
        }
        Value(DWORDLONG v)
        {
            ver.fileversion = v;
        }

        bool valid() const
        {
            return ver.fileversion > 0;
        }

        std::wstring String() const
        {
            return string;
        }

        FILETIME FileTime() const
        {
            return ver.filetime;
        }

        DWORDLONG FileVersion() const
        {
            return ver.fileversion;
        }

    protected:
        std::wstring string;

        union Version
        {
            FILETIME filetime;
            DWORDLONG fileversion;
        } ver;
    };



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
        Device(const Device&);
        virtual ~Device();

        const Device& operator = (const Device&);

        std::wstring propertyAsWString(const Property) const;
        GUID propertyAsGUID(const Property) const;
        DeviceID propertyAsID(const Property) const;
        DWORDLONG propertyAsDWORDLONG(const Property) const;
        FILETIME propertyAsFILETIME(const Property) const;
    protected:
        friend class DeviceManagerImp;
        DeviceImp* m_pImp;
    }; 

    class DevMgrProcessProgress
    {
    public:
        virtual ~DevMgrProcessProgress() {};

        virtual void BeforeProcess(unsigned int i, unsigned int total) = 0;
        virtual void AfterProcess(Device* pDevice, unsigned int i, unsigned int total) = 0;
    };

    class DeviceManagerImp;
    class DeviceManager
    {
    public:
        DeviceManager(EnumStyle);
        ~DeviceManager();
        bool Process(DevMgrProcessProgress* pProgress, bool reset = true);
        unsigned int Count();
        const Device* GetDevice(unsigned int index) const;
        Device* GetDevice(unsigned int index);
        void AddProperty(Property);
        void AddProperties(std::vector<Property>&);
    protected:
        DeviceManagerImp* m_pImp;
    };
};

