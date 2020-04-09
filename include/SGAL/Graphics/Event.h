/**
 * @file   Event.h
 * @author Max Ortner
 * @date   4-9-2020
 * 
 * This is where the events are handled and passed around. This event architecture is heavily
 * inspired by SFML's event handling system.
 * 
 * @copyright Copyright (c) 2020
**/
#pragma once

namespace sgal
{
    class Event
    {
        
    public:
        enum EventType
        {
            Closed
        };

        EventType type;

        union
        {
            
        };
        
    };
}