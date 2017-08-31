// EnumDevice.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <setupapi.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>


std::wstring get_property
(
    __in HDEVINFO        hDevInfo,
    __in SP_DEVINFO_DATA DeviceInfoData,
    __in PCWSTR          Label,
    __in DWORD           Property
)
{
    DWORD  DataT;
    LPTSTR buffer = NULL;
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
        Property,
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
            buffer = (LPTSTR)LocalAlloc(LPTR, buffersize * 2);
        }
        else
        {
            break;
        }
    }



    std::wstring returnStr;
    if (buffer)
        returnStr.assign(buffer, buffersize);

    std::wcout.clear();
    std::wcout << Label << returnStr << std::endl;
    //wprintf(L"%s %s\n", Label, buffer);
    if (buffer)
    {
        LocalFree(buffer);
    }



    return returnStr;
}

std::ostream& operator<<(std::ostream& os, REFGUID guid) {

    os << std::uppercase;
    os.width(8);
    os << std::hex << guid.Data1 << '-';

    os.width(4);
    os << std::hex << guid.Data2 << '-';

    os.width(4);
    os << std::hex << guid.Data3 << '-';

    os.width(2);
    os << std::hex
        << static_cast<short>(guid.Data4[0])
        << static_cast<short>(guid.Data4[1])
        << '-'
        << static_cast<short>(guid.Data4[2])
        << static_cast<short>(guid.Data4[3])
        << static_cast<short>(guid.Data4[4])
        << static_cast<short>(guid.Data4[5])
        << static_cast<short>(guid.Data4[6])
        << static_cast<short>(guid.Data4[7]);
    os << std::nouppercase;
    return os;
}

int main()
{
    HDEVINFO hDevInfo;
    SP_DEVINFO_DATA DeviceInfoData;
    DWORD i;

    // Create a HDEVINFO with all present devices.
    hDevInfo = SetupDiGetClassDevs(
        NULL,
        0, // Enumerator
        0,
        DIGCF_PRESENT | DIGCF_ALLCLASSES);

    if (INVALID_HANDLE_VALUE == hDevInfo)
    {
        // Insert error handling here.
        return 1;
    }

    // Enumerate through all devices in Set.

    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    for (i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData); i++)
    {
        LPTSTR buffer = NULL;
        DWORD  buffersize = 0;

        SP_DRVINFO_DATA DriverInfoData;
        std::memset(&DriverInfoData, 0, sizeof(SP_DRVINFO_DATA));
        DriverInfoData.cbSize = sizeof(SP_DRVINFO_DATA);

        SP_DEVINSTALL_PARAMS deviceInstallParams;
        ZeroMemory(&deviceInstallParams, sizeof(deviceInstallParams));
        deviceInstallParams.cbSize = sizeof(SP_DEVINSTALL_PARAMS);

        //std::wcout << L"Friendly name :" << get_property(hDevInfo, DeviceInfoData, L"Friendly name :", SPDRP_FRIENDLYNAME) << std::endl;
        //std::wcout << L"MFG:" << get_property(hDevInfo, DeviceInfoData, L"MFG:", SPDRP_MFG) << std::endl;
        //std::wcout << L"Enum name:" << get_property(hDevInfo, DeviceInfoData, L"Enum name:", SPDRP_ENUMERATOR_NAME) << std::endl;
        //std::wcout << L"Device type:" << get_property(hDevInfo, DeviceInfoData, L"Device type:", SPDRP_DRIVER) << std::endl;
        //std::wcout << L"Desc:" << get_property(hDevInfo, DeviceInfoData, L"Desc:", SPDRP_DEVICEDESC) << std::endl;
        //std::wcout << L"HardwareID:" << get_property(hDevInfo, DeviceInfoData, L"HardwareID:", SPDRP_HARDWAREID) << std::endl;
        //std::wcout << L"PhysicalID:" << get_property(hDevInfo, DeviceInfoData, L"PhysicalID:", SPDRP_PHYSICAL_DEVICE_OBJECT_NAME) << std::endl;
        //std::wcout << L"LocalPath:" << get_property(hDevInfo, DeviceInfoData, L"LocalPath:", SPDRP_LOCATION_PATHS) << std::endl;
        //std::wcout << L"Install State:" << get_property(hDevInfo, DeviceInfoData, L"Install State:", SPDRP_INSTALL_STATE) << std::endl;
        //std::wcout << L"PathInformation:" << get_property(hDevInfo, DeviceInfoData, L"PathInformation:", SPDRP_LOCATION_INFORMATION) << std::endl;
        get_property(hDevInfo, DeviceInfoData, L"Friendly name :", SPDRP_FRIENDLYNAME);
        get_property(hDevInfo, DeviceInfoData, L"MFG:", SPDRP_MFG) ;
        get_property(hDevInfo, DeviceInfoData, L"Enum name:", SPDRP_ENUMERATOR_NAME);
        get_property(hDevInfo, DeviceInfoData, L"Device type:", SPDRP_DRIVER) ;
        get_property(hDevInfo, DeviceInfoData, L"Desc:", SPDRP_DEVICEDESC);
        get_property(hDevInfo, DeviceInfoData, L"HardwareID:", SPDRP_HARDWAREID) ;
        get_property(hDevInfo, DeviceInfoData, L"PhysicalID:", SPDRP_PHYSICAL_DEVICE_OBJECT_NAME) ;
        get_property(hDevInfo, DeviceInfoData, L"LocalPath:", SPDRP_LOCATION_PATHS) ;
        get_property(hDevInfo, DeviceInfoData, L"Install State:", SPDRP_INSTALL_STATE) ;
        get_property(hDevInfo, DeviceInfoData, L"PathInformation:", SPDRP_LOCATION_INFORMATION);
        //while (!SetupDiGetDeviceInstanceId(
        //    hDevInfo,
        //    &DeviceInfoData,
        //    buffer,
        //    buffersize,
        //    &buffersize))
        //{
        //    if (buffer)
        //    {
        //        LocalFree(buffer);
        //    }

        //    if (ERROR_INSUFFICIENT_BUFFER == GetLastError())
        //    {
        //        // Change the buffer size.
        //        // Double the size to avoid problems on
        //        // W2k MBCS systems per KB 888609.
        //        buffer = (LPTSTR)LocalAlloc(LPTR, buffersize * 2);
        //    }
        //    else
        //    {
        //        wprintf(L"error: could not get device instance id (0x%x)\n", GetLastError());
        //        break;
        //    }

        //    if (buffer)
        //    {
        //        wprintf(L"\tDeviceInstanceId : %s\n", buffer);
        //    }
        //}



        get_property(hDevInfo, DeviceInfoData, L"Class :", SPDRP_CLASS);
        get_property(hDevInfo, DeviceInfoData, L"Class GUID :", SPDRP_CLASSGUID);

        if (SetupDiGetDeviceInstallParams(hDevInfo, &DeviceInfoData, &deviceInstallParams)) {
            //std::wstring dPath(deviceInstallParams.DriverPath);
            //std::wcout << dPath << std::endl;
        }


            deviceInstallParams.FlagsEx |= (DI_FLAGSEX_INSTALLEDDRIVER | DI_FLAGSEX_ALLOWEXCLUDEDDRVS);

            if (SetupDiSetDeviceInstallParams(hDevInfo, &DeviceInfoData, &deviceInstallParams)) {
                //
                // we were able to specify this flag, so proceed the easy way
                // we should get a list of no more than 1 driver
                //
                if (!SetupDiBuildDriverInfoList(hDevInfo, &DeviceInfoData, SPDIT_CLASSDRIVER)) {
                    //return FALSE;
                }

                int drvIndex = 0;
                while (SetupDiEnumDriverInfo(hDevInfo, &DeviceInfoData, SPDIT_CLASSDRIVER,
                    drvIndex++, &DriverInfoData)) {
                    //return FALSE;
                    //std::wstring dPath(DriverInfoData.ProviderName);
                    //std::wcout << dPath << std::endl;

                    DWORD realSize = 0;
                    std::vector<char> data;
                    data.resize(sizeof(SP_DRVINFO_DETAIL_DATA));
                    std::memset(data.data(), 0, data.size());
                    reinterpret_cast<PSP_DRVINFO_DETAIL_DATA>(data.data())->cbSize = sizeof(SP_DRVINFO_DETAIL_DATA);

                    while (!SetupDiGetDriverInfoDetail(hDevInfo, &DeviceInfoData, &DriverInfoData, reinterpret_cast<PSP_DRVINFO_DETAIL_DATA>(data.data()), data.size(), &realSize))
                        //while (!SetupDiGetDriverInfoDetail(hDevInfo, &DeviceInfoData, &DriverInfoData, data.data(), sizeof(SP_DRVINFO_DETAIL_DATA), &realSize))
                    {
                        //std::wstring dPath(drvInfoDetailData.InfFileName);
                        //std::wcout << dPath << std::endl;
                        data.resize(realSize);
                    }


                    std::wstring dPath(reinterpret_cast<PSP_DRVINFO_DETAIL_DATA>(data.data())->InfFileName);
                    std::vector<wchar_t> originalPath;
                    originalPath.resize(1024);
                    DWORD realsize = 0;
                    SetupGetInfDriverStoreLocation(dPath.c_str(), nullptr, nullptr, originalPath.data(), originalPath.size() * 2, &realsize);

                    std::wstring orignalPathStr(originalPath.data());
                    std::wcout << orignalPathStr << std::endl;
                }
                //
                // we've selected the current driver
                //
                //return TRUE;
            }
            deviceInstallParams.FlagsEx &= ~(DI_FLAGSEX_INSTALLEDDRIVER | DI_FLAGSEX_ALLOWEXCLUDEDDRVS);
        
        int driverIndex = 0;
        //while (SetupDiEnumDriverInfo(hDevInfo, &DeviceInfoData, SPDIT_CLASSDRIVER, i, &DriverInfoData))
        //{
        //    std::cout << DriverInfoData.Description << std::endl;
        //    driverIndex++;
        //}
    }


    if (NO_ERROR != GetLastError() && ERROR_NO_MORE_ITEMS != GetLastError())
    {
        // Insert error handling here.
        return 1;
    }

    //  Cleanup
    SetupDiDestroyDeviceInfoList(hDevInfo);

    return 0;
}

