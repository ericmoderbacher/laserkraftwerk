//
// Created by Eric Moderbacher on 1/26/25.
//

#ifndef TENOANACATLPROJECT_LASERKERNEL_H
#define TENOANACATLPROJECT_LASERKERNEL_H

#include <functional>
#include "HeliosDac.h"
#include <cinttypes>


class laserKraftWriteHead
        {
        public:
            HeliosPoint Point;
        };


class laserKraftWerker
        {
        public:
            std::size_t screenXSize(), screenYSize(); //im on this kick where i like to use size_t to say "the largest unsigned integral type on your system"
            std::size_t pointsPerFrame;
            std::size_t deviceNo;
            laserKraftWerker();
            std::function<std::size_t(HeliosPoint &, HeliosPoint &, const std::size_t &)> kernel; //esm todo validate that what i think is being said is being said.  then switch to maybe a void return type.  I think the first std::size_t is the return type of the funciont and the second one in the parenthesis () is the input types.
            laserKraftWriteHead toBe, asWas; //thatWhichHasNotComeToPass is intentionally missing

    // If you want to schedule changes:
    struct event {
        std::size_t when;
        std::function<std::size_t(HeliosPoint &, HeliosPoint &, const std::size_t &)> whatNow;
    };
    // A list of events
    std::vector<event> schedule;

    // Helper to add an event
    void addEvent(std::size_t when, std::function<std::size_t(HeliosPoint &, HeliosPoint &, const std::size_t &)> newKernel)
    {
        schedule.push_back({when, newKernel});
    }


            void start();
            void end();


        private:
            HeliosDac helios;
        };




#endif //TENOANACATLPROJECT_LASERKERNEL_H
