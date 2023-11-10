#ifndef PROV_SANDBOX_HPP
#define PROV_SANDBOX_HPP

#include "../Mid/Lib.hpp"
using namespace MID;
using namespace SampleOperators;

#include <random>

void sandbox(std::string const& fileName)
{
    Sample sample;

    for (int i = 0; i < 16; i++) {
        sample << Note(i, 1, 60 + rand() % 13);
        sample << PitchBend(i)((double)(rand() % 100) * 0.04 - 2.);
        if (i < 11) sample << Aftertouch(i)((double)(rand() % 100) * 0.01);
        else sample << Aftertouch(i)(60 + rand() % 2, (double)(rand() % 100) * 0.005);
    }
    sample << Instrument(5).accordion();
    sample << Pan(3)(0.92);
    sample << Pan(5.5)(0.65);
    sample << ChannelVolume(4.5)(0.2);
    sample << ChannelVolume(11)(0.35);
        /**/sample.save("@ 1.mid");
    
    auto res1 = sample.reverse(Selector::all(), PreserveCurves::simplyCopyEvents);
        /**/res1.save("@ 2.mid");
    
    auto res2 = sample.reverse(Selector::all(), PreserveCurves::preserveCurves);
        /**/res2.save("@ 3.mid");
    
    //auto res1 = sample.split(6, PreserveCurves::simplyCopyEvents);
    //auto res1b = res1.second.split(3.5, PreserveCurves::simplyCopyEvents);
    //    /**/res1.first.save("@ 2a.mid");
    //    /**/res1b.first.save("@ 2b.mid");
    //    /**/res1b.second.save("@ 2c.mid");
    //
    //auto res2 = sample.split(6, PreserveCurves::preserveCurves);
    //auto res2b = res2.second.split(3.5, PreserveCurves::preserveCurves);
    //    /**/res2.first.save("@ 3a.mid");
    //    /**/res2b.first.save("@ 3b.mid");
    //    /**/res2b.second.save("@ 3c.mid");
}

#endif
