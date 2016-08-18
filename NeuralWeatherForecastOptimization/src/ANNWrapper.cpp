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
ANNWrapper::ANNWrapper(DBInterface *dbInterface_) {
    // create and initialize LogWriter-object
    log = LogWriter("ANNWrapper", PATH_OF_LOGFILE);
    log << SLevel(INFO) << "Initialized ANNWrapper" << endl;

    // initialize DBInterface
    dbi = dbInterface_;

    // initialize ANN-objects
    ANNTemperature.init("ANNTemperature",dbi,PATH_OF_NET_STRUCTURE_WITHOUT_LOSS,"",PATH_OF_NET_SOLVER);
    ANNAirPressure.init("ANNAirPressure",dbi,PATH_OF_NET_STRUCTURE_WITHOUT_LOSS,"",PATH_OF_NET_SOLVER);

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
    inputValues.push_back({0.1,0.2,0.3,0.4,0.1,0.2});
    inputValues.push_back({0.2,0.3,0.4,0.1,0.2,0.3});
    inputValues.push_back({0.3,0.4,0.1,0.2,0.3,0.4});
    inputValues.push_back({0.4,0.1,0.2,0.3,0.4,0.1});
    vector<double> expectedOutputValues = {0.3, 0.4, 0.1, 0.2};
    ANNTemperature.train(inputValues,expectedOutputValues);
    //ANNAirPressure.train(inputValues,expectedOutputValues);
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
    //vector<double> inputValues = {0.1,0.2,0.3,0.4,0.1,0.2};
    //vector<double> inputValues = {0.2,0.3,0.4,0.1,0.2,0.3};
    //vector<double> inputValues = {0.3,0.4,0.1,0.2,0.3,0.4};
    vector<double> inputValues = {0.4,0.1,0.2,0.3,0.4,0.1};
    ANNTemperature.setTrainedWeightsCaffemodelPath(PATH_OF_TRAINED_WEIGHTS);
    result.data["ANNTemperature"] = ANNTemperature.forward(inputValues);
    result.data["ANNAirPressure"] = ANNAirPressure.forward(inputValues);
    // --- TODO -- dummy code ---

    return result;
}
