/**
 * ArtificialNeuroNetWrapper.h
 * Purpose: defines class ArtificialNeuroNetWrapper
 *
 * @author Wall.Of.Death
 * @version 1.0 20160811
 */

#ifndef ARTIFICIALNEURONETWRAPPER_H
#define ARTIFICIALNEURONETWRAPPER_H

#include "DataBuffer.h"
#include "ArtificialNeuroNet.h"
#include "logwriter.h"
#include "config.h"

/**
 * The ArtificialNeuroNetWrapper class
 * @brief a wrapper class for interfacing the artificial neuro nets used for forecast optimization
 *
 * This class provides an easy to use interface to interact with the artificial neuro nets.
 * The wrapper class mainly serves two purposes :
 *   1. easy starting of a training for all artificial neuro nets
 *   2. easy calculating a optimized forecast by calculating all outputs of all artificial neuro nets
 *
 */
class ArtificialNeuroNetWrapper {
    public:
        /* --- constructor / destructor --- */
        ArtificialNeuroNetWrapper();

        /* --- train neuro nets --- */
        void trainArtificialNeuroNets();

        /* --- calculate forecast outputs --- */
        DataBuffer calculateOutput();
    private:
        /* --- ANNs --- */
        ArtificialNeuroNet AirTemperature2MForecast;
        ArtificialNeuroNet Humidity2MForecast;

        /* --- miscellaneous --- */
        LogWriter log;
};

#endif // ARTIFICIALNEURONETWRAPPER_H

