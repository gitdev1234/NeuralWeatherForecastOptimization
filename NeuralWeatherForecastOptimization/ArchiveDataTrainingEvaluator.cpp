#include <ctime>
// ---
#include "LogWriter.h"
#include "TrainingEvaluator.h"

/**
 * main of ANNTrainingDaemon
 * @brief starts training of all neural nets by using train method of ANN-wrapper-class
 *
 */
//int main6() {
int main() {

    // create Log-Writer Object
    LogWriter log("ANNTrainingDaemon", PATH_OF_LOGFILE);

    // create and init singleton-DBInterface-object
    DBInterface& dbi = DBInterface::getInstance();
    dbi.init();

    log << SLevel(INFO) << "Started Training." << endl;
    // measure time-start
    clock_t begin = clock();

    TrainingEvaluator TEval(&dbi) ;
    TEval.setPredictant("Lufttemperatur_2m");
    //TEval.setPredictorList({"Lufttemperatur_2m","RelativeLuftfeuchte_2m","Windgeschwindigkeit_Stundenmittel","Windrichtung_Stundenmittel"});
    TEval.setPredictorList({"Lufttemperatur_2m","RelativeLuftfeuchte_2m"});
    TEval.setDataSourceList({"zTransformedForecast","zTransformedWeatherStation"});
    TEval.setPredictionWindowSize(2);

    TEval.evaluateTraining(TrainingApproach::ReLU);

    // measure time-end
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    log << SLevel(INFO) << "Finished Training. Elapsed Time : " << elapsed_secs << " seconds." << endl;

    return 0;
}

