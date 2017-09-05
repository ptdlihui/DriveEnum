#include "service.h"
#include <string>

namespace DriveEnum
{
    bool readInfImp(HDEVINFO& hDevInfo, SP_DEVINFO_DATA& DeviceInfoData, std::vector<std::wstring>& publicInfs, std::vector<std::wstring> & originalInfs)
    {
        SP_DRVINFO_DATA DriverInfoData;
        std::memset(&DriverInfoData, 0, sizeof(SP_DRVINFO_DATA));
        DriverInfoData.cbSize = sizeof(SP_DRVINFO_DATA);

        SP_DEVINSTALL_PARAMS deviceInstallParams;
        ZeroMemory(&deviceInstallParams, sizeof(deviceInstallParams));
        deviceInstallParams.cbSize = sizeof(SP_DEVINSTALL_PARAMS);

        if (!SetupDiGetDeviceInstallParams(hDevInfo, &DeviceInfoData, &deviceInstallParams))
            return false;

        deviceInstallParams.FlagsEx |= (DI_FLAGSEX_INSTALLEDDRIVER | DI_FLAGSEX_ALLOWEXCLUDEDDRVS);

        if (SetupDiSetDeviceInstallParams(hDevInfo, &DeviceInfoData, &deviceInstallParams))
        {
            if (!SetupDiBuildDriverInfoList(hDevInfo, &DeviceInfoData, SPDIT_CLASSDRIVER)) {
                return false;
            }

            int drvIndex = 0;
            while (SetupDiEnumDriverInfo(hDevInfo, &DeviceInfoData, SPDIT_CLASSDRIVER,
                drvIndex++, &DriverInfoData))
            {
                DWORD realSize = 0;
                std::vector<char> data;
                data.resize(sizeof(SP_DRVINFO_DETAIL_DATA));
                std::memset(data.data(), 0, data.size());
                reinterpret_cast<PSP_DRVINFO_DETAIL_DATA>(data.data())->cbSize = sizeof(SP_DRVINFO_DETAIL_DATA);

                while (!SetupDiGetDriverInfoDetail(hDevInfo, &DeviceInfoData, &DriverInfoData, reinterpret_cast<PSP_DRVINFO_DETAIL_DATA>(data.data()), data.size(), &realSize))
                {
                    data.resize(realSize);
                }

                std::wstring dPath(reinterpret_cast<PSP_DRVINFO_DETAIL_DATA>(data.data())->InfFileName);
                std::vector<wchar_t> originalPath;
                originalPath.resize(1024);
                DWORD realsize = 0;
                SetupGetInfDriverStoreLocation(dPath.c_str(), nullptr, nullptr, originalPath.data(), originalPath.size() * 2, &realsize);

                publicInfs.push_back(dPath);
                std::wstring orignalPathStr(originalPath.data());
                originalInfs.push_back(orignalPathStr);

            }
        }
        else
            return false;

        deviceInstallParams.FlagsEx &= ~(DI_FLAGSEX_INSTALLEDDRIVER | DI_FLAGSEX_ALLOWEXCLUDEDDRVS);

        if (NO_ERROR != GetLastError() && ERROR_NO_MORE_ITEMS != GetLastError())
        {
            return true;
        }

        return false;
    }

    std::wstring EnumCallback::readInf(HDEVINFO devinfo, SP_DEVINFO_DATA data, Property prop)
    {
        if (m_publicInfs.size() == 0
            || m_originalInfs.size() == 0)
        {
            m_publicInfs.clear();
            m_originalInfs.clear();

            if (readInfImp(devinfo, data, m_publicInfs, m_originalInfs) == false)
                return std::wstring();
        }

        unsigned int index = prop - ePropertyCount;
        unsigned int propIndex = (index - 1) / 2 ;
        bool isOriginal = ((index - 1) % 2 == 1);

        std::vector<std::wstring>& infs = isOriginal ? m_originalInfs : m_publicInfs;

        if (propIndex < infs.size())
            return infs[propIndex];

        return std::wstring();

    }

    std::wstring EnumCallback::readNormalProperty(HDEVINFO hDevInfo, SP_DEVINFO_DATA DeviceInfoData, Property prop)
    {
        DWORD  DataT;
        wchar_t* buffer = NULL;
        DWORD  buffersize = 0;

        //
        // Call function with null to begin with, 
        // then use the returned buffer size (doubled)
        // to Alloc the buffer. Keep calling until
        // success or an unknown failure.
        //
        //  Double the returned buffersize to correct
        //  for underlying legacy CM functions that 
        //  return an incorrect buffersize value on 
        //  DBCS/MBCS systems.
        // 
        while (!SetupDiGetDeviceRegistryProperty(
            hDevInfo,
            &DeviceInfoData,
            prop,
            &DataT,
            (PBYTE)buffer,
            buffersize,
            &buffersize))
        {
            if (ERROR_INSUFFICIENT_BUFFER == GetLastError())
            {
                // Change the buffer size.
                if (buffer)
                {
                    LocalFree(buffer);
                }
                // Double the size to avoid problems on 
                // W2k MBCS systems per KB 888609. 
                buffer = (wchar_t*)LocalAlloc(LPTR, buffersize * 2);
            }
            else
            {
                break;
            }
        }



        std::wstring returnStr;
        if (buffer)
            returnStr.assign(buffer, buffersize);

        //wprintf(L"%s %s\n", Label, buffer);
        if (buffer)
        {
            LocalFree(buffer);
        }

        return returnStr;
    }


    
}