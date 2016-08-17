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
#include "DBInterface.h"
// caffe
#include "caffe/caffe.hpp"
#include "caffe/util/io.hpp"
#include "caffe/blob.hpp"
#include "caffe/common.hpp"
#include "caffe/sgd_solvers.hpp"
#include "caffe/solver.hpp"
#include "google/protobuf/text_format.h"

using namespace caffe;
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
        void init(const string &name_, DBInterface *dbInterface_, const string &netStructurePrototxtPath_,
                  const string &trainedWeightsCaffemodelPath_, const string &solverParametersPrototxtPath_);

        /* --- getter / setter --- */
        string getNetStructurePrototxtPath     () const {return netStructurePrototxtPath     ;};
        string getTrainedWeightsCaffemodelPath () const {return trainedWeightsCaffemodelPath ;};
        string getSolverParametersPrototxtPath () const {return solverParametersPrototxtPath ;};

        void setNetStructurePrototxtPath     (const string& val_) {netStructurePrototxtPath     = val_;};
        void setTrainedWeightsCaffemodelPath (const string& val_) {trainedWeightsCaffemodelPath = val_;};
        void setSolverParametersPrototxtPath (const string& val_) {solverParametersPrototxtPath = val_;};

        /* --- pushing values forward (from input to output) --- */
        double forward (vector<double> inputValues_);

        /* --- train / optimize weights --- */
        bool train (vector< vector<double> > inputValues_, vector<double> expectedOutputValues_);

private:
        // artificial neural net
        Net<double> *net;
        caffe::shared_ptr<Solver<double> > solver_;
        // paths of important files
        string netStructurePrototxtPath;
        string trainedWeightsCaffemodelPath;
        string solverParametersPrototxtPath;

        /* --- miscellaneous --- */
        LogWriter log;
        bool checkValuesAreValid(const vector<double> &values_);
        bool checkValuesAreValid(const vector<vector<double>> &values_);
        DBInterface* dbi;

        /* --- miscellaneous --- */
        void  setDataOfBLOB(Blob<double>* blobToModify_,int indexNum_, int indexChannel_, int indexHeight_, int indexWidth_, double value_);
        double getDataOfBLOB(Blob<double>* blobToReadFrom_, int indexNum_, int indexChannel_, int indexHeight_, int indexWidth_);

};



#endif // ARTIFICIALNEURONET_H
