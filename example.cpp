#include "deviceenum.h"
#include "deviceutil.h"
#include <iostream>

class Progress : public DriveEnum::DevMgrProcessProgress
{
public:
    void BeforeProcess(unsigned int i, unsigned int tota) override {};
    void AfterProcess(DriveEnum::Device* pDevice, unsigned int i, unsigned int total) override
    {
        std::cout << i / (float)(total) * 100 << "% completes" << std::endl;
    }
};

int main(int argc, char *argv[])
{
    // Construct a devMgr for the current devices
    DriveEnum::DeviceManager devMgr(DriveEnum::eCurrent);

    // Add properties we want, device does not have every device.
    devMgr.AddProperty(DriveEnum::eClassGUID);
    devMgr.AddProperty(DriveEnum::eEnumeratorName);
    devMgr.AddProperty(DriveEnum::eDescription);
    devMgr.AddProperty(DriveEnum::ePublicInf0);
    devMgr.AddProperty(DriveEnum::eOriginalInf0);
    devMgr.AddProperty(DriveEnum::eDriverDate0);
    devMgr.AddProperty(DriveEnum::eDriverVersion0);
    devMgr.AddProperty(DriveEnum::eMFG);
    devMgr.AddProperty(DriveEnum::eHardwareID);
    devMgr.AddProperty(DriveEnum::eLocationInformation);
    devMgr.AddProperty(DriveEnum::eLocationPaths);

    //Processing...
    Progress prog;
    devMgr.Process(&prog);

    unsigned int devCount = devMgr.Count();

    //Output device
    for (unsigned int i = 0; i < devCount; i++)
    {
        DriveEnum::Device device = *devMgr.GetDevice(i);

        std::wcout << "---------------------------------------------------------------------" << std::endl;
        std::wcout << "Description:" << device.propertyAsWString(DriveEnum::eDescription) << std::endl;
        std::wcout << "Name:" << device.propertyAsWString(DriveEnum::eEnumeratorName) << std::endl;
        std::wcout << "Provider:" << device.propertyAsWString(DriveEnum::eMFG) << std::endl;
        std::wcout << "ClassGUID:" << device.propertyAsWString(DriveEnum::eClassGUID) << std::endl;
        std::wcout << "Path:" << device.propertyAsWString(DriveEnum::eLocationPaths) << std::endl;
     }

    // Constuct the properteis we want to sort - this is what driver needs
    std::vector<DriveEnum::Property> slots;
    slots.push_back(DriveEnum::eClassGUID);
    slots.push_back(DriveEnum::ePublicInf0);
    slots.push_back(DriveEnum::eOriginalInf0);
    slots.push_back(DriveEnum::eMFG);
    slots.push_back(DriveEnum::eEnumeratorName);

    // Construct the DeviceSort with the correct data structure 
    // GUID : ClassGUID
    // PublicInf0 : std::wstring
    // OriginalInf0 : std::wstring
    // MFG(Provider) : std::wstring
    // EnumeratorName : std::wstring
    DriveEnum::DeviceSort<GUID, std::wstring, std::wstring, std::wstring, std::wstring> sort(devMgr, slots);
    sort.sorting();

    // Check out how many device types
    slots.clear();
    slots.push_back(DriveEnum::eEnumeratorName);
    DriveEnum::DeviceSort<std::wstring> devType(devMgr, slots);
    devType.sorting();


    return 0;
}