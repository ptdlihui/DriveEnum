#include "deviceenum.h"

namespace DriveEnum
{
    class DeviceImp
    {
    public:
        DeviceImp() {};

        DeviceImp(const DeviceImp& right)
        {
            copyFrom(right);
        }

        std::wstring propertyAsWString(const Property prop) const
        {
            Properties::const_iterator ci = m_properties.find(prop);
            if (ci != m_properties.cend())
                return ci->second;

            return std::wstring();
        }


        GUID propertyAsGUID(const Property prop) const
        {
            Properties::const_iterator ci = m_properties.find(prop);
            if (ci != m_properties.cend())
            {
                std::wstring guidStr = ci->second;
                GUID ret;
                HRESULT hr = CLSIDFromString(guidStr.data(), &ret);
                if (hr == S_OK)
                    return ret;
                
            }

            return GUID();
        }
        DeviceID propertyAsID(const Property prop) const
        {
            return DeviceID();
        }

        Properties& properties()
        {
            return m_properties;
        }

        const Properties& properties() const
        {
            return m_properties;
        }

        void copyFrom(const DeviceImp& right)
        {
            m_properties = right.m_properties;
        }
    protected:
        Properties m_properties;
    };


    Device::Device()
    {
        m_pImp = new DeviceImp();
    }

    Device::Device(const Device& source)
    {
        m_pImp = new DeviceImp(*source.m_pImp);
    }

    const Device& Device::operator= (const Device& right)
    {
        if (this == &right)
            return *this;

        m_pImp->copyFrom(*right.m_pImp);
        return *this;
    }

    std::wstring Device::propertyAsWString(const Property prop) const
    {
        return m_pImp->propertyAsWString(prop);
    }

    GUID Device::propertyAsGUID(const Property prop) const
    {
        return m_pImp->propertyAsGUID(prop);
    }

    DeviceID Device::propertyAsID(const Property prop) const
    {
        return m_pImp->propertyAsID(prop);
    }

    typedef std::vector<Device*> DeviceArray;
    class DeviceManagerImp
    {
    public:
        void init()
        {

        }
        void clear()
        {
            for (auto & instance : m_devices)
                delete instance;

            m_devices.clear();
        }
    protected:
        DeviceArray m_devices;
    };

}