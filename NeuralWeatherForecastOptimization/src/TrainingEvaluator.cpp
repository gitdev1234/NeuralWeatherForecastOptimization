#include "TrainingEvaluator.h"

double TrainingEvaluator::evaluateTraining(TrainingApproach trainingApproach_) {
    switch (trainingApproach_) {
        case TrainingApproach::MOS                           : evaluateMOS();                               break;
        case TrainingApproach::TrainOnOnlyTemperature        : evaluateTrainingWithOnlyTemperature();       break;
        case TrainingApproach::TrainOnErrorOfTemperature     : evaluateTrainingWithErrorOfTemperature();    break;
        case TrainingApproach::TrainOnTemperatureAndHumidity : evaluateTrainingWithTemperatureAndHumidity();break;
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


