/**
 * ArtificialNeuroNet.h
 * Purpose: defines class ArtificialNeuroNet
 *
 * @author Wall.Of.Death
 * @version 1.0 20160811
 */

#ifndef ARTIFICIALNEURONET_H
#define ARTIFICIALNEURONET_H

#include <vector>
#include <cstdlib>
// ---
#include "Config.h"
#include "LogWriter.h"

using namespace std;


/**
 * @brief The ArtificialNeuroNet class is a implementation of an artificial neural net for weather-forecasting
 *
 * The ArtificialNeuroNet class is an implementation of an artificial neural network for optimizing weather
 * forecasts, which uses the caffe-framework.
 *
 * ArtificialNeuroNet provides the possibility for
 *  1. training artificial neural nets with multiple input neurons and one output neuron
 *  2. calculating an output value by given input values.
 *
 *
 */
class ArtificialNeuroNet {
    public:
        /* --- constructors / initialization / destructors --- */
        ArtificialNeuroNet() {};
        void init(string name_);

        /* --- pushing values forward (from input to output) --- */
        double forward (vector<double> inputValues_);

        /* --- train / optimize weights --- */
        bool train (vector< vector<double> > inputValues_, vector<double> expectedOutputValues_);
    private:
        /* --- miscellaneous --- */
        LogWriter log;

};



#endif // ARTIFICIALNEURONET_H
