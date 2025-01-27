//
// Created by Eric Moderbacher on 1/26/25.
//

#include "laserKraftWerker.h"

#include <iostream>

laserKraftWerker::laserKraftWerker() {
    //aserKraftWriteHead toBe(); todo add this initializer back... i think

    std::cout << "Thank you for using a laserKraftWerker to describe your laser project and manage your display" << std::endl;

    std::size_t attemptsToOpenDevice(0);
    while (true) {
        //this is a laser show lets get ourselves a laser!
        int numDevs = helios.OpenDevices();

        if (numDevs <= 0) {
            std::cerr << "No DACs found on attempt " << attemptsToOpenDevice << std::endl;

            return; //esm todo validate that this is is from the while loop and not the function... why is this fuzzy? 😢
        }
        std::cout << "Found " << numDevs << " DACs:" << std::endl;
        for (int i = 0; i < numDevs; i++) {
            char name[32];
            if (helios.GetName(i, name) == HELIOS_SUCCESS)
                std::cout << "- " << name << std::endl;
            else
                std::cout << "- (unknown dac)" << std::endl;
        }
    }
}

