#include <ctime>
// ---
#include "logwriter.h"
#include "ArtificialNeuroNetWrapper.h"

/**
 * main of ForecastCalculator
 * @brief starts calculation of output within ANN-wrapper class and outputs optimized WeatherForecast
 *
 */
int main3() {
//int main() {
    // create Log-Writer Object
    LogWriter log("ForecastCalculator", PATH_OF_LOGFILE);

    log << SLevel(INFO) << "Started calculation of forecast." << endl;
    // measure time-start
    clock_t begin = clock();

    // train artificial neuro nets
    ArtificialNeuroNetWrapper ANNWrapper;
    DataBuffer optimizedForecast = ANNWrapper.calculateOutput();

    if (optimizedForecast.data.size() > 0) {
        log << SLevel(INFO) << "Succesfully calculated optimized forecast." << endl;
        cout << "Optimized Forecast : " << endl << optimizedForecast << endl;
    } else {
        log << SLevel(ERROR) << "Received empty forecast from ArtificialNeuroNetWrapper." << endl;
    }

    // measure time-end
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    log << SLevel(INFO) << "Finished calculation of forecast. Elapsed Time : " << elapsed_secs << " seconds." << endl;
}

