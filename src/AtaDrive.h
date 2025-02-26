﻿/*
 *  Emu80 v. 4.x
 *  © Viktor Pykhonin <pyk@mail.ru>, 2018-2019
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

// AtaDrive.h

// Реализация Ata, заголовочный файл

#ifndef ATADRIVE_H
#define ATADRIVE_H

#include "Pal.h"
#include "PalFile.h"
#include "EmuObjects.h"

class DiskImage;

class AtaDrive : public EmuObject
{
    public:
        //AtaDrive();

        bool setProperty(const std::string& propertyName, const EmuValuesList& values) override;
        void reset() override; // Chip reset

        void notify(EmuObject* sender, int data) override;

        void writeReg(int addr, uint16_t value);
        uint16_t readReg(int addr);

        void assignDiskImage(DiskImage* image);

        static EmuObject* create(const EmuValuesList&) {return new AtaDrive();}

    private:
        //PalFile m_file;
        //bool m_readOnly = false;
        //std::string m_fileName = "";
        DiskImage* m_image = nullptr;

        int m_dev = 0; // ignore for now
        bool m_useLba = false;
        bool m_lba = true;
        int m_cylinders;
        int m_heads;
        int m_sectors;
        int m_cylinder;
        int m_head;
        int m_sector;
        uint32_t m_lbaAddress = 0;
        int m_sectorCount = 0;
        uint8_t m_lastCommand = 0;
        int m_dataCounter;
        uint8_t* m_dataPtr;
        uint8_t m_sectorBuf[512];
        bool m_prefilledData = false;

        void putWord(int wordOffset, uint16_t word);
        void putStr(int wordOffset, const char* str);

        void identify();
        void readSectors();
        void writeSectors();

        void setReadOnly(bool ro);

        void seek();

        bool m_vectorGeometry = false;
        void setVectorGeometry();
};


#endif // ATADRIVE_H
