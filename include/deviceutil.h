#pragma once

#include "deviceenum.h"
#include <tuple>
#include <vector>
#include <unordered_set>
#include <set>
#include <string>

bool operator < (const GUID left, const GUID right) noexcept
{
    std::wstring ls, rs;

    wchar_t l[64];
    wchar_t r[64];
    StringFromGUID2(left, l, 64);
    StringFromGUID2(right, r, 64);

    ls = l;
    rs = r;

    return ls < rs;
}

namespace DriveEnum
{
    template <typename T>
    bool GetProperty(const Device& device, Property prop, T& v)
    {
        return false;
    }

    template <>
    bool GetProperty<std::wstring>(const Device& device, Property prop, std::wstring& v)
    {
        v = device.propertyAsWString(prop);
        return v.size() > 0;
    }

    template <>
    bool GetProperty<GUID>(const Device& device, Property prop, GUID& v)
    {
        v = device.propertyAsGUID(prop);
        return v != GUID();
    }

    template<>
    bool GetProperty<DeviceID>(const Device& device, Property prop, DeviceID& v)
    {
        v = device.propertyAsID(prop);
        return v.PID != 0 && v.VID != 0;
    }

    template <>
    bool GetProperty<FILETIME>(const Device& device, Property prop, FILETIME& v)
    {
        v = device.propertyAsFILETIME(prop);
        return v.dwHighDateTime != 0 && v.dwLowDateTime != 0;
    }

    template <>
    bool GetProperty<DWORDLONG>(const Device& device, Property prop, DWORDLONG& v)
    {
        v = device.propertyAsDWORDLONG(prop);
        return v != 0;
    }


    
    template <typename T, size_t dim>
    struct type_list
    {
        template <std::size_t N>
        using type = typename std::tuple_element<N < dim ? N : 0, T>::type;
    };

    template <typename... Args>
    class DeviceSort
    {
    public:
        typedef std::tuple<Args...> Object;
        typedef std::set<Object> ObjectSet;

        DeviceSort(DeviceManager& devMgr, std::vector<Property> slots)
            : m_devMgr(devMgr)
            , m_slots(slots)
        {
        }

#define GET_PROPERTY(index) (GetProperty<type_list<Object, PropertySize>::type<index>>(*pDevice, m_slots[index], std::get<index < PropertySize ? index : 0>(item))) 
#define CHECK_PROPERTY(index) if (PropertySize > index) {if (!GET_PROPERTY(index)) { continue;}};
        void sorting()
        {
            m_sorting.clear();

            unsigned int deviceCount = m_devMgr.Count();
            constexpr size_t PropertySize = std::tuple_size<Object>::value;

            for (unsigned int i = 0; i < deviceCount; i++)
            {
                Object item;
                bool slotFull = true;
                Device* pDevice = m_devMgr.GetDevice(i);

                CHECK_PROPERTY(0);
                CHECK_PROPERTY(1);
                CHECK_PROPERTY(2);
                CHECK_PROPERTY(3);
                CHECK_PROPERTY(4);
                CHECK_PROPERTY(5);
                CHECK_PROPERTY(6);
                CHECK_PROPERTY(7);
                CHECK_PROPERTY(8);
                CHECK_PROPERTY(9);

                m_sorting.insert(item);
            }


        }

        ObjectSet& result()
        {
            return m_sorting;
        }

        const ObjectSet& result() const
        {
            return m_sorting;
        }

    protected:
        std::vector<Property> m_slots;
        ObjectSet m_sorting;
        DeviceManager& m_devMgr;
    };
}

