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

    // Ping-pong buffers
    std::array<HeliosPoint, 15000> bufferA, bufferB;
    HeliosPoint toBe, asWas;
    bool useBufferA = true; // Flag to switch between buffers
    std::size_t index = 0;
    while (true) { // Infinite loop to keep the laser running
        std::array<HeliosPoint, 15000> &currentBuffer = useBufferA ? bufferA : bufferB;

        // Populate current buffer with new points

        std::for_each(currentBuffer.begin(), currentBuffer.end(),
                      [this, &index, &toBe, &asWas](HeliosPoint &point) {
                          this->kernel(toBe, asWas, index++);
                          point = toBe; // Store the updated point
                      });

        // Wait for the DAC to be ready
        while (helios.GetStatus(deviceNo) != 1);

        // Debugging output to verify the first point
//        std::cout << "First Point (Buffer " << (useBufferA ? "A" : "B") << "): x=" << currentBuffer[0].x
//                  << ", y=" << currentBuffer[0].y
//                  << ", r=" << static_cast<int>(currentBuffer[0].r)
//                  << ", g=" << static_cast<int>(currentBuffer[0].g)
//                  << ", b=" << static_cast<int>(currentBuffer[0].b)
//                  << std::endl;

        // Send frame to the laser DAC
        helios.WriteFrame(deviceNo, 15000, HELIOS_FLAGS_DEFAULT, currentBuffer.data(), 15000);

        // Swap buffers
        useBufferA = !useBufferA;
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

