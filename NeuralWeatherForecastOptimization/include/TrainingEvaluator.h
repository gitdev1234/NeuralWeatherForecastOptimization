#ifndef TRAININGEVALUATOR_H
#define TRAININGEVALUATOR_H

#include "ANNWrapper.h"
#include "DBInterface.h"

enum TrainingApproach {MOS, TrainOnOnlyTemperature, TrainOnErrorOfTemperature, TrainOnTemperatureAndHumidity};

class TrainingEvaluator : public ANNWrapper {
    public:
        TrainingEvaluator(DBInterface *dbInterface_) : ANNWrapper(dbInterface_)  {};
        double evaluateTraining(TrainingApproach trainingApproach_);
    private:
        void evaluateMOS();
        void evaluateTrainingWithOnlyTemperature();
        void evaluateTrainingWithErrorOfTemperature();
        void evaluateTrainingWithTemperatureAndHumidity();
};

#endif // TRAININGEVALUATOR_H
