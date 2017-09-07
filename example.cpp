#include "deviceenum.h"

int main(int argc, char *argv[])
{
    DriveEnum::DeviceManager devMgr(DriveEnum::eCurrent);

    devMgr.AddProperty(DriveEnum::eLocationPaths);
    devMgr.AddProperty(DriveEnum::eAddress);
    devMgr.AddProperty(DriveEnum::eDescription);
    devMgr.AddProperty(DriveEnum::ePublicInf0);
    devMgr.AddProperty(DriveEnum::eOriginalInf0);

    devMgr.StartUp();

    return 0;
}