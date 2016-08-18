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
    vector<double> expectedOutputValues = {0.4,0.3,0.5,0.4,0.6,0.5,0.1,0.0,0.2,0.1,0.3,0.2};
    generateDataSets(&inputValues,&expectedOutputValues);


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
    //vector<double> inputValues = {0.1,0.0,0.2,0.1,0.3,0.2};
    //vector<double> inputValues = {0.0,0.2,0.1,0.3,0.2,0.4};
    //vector<double> inputValues = {0.2,0.1,0.3,0.2,0.4,0.3};
    //vector<double> inputValues = {0.1,0.3,0.2,0.4,0.3,0.5};
    //vector<double> inputValues = {0.3,0.2,0.4,0.3,0.5,0.4};
    vector<double> inputValues = {0.2,0.4,0.3,0.5,0.4,0.6};
    //vector<double> inputValues = {0.4,0.3,0.5,0.4,0.6,0.5};
    // ---
    //vector<double> inputValues = {0.3,0.5,0.4,0.6,0.5,0.1};
    //vector<double> inputValues = {0.5,0.4,0.6,0.5,0.1,0.0};
    //vector<double> inputValues = {0.4,0.6,0.5,0.1,0.0,0.2};
    //vector<double> inputValues = {0.6,0.5,0.1,0.0,0.2,0.1};
    //vector<double> inputValues = {0.5,0.1,0.0,0.2,0.1,0.3};
    ANNTemperature.setTrainedWeightsCaffemodelPath(PATH_OF_TRAINED_WEIGHTS);
    result.data["ANNTemperature"] = ANNTemperature.forward(inputValues);
    result.data["ANNAirPressure"] = ANNAirPressure.forward(inputValues);
    // --- TODO -- dummy code ---

    return result;
}

bool ANNWrapper::checkIfDateTimeIsAEvaluationDate(tm dateTime_) {
    QDateTime currentDateTime = QDateTime(QDate(dateTime_.tm_year, dateTime_.tm_mon, dateTime_.tm_mday),
                                          QTime(dateTime_.tm_hour, dateTime_.tm_min, dateTime_.tm_sec));
    // calculate difference in hours between current and start
    int secondsSinceStart = START_DATE_TIME_OF_TRAINING_SET.secsTo(currentDateTime);
    int hoursSinceStart = secondsSinceStart / 3600;

    int distanceBetweenEvalutionData = double(TOTAL_NUMBER_OF_TRAINING_SAMPLES) * double(PROPORTION_OF_TRAINING_SET);
    return (hoursSinceStart % distanceBetweenEvalutionData == 0);

}

bool ANNWrapper::checkIfTrainingsSetIndexIsAEvaluationIndex(int index_) {
    int numberOfEvaluationDataSets = double(TOTAL_NUMBER_OF_TRAINING_SAMPLES) * double(1-PROPORTION_OF_TRAINING_SET);
    int distanceBetweenEvaluationData = TOTAL_NUMBER_OF_TRAINING_SAMPLES / numberOfEvaluationDataSets;
    return (index_ % distanceBetweenEvaluationData == 0);
}

void ANNWrapper::generateDataSets(vector<vector<double> > *inputValues_, vector<double> *expectedOutputValues_) {
    inputValues_->clear();
    expectedOutputValues_->clear();

    inputValues_->resize(TOTAL_NUMBER_OF_TRAINING_SAMPLES);
    expectedOutputValues_->resize(TOTAL_NUMBER_OF_TRAINING_SAMPLES);

    for (int DataSourceIndex = 0; DataSourceIndex < DATASOURCE_LIST.size(); DataSourceIndex++) {
        DataBuffer trainingSet;
        trainingSet.dataSource = DATASOURCE_LIST[DataSourceIndex];
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
        for (int PredictorIndex = 0; PredictorIndex < PREDICTOR_LIST.size(); PredictorIndex++) {
            trainingSet.data[PREDICTOR_LIST[PredictorIndex]] = 0;
        }

        // read all data of all predictors and current datasource
        //
        vector<DataBuffer> trainingSetBuffer = dbi->readFromDataBase(trainingSet);

        // iterate through all databuffers
        for(int trainingSetIndex = 0; trainingSetIndex < trainingSetBuffer.size(); trainingSetIndex++) {
            // make sure data is not of evalution training set
            if (!checkIfTrainingsSetIndexIsAEvaluationIndex(trainingSetIndex)) {
                // make sure there is enought previous data
                if (trainingSetIndex >= PREDICTION_WINDOW_SIZE-1) {
                    // go from current position backwards to get current data and
                    // all needed previous data
                    for (int predictionWindowIndex = trainingSetIndex; predictionWindowIndex > trainingSetIndex - PREDICTION_WINDOW_SIZE + 1; predictionWindowIndex--) {
                        vector<double> inputValuesOfCurrentDataSource;
                        typedef std::map<string, double>::iterator it_type;
                        for(it_type iterator = trainingSetBuffer[predictionWindowIndex].data.begin(); iterator != trainingSetBuffer[predictionWindowIndex].data.end(); iterator++) {
                            inputValuesOfCurrentDataSource.push_back(iterator->second);
                        }
                        vector<double> tempVector = inputValues_->at(trainingSetIndex);
                        tempVector.insert(tempVector.end(),inputValuesOfCurrentDataSource.begin(),inputValuesOfCurrentDataSource.end());
                        inputValues_->at(trainingSetIndex) = tempVector;
                    }

                }
            }
        }
    }

}
