/**
 * ArtificialNeuroNet.cpp
 * Purpose: implements class ArtificialNeuroNet
 *
 * @author Wall.Of.Death
 * @version 1.0 20160811
 */

#include "../include/ArtificialNeuroNet.h"

/* --- constructors / initialization / destructors --- */

/**
 * ArtificialNeuroNet::init
 * @brief initializes LogWriter
 */
void ArtificialNeuroNet::init(string name_){
    // create LogWriter-object
    log = LogWriter("ArtificialNeuroNet : " + name_ , PATH_OF_LOGFILE);

    log << SLevel(INFO) << "Initialized ArtificialNeuroNet with name of " << name_ << "." << endl;
}


/* --- pushing values forward (from input to output) --- */

/**
 * @brief ArtificialNeuroNet::forward propagates a vector of double values through the net
 * @param inputValues_ vector of double values which are to propagate through the net
 * @return returns the vector of output values of the net
 *
 * NOTICE : This is to use for nets with one to many input-neurons and one to many output-neurons
 */
vector<double> ArtificialNeuroNet::forward(vector<double> inputValues_) {
    vector<double> result;

    // --- TODO -- dummy code ---
    result.push_back(rand() % 20);
    // --- TODO -- dummy code ---

    // return vector of values
    return result;
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
    log << SLevel(INFO) << "successfully trained artificial neural net with " <<
           inputValues_.size() << " training samples." << endl;
    // --- TODO -- dummy code ---
    return true;
    // --- TODO -- dummy code ---
}
