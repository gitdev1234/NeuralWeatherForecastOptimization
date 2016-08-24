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

    vector< vector<double> > inputValues;
    vector<double> expectedOutputValues;

    // define training parameter
    // measurements to use for prediction
    vector<string> predictorList = {"Lufttemperatur_2m"};
    // measurement to predict
    string predictant = "Lufttemperatur_2m";
    // dataSources to use for prediction
    vector<string> dataSourceList = {"WeatherStation","Forecast"};
    // amount of previous values to consider
    int predictionWindowSize = 3;
    // yes, generate datasets for training, not for evaluating
    bool trainingDataSet = true;

    // generate input and expected output data
    generateDataSets(&inputValues,&expectedOutputValues,predictorList,predictant,dataSourceList,predictionWindowSize,trainingDataSet);

    // normalize and scale data-sets
    inputValues = zTransformVector(inputValues);
    inputValues = scaleVector(inputValues,SCALING_FACTOR,true);
    expectedOutputValues = zTransformVector(expectedOutputValues);
    expectedOutputValues = scaleVector(expectedOutputValues,SCALING_FACTOR,true);

    // start training
    ANNTemperature.train(inputValues,expectedOutputValues);

    /* --- ANN for Pressure --- */


    // define training parameter
    // measurements to use for prediction
    predictorList = {"Luftdruck_2m"};
    // measurement to predict
    predictant = "Luftdruck_2m";
    // dataSources to use for prediction
    dataSourceList = {"WeatherStation","Forecast"};
    // amount of previous values to consider
    predictionWindowSize = 3;
    // yes, generate datasets for training, not for evaluating
    trainingDataSet = true;

    // generate input and expected output data
    generateDataSets(&inputValues,&expectedOutputValues,predictorList,predictant,dataSourceList,predictionWindowSize,trainingDataSet);

    // normalize and scale data-sets
    inputValues = zTransformVector(inputValues);
    inputValues = scaleVector(inputValues,SCALING_FACTOR,true);
    expectedOutputValues = zTransformVector(expectedOutputValues);
    expectedOutputValues = scaleVector(expectedOutputValues,SCALING_FACTOR,true);

    // start training
    ANNAirPressure.train(inputValues,expectedOutputValues);


}

/* --- calculate forecast outputs --- */

/**
 * ANNWrapper::calculateOutput
 * @brief calculates the output of all artificial neuron nets inside this class and outputs them
 * @return returns DataBuffer which contains all outputs of all neuro nets inside this class
 */
DataBuffer ANNWrapper::calculateOutput() {
    DataBuffer result;

    /* --- ANN Temperature --- */

    vector< vector<double> > inputValuesALL;
    vector<double> expectedOutputValuesALL;

    // define training parameter
    // measurements to use for prediction
    vector<string> predictorList = {"Lufttemperatur_2m"};
    // measurement to predict
    string predictant = "Lufttemperatur_2m";
    // dataSources to use for prediction
    vector<string> dataSourceList = {"WeatherStation","Forecast"};
    // amount of previous values to consider
    int predictionWindowSize = 3;
    // yes, generate datasets for training/calculating output, not for evaluating
    bool trainingDataSet = true;

    // generate input and expected output data
    generateDataSets(&inputValuesALL,&expectedOutputValuesALL,predictorList,predictant,dataSourceList,predictionWindowSize,trainingDataSet);

    // normalize and scale data-sets
    inputValuesALL = zTransformVector(inputValuesALL);
    inputValuesALL = scaleVector(inputValuesALL,SCALING_FACTOR,true);

    // get last dataset for last calculateable forecast
    vector<double> inputValues = inputValuesALL[inputValuesALL.size()-1];

    // set path for trained weights
    ANNTemperature.setTrainedWeightsCaffemodelPath(PATH_OF_TRAINED_WEIGHTS);

    // calculate output of net
    double scaledAndTransformedOutput = ANNTemperature.forward(inputValues);
    // redo z-transformation
    double scaledOutput = reZTransformVector({scaledAndTransformedOutput},expectedOutputValuesALL)[0];
    // redo scaling
    double realOutput = scaleVector(scaledOutput,SCALING_FACTOR,false);
    result.data["ANNTemperature"] = realOutput;

    /* --- ANN Air Pressure --- */

    // define training parameter
    // measurements to use for prediction
    predictorList = {"Luftdruck_2m"};
    // measurement to predict
    predictant = "Luftdruck_2m";
    // dataSources to use for prediction
    dataSourceList = {"WeatherStation","Forecast"};
    // amount of previous values to consider
    predictionWindowSize = 3;
    // yes, generate datasets for training/calculating output, not for evaluating
    trainingDataSet = true;

    // generate input and expected output data
    generateDataSets(&inputValuesALL,&expectedOutputValuesALL,predictorList,predictant,dataSourceList,predictionWindowSize,trainingDataSet);

    // normalize and scale data-sets
    inputValuesALL = zTransformVector(inputValuesALL);
    inputValuesALL = scaleVector(inputValuesALL,SCALING_FACTOR,true);

    // get last dataset for last calculateable forecast
    inputValues = inputValuesALL[inputValuesALL.size()-1];

    // set path for trained weights
    ANNAirPressure.setTrainedWeightsCaffemodelPath(PATH_OF_TRAINED_WEIGHTS);

    // calculate output of net
    scaledAndTransformedOutput = ANNAirPressure.forward(inputValues);
    // redo z-transformation
    scaledOutput = reZTransformVector({scaledAndTransformedOutput},expectedOutputValuesALL)[0];
    // redo scaling
    realOutput = scaleVector(scaledOutput,SCALING_FACTOR,false);
    result.data["ANNAirPressure"] = realOutput;

    return result;
}

bool ANNWrapper::checkIfTrainingsSetIndexIsAEvaluationIndex(int index_) {
    int numberOfEvaluationDataSets = double(TOTAL_NUMBER_OF_TRAINING_SAMPLES) * double(1-PROPORTION_OF_TRAINING_SET);
    int distanceBetweenEvaluationData = TOTAL_NUMBER_OF_TRAINING_SAMPLES / numberOfEvaluationDataSets;
    return (index_ % distanceBetweenEvaluationData == 0);
}

void ANNWrapper::generateDataSets(vector<vector<double> > *inputValues_,
                                  vector<double>          *expectedOutputValues_,
                                  const vector<string>    &predictorList_,
                                  const string            &predictant_,
                                  const vector<string>    &dataSourceList_,
                                  const int                predictionWindowSize_ ,
                                  bool                     trainingDataSet_) {
    inputValues_->clear();
    expectedOutputValues_->clear();

    inputValues_->resize(TOTAL_NUMBER_OF_TRAINING_SAMPLES);
    expectedOutputValues_->resize(TOTAL_NUMBER_OF_TRAINING_SAMPLES);

    for (int DataSourceIndex = 0; DataSourceIndex < dataSourceList_.size(); DataSourceIndex++) {
        DataBuffer trainingSet;
        trainingSet.dataSource = dataSourceList_[DataSourceIndex];
        trainingSet.useDataSource = true;
        trainingSet.startDateTime.tm_year = START_DATE_TIME_OF_TRAINING_SET.date().year();
        trainingSet.startDateTime.tm_mon  = START_DATE_TIME_OF_TRAINING_SET.date().month();
        trainingSet.startDateTime.tm_mday = START_DATE_TIME_OF_TRAINING_SET.date().day();
        trainingSet.startDateTime.tm_hour = START_DATE_TIME_OF_TRAINING_SET.time().hour();
        trainingSet.startDateTime.tm_min  = START_DATE_TIME_OF_TRAINING_SET.time().minute();
        trainingSet.startDateTime.tm_sec  = START_DATE_TIME_OF_TRAINING_SET.time().second();
        trainingSet.endDateTime.tm_year   =   END_DATE_TIME_OF_TRAINING_SET.date().year();
        trainingSet.endDateTime.tm_mon    =   END_DATE_TIME_OF_TRAINING_SET.date().month();
        trainingSet.endDateTime.tm_mday   =   END_DATE_TIME_OF_TRAINING_SET.date().day();
        trainingSet.endDateTime.tm_hour   =   END_DATE_TIME_OF_TRAINING_SET.time().hour();
        trainingSet.endDateTime.tm_min    =   END_DATE_TIME_OF_TRAINING_SET.time().minute();
        trainingSet.endDateTime.tm_sec    =   END_DATE_TIME_OF_TRAINING_SET.time().second();
        trainingSet.useDateTimes = true;
        for (int PredictorIndex = 0; PredictorIndex < predictorList_.size(); PredictorIndex++) {
            trainingSet.data[predictorList_[PredictorIndex]] = 0;
        }

        // read all data of all predictors and current datasource
        //
        vector<DataBuffer> trainingSetBuffer = dbi->readFromDataBase(trainingSet);

        // iterate through all databuffers
        for(int trainingSetIndex = 0; trainingSetIndex < trainingSetBuffer.size(); trainingSetIndex++) {
            // make sure data is not of evalution training set
            if ( ( ( trainingDataSet_) && (!checkIfTrainingsSetIndexIsAEvaluationIndex(trainingSetIndex)) ) ||
                 ( (!trainingDataSet_) && ( checkIfTrainingsSetIndexIsAEvaluationIndex(trainingSetIndex)) ) ){
                // make sure there is enought previous data
                // and that there is a expected output data
                if ( (trainingSetIndex >= predictionWindowSize_ - 1) &&
                     (trainingSetIndex <= TOTAL_NUMBER_OF_TRAINING_SAMPLES - DISTANCE_OF_PREDICTION - 1) ){
                    // go from current position backwards to get current data and
                    // all needed previous data
                    for (int predictionWindowIndex = trainingSetIndex; predictionWindowIndex >= trainingSetIndex - predictionWindowSize_ + 1; predictionWindowIndex--) {
                        vector<double> inputValuesOfCurrentDataSource;
                        typedef std::map<string, double>::iterator it_type;
                        for(it_type iterator = trainingSetBuffer[predictionWindowIndex].data.begin(); iterator != trainingSetBuffer[predictionWindowIndex].data.end(); iterator++) {
                            // get input value
                            inputValuesOfCurrentDataSource.push_back(iterator->second);
                        }
                        vector<double> tempVector = inputValues_->at(trainingSetIndex);
                        tempVector.insert(tempVector.end(),inputValuesOfCurrentDataSource.begin(),inputValuesOfCurrentDataSource.end());
                        inputValues_->at(trainingSetIndex) = tempVector;
                    }
                    // get expected output value
                    int indexOfExpectedOutput = trainingSetIndex + DISTANCE_OF_PREDICTION;
                    expectedOutputValues_->at(trainingSetIndex) =  trainingSetBuffer[indexOfExpectedOutput].data[predictant_];
                }
            }
        }
    }

    vector<vector<double>>::iterator i = inputValues_->begin();
    vector<double>::iterator i2 = expectedOutputValues_->begin();
    int index = 0;
    while(i != inputValues_->end()) {
        if(inputValues_->at(index).size() == 0) {
            i = inputValues_->erase(i);
            i2 = expectedOutputValues_->erase(i2);
        } else {
            ++i;
            ++i2;
            index++;
        }
    }

}



/* --- preprocess data --- */

vector<double> ANNWrapper::zTransformVector(const vector<double>& vectorToTransform_) {

    vector<double> result = vectorToTransform_;

    // calculate mean
    double sum = std::accumulate(vectorToTransform_.begin(), vectorToTransform_.end(), 0.0);
    double mean = double(sum) / double(vectorToTransform_.size());
    // calculate standard deviation
    double sq_sum = std::inner_product(vectorToTransform_.begin(), vectorToTransform_.end(), vectorToTransform_.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / double(vectorToTransform_.size()-1) - mean * mean);

    for (int i = 0 ; i < vectorToTransform_.size(); i++) {
        result[i] = (double(vectorToTransform_[i]) - double(mean)) / double(stdev);
    }

    return result;

}

vector<vector<double> > ANNWrapper::zTransformVector(const vector<vector<double> > &vectorToTransform_) {
    vector< vector<double> > result = vectorToTransform_;

    for (int i = 0 ; i < vectorToTransform_.size(); i++) {
        result[i] = zTransformVector(vectorToTransform_[i]);
    }

    return result;
}

vector<double> ANNWrapper::reZTransformVector(const vector<double> &vectorToReTransform_, const vector<double> &vectorBeforeZTransform_) {
    vector<double> result = vectorToReTransform_;

    // calculate mean
    double sum = std::accumulate(vectorBeforeZTransform_.begin(), vectorBeforeZTransform_.end(), 0.0);
    double mean = double(sum) / double(vectorBeforeZTransform_.size());
    // calculate standard deviation
    double sq_sum = std::inner_product(vectorBeforeZTransform_.begin(), vectorBeforeZTransform_.end(), vectorBeforeZTransform_.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / double(vectorBeforeZTransform_.size()-1) - mean * mean);

    for (int i = 0 ; i < vectorToReTransform_.size(); i++) {
        result[i] = double(vectorToReTransform_[i]) * double(stdev) + double(mean);
    }

    return result;
}

vector<vector<double> > ANNWrapper::reZTransformVector(const vector<vector<double> > &vectorToTransform_, const vector<vector<double>> &vectorBeforeZTransform_) {
    vector< vector<double> > result = vectorToTransform_;

    for (int i = 0 ; i < vectorToTransform_.size(); i++) {
        result[i] = reZTransformVector(vectorToTransform_[i],vectorBeforeZTransform_[i]);
    }

    return result;
}

double ANNWrapper::scaleVector(const double &valueToScale_, double scaleFactor_, bool minimize_) {
    if (minimize_) {
        return valueToScale_ / scaleFactor_;
    } else {
        return valueToScale_ * scaleFactor_;
    }
}

vector<double> ANNWrapper::scaleVector(const vector<double> &vectorToScale_, double scaleFactor_, bool minimize_) {
    vector<double> result = vectorToScale_;

    for (int i = 0 ; i < vectorToScale_.size(); i++) {
        if (minimize_) {
            result[i] = double(vectorToScale_[i]) / double(scaleFactor_);
        } else {
            result[i] = double(vectorToScale_[i]) * double(scaleFactor_);
        }
    }

    return result;
}

vector<vector<double> > ANNWrapper::scaleVector(const vector<vector<double> > &vectorToScale_, double scaleFactor_, bool minimize_) {
    vector< vector<double> > result = vectorToScale_;

    for (int i = 0 ; i < vectorToScale_.size(); i++) {
        result[i] = scaleVector(vectorToScale_[i],scaleFactor_,minimize_);
    }

    return result;
}

vector<double> ANNWrapper::moveVectorByValue(const vector<double> &vectorToMove_, double delta_) {
    vector<double> result = vectorToMove_;

    for (int i = 0 ; i < vectorToMove_.size(); i++) {
        result[i] += delta_;
    }

    return result;
}

vector<vector<double>> ANNWrapper::moveVectorByValue(const vector<vector<double>> &vectorToMove_, double delta_) {
    vector<vector<double>> result = vectorToMove_;

    for (int i = 0 ; i < vectorToMove_.size(); i++) {
        result[i] = moveVectorByValue(result[i],delta_);
    }

    return result;
}
