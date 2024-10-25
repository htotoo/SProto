
#ifndef __SPROTOPARSEHELPER_H
#define __SPROTOPARSEHELPER_H

#include "sproto.h"

/*
    This helper will help you wait until tha buffer array has a full packet in it. can feed byte by byte. 

    Workflow: create an uint8_t[] array, and create a SProtoParseHelper() object.
    If there is a new byte come from communication feed it with GotByte(). If it returns false, ignore it, and wait for further bytes. If it returns true, parse the whole array with SProto:: class. 
    If you parsed it, call the PacketParsed() BEFORE feed this a new byte.
    This class intended to be small memory footprint, and should use not so much processor with maximum error handling ability.
*/
class SProtoParseHelper
{
    public:
        SProtoParseHelper() {m_packetArr = NULL; m_maxSize = 0;};
        SProtoParseHelper(uint8_t* packetArr, uint32_t maxSize) { m_packetArr = packetArr; m_maxSize = maxSize; m_writePointer = 0; }
        void SetBuffer(uint8_t* packetArr, uint32_t maxSize) { m_packetArr = packetArr; m_maxSize = maxSize; m_writePointer = 0; }
        uint8_t* GetBuffer() { return m_packetArr; }
        void HandleError()
        {
          if (!m_packetArr) return;
            //check if there is another start byte. is so, copy it to array's 0 point
            uint32_t delta = 0;
            hasStart = false; //don't set it to true, since the parser will check for it.
            for (uint32_t i = 1; i<m_writePointer; ++i) //start byte searcher
            {
                if ( SPROTO_STARTBYTE ==  m_packetArr[i] || SPROTO_STARTBYTEMINI ==  m_packetArr[i])
                {
                    delta = i;
                    break;
                }
            }
            if (0 == delta)
            {
                m_packetArr[0] = 0; //reset it
                m_writePointer = 0; //no another start byte, so reset
            }
            else
            {
                for (uint32_t i = 0; i < (m_writePointer - delta); i++)
                {
                    m_packetArr[i] = m_packetArr[i+delta]; //shift it
                }
                m_writePointer -= delta;
            }
            return;
        }
        bool GotByte(uint8_t b, bool fakeData = false) { //don't call it from interrupt! took too long time, also has more parsing job after return true
            if (!m_packetArr) return false;
            if (!fakeData) //fake data true means just recheck the remaining data
            {
                m_packetArr[m_writePointer] = b;
                if (m_writePointer+1 < m_maxSize) ++m_writePointer;
            }
            front: 
            if (!hasStart)
            {
                if ( SPROTO_STARTBYTE ==  m_packetArr[0] || SPROTO_STARTBYTEMINI ==  m_packetArr[0])
                {
                    hasStart = true;
                    hasFullHeader = false;
                }
                else
                {
                    m_writePointer = 0; //reset, since not a start byte
                }
                if (m_writePointer <= 1) return false; //only if ther was no error handling, othervise check the remaining bytes too
            }
            if (!hasFullHeader)
            {
                uint32_t hl = SProto::GetHeaderLength(m_packetArr); //returns based on first byte
                if (m_writePointer >= hl)
                {
                    //should got the full header packet, so check it
                    if (SProto::IsValidHeader(m_packetArr))
                    {
                        hasFullHeader = true;
                    }
                    else
                    {
                        hasStart = false;
                        HandleError();
                        goto front;
                    }
                    //no return, to check if it is a full packet already (eg if error handling and copy hapened)
                }
            }
            if (hasFullHeader)
            {
                uint32_t hl = SProto::GetFullPacketLength(m_packetArr);
                if (m_writePointer < hl) return false; //not yet
                if (SProto::IsValidPacket(m_packetArr))
                {
                    return true; //got a full packet, let the upper layers parse it
                }
                else
                {
                    hasStart = false;
                    hasFullHeader = false;
                    HandleError();
                    goto front; //bad packet, so check it again for good parts.
                }
            }
        } 
        bool PacketParsed(bool mayHasMore = false) {
            if (!m_packetArr) return false;
            if (!mayHasMore)
            {
                hasStart = false;
                hasFullHeader = false;
                m_writePointer = 0;
                m_packetArr[0] = 0;
                return false;
            }
            else
            {
                //if added more bytes than a packet, and not parsed immediately, try to get additional data from the array
                hasStart = false;
                hasFullHeader = false;
                m_packetArr[0] = 0; //skip to next header
                HandleError();
                return GotByte(0, true);
            }
        }
    protected:
        uint8_t* m_packetArr = nullptr;
        uint32_t m_maxSize = 0;
        uint32_t m_writePointer = 0;
        bool hasStart = false;
        bool hasFullHeader = false;
};


#endif
