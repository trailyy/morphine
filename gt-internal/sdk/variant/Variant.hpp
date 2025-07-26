#pragma once
#include <cstdint>
#include <string>
#include "../proton/Vector.hpp"
#include "../enums.hpp"
#include "../../utils/utils.hpp"

#include "../../lib/xorstr/xorstr.hpp"

#define C_VAR_SPACE_BYTES 16
#define C_MAX_VARIANT_LIST_PARMS 7

class Variant
{
public:
    Variant()
    {
        Default();
    }

    Variant(const Variant& v)
    {
        Default();
        *this = v;
    }

    Variant(int32_t var)
    {
        Default();
        Set(var);
    }

    Variant(uint32_t var)
    {
        Default();
        Set(var);
    }

    Variant(float var)
    {
        Default();
        Set(var);
    }

    Variant(float x, float y)
    {
        Default();
        Set(Vector2(x, y));
    }

    Variant(float x, float y, float z)
    {
        Default();
        Set(Vector(x, y, z));
    }

    Variant(const Vector2& v2)
    {
        Default();
        Set(v2);
    }

    Variant(const Vector& v3)
    {
        Default();
        Set(v3);
    }

    Variant(const Vector4& r)
    {
        Default();
        Set(r);
    }

    Variant(const std::string& var)
    {
        Default();
        Set(var);
    }

    Variant(const char* var)
    {
        Default();
        Set(std::string(var));
    }

    void Reset()
    {
        m_type = eVariantType::UNUSED;
    }

    void Set(const Variant& v)
    {
        switch (v.GetType())
        {
            case eVariantType::FLOAT: Set(v.GetFloat()); break;
            case eVariantType::STRING: Set(v.GetString()); break;
            case eVariantType::VECTOR2: Set(v.GetVec2()); break;
            case eVariantType::VECTOR3: Set(v.GetVec3()); break;
            case eVariantType::UINT32: Set(v.GetUInt()); break;
            case eVariantType::INT32: Set(v.GetInt()); break;
            case eVariantType::RECT: Set(v.GetVec4()); break;
            case eVariantType::ENTITY:
            case eVariantType::COMPONENT:
            default: break;
        }
    }

    void Set(float var)
    {
        m_type = eVariantType::FLOAT;
        *((float*)m_var) = var;
    }

    void Set(uint32_t var)
    {
        m_type = eVariantType::UINT32;
        *((uint32_t*)m_var) = var;
    }

    void Set(int32_t var)
    {
        m_type = eVariantType::INT32;
        *((int32_t*)m_var) = var;
    }

    void Set(const char* var)
    {
        m_type = eVariantType::STRING;
        m_string = var;
    }

    void Set(std::string const& var)
    {
        m_type = eVariantType::STRING;
        m_string = var;
    }

    void Set(Vector2 const& var)
    {
        m_type = eVariantType::VECTOR2;
        *((Vector2*)m_var) = var;
    }

    void Set(float x, float y)
    {
        Set(Vector2(x, y));
    }

    void Set(Vector const& var)
    {
        m_type = eVariantType::VECTOR3;
        *((Vector*)m_var) = var;
    }

    void Set(Vector4 const& var)
    {
        m_type = eVariantType::RECT;
        *((Vector4*)m_var) = var;
    }

    void Set(float x, float y, float z)
    {
        Set(Vector(x, y, z));
    }

    void operator=(float var)
    {
        Set(var);
    }

    void operator=(int32_t var)
    {
        Set(var);
    }

    void operator=(uint32_t var)
    {
        Set(var);
    }

    void operator=(const char* var)
    {
        Set(var);
    }

    void operator=(std::string const& var)
    {
        Set(var);
    }

    void operator=(Vector2 const& var)
    {
        Set(var);
    }

    void operator=(Vector const& var)
    {
        Set(var);
    }

    void operator=(Vector4 const& var)
    {
        Set(var);
    }

    float& GetFloat()
    {
        if (m_type == eVariantType::UNUSED)
            Set(float(0));

        return *((float*)m_var);
    }

    int32_t& GetInt()
    {
        if (m_type == eVariantType::UNUSED)
            Set(int32_t(0));

        return *((int32_t*)m_var);
    }

    uint32_t& GetUInt()
    {
        if (m_type == eVariantType::UNUSED)
            Set(uint32_t(0));

        return *((uint32_t*)m_var);
    }

    std::string& GetString()
    {
        return m_string;
    }

    Vector2& GetVec2()
    {
        if (m_type == eVariantType::UNUSED)
            Set(Vector2(0, 0));

        return *((Vector2*)m_var);
    }

    Vector& GetVec3()
    {
        if (m_type == eVariantType::UNUSED)
            Set(Vector(0, 0, 0));

        return *((Vector*)m_var);
    }

    Vector4& GetVec4()
    {
        if (m_type == eVariantType::UNUSED)
            Set(Vector4(0, 0, 0, 0));

        return *((Vector4*)m_var);
    }

    const float& GetFloat() const
    {
        return *((float*)m_var);
    }

    const int32_t& GetInt() const
    {
        return *((int32_t*)m_var);
    }

    const uint32_t& GetUInt() const
    {
        return *((uint32_t*)m_var);
    }

    const std::string& GetString() const
    {
        return m_string;
    }

    const Vector2& GetVec2() const
    {
        return *((Vector2*)m_var);
    }

    const Vector& GetVec3() const
    {
        return *((Vector*)m_var);
    }

    const Vector4& GetVec4() const
    {
        return *((Vector4*)m_var);
    }

    eVariantType GetType() const
    {
        return m_type;
    }

    std::string PrintType() const
    {
        switch (GetType())
        {
            case eVariantType::FLOAT:       return "float"; break;
            case eVariantType::STRING:      return "string"; break;
            case eVariantType::VECTOR2:     return "vec2"; break;
            case eVariantType::VECTOR3:     return "vec"; break;
            case eVariantType::UINT32:      return "uint32"; break;
            case eVariantType::INT32:       return "int32"; break;
            case eVariantType::RECT:        return "rect"; break;
            case eVariantType::ENTITY:      return "entity"; break;
            case eVariantType::COMPONENT:   return "component"; break;
            case eVariantType::UNUSED:      return "unused"; break;
            default: break;
        }

        return "<unknown>";
    }

    std::string Print()
    {
        switch (GetType())
        {
            case eVariantType::FLOAT: return std::to_string(GetFloat()); break;
            case eVariantType::STRING: return GetString(); break;
            case eVariantType::VECTOR2: return GetVec2().dump(); break;
            case eVariantType::VECTOR3: return GetVec3().dump(); break;
            case eVariantType::UINT32: return std::to_string(GetUInt()); break;
            case eVariantType::INT32: return std::to_string(GetInt()); break;
            case eVariantType::RECT: return GetVec4().dump(); break;

            case eVariantType::ENTITY:
            case eVariantType::COMPONENT:
            {
                std::stringstream ss = { };
                ss << "0x" << std::hex << std::uppercase << m_pointer;
                return ss.str();
            }

            case eVariantType::UNUSED: return "unused"; break;
            default: return "<invalid>"; break;
        }
    }

    Variant& operator=(const Variant& v)
    {
        m_type = v.m_type;
        m_pointer = v.m_pointer;
        memcpy(m_var, v.m_var, C_VAR_SPACE_BYTES);
        m_string = v.m_string;
        return *this;
    }

    Variant& operator+=(const Variant& v)
    {
        if (GetType() == v.GetType())
        {
            switch (GetType())
            {
                case eVariantType::FLOAT: Set(GetFloat() + v.GetFloat()); break;
                case eVariantType::STRING: Set(GetString() + v.GetString()); break;
                case eVariantType::VECTOR2: Set(GetVec2() + v.GetVec2()); break;
                case eVariantType::VECTOR3: Set(GetVec3() + v.GetVec3()); break;
                case eVariantType::UINT32: Set(GetUInt() + v.GetUInt()); break;
                case eVariantType::INT32: Set(GetInt() + v.GetInt()); break;
                default: break;
            }
        }
        return *this;
    }

    Variant& operator-=(const Variant& v)
    {
        if (GetType() == v.GetType())
        {
            switch (GetType())
            {
                case eVariantType::FLOAT: Set(GetFloat() + v.GetFloat()); break;
                case eVariantType::VECTOR2: Set(GetVec2() + v.GetVec2()); break;
                case eVariantType::VECTOR3: Set(GetVec3() + v.GetVec3()); break;
                case eVariantType::UINT32: Set(GetUInt() + v.GetUInt()); break;
                case eVariantType::INT32: Set(GetInt() + v.GetInt()); break;
                default: break;
            }
        }
        return *this;
    }

    bool operator==(const Variant& v) const
    {
        if (GetType() != v.GetType())
            return false;

        switch (GetType())
        {
            case eVariantType::UNUSED: return true;
            case eVariantType::FLOAT: return GetFloat() == v.GetFloat();
            case eVariantType::STRING: return GetString() == v.GetString();
            case eVariantType::VECTOR2: return GetVec2() == v.GetVec2();
            case eVariantType::VECTOR3: return GetVec3() == v.GetVec3();
            case eVariantType::UINT32: return GetUInt() == v.GetUInt();
            case eVariantType::RECT: return GetVec4() == v.GetVec4();
            case eVariantType::INT32: return GetInt() == v.GetInt();
            case eVariantType::ENTITY:
            case eVariantType::COMPONENT:
            default: return false;
        }
    }

    bool operator!=(const Variant& v) const
    {
        return !operator==(v);
    }

    friend class VariantList;

private:
    eVariantType m_type;
    void* m_pointer;
    uint8_t m_var[C_VAR_SPACE_BYTES];
    std::string m_string;

    inline void Default()
    {
        m_type = eVariantType::UNUSED;
    }
};

inline Variant operator+(Variant lhs, const Variant& v)
{
    lhs += v;
    return lhs;
}

inline Variant operator-(Variant lhs, const Variant& v)
{
    lhs -= v;
    return lhs;
}

/*
* 
* 
* 
*/

class VariantList
{
    int GetSize(eVariantType type) const
    {
        switch (type)
        {
            case eVariantType::UNUSED:
            case eVariantType::COMPONENT:
            case eVariantType::ENTITY: return 0;
            case eVariantType::UINT32:
            case eVariantType::INT32:
            case eVariantType::FLOAT: return 4;
            case eVariantType::VECTOR2: return sizeof(Vector2);
            case eVariantType::VECTOR3: return sizeof(Vector);
            case eVariantType::RECT: return sizeof(Vector4);
            default: return 0;
        }
    }

public:
    VariantList() = default;
    explicit VariantList(uint8_t* data) { SerializeFromMemory(data); }

    Variant& get(int parmNum)
    {
        return m_variant[parmNum];
    }

    Variant& operator[](int num)
    {
        return m_variant[num];
    }

    VariantList(Variant v0)
    {
        m_variant[0] = v0;
    }

    VariantList(Variant v0, Variant v1)
    {
        m_variant[0] = v0;
        m_variant[1] = v1;
    }

    VariantList(Variant v0, Variant v1, Variant v2)
    {
        m_variant[0] = v0;
        m_variant[1] = v1;
        m_variant[2] = v2;
    }

    VariantList(Variant v0, Variant v1, Variant v2, Variant v3)
    {
        m_variant[0] = v0;
        m_variant[1] = v1;
        m_variant[2] = v2;
        m_variant[3] = v3;
    }

    VariantList(Variant v0, Variant v1, Variant v2, Variant v3, Variant v4)
    {
        m_variant[0] = v0;
        m_variant[1] = v1;
        m_variant[2] = v2;
        m_variant[3] = v3;
        m_variant[4] = v4;
    }

    VariantList(Variant v0, Variant v1, Variant v2, Variant v3, Variant v4, Variant v5)
    {
        m_variant[0] = v0;
        m_variant[1] = v1;
        m_variant[2] = v2;
        m_variant[3] = v3;
        m_variant[4] = v4;
        m_variant[5] = v5;
    }

    void Reset()
    {
        for (int i = 0; i < C_MAX_VARIANT_LIST_PARMS; i++)
            m_variant[i].Reset();
    }

    uint32_t GetRequiredMemory()
    {
        int vars_used = 0;
        int mem_needed = 0;
        int var_size;

        for (int i = 0; i < C_MAX_VARIANT_LIST_PARMS; i++)
        {
            if (m_variant[i].GetType() == eVariantType::STRING)
                var_size = m_variant[i].GetString().size() + 4;
            else
                var_size = GetSize(m_variant[i].GetType());

            if (var_size > 0)
            {
                vars_used++;
                mem_needed += var_size;
            }
        }

        int total = mem_needed + 1 + (vars_used * 2);
        return total;
    }

    uint8_t* SerializeToMemory(uint32_t* size, uint8_t* data = NULL) const
    {
        int vars_used = 0;
        int mem_needed = 0;
        int var_size;

        for (int i = 0; i < C_MAX_VARIANT_LIST_PARMS; i++)
        {
            if (m_variant[i].GetType() == eVariantType::STRING)
                var_size = m_variant[i].GetString().size() + 4;
            else
                var_size = GetSize(m_variant[i].GetType());

            if (var_size > 0)
            {
                vars_used++;
                mem_needed += var_size;
            }
        }

        int total = mem_needed + 1 + (vars_used * 2);

        if (!data)
            data = new uint8_t[total];

        uint8_t* p = data;
        *(p++) = uint8_t(vars_used);

        for (int idx = 0; idx < C_MAX_VARIANT_LIST_PARMS; idx++)
        {
            uint8_t type = uint8_t(m_variant[idx].GetType());

            if (m_variant[idx].GetType() == eVariantType::STRING)
            {
                uint32_t len = m_variant[idx].GetString().size();

                memcpy(p++, &idx, 1);
                memcpy(p++, &type, 1);

                memcpy(p, &len, sizeof(len));
                p += sizeof(len);

                memcpy(p, m_variant[idx].GetString().c_str(), len);
                p += len;
            }
            else
            {
                var_size = GetSize(m_variant[idx].GetType());
                if (var_size > 0)
                {
                    memcpy(p++, &idx, 1);
                    memcpy(p++, &type, 1);

                    memcpy(p, m_variant[idx].m_var, var_size);
                    p += var_size;
                }
            }
        }

        if (size)
            *size = total;

        return data;
    }

    bool SerializeFromMemory(uint8_t* data, int* read = 0)
    {
        uint8_t* p = data;
        uint8_t count = *(p++);

        for (int i = 0; i < count; i++)
        {
            uint8_t index = *(p++);
            uint8_t type = *(p++);

            switch (eVariantType(type))
            {
                case eVariantType::STRING:
                {
                    uint32_t len;
                    memcpy(&len, p, sizeof(len));
                    p += sizeof(len);

                    std::string v;
                    v.resize(len);
                    memcpy(&v[0], p, len);
                    p += len;

                    m_variant[index].Set(v);
                    break;
                }
                case eVariantType::UINT32:
                {
                    uint32_t v;
                    memcpy(&v, p, sizeof(uint32_t));
                    p += sizeof(uint32_t);

                    m_variant[index].Set(v);
                    break;
                }
                case eVariantType::INT32:
                {
                    int32_t v;
                    memcpy(&v, p, sizeof(int32_t));
                    p += sizeof(int32_t);

                    m_variant[index].Set(v);
                    break;
                }
                case eVariantType::FLOAT:
                {
                    float v;
                    memcpy(&v, p, sizeof(float));
                    p += sizeof(float);

                    m_variant[index].Set(v);
                    break;
                }
                case eVariantType::VECTOR2:
                {
                    Vector2 v;
                    memcpy(&v, p, sizeof(Vector2));
                    p += sizeof(Vector2);

                    m_variant[index].Set(v);
                    break;
                }
                case eVariantType::VECTOR3:
                {
                    Vector v;
                    memcpy(&v, p, sizeof(Vector));
                    p += sizeof(Vector);

                    m_variant[index].Set(v);
                    break;
                }
                case eVariantType::RECT:
                {
                    Vector4 v;
                    memcpy(&v, p, sizeof(Vector4));
                    p += sizeof(Vector4);

                    m_variant[index].Set(v);
                    break;
                }
                default:
                {
                    if (read)
                        *read = 0;

                    return false;
                }
            }
        }

        if (read)
            *read = int(p - data);

        return true;
    }

    Variant m_variant[C_MAX_VARIANT_LIST_PARMS];

    void Print()
    {
        print(_("------- variantlist -------"));
        for (int i = 0; i < C_MAX_VARIANT_LIST_PARMS; i++)
        {
            if (m_variant[i].GetType() == eVariantType::UNUSED)
                continue;

            print(std::string(std::to_string(i) + _(": (") + m_variant[i].PrintType() + _(") ") + m_variant[i].Print()).c_str());
        }
        print(_("--------------------------"));
    }
};