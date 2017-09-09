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
    DriveEnum::DeviceManager devMgr(DriveEnum::eCurrent);

    devMgr.AddProperty(DriveEnum::eClassGUID);
    devMgr.AddProperty(DriveEnum::eEnumeratorName);
    devMgr.AddProperty(DriveEnum::eDescription);
    devMgr.AddProperty(DriveEnum::ePublicInf0);
    devMgr.AddProperty(DriveEnum::eOriginalInf0);
    devMgr.AddProperty(DriveEnum::eDriverDate0);
    devMgr.AddProperty(DriveEnum::eDriverVersion0);
    devMgr.AddProperty(DriveEnum::eMFG);
    devMgr.AddProperty(DriveEnum::eHardwareID);

    Progress prog;
    devMgr.Process(&prog);

    std::vector<DriveEnum::Property> slots;
    slots.push_back(DriveEnum::eClassGUID);
    slots.push_back(DriveEnum::ePublicInf0);
    slots.push_back(DriveEnum::eOriginalInf0);
    slots.push_back(DriveEnum::eMFG);
    slots.push_back(DriveEnum::eEnumeratorName);

    DriveEnum::DeviceSort<GUID, std::wstring, std::wstring, std::wstring, std::wstring> sort(devMgr, slots);
    sort.sorting();

    slots.clear();
    slots.push_back(DriveEnum::eEnumeratorName);
    DriveEnum::DeviceSort<std::wstring> devType(devMgr, slots);
    devType.sorting();


    return 0;
}