/**
 * ArtificialNeuroNet.cpp
 * Purpose: implements class ArtificialNeuroNet
 *
 * @author Wall.Of.Death
 * @version 1.0 20160811
 */

#include "ArtificialNeuroNet.h"

/* --- constructors / initialization / destructors --- */

/**
 * ArtificialNeuroNet::init
 * @brief initializes LogWriter
 */
void ArtificialNeuroNet::init(string name_, DBInterface* dbInterface_){
    // create LogWriter-object
    log = LogWriter("ArtificialNeuroNet : " + name_ , PATH_OF_LOGFILE);

    log << SLevel(INFO) << "Initialized ArtificialNeuroNet with name of " << name_ << "." << endl;
    dbi = &dbInterface_->getInstance();
}


/* --- pushing values forward (from input to output) --- */

/**
 * @brief ArtificialNeuroNet::forward propagates a vector of double values through the net
 * @param inputValues_ vector of double values which are to propagate through the net
 * @return returns the vector of output values of the net
 *
 * NOTICE : This is to use for nets with one to many input-neurons and one to many output-neurons
 */
double ArtificialNeuroNet::forward(vector<double> inputValues_) {
    double result;

    if (!checkValuesAreValid(inputValues_)) {
        log << SLevel(ERROR) << "Aborted forward because of invalid input-values. (Probably empty" <<
                                " vector or values out of range [INFLUXDB_MIN..INFLUXDB_MAX]" << endl;
        dbi->writeStatusOK(false);
        return 0;
    } else {

        // --- TODO -- dummy code ---
        result = rand() % 20;
        // --- TODO -- dummy code ---

        // return vector of values
        return result;
    }
}

/* --- train / optimize weights --- */

/**
 * ArtificialNeuroNet::train
 * @brief trains the network with the given inputs and expected outputs
 * @param inputValues_ vector of inputValues
 * @param expectedOutputValues_ vector of output values
 * @return returns true if training has succesfully ended, otherwise false
 *
 * The train function executes a learning to the net by doing the following steps :
 *   1. create a solver object which encapsulate and controls the net solverParametersPrototxtPath-file
 *   2. load the input data and the expected output data to the net
 *   3. execute solver_->Solve, which
 *        3.1 automatically loops through the input data
 *        3.2 propagates the input data through the net,
 *        3.3 calculates the current loss of the output
 *        3.4 calculates deltas for every weight, depending on the loss
 *        3.5 calculates new weights
 *        3.6 outputs preliminary results and the final result of the trained weights
 *            into *.caffemodel - files
 *
 * NOTICE : the size of outer dimension of inputValues and expected output values has to be equal
 *          otherwise the function stops and returns false
 * NOTICE : the file, which is located at getSolverParametersPrototxtPath has to be a valid
 *          google-protobuf file which can be used to specify a caffe-solver, otherwise the
 *          function stops and returns false
 *
 */
bool ArtificialNeuroNet::train(vector<vector<double> > inputValues_, vector<double> expectedOutputValues_) {
    if ( (!checkValuesAreValid(inputValues_)) || (!checkValuesAreValid(inputValues_)) ) {
        log << SLevel(ERROR) << "Aborted train because of invalid input-values or expected outputValues. " <<
               "(Probably empty vectors or values out of range [INFLUXDB_MIN..INFLUXDB_MAX]" << endl;
        dbi->writeStatusOK(false);
        return 0;
    } else {

        log << SLevel(INFO) << "successfully trained artificial neural net with " <<
               inputValues_.size() << " training samples." << endl;
        // --- TODO -- dummy code ---
        return true;
        // --- TODO -- dummy code ---
    }
}

/**
 * ArtificialNeuroNet::checkInputValuesAreValid
 * @brief iterates inputValues_ and checks if they are valid values
 * @param inputValues_ vector of double values to be checked
 * @return returns false if values_ either contains no values or
 *         contains values > INFLUXDB_MAX or values < INFLUXDB_MIN
 *         otherwise true
 *
 */
bool ArtificialNeuroNet::checkValuesAreValid(const vector<double> &values_) {
    if (values_.size() == 0) {
        return false;
    } else {
        for (int i = 0; i < values_.size() ; i++) {
            if ( (values_[i] > INFLUXDB_MAX) || (values_[i] < INFLUXDB_MIN) )  {
                return false;
            }
        }
    }
    return true;
}

/**
 * ArtificialNeuroNet::checkInputValuesAreValid
 * @brief iterates inputValues_ and checks if they are valid values
 * @param inputValues_ vector of vector of double values to be checked
 * @return returns false if values_ either contains no values or
 *         contains values > INFLUXDB_MAX or values < INFLUXDB_MIN
 *         otherwise true
 *
 */
bool ArtificialNeuroNet::checkValuesAreValid(const vector<vector<double> > &values_) {
    if (values_.size() == 0) {
        return false;
    } else {
        for (int i = 0; i < values_.size() ; i++) {
            if (!checkValuesAreValid(values_[i]))  {
                return false;
            }
        }
    }
    return true;
}
