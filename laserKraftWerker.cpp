//
// Created by Eric Moderbacher on 1/26/25.
//
#include "laserKraftWerker.h"

#include <iostream>
#include <algorithm> // for std::for_each
#include <cmath>

std::size_t frameCount = 0; // Tracks the number of frames sent to the laser
constexpr std::size_t framesPerUpdate = 3; // For game-of-life or other usage (if needed)

// If you have a schedule of events, presumably it's declared in laserKraftWerker.h as:
//   std::vector<event> schedule;
//   where `event` is: struct event { std::size_t when; std::function<std::size_t(...)> whatNow; };

void laserKraftWerker::start()
{
    if (!this->kernel) {
        std::cout << "The Kernel is not set." << std::endl;
        return;
    }

    // Create two buffers for ping-pong
    std::array<HeliosPoint, 15000> bufferA, bufferB;
    HeliosPoint toBe, asWas;

    bool useBufferA = true;

    // We'll interpret each "loop iteration" as 1 second of show time
    // because we generate 15,000 points per buffer at 15k pps => 1 second
    std::size_t timeInSeconds = 0;

    // If you still want an overall point counter:
    std::size_t globalPointIndex = 0;

    // Infinite loop
    while (true)
    {
        // Choose the current buffer
        std::array<HeliosPoint, 15000> &currentBuffer =
                (useBufferA ? bufferA : bufferB);

        // 1) Fill this buffer with 15,000 calls to the current kernel
        std::for_each(currentBuffer.begin(), currentBuffer.end(),
                      [this, &globalPointIndex, &toBe, &asWas](HeliosPoint &point)
                      {
                          // Call the current kernel
                          this->kernel(toBe, asWas, globalPointIndex++);
                          point = toBe;
                      });

        // 2) Wait for the DAC to be ready
        while (helios.GetStatus(deviceNo) != 1) {
            // Possibly sleep
            // std::this_thread::sleep_for(std::chrono::microseconds(100));
        }

        // 3) Send frame to the laser DAC
        helios.WriteFrame(deviceNo, 15000, HELIOS_FLAGS_DEFAULT,
                          currentBuffer.data(), 15000);

        // 4) Swap buffers
        useBufferA = !useBufferA;

        // 5) We just finished "one second" of data
        timeInSeconds++;

        // 6) Check if we have any scheduled event at this second
        //    (Only relevant if you have a schedule in your .h)
        for (auto &evt : this->schedule) {
            if (evt.when == timeInSeconds) {
                std::cout << "[INFO] Time=" << timeInSeconds
                          << " => switching kernel.\n";
                this->kernel = evt.whatNow;
                // If you want one-time events only, you could remove from schedule
                // or mark them used. For now, we just keep them.
            }
        }

        // ... loop back and fill the next second ...
    }
}

laserKraftWerker::laserKraftWerker()
{
    std::cout << "laserKraftWerker is connecting to the laser projector(s)" << std::endl;

    this->pointsPerFrame = 1000; // or any other usage in your code

    std::size_t attemptsToOpenDevice(0);

    while (true) {
        // Attempt to open devices
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
                return; // success
            }
            else
            {
                std::cout << "- (unknown dac)" << std::endl;
                return;
            }
        }
    }
}
