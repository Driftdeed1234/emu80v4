﻿/*
 *  Emu80 v. 4.x
 *  © Viktor Pykhonin <pyk@mail.ru>, 2016-2023
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// emulation.h

#ifndef EMULATION_H
#define EMULATION_H

#include <list>

#include "PalKeys.h"
#include "EmuTypes.h"
#include "EmuObjects.h"

class Cpu;
class EmuWindow;
class SoundMixer;
class EmuConfig;
class WavReader;
class PrnWriter;
class Platform;


/*struct DevListItem
{
    uint64_t clock;
    ActiveDevice* device;
    DevListItem* next;
};*/

struct DebuggerOptions {
    bool mnemo8080UpperCase = true;
    bool mnemoZ80UpperCase = false;
    bool forceZ80Mnemonics = false;
    bool swapF5F9 = true;
};

class Emulation : public ParentObject
{
    public:
        Emulation(int argc, char** argv); //: EmuObject();

        virtual ~Emulation();

        bool setProperty(const std::string& propertyName, const EmuValuesList& values) override;
        std::string getPropertyStringValue(const std::string& propertyName) override;
        void addChild(EmuObject* child) override;

        void addObject(EmuObject* obj);
        void removeObject(EmuObject* obj);
        EmuObject* findObject(std::string obj);

        void registerActiveDevice(IActive* device);
        void unregisterActiveDevice(IActive* device);

        inline void debugRequest(Cpu* cpu) {m_debugReqCpu = cpu;}
        inline void debugRun() {m_debugReqCpu = nullptr;}
        inline bool isDebuggerActive() {return m_debugReqCpu;}

        void processKey(EmuWindow* wnd, PalKeyCode keyCode, bool isPressed, unsigned unicodeKey = 0);
        void resetKeys(EmuWindow* wnd);
        void draw();
        void sysReq(EmuWindow* wnd, SysReq sr);
        void setWndFocus(EmuWindow* wnd);
        void dropFile(EmuWindow* wnd, const std::string& fileName);
        void restoreFocus();
        void newPlatform(const std::string& platformName);

        void mainLoopCycle();
        void exec(uint64_t ticks, bool forced = false);

        //inline Platform* getPlatform() {return m_platform;} //!!!
        inline uint64_t getCurClock() {return m_curClock;}
        inline SoundMixer* getSoundMixer() {return m_mixer;}
        inline EmuConfig* getConfig() {return m_config;}
        inline WavReader* getWavReader() {return m_wavReader;}
        inline PrnWriter* getPrnWriter() {return m_prnWriter;}

        void setFrequency(int64_t freq) override;
        int64_t getFrequency() {return m_frequency;}
        void setSampleRate(int sampleRate);             // установка частоты дискретизации звуковой карты
        int getSampleRate() {return m_sampleRate;}
        void setFrameRate(int frameRate);               // установка частоты кадров, 0 - max
        void setVsync(bool vsync);                      // установка vsync
        bool getVsync() {return m_vsync;}
        void setSpeedUpFactor(unsigned speed);

        unsigned getSpeedUpFactor() {return m_speedUpFactor;}
        bool getPausedState() {return m_isPaused;}

        void processCmdLine();

        const DebuggerOptions& getDebuggerOptions() {return m_debuggerOptions;}

    private:
        std::vector<IActive*> m_activeDevVector;
        IActive** m_activeDevices = nullptr;
        int nDevices = 0;
        bool inCycle;
        uint64_t m_clockOffset = 0;
        uint64_t m_sysClock;
        uint64_t m_prevSysClock = 0;
        Cpu* m_debugReqCpu = nullptr;

        bool m_isPaused = false;
        unsigned m_speedUpFactor = 1;

        uint64_t m_frequency;
        unsigned m_frameRate;
        bool m_vsync;
        unsigned m_sampleRate;

        std::list<EmuObject*> m_objectList;
        std::list<Platform*> m_platformList;

        uint64_t m_curClock = 0;

        EmuConfig* m_config;
        SoundMixer* m_mixer;
        WavReader* m_wavReader;
        PrnWriter* m_prnWriter;

        Platform* m_lastActivePlatform = nullptr;

        Platform* platformByWindow(EmuWindow* window);

        void checkPlatforms();

        // параметры командной строки
        int m_argc;
        char** m_argv;

        bool m_platformCreatedFromCmdLine = false;
        void runPlatform (const std::string& platformName);

        DebuggerOptions m_debuggerOptions;
};


#endif  // EMULATION_H

