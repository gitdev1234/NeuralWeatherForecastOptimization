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
void ArtificialNeuroNet::init(const string& name_, DBInterface* dbInterface_, const string &netStructurePrototxtPath_,
                      const string &trainedWeightsCaffemodelPath_, const string &solverParametersPrototxtPath_){
    // create LogWriter-object
    log = LogWriter("ArtificialNeuroNet : " + name_ , PATH_OF_LOGFILE);

    log << SLevel(INFO) << "Initialized ArtificialNeuroNet with name of " << name_ << "." << endl;
    dbi = &dbInterface_->getInstance();

    // set processing source
    #ifdef CPU_ONLY
      Caffe::set_mode(Caffe::CPU);
    #else
      Caffe::set_mode(Caffe::GPU);
    #endif

    // set paths
    setNetStructurePrototxtPath     (netStructurePrototxtPath_);
    setTrainedWeightsCaffemodelPath (trainedWeightsCaffemodelPath_);
    setSolverParametersPrototxtPath (solverParametersPrototxtPath_);


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
        // load network-structure from prototxt-file
        net = new Net<double>(getNetStructurePrototxtPath(),caffe::TEST);

        // load weights
        string trainedWeightsCaffemodelPath_l = getTrainedWeightsCaffemodelPath();
        if (trainedWeightsCaffemodelPath_l != "") { // TODO
            trainedWeightsCaffemodelPath_l = "/home/anon/Desktop/PrivateProjects/Programming/C++/Caffe_Deep_Learning_Framework/Caffe_FunctionApproximation/build-caffe_FunctionApproximation-Unnamed-Debug/train_iter_450000.caffemodel";
            net->CopyTrainedLayersFrom(trainedWeightsCaffemodelPath_l);
        }

        // create BLOB for input layer
        Blob<double>* inputLayer = net->input_blobs()[0];

        // set dimesions of input layer
        // --> for normal caffe works with images, therefore the data
        // --> typically is 4 dimensional
        // --> numberOfImages * numberOfColorChannels * numberOfPixelsInDirectionOfHeight * numberOfPixelsInDirectionOfWidth
        // --> in this case we use 1-dimensional data, therefore the data-dimension is 1*1*1*1
        int num      = 1;
        int channels = inputValues_.size();
        int height   = 1;
        int width    = 1;
        vector<int> dimensionsOfInputData = {num,channels,height,width};
        inputLayer->Reshape(dimensionsOfInputData);

        // forward dimension change to all layers.
        net->Reshape();

        // insert inputValue into inputLayer

        // iterate all neurons for every inputData-dataset
        for (unsigned int inputNeuronIndex = 0; inputNeuronIndex < inputValues_.size(); inputNeuronIndex++) {
            setDataOfBLOB(inputLayer,0,inputNeuronIndex,0,0,inputValues_[inputNeuronIndex]);
        }

        // propagate inputValue through layers
        net->Forward();

        // create BLOB for outputLayer
        Blob<double>* outputLayer = net->output_blobs()[0];
        cout << "num : " << outputLayer->num() << endl;
        cout << "channels : " << outputLayer->channels() << endl;
        cout << "height : " << outputLayer->height() << endl;
        cout << "width : " << outputLayer->width() << endl;

        // copy values in output Layer to 1-dimensional-vector of values
        result = getDataOfBLOB(outputLayer,0,0,0,0);

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



/**
 * @brief ArtificialNeuroNet::setDataOfBLOB sets the data at the given indexes within the blobToModify_ to value_
 * @param blobToModify_ the blob which is to modify
 * @param indexNum_     the index of the first dimension (index of image) valid indexes are from zero to blobToModify_->num() - 1
 * @param indexChannel_ the index of the second dimension (index of channel) valid indexes are from zero to blobToModify_->channels() - 1
 * @param indexHeight_  the index of the third dimension (y-index of pixel) valid indexes are from zero to blobToModify_->height() - 1
 * @param indexWidth_   the index of the fourth dimension (x-index of pixel) valid indexes are from zero to blobToModify_->width - 1
 * @param value_        the new value for data at the given indexes within the blobToModify_
 *
 * for normal caffe works with images, therefore the data typically is 4 dimensional
 * --> numberOfImages * numberOfColorChannels * numberOfPixelsInDirectionOfHeight * numberOfPixelsInDirectionOfWidth
 *
 * To use blobs in whatever way (e.g. normal data which is not images) it might
 * be useful to set the content of the blob manually.
 * This function provides the functionality of doing this.
 *
 * NOTICE : If an invalid index is handed into the function, the function just does nothing, except for
 *          printing an error to the console.
 *
 * NOTICE : ALL INDEXES USED FOR ACCESSING THE BLOB ARE ZERO-BASED
 *
 */
void ArtificialNeuroNet::setDataOfBLOB(Blob<double> *blobToModify_, int indexNum_, int indexChannel_, int indexHeight_, int indexWidth_, double value_) {
    // check if index is invalid
    if ( (indexNum_     < 0) || (indexNum_     > blobToModify_->num()      - 1) ||
         (indexChannel_ < 0) || (indexChannel_ > blobToModify_->channels() - 1) ||
         (indexHeight_  < 0) || (indexHeight_  > blobToModify_->height()   - 1) ||
         (indexWidth_   < 0) || (indexWidth_   > blobToModify_->width()    - 1) ){
        cout << "Error : please use valid indexes!" << endl;
    } else {
        // create a pointer, that points to the first value inside the blobToModify
        double* pointerToBlobValue = blobToModify_->mutable_cpu_data();

        // calculate the address of the requested indexes
        int addressIncrement  = indexNum_     * blobToModify_->channels() * blobToModify_->height() * blobToModify_->width();
            addressIncrement += indexChannel_ * blobToModify_->height()   * blobToModify_->width();
            addressIncrement += indexHeight_  * blobToModify_->width();
            addressIncrement += indexWidth_;

         // let the pointer point to the requested address
         pointerToBlobValue += addressIncrement;

         // set the value at the requested request
         *pointerToBlobValue = value_;
    }
}

/**
 * @brief ArtificialNeuroNet::getDataOfBLOB reads the data stored at the given index within blobToReadFrom_
 * @param blobToReadFrom_ the blob to read from
 * @param indexNum_     the index of the first dimension (index of image) valid indexes are from zero to blobToModify_->num() - 1
 * @param indexChannel_ the index of the second dimension (index of channel) valid indexes are from zero to blobToModify_->channels() - 1
 * @param indexHeight_  the index of the third dimension (y-index of pixel) valid indexes are from zero to blobToModify_->height() - 1
 * @param indexWidth_   the index of the fourth dimension (x-index of pixel) valid indexes are from zero to blobToModify_->width - 1
 * @return returns the data stored at the given index within blobToReadFrom_
 *
 * for normal caffe works with images, therefore the data typically is 4 dimensional
 * --> numberOfImages * numberOfColorChannels * numberOfPixelsInDirectionOfHeight * numberOfPixelsInDirectionOfWidth
 *
 * To use blobs in whatever way (e.g. normal data which is not images) it might
 * be useful to get the content of the blob manually.
 * This function provides the functionality of doing this.
 *
 * NOTICE : If an invalid index is handed into the function, the function returns zero and
 *          prints an error to console
 *
 * NOTICE : ALL INDEXES USED FOR ACCESSING THE BLOB ARE ZERO-BASED
 *
 */
double ArtificialNeuroNet::getDataOfBLOB(Blob<double> *blobToReadFrom_, int indexNum_, int indexChannel_, int indexHeight_, int indexWidth_) {
    // check if index is invalid
    if ( (indexNum_     < 0) || (indexNum_     > blobToReadFrom_->num()      - 1) ||
         (indexChannel_ < 0) || (indexChannel_ > blobToReadFrom_->channels() - 1) ||
         (indexHeight_  < 0) || (indexHeight_  > blobToReadFrom_->height()   - 1) ||
         (indexWidth_   < 0) || (indexWidth_   > blobToReadFrom_->width()    - 1) ){
        cout << "Error : please use valid indexes!" << endl;
        return 0;
    } else {
        return blobToReadFrom_->data_at(indexNum_,indexChannel_,indexHeight_,indexWidth_);
    }
}
