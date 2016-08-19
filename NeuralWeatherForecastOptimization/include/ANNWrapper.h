/**
 * ANNWrapper.h
 * Purpose: defines class ANNWrapper
 *
 * @author Wall.Of.Death
 * @version 1.0 20160811
 */

#ifndef ANNWRAPPER_H
#define ANNWRAPPER_H

#include "DataBuffer.h"
#include "ArtificialNeuroNet.h"
#include "LogWriter.h"
#include "Config.h"
#include "DBInterface.h"

/**
 * The ANNWrapper class
 * @brief a wrapper class for interfacing the artificial neuro nets used for forecast optimization
 *
 * This class provides an easy to use interface to interact with the artificial neuro nets.
 * The wrapper class mainly serves two purposes :
 *   1. easy starting of a training for all artificial neuro nets
 *   2. easy calculating a optimized forecast by calculating all outputs of all artificial neuro nets
 *
 */
class ANNWrapper {
    public:
        /* --- constructor / destructor --- */
        ANNWrapper(DBInterface* dbInterface_);

        /* --- train neuro nets --- */
        void trainArtificialNeuroNets();

        /* --- calculate forecast outputs --- */
        DataBuffer calculateOutput();
    private:
        /* --- ANNs --- */
        ArtificialNeuroNet ANNTemperature;
        ArtificialNeuroNet ANNAirPressure;

        /* --- training ---*/
        bool checkIfTrainingsSetIndexIsAEvaluationIndex(int index_);
        void generateDataSets(vector<vector<double> > *inputValues_, vector<double> *expectedOutputValues_);

        /* --- preprocess data --- */
        vector<double> zTransformVector(const vector<double> &vectorToTransform_);
        vector<double> reZTransformVector(const vector<double> &vectorToReTransform_, const vector<double> &vectorBeforeZTransform_);
        double scaleVector(const double &valueToScale_, double scaleFactor_, bool minimize_);
        vector<double> scaleVector(const vector<double> &vectorToScale_, double scaleFactor_, bool minimize_);
        vector<vector<double>> scaleVector(const vector<vector<double>> &vectorToScale_, double scaleFactor_, bool minimize_);


        /* --- miscellaneous --- */
        LogWriter log;
        DBInterface* dbi;
};

#endif // ANNWRAPPER_H

