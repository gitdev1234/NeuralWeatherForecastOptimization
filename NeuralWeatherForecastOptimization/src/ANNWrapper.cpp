/**
 * ANNWrapper.cpp
 * Purpose: implements class ANNWrapper
 *
 * @author Wall.Of.Death
 * @version 1.0 20160811
 */

#include "ANNWrapper.h"

/* --- constructor / destructor --- */

/**
 * ANNWrapper::ANNWrapper
 * @brief constructor of ANN-wrapper class, initializes LogWriter and ANN-objects
 *
 */
ANNWrapper::ANNWrapper() {
    // create and initialize LogWriter-object
    log = LogWriter("ANNWrapper", PATH_OF_LOGFILE);
    log << SLevel(INFO) << "Initialized ANNWrapper" << endl;

    // initialize ANN-objects
    AirTemperature2MForecast.init("AirTemperature2MForecast");
          Humidity2MForecast.init("Humidity2MForecast");
}

/* --- train neuro nets --- */

/**
 * ANNWrapper::trainArtificialNeuroNets
 * @brief trains all artificial neuro nets inside this class
 */
void ANNWrapper::trainArtificialNeuroNets() {
    log << SLevel(INFO) << "Successfully trained Artificial Neuro Net for Temperature 2m" << endl;

    // --- TODO -- dummy code ---
    vector< vector<double> > inputValues;
    vector<double> expectedOutputValues;
    AirTemperature2MForecast.train(inputValues,expectedOutputValues);
          Humidity2MForecast.train(inputValues,expectedOutputValues);
    // --- TODO -- dummy code ---
}

/* --- calculate forecast outputs --- */

/**
 * ANNWrapper::calculateOutput
 * @brief calculates the output of all artificial neuron nets inside this class and outputs them
 * @return returns DataBuffer which contains all outputs of all neuro nets inside this class
 */
DataBuffer ANNWrapper::calculateOutput() {
    DataBuffer result;

    // --- TODO -- dummy code ---
    vector<double> inputValues;
    result.data["AirTemperature2MForecast"] = AirTemperature2MForecast.forward(inputValues)[0];
    result.data[      "Humidity2MForecast"] =       Humidity2MForecast.forward(inputValues)[0];
    // --- TODO -- dummy code ---

    return result;
}
