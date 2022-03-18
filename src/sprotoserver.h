
#ifndef __SPROTOSERVER_H
#define __SPROTOSERVER_H

#include "sproto.h"

// This it the type of the tick counter. can be uint8_t, or uint32_t, or any number.
// If you increase it only hourle, and the treshold is low, you should use uint8_t.
// Depends on your tick handling.
#ifndef SPROTO_TICK_TYPE
    #define SPROTO_TICK_TYPE uint32_t
#endif

#include <map>
#include <list>

class SProtoServerMeasEntry
{
    public:
        SProtoServerMeasEntry() { };
        SProtoServerMeasEntry(uint16_t p_measTypeId, uint8_t p_serial, uint8_t p_timeFrame)
        {
            measTypeId = p_measTypeId;
            serial = p_serial;
            timeFrame = p_timeFrame;
            ticksSinceGotData = 0;
        }
        bool Equals(uint16_t p_measTypeId, uint8_t p_serial, uint8_t p_timeFrame)
        {
            if (p_measTypeId != measTypeId) return false;
            if (p_serial != serial) return false;
            if (p_timeFrame != timeFrame) return false;
            return true;
        }
        uint16_t measTypeId;
        uint8_t serial;
        uint8_t timeFrame;
        SPROTO_TICK_TYPE ticksSinceGotData = 0;
};

class SProtoServerMeasList
{
    public:
        SProtoServerMeasEntry* FindMeasInRegistry( uint16_t measTypeId, uint8_t serial, uint8_t timeFrame)
        {
            for (std::list<SProtoServerMeasEntry>::iterator it = list.begin(); it != list.end(); ++it){
                if (it->Equals(measTypeId, serial, timeFrame))
                {
                    return &(*it);
                }
            }
            return nullptr;
        }

        bool AddMeasToRegistry( uint16_t measTypeId, uint8_t serial, uint8_t timeFrame)
        {
            SProtoServerMeasEntry* ret = FindMeasInRegistry( measTypeId, serial, timeFrame);
            if (ret != nullptr) 
            {
                ret->ticksSinceGotData = 0;
                return false;
            }
            list.emplace_back(measTypeId, serial, timeFrame);
            return true;
        }

        void TickTime(SPROTO_TICK_TYPE elpased)
        {
            for (std::list<SProtoServerMeasEntry>::iterator it = list.begin(); it != list.end(); ++it)
            {
                it->ticksSinceGotData += elpased;
            }
        }
        void CollectExpired(SPROTO_TICK_TYPE treshold, uint16_t stationId, bool(*callback)(uint16_t , uint16_t , uint8_t , uint8_t ))
        {
            for (std::list<SProtoServerMeasEntry>::iterator it = list.begin(); it != list.end(); ++it)
            {
                if (it->ticksSinceGotData >= treshold)
                {
                    //remove it, and call callback
                    bool deleteIt = true;
                    if (callback != nullptr) deleteIt = callback(stationId, it->measTypeId, it->serial, it->timeFrame);
                    if (deleteIt) it = list.erase(it);
                }
            }
        }


    protected:
        std::list<SProtoServerMeasEntry> list;
};



class SProtoServer
{
  public:
    SProtoServerMeasEntry* FindMeasInRegistry(uint16_t station, SPROTO_MEASHEADERSTRUCT* meas){ return FindMeasInRegistry(station, meas->measTypeId, meas->serial, meas->timeFrame); };
    SProtoServerMeasEntry* FindMeasInRegistry(uint16_t station, uint16_t measTypeId, uint8_t serial, uint8_t timeFrame)
    {
        std::map<uint16_t, SProtoServerMeasList*>::iterator iter = list.find(station);
        if (iter == list.end()) return nullptr;
        return iter->second->FindMeasInRegistry(measTypeId, serial, timeFrame);
    }
    //if new data arrives, call it, so the server will know about it's descriptor. return value: true if it is a new data type. false if seen already.
    bool ArriveMeasInRegistry(uint16_t station, SPROTO_MEASHEADERSTRUCT* meas){ return ArriveMeasInRegistry(station, meas->measTypeId, meas->serial, meas->timeFrame); };
    bool ArriveMeasInRegistry(uint16_t station, uint16_t measTypeId, uint8_t serial, uint8_t timeFrame)
    {
        std::map<uint16_t, SProtoServerMeasList*>::iterator iter = list.find(station);
        SProtoServerMeasList* lptr = nullptr;
        if (iter == list.end())
        { //create new
            lptr = new SProtoServerMeasList();
            list[station] = lptr;
        }
        else
        {
            lptr = iter->second;
        }
        if (lptr == nullptr) return false; //bc bad
        return lptr->AddMeasToRegistry(measTypeId, serial, timeFrame);
    };

    void TickTime(SPROTO_TICK_TYPE elpased)
    {
        //NOT THREAD SAFE! IMPLEMENT IT ON YOUR OWN CODE PART IF NEEDED
        for(std::map<uint16_t, SProtoServerMeasList*>::iterator iter = list.begin(); iter != list.end(); ++iter)
        {
            iter->second->TickTime(elpased);
        }
    }

    void CollectExpired(SPROTO_TICK_TYPE treshold, bool(*callback)(uint16_t , uint16_t , uint8_t , uint8_t ))
    {
        //NOT THREAD SAFE! IMPLEMENT IT ON YOUR OWN CODE PART IF NEEDED
        for(std::map<uint16_t, SProtoServerMeasList*>::iterator iter = list.begin(); iter != list.end(); ++iter)
        {
            iter->second->CollectExpired(treshold, iter->first, callback);
        }
    }

  protected:
    std::map<uint16_t, SProtoServerMeasList*> list;
};

#endif
