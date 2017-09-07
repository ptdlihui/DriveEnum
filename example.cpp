#include "deviceenum.h"
#include "deviceutil.h"

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

    devMgr.StartUp();

    std::vector<DriveEnum::Property> slots;
    slots.push_back(DriveEnum::eClassGUID);
    slots.push_back(DriveEnum::ePublicInf0);
    slots.push_back(DriveEnum::eOriginalInf0);
    slots.push_back(DriveEnum::eMFG);
    slots.push_back(DriveEnum::eEnumeratorName);

    DriveEnum::DeviceSort<GUID, std::wstring, std::wstring, std::wstring, std::wstring> sort(devMgr, slots);
    sort.sorting();

    return 0;
}