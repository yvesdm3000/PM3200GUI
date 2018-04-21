/***************************************************************
 * Name:      PM3200IO.h
 * Purpose:   Defines PM3200 IO
 * Author:    Yves De Muyter (yves@alfavisio.be)
 * Created:   2018-04-02
 * Copyright: Yves De Muyter (yves@alfavisio.be)
 * License:   MIT
 **************************************************************/
/*
Copyright 2018 Yves De Muyter (yves@alfavisio.be)

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef PM3200IO_H
#define PM3200IO_H

#include <wx/string.h>
#include <memory>

class ModBusCtx;

class PM3200Registers
{
public:
    PM3200Registers() : m_registerStart(0), m_data(){}
    PM3200Registers(PM3200Registers&& other) :
        m_registerStart(other.m_registerStart),
        m_data(std::move(other.m_data))
    {
    }
    PM3200Registers(int startRegister, std::vector<uint16_t>&& data) :
        m_registerStart(startRegister),
        m_data(std::move(data))
    {
    }
    PM3200Registers(const PM3200Registers& other) :
        m_registerStart(other.m_registerStart),
        m_data(other.m_data)
    {
    }
    void swap(PM3200Registers& other)
    {
        std::swap(m_registerStart, other.m_registerStart);
        std::swap(m_data, other.m_data);
    }
    int GetRegisterStart() const { return m_registerStart; }
    size_t GetRegisterCount() { return m_data.size(); }
    bool HasRegisters(int reg, int count)
    {
        if (reg < m_registerStart)
            return false;
        if (reg + count > m_registerStart + (int)m_data.size())
            return false;
        return true;
    }
    uint16_t GetRegisterUInt16(int reg) const;
    uint32_t GetRegisterUInt32(int reg) const;
    uint64_t GetRegisterUInt64(int reg) const;
    float GetRegisterFloat32(int reg) const;
    int64_t GetRegisterInt64(int reg) const;
    time_t GetRegisterDateTime(int reg) const;
private:
    int m_registerStart;
    std::vector<uint16_t> m_data;

};

class PM3200IO
{
public:
    PM3200IO() : m_modbus(nullptr)
    {
    }
    virtual ~PM3200IO(){}
    bool IsConnected() const { return m_modbus.use_count() != 0; }
    bool Connect(const wxString& comport, uint32_t baudrate, int slaveAddress);
    void Disconnect();
    PM3200Registers ReadRegisters(int registerStart, int registerCount);
    const std::string GetName() const { return m_name; }
private:
    std::shared_ptr<ModBusCtx> m_modbus;
    std::string m_name;
};

#endif // PM3200IO_H
