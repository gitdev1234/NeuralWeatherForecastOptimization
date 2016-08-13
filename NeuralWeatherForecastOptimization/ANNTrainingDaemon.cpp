#include <ctime>
// ---
#include "LogWriter.h"
#include "ANNWrapper.h"

/**
 * main of ANNTrainingDaemon
 * @brief starts training of all neural nets by using train method of ANN-wrapper-class
 *
 */
int main2() {
//int main() {
    // create Log-Writer Object
    LogWriter log("ANNTrainingDaemon", PATH_OF_LOGFILE);

    log << SLevel(INFO) << "Started Training." << endl;
    // measure time-start
    clock_t begin = clock();

    // train artificial neuro nets
    ANNWrapper ANNWrap;
    ANNWrap.trainArtificialNeuroNets();

    // measure time-end
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    log << SLevel(INFO) << "Finished Training. Elapsed Time : " << elapsed_secs << " seconds." << endl;

    return 0;
}
