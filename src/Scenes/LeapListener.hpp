#ifndef ERICS3DENGINE_LEAPLISTENER_HPP
#define ERICS3DENGINE_LEAPLISTENER_HPP

#include <Leap.h>
#include <string>
#include <iostream>

using namespace Leap;

class LeapListener : public Listener {
    public:
        virtual void onInit(const Controller &);
        virtual void onConnect(const Controller &);
        virtual void onDisconnect(const Controller &);
        virtual void onExit(const Controller &);
        virtual void onFrame(const Controller &);
        virtual void onFocusGained(const Controller &);
        virtual void onFocusLost(const Controller &);
        virtual void onDeviceChange(const Controller &);
        virtual void onServiceConnect(const Controller &);
        virtual void onServiceDisconnect(const Controller &);

    private:
    const std::string fingerNames[5] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
    const std::string boneNames[4] = {"Metacarpal", "Proximal", "Middle", "Distal"};
    const std::string stateNames[4] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};
};

#endif //ERICS3DENGINE_LEAPLISTENER_HPP