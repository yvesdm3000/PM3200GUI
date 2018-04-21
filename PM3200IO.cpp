/***************************************************************
 * Name:      PM3200IO.cpp
 * Purpose:   Code for PM3200 IO
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
#include "PM3200IO.h"
#include <modbus.h>
#include <modbus-rtu.h>
#include <limits>

class ModBusCtx
{
public:
    ModBusCtx( const wxString& comport, int baud, int slaveAddress) :
        m_slaveAddress(slaveAddress),
        m_registerStart(0)
    {
        if (baud == 0)
            baud = 19200;
        m_modbus = modbus_new_rtu(comport.mb_str(), baud, 'E', 8, 1);
//        if (m_modbus)
//            modbus_set_debug(m_modbus, 1);
    }
    ~ModBusCtx()
    {
        if (m_modbus)
        {
            modbus_close(m_modbus);
            modbus_free(m_modbus);
        }
    }
    bool Connect()
    {
        if (!m_modbus)
            return false;
        int rc = modbus_connect(m_modbus);
        if (rc != 0)
            return false;
        rc = modbus_set_slave(m_modbus, m_slaveAddress);
        if (rc != 0)
        {
            modbus_close(m_modbus);
            return false;
        }
        freopen("C:\\projects\\logfile_out.txt", "w+", stdout);
        freopen("C:\\projects\\logfile_err.txt", "w+", stderr);

        fprintf(stdout,"Start logging\n");
        fflush(stdout);
        fprintf(stderr,"Start logging\n");
        fflush(stderr);

        return true;
    }
    void Disconnect()
    {
        if (m_modbus)
        {
            modbus_close(m_modbus);
        }
    }
    std::string ReadString(int start, int count)
    {
        uint8_t data[count * sizeof(uint16_t) + 1];
        int cnt = modbus_read_registers(m_modbus, start-1, count, (uint16_t*)data);
        if (cnt == -1)
        {
            fprintf(stderr,"modbus_read_registers(%d,%d) failed: %s (%d)\n", start, count, modbus_strerror(errno), errno);
            return "";
        }

        for (size_t i=0; i<cnt*sizeof(uint16_t); i+=2)
        {
            std::swap(data[i], data[i+1]);
        }
        int len = cnt * sizeof(uint16_t);
        for (int i= len - 1; i > 0; --i)
        {
            if (data[i] == ' ')
                len--;
            else
                break;
        }

        data[len] = '\0';

        std::string ret;
        ret.assign(data,data+len);
        printf("Read name: %s\n", ret.c_str());
        fflush(stdout);
        return ret;
    }
    PM3200Registers ReceiveRegisters(int start, int count)
    {
        if (!m_modbus)
            return PM3200Registers();
        std::vector<uint16_t> data(count,0);
        int readOffset = 0;
        int readCount = 125;
        while (readOffset < count)
        {
            if (readCount > count - readOffset )
            {
                readCount = count - readOffset;
            }
            int cnt = modbus_read_registers(m_modbus, start + readOffset-1, readCount, &data[readOffset]);
            if (cnt == -1)
            {
                if (errno == 10060)
                {
                    modbus_close(m_modbus);
                    modbus_free(m_modbus);
                    m_modbus = NULL;
                }
                fprintf(stderr,"modbus_read_registers(%d,%d) failed: %s (%d)\n", start + readOffset, readCount, modbus_strerror(errno), errno);

                return PM3200Registers();
            }
            readOffset += readCount;
        }
        return PM3200Registers(start, std::move(data));
    }
    bool IsConnected() { return m_modbus != NULL; }
private:
    modbus_t* m_modbus;
    int m_slaveAddress;
    int m_registerStart;
};

bool PM3200IO::Connect(const wxString& comport, uint32_t baudrate, int slaveAddress)
{
    auto modbus = std::make_shared<ModBusCtx>(comport, baudrate, slaveAddress);
    if (!modbus->Connect())
        return false;
    auto name = modbus->ReadString(70, 20);
    name += " ";
    name += modbus->ReadString(30, 20);
    fprintf(stderr,"Connected to power meter '%s'\n",m_name.c_str());
    name += " ";
    name += modbus->ReadString(50, 20);
    name += " ";
    std::atomic_thread_fence(std::memory_order_release);
    m_modbus = modbus;
    m_name = name;
    return true;
}

void PM3200IO::Disconnect()
{
    m_modbus.reset();
}

PM3200Registers PM3200IO::ReadRegisters(int registerStart, int registerCount)
{
    if (!IsConnected())
        return PM3200Registers();
    auto modbus = m_modbus;
    std::atomic_thread_fence(std::memory_order_release);
    if (!modbus)
        return PM3200Registers();
    return modbus->ReceiveRegisters(registerStart, registerCount);
}

uint32_t PM3200Registers::GetRegisterUInt32(int reg) const
{
    if (reg < m_registerStart)
        return std::numeric_limits<uint32_t>::max();
    if (reg > m_registerStart + (int)m_data.size())
        return std::numeric_limits<uint32_t>::max();
    return MODBUS_GET_INT32_FROM_INT16(m_data,reg-m_registerStart);
    //return *(uint32_t*)&(m_data[reg-m_registerStart]);
}
int64_t PM3200Registers::GetRegisterInt64(int reg) const
{
    if (reg < m_registerStart)
        return std::numeric_limits<int64_t>::max();
    if (reg > m_registerStart +(int) m_data.size())
        return std::numeric_limits<int64_t>::max();
    return MODBUS_GET_INT64_FROM_INT16(m_data,reg-m_registerStart);
        /*
    uint16_t data[4] {
        m_data[reg-m_registerStart+3],
        m_data[reg-m_registerStart+2],
        m_data[reg-m_registerStart+1],
        m_data[reg-m_registerStart+0]
        } ;
    return *(int64_t*)data;
    */
}
uint16_t PM3200Registers::GetRegisterUInt16(int reg) const
{
    if (reg < m_registerStart)
        return std::numeric_limits<uint16_t>::max();
    if (reg >= m_registerStart + (int)m_data.size())
        return std::numeric_limits<uint16_t>::max();
    return m_data[reg-m_registerStart];
}
float PM3200Registers::GetRegisterFloat32(int reg) const
{
    if (reg < m_registerStart)
        return nanf("");
    if (reg + 1 > m_registerStart + (int)m_data.size())
        return nanf("");
    return modbus_get_float_dcba( &(m_data[reg-m_registerStart]) );
}
time_t PM3200Registers::GetRegisterDateTime(int reg) const
{
    if (reg < m_registerStart)
        return 0;
    if (reg > m_registerStart + (int)m_data.size())
        return 0;

    auto val = GetRegisterUInt16(reg);
    int year = val & 0x007f;
    val = GetRegisterUInt16(reg+1);
    int month = val & 0x0f00;
    int wd = val & 0x00e0;
    int day = val & 0x01f;
    val = GetRegisterUInt16(reg+2);
    int hours = val & 0x1F00;
    int minutes = val & 0x003F;
    int iV = val & 0x0080;
    if (iV)
        return 0;
    struct tm source;
    memset(&source, 0, sizeof(struct tm));
    source.tm_hour = hours;
    source.tm_min = minutes;
    source.tm_mday = day;
    source.tm_mon = month;
    source.tm_year = year;
    return mktime(&source);
}


