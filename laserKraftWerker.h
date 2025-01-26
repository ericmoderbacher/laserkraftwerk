//
// Created by Eric Moderbacher on 1/26/25.
//

#ifndef TENOANACATLPROJECT_LASERKERNEL_H
#define TENOANACATLPROJECT_LASERKERNEL_H

#include <functional>
#include <HeliosDac.h>


class laserKraftWriteHead
        {
            std::size_t X, Y;
            uint_8 red;
            uint_8 green;
            uint_8 blue;
        };


class laserKraftWerker{ //watch out sweety, you might cut yourself on that edge!
    public:
        std::size_t screenXSize, screenYSize; //im on this kick where i like to use size_t to say "the largest unsigned integral type on your system"
        laserKraftWerker();
        std::function<std::size_t(const std::size_t &)> laserKraftKernel; //esm todo validate that what i think is being said is being said.  then switch to maybe a void return type.  I think the first std::size_t is the return type of the funciont and the second one in the parenthesis () is the input types.
        laserKraftWriteHead toBe, asWas; //thatWhichHasNotComeToPass is intentionally missing
    private:
    HeliosDac helios;
};




#endif //TENOANACATLPROJECT_LASERKERNEL_H
