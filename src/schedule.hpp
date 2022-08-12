#pragma once

#include <string>
#include <vector>

namespace iso {
struct EventData {
    std::string name;     // Code name of the event. This name will be added into event queue when emitted.
    bool speed_dependant; // If the event depends on simulation time or world time. 
    int time;             // Current amount of time units passed since last update.
    int required_time;    // This amount of time unit needs to have passed until the event is emitted. 
};

class ScheduledEvent {
    protected:
        EventData data;
    
    public:
        ScheduledEvent(const EventData& data);
        ~ScheduledEvent();

        void resetTime();
        void progressEvent();
        const std::string& getEventName() const;
        int  getRequiredTime()  const;
        int  getCurrentTime()   const;
        bool isSpeedDependant() const;
};

typedef std::vector <ScheduledEvent> Schedule;
}