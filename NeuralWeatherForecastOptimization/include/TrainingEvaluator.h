#ifndef TRAININGEVALUATOR_H
#define TRAININGEVALUATOR_H

#include <fstream>
// ---
#include "ANNWrapper.h"
#include "DBInterface.h"

enum TrainingApproach {MOS, TrainOnOnlyTemperature, TrainOnErrorOfTemperature, TrainOnTemperatureAndHumidity, ReLU};

class TrainingEvaluator : public ANNWrapper {
    public:
        /* --- constructor / destructor --- */
        TrainingEvaluator(DBInterface *dbInterface_) : ANNWrapper(dbInterface_)  {};

        /* --- getter / setter --- */
        vector<string> getPredictorList()        {return predictorList;       };
        string         getPredictant()           {return predictant;          };
        vector<string> getDataSourceList()       {return dataSourceList;      };
        int            getPredictionWindowSize() {return predictionWindowSize;};
        void           setPredictorList          (vector<string> val_) {predictorList        = val_;};
        void           setPredictant             (string         val_) {predictant           = val_;};
        void           setDataSourceList         (vector<string> val_) {dataSourceList       = val_;};
        void           setPredictionWindowSize   (int            val_) {predictionWindowSize = val_;};


        /* --- training --- */
        double evaluateTraining(TrainingApproach trainingApproach_);
    private:
        void evaluateMOS();
        void evaluateTrainingWithOnlyTemperature();
        void evaluateTrainingWithErrorOfTemperature();
        void evaluateTrainingWithTemperatureAndHumidity();
        // ---
        void evaluateReLU();

        vector<double> getOriginalNotNormalizedData(string nameOfValue_);
        double calcMSE(vector<double> forecast_, vector<double> realValues_);

        vector<string> predictorList;
        string predictant;
        vector<string> dataSourceList;
        int predictionWindowSize;

};
#endif // TRAININGEVALUATOR_H
