#include "TrainingEvaluator.h"

double TrainingEvaluator::evaluateTraining(TrainingApproach trainingApproach_) {
    switch (trainingApproach_) {
        case TrainingApproach::MOS                           : evaluateMOS();                               break;
        case TrainingApproach::TrainOnOnlyTemperature        : evaluateTrainingWithOnlyTemperature();       break;
        case TrainingApproach::TrainOnErrorOfTemperature     : evaluateTrainingWithErrorOfTemperature();    break;
        case TrainingApproach::TrainOnTemperatureAndHumidity : evaluateTrainingWithTemperatureAndHumidity();break;
        // ---
        case TrainingApproach::ReLU : evaluateReLU(); break;
    }
}

void TrainingEvaluator::evaluateMOS() {

}

void TrainingEvaluator::evaluateTrainingWithOnlyTemperature() {

}

void TrainingEvaluator::evaluateTrainingWithErrorOfTemperature() {

}

void TrainingEvaluator::evaluateTrainingWithTemperatureAndHumidity() {

}

void TrainingEvaluator::evaluateReLU() {
    vector< vector<double> > inputValues;
    vector<double> expectedOutputValues;

    // generate input and expected output data
    generateDataSets(&inputValues,&expectedOutputValues,predictorList,predictant,dataSourceList,predictionWindowSize,true);

    // normalize and scale data-sets
    // z-Transform is alrady done before
    inputValues = scaleVector(inputValues,SCALING_FACTOR,true);
    vector<double> expectedOutputValuesBeforeNormalization = expectedOutputValues;
    expectedOutputValues = scaleVector(expectedOutputValues,SCALING_FACTOR,true);

    // start training
    ANNTemperature.train(inputValues,expectedOutputValues);

    // get test data sets
    double MSE = 0.0;
    vector<double> forecastValues;
    generateDataSets(&inputValues,&expectedOutputValues,predictorList,predictant,dataSourceList,predictionWindowSize,false);
    for (int i = 0; i < inputValues.size(); i++) {
        forecastValues.push_back(ANNTemperature.forward(inputValues[i]));
    }

    vector<double> expectedOutputWithoutZTransform;
    generateDataSets(&inputValues,&expectedOutputValues,{predictant},predictant,{"WeatherStation"},predictionWindowSize,false);

    forecastValues = reZTransformVector(forecastValues,expectedOutputValuesBeforeNormalization);

    MSE = calcMSE(forecastValues,expectedOutputValuesBeforeNormalization);
    cout << "MSE : " << MSE << endl;

}

vector<double> TrainingEvaluator::getOriginalNotNormalizedData(string nameOfValue_) {
    // get original data without z-transformation
    DataBuffer dataBuffer;
    dataBuffer.dataSource = "WeatherStation";
    dataBuffer.useDataSource = true;
    dataBuffer.startDateTime.tm_year = START_DATE_TIME_OF_TRAINING_SET.date().year();
    dataBuffer.startDateTime.tm_mon  = START_DATE_TIME_OF_TRAINING_SET.date().month();
    dataBuffer.startDateTime.tm_mday = START_DATE_TIME_OF_TRAINING_SET.date().day();
    dataBuffer.startDateTime.tm_hour = START_DATE_TIME_OF_TRAINING_SET.time().hour();
    dataBuffer.startDateTime.tm_min  = START_DATE_TIME_OF_TRAINING_SET.time().minute();
    dataBuffer.startDateTime.tm_sec  = START_DATE_TIME_OF_TRAINING_SET.time().second();
    dataBuffer.endDateTime.tm_year   =   END_DATE_TIME_OF_TRAINING_SET.date().year();
    dataBuffer.endDateTime.tm_mon    =   END_DATE_TIME_OF_TRAINING_SET.date().month();
    dataBuffer.endDateTime.tm_mday   =   END_DATE_TIME_OF_TRAINING_SET.date().day();
    dataBuffer.endDateTime.tm_hour   =   END_DATE_TIME_OF_TRAINING_SET.time().hour();
    dataBuffer.endDateTime.tm_min    =   END_DATE_TIME_OF_TRAINING_SET.time().minute();
    dataBuffer.endDateTime.tm_sec    =   END_DATE_TIME_OF_TRAINING_SET.time().second();
    dataBuffer.useDateTimes = true;
    dataBuffer.data[nameOfValue_] = 0;

    // get all original data from weatherstation
    vector<DataBuffer> resultDataBuffer = dbi->readFromDataBase(dataBuffer);
    // copy data into normal vector
    vector<double> result;
    for (int i = 0; i < resultDataBuffer.size(); i++) {
        if (checkIfTrainingsSetIndexIsAEvaluationIndex(i))
        result.push_back(resultDataBuffer[i].data[nameOfValue_]);
    }

    // return vector
    return result;
}

double TrainingEvaluator::calcMSE(vector<double> forecast_, vector<double> realValues_) {
    double MSE = 0.0;
    if (forecast_.size() == realValues_.size()) {
        for (int i = 0; i < forecast_.size(); i++) {
            MSE += (forecast_[i]-realValues_[i]) * (forecast_[i]-realValues_[i]);
        }
    }
    MSE /= forecast_.size();
    return MSE;
}


