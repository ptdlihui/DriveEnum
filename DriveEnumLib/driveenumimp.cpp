#include "deviceenum.h"
#include "service.h"
#include <set>

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

    Device::~Device()
    {
        if (m_pImp)
        {
            delete m_pImp;
            m_pImp = nullptr;
        }
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
        DeviceManagerImp(EnumStyle style) : m_style(style)
        {
            init();
        }

        ~DeviceManagerImp()
        {
            clear();
        }

        bool startup()
        {
            if (m_devices.size() == 0)
                return false;
            if (m_properties.size() == 0)
                return false;

            EnumGetProperty callback(m_devices, m_properties);

            EnumService service(m_style);

            bool bRet = service.EnumBody(&callback);

            return bRet;
        }

        void addProperty(Property prop)
        {
            m_properties.insert(prop);
        }

        unsigned int count() const
        {
            return m_devices.size();
        }

        Device* getDeviceConst(unsigned int index) const
        {
            if (index < m_devices.size())
                return m_devices[index];

            return nullptr;
        }

        Device* getDevice(unsigned int index)
        {
            return getDeviceConst(index);
        }
    protected:

        void init()
        {
            EnumService service(m_style);
            EnumCounter callback;
            service.EnumBody(&callback);

            clear();
            m_devices.resize(callback.count());
            for (unsigned int i = 0; i < callback.count(); i++)
            {
                m_devices[i] = new Device();
            }
        }


        void clear()
        {
            for (auto & instance : m_devices)
                delete instance;

            m_devices.clear();
            m_properties.clear();
        }


    protected:
        class EnumCounter : public EnumCallback
        {
        public:
            EnumCounter() : m_count(0)
            {}

            virtual void Process(HDEVINFO& devInfo, SP_DEVINFO_DATA& data, DWORD index) override
            {
                m_count++;
            }

            unsigned int count() const
            {
                return m_count;
            }

        protected:
            unsigned int m_count;
        };

        class EnumGetProperty : public EnumCallback
        {
        public:
            EnumGetProperty(DeviceArray& devices, std::set<Property>& properties)
                : m_devices(devices)
                , m_properties(properties)
            {}

            virtual void Process(HDEVINFO& devInfo, SP_DEVINFO_DATA& data, DWORD index) override
            {
                if (index >= m_devices.size())
                    return;

                Device device;

                for (auto& instance : m_properties)
                {
                    std::wstring value = readProperty(devInfo, data, instance);
                    if (value.size() > 0)
                        device.m_pImp->properties()[instance] = value;
                }

                *m_devices[index] = device;
            }
        protected:
            DeviceArray& m_devices;
            std::set<Property>& m_properties;
        };

    protected:
        DeviceArray m_devices;
        EnumStyle m_style;
        std::set<Property> m_properties;
    };

    DeviceManager::DeviceManager(EnumStyle style)
    {
        m_pImp = new DeviceManagerImp(style);
    }
    
    DeviceManager::~DeviceManager()
    {
        if (m_pImp)
        {
            delete m_pImp;
            m_pImp = nullptr;
        }
    }

    bool DeviceManager::StartUp()
    {
        return m_pImp->startup();
    }

    unsigned int DeviceManager::Count()
    {
        return m_pImp->count();
    }

    const Device* DeviceManager::GetDevice(unsigned int index) const
    {
        return m_pImp->getDeviceConst(index);
    }

    Device* DeviceManager::GetDevice(unsigned int index)
    {
        return m_pImp->getDevice(index);
    }

    void DeviceManager::AddProperty(Property prop)
    {
        return m_pImp->addProperty(prop);
    }
}