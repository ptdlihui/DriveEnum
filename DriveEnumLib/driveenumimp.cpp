#include "deviceenum.h"
#include "service.h"
#include <set>
#include <unordered_map>

namespace DriveEnum
{
    typedef std::unordered_map<Property, Value> Properties;

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
                return ci->second.String();

            return std::wstring();
        }


        GUID propertyAsGUID(const Property prop) const
        {
            Properties::const_iterator ci = m_properties.find(prop);
            if (ci != m_properties.cend())
            {
                std::wstring guidStr = ci->second.String();
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

        DWORDLONG propertyAsDWORDLONG(const Property prop) const
        {
            Properties::const_iterator ci = m_properties.find(prop);

            if (ci != m_properties.cend())
            {
                return ci->second.FileVersion();
            }
            return 0;
        }

        FILETIME propertyAsFILETIME(const Property prop) const
        {
            Properties::const_iterator ci = m_properties.find(prop);
            if (ci != m_properties.cend())
            {
                return ci->second.FileTime();
            }

            return {0, 0};
        }

        Properties& properties()
        {
            return m_properties;
        }

        bool hasProperty(Property prop) const
        {
            return m_properties.find(prop) != m_properties.end();
        }

        const Properties& properties() const
        {
            return m_properties;
        }

        void copyFrom(const DeviceImp& right)
        {
            m_properties = right.m_properties;
        }

        void clear()
        {
            m_properties.clear();
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

        m_pImp->clear();
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

    DWORDLONG Device::propertyAsDWORDLONG(const Property prop) const
    {
        return m_pImp->propertyAsDWORDLONG(prop);
    }

    FILETIME Device::propertyAsFILETIME(const Property prop) const
    {
        return m_pImp->propertyAsFILETIME(prop);
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

        bool process(DevMgrProcessProgress* pProgress, bool reset = true)
        {
            if (m_devices.size() == 0)
                return false;
            if (m_properties.size() == 0)
                return false;

            if (reset)
            {
                for (auto& instance : m_devices)
                    instance->m_pImp->clear();
            }

            EnumGetProperty callback(m_devices, m_properties, pProgress);

            EnumService service(m_style);

            bool bRet = service.EnumBody(&callback);

            return bRet;
        }

        void addProperty(Property prop)
        {
            m_properties.insert(prop);
        }

        void addProperties(std::vector<Property>& properties)
        {
            for (auto& instance : properties)
                m_properties.insert(instance);
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
            EnumGetProperty(DeviceArray& devices, std::set<Property>& properties, DevMgrProcessProgress* pProgress)
                : m_devices(devices)
                , m_properties(properties)
                , m_pProgress(pProgress)
            {}

            virtual void Process(HDEVINFO& devInfo, SP_DEVINFO_DATA& data, DWORD index) override
            {
                if (index >= m_devices.size())
                    return;

                if (m_pProgress)
                    m_pProgress->BeforeProcess(index, m_devices.size());

                Device device;

                for (auto& instance : m_properties)
                {
                    if (!device.m_pImp->hasProperty(instance))
                    {
                        Value value = readProperty(devInfo, data, instance);
                        if (value.valid())
                            device.m_pImp->properties()[instance] = value;
                    }
                }

                *m_devices[index] = device;

                if (m_pProgress)
                    m_pProgress->AfterProcess(m_devices[index], index, m_devices.size());
            }
        protected:
            DeviceArray& m_devices;
            std::set<Property>& m_properties;
            DevMgrProcessProgress* m_pProgress;
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

    bool DeviceManager::Process(DevMgrProcessProgress* pProgress, bool reset)
    {
        return m_pImp->process(pProgress, reset);
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

    void DeviceManager::AddProperties(std::vector<Property>& props)
    {
        return m_pImp->addProperties(props);
    }
}