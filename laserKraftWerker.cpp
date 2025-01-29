//
// Created by Eric Moderbacher on 1/26/25.
//

#include "laserKraftWerker.h"

#include <iostream>



void laserKraftWerker::start() {
    if (!this->kernel) {
        std::cout << "The Kernel is not set." << std::endl;
        return;
    }

    std::array<HeliosPoint, 1000> animationFrame;
    HeliosPoint toBe, asWas;

    while (true) { // Infinite loop to keep the laser running
        std::size_t index = 0;

        // Populate animation frame with new points
        std::for_each(animationFrame.begin(), animationFrame.end(),
                      [this, &index, &toBe, &asWas](HeliosPoint &point) {
                          this->kernel(toBe, asWas, index++);
                          point = toBe; // Store the updated point in animationFrame
                      });

        // Wait for the DAC to be ready
        while (helios.GetStatus(deviceNo) != 1);

        // Debugging output to verify the first point
        std::cout << "First Point: x=" << animationFrame[0].x
                  << ", y=" << animationFrame[0].y
                  << ", r=" << static_cast<int>(animationFrame[0].r)
                  << ", g=" << static_cast<int>(animationFrame[0].g)
                  << ", b=" << static_cast<int>(animationFrame[0].b)
                  << std::endl;

        // Send frame to the laser DAC
        helios.WriteFrame(deviceNo, 12000, HELIOS_FLAGS_DEFAULT, &animationFrame[0], 1000 );
    }
}


laserKraftWerker::laserKraftWerker() {
    //laserKraftWriteHead toBe(); todo add this initializer back... i think
    std::cout << "laserKraftWerker is connecting to the laser projector(s)" << std::endl;

    this->pointsPerFrame = 1000;

    std::size_t attemptsToOpenDevice(0);

    while (true) {
        //this is a laser show lets get ourselves a laser!
        int numDevs = helios.OpenDevices();

        if (numDevs <= 0) {
            std::cerr << "No DACs found on attempt " << attemptsToOpenDevice << std::endl;
            attemptsToOpenDevice++;
        }
        std::cout << "Found " << numDevs << " DACs:" << std::endl;
        for (int i = 0; i < numDevs; i++)
        {
            char name[32];
            if (helios.GetName(i, name) == HELIOS_SUCCESS)
            {
                std::cout << "- " << name << std::endl;
                deviceNo = i;
                return;
            }
            else
            {
                std::cout << "- (unknown dac)" << std::endl;
                return;
            }

        }

    }
}

