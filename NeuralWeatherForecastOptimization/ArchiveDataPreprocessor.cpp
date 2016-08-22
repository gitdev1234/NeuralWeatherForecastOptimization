#include <iostream>
#include <fstream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <QDateTime>
// ---
#include "DBInterface.h"
#include "DataBuffer.h"

using namespace boost::gregorian;


using namespace std;

struct tm incHour(struct tm time_, int hourDelta_) {
    QDateTime qtDate(QDate(time_.tm_year, time_.tm_mon, time_.tm_mday),
                     QTime(time_.tm_hour, time_.tm_min, time_.tm_sec));
    qtDate.setTimeSpec(Qt::UTC);
    qtDate = qtDate.addSecs(3600*hourDelta_);
    time_.tm_year = qtDate.date().year();
    time_.tm_mon  = qtDate.date().month();
    time_.tm_mday = qtDate.date().day();
    time_.tm_hour = qtDate.time().hour();
    time_.tm_min  = qtDate.time().minute();
    time_.tm_sec  = qtDate.time().second();
    return time_;
}

void preProcessArchiveData(string nameToPreProcess_);
void   createMissingForecastDataWithWeatherStationPlusMeanError(string nameOfValueToAnalyze_, double meanError_, string dataSourceToAnalyze_, string dataSourceToCompareWith_ , int stepWidth_, struct tm startDate_, struct tm endDate_);
double analyzeMissingData(string nameOfValueToAnalyze_, string dataSourceToAnalyze_, string dataSourceToCompareWith_ , int stepWidth_, struct tm startDate_, struct tm endDate_);
void   createMissingForecastDataWithSlopeOfNeighbours(string nameOfValueToAnalyze_, string dataSourceToAnalyze_, tm startDate_, tm endDate_) ;
void   createErrorMeasurement(string nameOfValueToAnalyze_);
void createSlopeMeasurement(string nameOfValueToAnalyze_, string dataSourceToAnalyze_, struct tm startDate_, struct tm endDate_);
void createZTransformMeasurement(string nameOfValueToAnalyze_, string dataSourceToAnalyze_, struct tm startDate_, struct tm endDate_);
vector<double> zTransformVector(const vector<double>& vectorToTransform_);
void dataToCSV(const string& path_, string nameOfValueToAnalyze_, string dataSourceToAnalyze_, struct tm startDate_, struct tm endDate_);

/**
 * main of ArchiveDataPreprocessor
 * @brief iterates through all data-points in influxDB and creates missing datapoints, using linear lines of best fit
 *
 */
int main4() {
//int main() {



    cout << "started with preProcessing of Lufttemperatur_2m" << endl;
    preProcessArchiveData("Lufttemperatur_2m");
    cout << "finished" << endl;
    cout << "started with preProcessing of Niederschlagshoehe" << endl;
    // //preProcessArchiveData("Niederschlagshoehe");
    cout << "finished" << endl;
    cout << "started with preProcessing of RelativeLuftfeuchte_2m" << endl;
    preProcessArchiveData("RelativeLuftfeuchte_2m");
    cout << "finished" << endl;
    cout << "started with preProcessing of Temperatur_Im_Boden_Minus_5cm" << endl;
    // //preProcessArchiveData("Temperatur_Im_Boden_Minus_5cm");
    cout << "finished" << endl;
    cout << "started with preProcessing of Windgeschwindigkeit_Stundenmittel" << endl;
    preProcessArchiveData("Windgeschwindigkeit_Stundenmittel");
    cout << "finished" << endl;
    cout << "started with preProcessing of Windrichtung_Stundenmittel" << endl;
    preProcessArchiveData("Windrichtung_Stundenmittel");
    cout << "finished" << endl;
    return 0;
}

void preProcessArchiveData(string nameToPreProcess_) {

    DataBuffer firstDataBuffer;
    LogWriter log("ArchiveDataPreprocessor", PATH_OF_LOGFILE+"x");

    // create start date time
    struct tm startDateTime;
    startDateTime.tm_year = 2014;
    startDateTime.tm_mon  = 8;
    startDateTime.tm_mday = 19;
    startDateTime.tm_hour = 0;
    startDateTime.tm_min  = 0;
    startDateTime.tm_sec  = 0;

    // create end date times
    struct tm endDateTime;
    endDateTime.tm_year = 2016;
    endDateTime.tm_mon  = 7;
    endDateTime.tm_mday = 16;
    endDateTime.tm_hour = 21;
    endDateTime.tm_min  = 0;
    endDateTime.tm_sec  = 0;

    dataToCSV(nameToPreProcess_+".csv",nameToPreProcess_,"Error",startDateTime,endDateTime);

    /*
    //double meanError = analyzeMissingData(nameToPreProcess_,"Forecast","WeatherStation",3,startDateTime,endDateTime);

    cout << "started creating missing values for by weather station plus mean error" << endl;
    //createMissingForecastDataWithWeatherStationPlusMeanError(nameToPreProcess_,meanError,"Forecast","WeatherStation",3,startDateTime,endDateTime);
    cout << "finished" << endl;
    cout << "started creating missing values for by slope of neighbours" << endl;
    endDateTime.tm_hour = 18;
    //createMissingForecastDataWithSlopeOfNeighbours(nameToPreProcess_,"Forecast",startDateTime,endDateTime);
    endDateTime.tm_hour = 21;
    cout << "finished" << endl;

    struct tm startDateTime2;
    startDateTime2.tm_year = 2016;
    startDateTime2.tm_mon  = 7;
    startDateTime2.tm_mday = 16;
    startDateTime2.tm_hour = 0;
    startDateTime2.tm_min  = 0;
    startDateTime2.tm_sec  = 0;
    struct tm endDateTime2 = endDateTime;
    endDateTime2.tm_mday = endDateTime.tm_mday + 1;
    endDateTime2.tm_hour = 0;

    cout << "started creating missing values for last day" << endl;
    //createMissingForecastDataWithWeatherStationPlusMeanError(nameToPreProcess_,meanError,"Forecast","WeatherStation",1,startDateTime2,endDateTime2);
    cout << "finished" << endl;

    cout << "started analyzation if everything is complete" << endl;
    //analyzeMissingData(nameToPreProcess_,"Forecast","WeatherStation",1,startDateTime,endDateTime);
    cout << "finished" << endl;

    cout << "started creating Error Measurement" << endl;
    //createErrorMeasurement(nameToPreProcess_);
    cout << "finished" << endl;

    cout << "started creating slope measurement for weather station " << endl;
    //createSlopeMeasurement(nameToPreProcess_,"WeatherStation",startDateTime,endDateTime);
    cout << "finished" << endl;

    cout << "started creating slope measurement for forecast" << endl;
    //createSlopeMeasurement(nameToPreProcess_,"Forecast",startDateTime,endDateTime);
    cout << "finished" << endl;

    cout << "started creating slope measurement for error" << endl;
    //createSlopeMeasurement(nameToPreProcess_,"Error",startDateTime,endDateTime);
    cout << "finished" << endl;

    cout << "started creating zTransform measurement for weather station " << endl;
    createZTransformMeasurement(nameToPreProcess_,"WeatherStation",startDateTime,endDateTime);
    cout << "finished" << endl;

    cout << "started creating zTransform measurement for forecast" << endl;
    createZTransformMeasurement(nameToPreProcess_,"Forecast",startDateTime,endDateTime);
    cout << "finished" << endl;

    cout << "started creating zTransform measurement for error" << endl;
    createZTransformMeasurement(nameToPreProcess_,"Error",startDateTime,endDateTime);
    cout << "finished" << endl;

    cout << "Finished preprocessing" << endl;
    */
}

/**
 * createMissingForecastDataWithWeatherStationPlusMeanError
 * @brief iterates through all datapoints of nameOfValueToAnalyze_ and creates missing values
 * @param nameOfValueToAnalyze_ value to iterate through
 *
 * iterates through all data-points in three-hour-steps and adds
 * every missing data-point in forecast measurement with
 * forecast-value = WeatherStationValue + meanError
 */
void createMissingForecastDataWithWeatherStationPlusMeanError(string nameOfValueToAnalyze_, double meanError_,
     string dataSourceToAnalyze_, string dataSourceToCompareWith_ , int stepWidth_, struct tm startDate_, struct tm endDate_) {

    // create and init singleton-DBInterface-object
    DBInterface& dbi = DBInterface::getInstance();
    // todo
    dbi.init();
    dbi.writeStatusOK(true);

    // define start date
    DataBuffer WeatherStationBuffer;
    WeatherStationBuffer.startDateTime = startDate_;
    WeatherStationBuffer.endDateTime = WeatherStationBuffer.startDateTime;
    WeatherStationBuffer.useDateTimes = true;
    WeatherStationBuffer.dataSource = dataSourceToCompareWith_;
    WeatherStationBuffer.useDataSource = true;
    WeatherStationBuffer.data[nameOfValueToAnalyze_] = 0;

    bool lastDateReached = false;
    int dataCountTotal = 0;

    // iterate through data and generate missing
    while (!lastDateReached) {
        DataBuffer ForecastBuffer = WeatherStationBuffer;
        ForecastBuffer.dataSource = "Forecast";
        if ( (WeatherStationBuffer.startDateTime.tm_year >= endDate_.tm_year) &&
             (WeatherStationBuffer.startDateTime.tm_mon  >= endDate_.tm_mon ) &&
             (WeatherStationBuffer.startDateTime.tm_mday >= endDate_.tm_mday) &&
             (WeatherStationBuffer.startDateTime.tm_hour >= endDate_.tm_hour) &&
             (WeatherStationBuffer.startDateTime.tm_min  >= endDate_.tm_min ) &&
             (WeatherStationBuffer.startDateTime.tm_sec  >= endDate_.tm_sec ) ){
            lastDateReached = true;
        }

        vector<DataBuffer> resultForecast       = dbi.readFromDataBase(ForecastBuffer);
        // if data is missing insert
        if (resultForecast.size() == 0) {
            vector<DataBuffer> resultWeatherStation = dbi.readFromDataBase(WeatherStationBuffer);


            if ( (resultWeatherStation.size() > 0) && (resultForecast.size() == 0 ) ) {

                double valueWeatherStation = resultWeatherStation.data()->data[nameOfValueToAnalyze_];
                DataBuffer newForecastValueForMissingDate = WeatherStationBuffer;
                newForecastValueForMissingDate.dataSource = dataSourceToAnalyze_;
                newForecastValueForMissingDate.data[nameOfValueToAnalyze_] = valueWeatherStation + meanError_;
                dbi.writeToDataBase(newForecastValueForMissingDate);
                if (dbi.getDBFailure()) {
                    cout << "Error during inserting" << endl;
                }

            }
        }


        // count all values existing or not existing
        dataCountTotal++;
        if (dataCountTotal % 100 == 0) {
            cout << "iterated " << dataCountTotal << " dates" << endl;
        }

        WeatherStationBuffer.startDateTime = incHour(WeatherStationBuffer.startDateTime,stepWidth_);
        WeatherStationBuffer.endDateTime   = WeatherStationBuffer.startDateTime;
    }
}

/**
 * @brief analyzeMissingData
 * @return returns mean error
 */
double analyzeMissingData(string nameOfValueToAnalyze_, string dataSourceToAnalyze_, string dataSourceToCompareWith_ , int stepWidth_, struct tm startDate_, struct tm endDate_) {

    cout << "started analyzing missing data" << endl;
    // create and init singleton-DBInterface-object
    DBInterface& dbi = DBInterface::getInstance();
    // todo
    dbi.init();
    dbi.writeStatusOK(true);

    // define start date
    DataBuffer normalValueBuffer;
    normalValueBuffer.startDateTime = startDate_;
    normalValueBuffer.endDateTime = normalValueBuffer.startDateTime;
    normalValueBuffer.useDateTimes = true;
    normalValueBuffer.dataSource = dataSourceToAnalyze_;
    normalValueBuffer.useDataSource = true;
    normalValueBuffer.data[nameOfValueToAnalyze_] = 0;

    bool lastDateReached = false;
    vector<double> ErrorValues;
    int dataCountTotal                      = 0;
    int dataCountMissingFromWeatherStation  = 0;
    int dataCountMissingFromForecast        = 0;
    int dataCountMissingFromBoth            = 0;

    // iterate until end-date
    while (!lastDateReached) {
        DataBuffer ForecastBuffer = normalValueBuffer;
        ForecastBuffer.dataSource = dataSourceToCompareWith_;
        if ( (normalValueBuffer.startDateTime.tm_year >= endDate_.tm_year) &&
             (normalValueBuffer.startDateTime.tm_mon  >= endDate_.tm_mon ) &&
             (normalValueBuffer.startDateTime.tm_mday >= endDate_.tm_mday) &&
             (normalValueBuffer.startDateTime.tm_hour >= endDate_.tm_hour) &&
             (normalValueBuffer.startDateTime.tm_min  >= endDate_.tm_min ) &&
             (normalValueBuffer.startDateTime.tm_sec  >= endDate_.tm_sec ) ){
            lastDateReached = true;
        }

        vector<DataBuffer> resultWeatherStation = dbi.readFromDataBase(normalValueBuffer);
        vector<DataBuffer> resultForecast       = dbi.readFromDataBase(ForecastBuffer);

        // add error of all existing values to vector
        if ( (resultWeatherStation.size() > 0) && (resultForecast.size() > 0 ) ) {
            double valueWeatherStation = resultWeatherStation.data()->data[nameOfValueToAnalyze_];
            double valueForecast       =       resultForecast.data()->data[nameOfValueToAnalyze_];
            double Error = valueWeatherStation - valueForecast;
            ErrorValues.push_back(Error);

        // count non-existing values
        } else {
            if (resultWeatherStation.size() == 0) {
                dataCountMissingFromWeatherStation++;
            }
            if (resultForecast.size() == 0) {
                dataCountMissingFromForecast++;
            }
            if ( (resultWeatherStation.size() == 0) && (resultForecast.size() == 0) ) {
                dataCountMissingFromBoth++;
            }
        }

        // count all values existing or not existing
        dataCountTotal++;
        if (dataCountTotal % 100 == 0) {
            cout << "iterated " << dataCountTotal << " dates" << endl;
        }

        // increment date-time by three hours
        normalValueBuffer.startDateTime = incHour(normalValueBuffer.startDateTime,stepWidth_);
        normalValueBuffer.endDateTime   = normalValueBuffer.startDateTime;

    }

    cout << "total data count : " << dataCountTotal << endl;
    cout << "missing weatherstation data count : " << dataCountMissingFromWeatherStation <<
             "this is " << double(dataCountMissingFromWeatherStation) / double(dataCountTotal) * 100.0 << "%" << endl;
    cout << "missing forecast       data count : " << dataCountMissingFromForecast <<
            "this is " << double(dataCountMissingFromForecast) / double(dataCountTotal) * 100.0 << "%" << endl;
    cout << "missing both           data count : " << dataCountMissingFromBoth <<
            "this is " << double(dataCountMissingFromBoth) / double(dataCountTotal) * 100.0 << "%" << endl;

    double sum = std::accumulate(ErrorValues.begin(), ErrorValues.end(), 0.0);
    double mean = sum / ErrorValues.size();

    double sq_sum = std::inner_product(ErrorValues.begin(), ErrorValues.end(), ErrorValues.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / ErrorValues.size() - mean * mean);

    cout << "mean  of error : " << mean << endl;
    cout << "stdev of error : " << stdev << endl;

    cout << "finished" << endl;

    return mean;

}


void createErrorMeasurement(string nameOfValueToAnalyze_) {

    // create and init singleton-DBInterface-object
    DBInterface& dbi = DBInterface::getInstance();
    // todo
    dbi.init();
    dbi.writeStatusOK(true);

    DataBuffer WeatherStationBuffer;
    WeatherStationBuffer.startDateTime.tm_year = 2014;
    WeatherStationBuffer.startDateTime.tm_mon  = 8;
    WeatherStationBuffer.startDateTime.tm_mday = 19;
    WeatherStationBuffer.startDateTime.tm_hour = 0;
    WeatherStationBuffer.startDateTime.tm_min  = 0;
    WeatherStationBuffer.startDateTime.tm_sec  = 0;
    WeatherStationBuffer.endDateTime = WeatherStationBuffer.startDateTime;
    WeatherStationBuffer.useDateTimes = true;
    WeatherStationBuffer.dataSource = "WeatherStation";
    WeatherStationBuffer.useDataSource = true;
    WeatherStationBuffer.data[nameOfValueToAnalyze_] = 0;

    bool lastDateReached = false;
    vector<double> ErrorValues;
    int dataCountTotal = 0;

    // get standard deviation
    while (!lastDateReached) {
        DataBuffer ForecastBuffer = WeatherStationBuffer;
        ForecastBuffer.dataSource = "Forecast";
        if ( (WeatherStationBuffer.startDateTime.tm_year >= 2016) &&
             (WeatherStationBuffer.startDateTime.tm_mon  >= 7   ) &&
             (WeatherStationBuffer.startDateTime.tm_mday >= 16  ) &&
             (WeatherStationBuffer.startDateTime.tm_hour >= 21  ) &&
             (WeatherStationBuffer.startDateTime.tm_min  >= 0   ) &&
             (WeatherStationBuffer.startDateTime.tm_sec  >= 0   ) ){
            lastDateReached = true;
        }

        vector<DataBuffer> resultWeatherStation = dbi.readFromDataBase(WeatherStationBuffer);
        vector<DataBuffer> resultForecast       = dbi.readFromDataBase(ForecastBuffer);

        if ( (resultWeatherStation.size() > 0) && (resultForecast.size() > 0 ) ) {
            double valueWeatherStation = resultWeatherStation.data()->data[nameOfValueToAnalyze_];
            double valueForecast       =       resultForecast.data()->data[nameOfValueToAnalyze_];
            double Error = valueWeatherStation - valueForecast;
            DataBuffer ErrorBuffer = WeatherStationBuffer;
            ErrorBuffer.dataSource = "Error";
            ErrorBuffer.data[nameOfValueToAnalyze_] = Error;
            dbi.writeToDataBase(ErrorBuffer);
            if (dbi.getDBFailure()) {
                cout << "Error during insert" << endl;
            }
        }

        // count all values existing or not existing
        dataCountTotal++;
        if (dataCountTotal % 100 == 0) {
            cout << "iterated " << dataCountTotal << " dates" << endl;
        }

        WeatherStationBuffer.startDateTime = incHour(WeatherStationBuffer.startDateTime,1);
        WeatherStationBuffer.endDateTime   = WeatherStationBuffer.startDateTime;
    }
}

void createSlopeMeasurement(string nameOfValueToAnalyze_, string dataSourceToAnalyze_, struct tm startDate_, struct tm endDate_) {

    // create and init singleton-DBInterface-object
    DBInterface& dbi = DBInterface::getInstance();
    // todo
    dbi.init();
    dbi.writeStatusOK(true);

    DataBuffer CurrentBuffer;
    CurrentBuffer.startDateTime = startDate_;
    CurrentBuffer.endDateTime = CurrentBuffer.startDateTime;
    CurrentBuffer.useDateTimes = true;
    CurrentBuffer.dataSource = dataSourceToAnalyze_;
    CurrentBuffer.useDataSource = true;
    CurrentBuffer.data[nameOfValueToAnalyze_] = 0;

    bool lastDateReached = false;
    int dataCountTotal = 0;

    CurrentBuffer.endDateTime   = CurrentBuffer.startDateTime;

    while (!lastDateReached) {
        DataBuffer ForecastBuffer = CurrentBuffer;
        ForecastBuffer.dataSource = "Forecast";
        if ( (CurrentBuffer.startDateTime.tm_year >= endDate_.tm_year) &&
             (CurrentBuffer.startDateTime.tm_mon  >= endDate_.tm_mon ) &&
             (CurrentBuffer.startDateTime.tm_mday >= endDate_.tm_mday) &&
             (CurrentBuffer.startDateTime.tm_hour >= endDate_.tm_hour) &&
             (CurrentBuffer.startDateTime.tm_min  >= endDate_.tm_min ) &&
             (CurrentBuffer.startDateTime.tm_sec  >= endDate_.tm_sec ) ){
            lastDateReached = true;
        }

        DataBuffer PreviousDataBuffer = CurrentBuffer;
        PreviousDataBuffer.startDateTime = incHour(PreviousDataBuffer.startDateTime,-1);
        PreviousDataBuffer.endDateTime   = PreviousDataBuffer.startDateTime;
        DataBuffer NextDataBuffer = CurrentBuffer;
        NextDataBuffer.startDateTime = incHour(NextDataBuffer.startDateTime,1);
        NextDataBuffer.endDateTime   = NextDataBuffer.startDateTime;

        vector<DataBuffer> resultPrevious = dbi.readFromDataBase(PreviousDataBuffer);
        vector<DataBuffer> resultNext     = dbi.readFromDataBase(NextDataBuffer);

        if ( (resultPrevious.size() > 0) && (resultNext.size() > 0 ) ) {
            double previousValue = resultPrevious.data()->data[nameOfValueToAnalyze_];
            double nextValue     =     resultNext.data()->data[nameOfValueToAnalyze_];
            double slope         = double(nextValue - previousValue) / 3.0;

            DataBuffer SlopeBuffer = CurrentBuffer;
            SlopeBuffer.dataSource = "Slope"+CurrentBuffer.dataSource;
            SlopeBuffer.data[nameOfValueToAnalyze_] = slope;
            dbi.writeToDataBase(SlopeBuffer);
            if (dbi.getDBFailure()) {
                cout << "Error during insert" << endl;
            }
        }

        // count all values existing or not existing
        dataCountTotal++;
        if (dataCountTotal % 100 == 0) {
            cout << "iterated " << dataCountTotal << " dates" << endl;
        }

        CurrentBuffer.startDateTime = incHour(CurrentBuffer.startDateTime,1);
        CurrentBuffer.endDateTime   = CurrentBuffer.startDateTime;
    }

    // add slope for very first value by using second
    DataBuffer SlopeBuffer = CurrentBuffer;
    SlopeBuffer.startDateTime = incHour(startDate_,1);
    SlopeBuffer.endDateTime = SlopeBuffer.startDateTime;
    SlopeBuffer.dataSource = "Slope";
    vector<DataBuffer> resultSecond = dbi.readFromDataBase(SlopeBuffer);
    if (resultSecond.size() > 0) {
        SlopeBuffer.startDateTime = startDate_;
        SlopeBuffer.endDateTime   = SlopeBuffer.startDateTime;
        SlopeBuffer.dataSource = "Slope"+CurrentBuffer.dataSource;
        SlopeBuffer.data[nameOfValueToAnalyze_] = resultSecond.data()->data[nameOfValueToAnalyze_];
        dbi.writeToDataBase(SlopeBuffer);
        if (dbi.getDBFailure()) {
            cout << "Error during insert" << endl;
        }
    }

    // add slope for very last value by using the one before the last
    SlopeBuffer = CurrentBuffer;
    SlopeBuffer.startDateTime = incHour(endDate_,-1);
    SlopeBuffer.endDateTime = SlopeBuffer.startDateTime;
    SlopeBuffer.dataSource = "Slope";
    vector<DataBuffer> resultBeforeLast = dbi.readFromDataBase(SlopeBuffer);
    if (resultBeforeLast.size() > 0) {
        SlopeBuffer.startDateTime = endDate_;
        SlopeBuffer.endDateTime   = SlopeBuffer.startDateTime;
        SlopeBuffer.dataSource = "Slope"+CurrentBuffer.dataSource;
        SlopeBuffer.data[nameOfValueToAnalyze_] = resultBeforeLast.data()->data[nameOfValueToAnalyze_];
        dbi.writeToDataBase(SlopeBuffer);
        if (dbi.getDBFailure()) {
            cout << "Error during insert" << endl;
        }
    }


}


void createMissingForecastDataWithSlopeOfNeighbours(string nameOfValueToAnalyze_, string dataSourceToAnalyze_, struct tm startDate_, struct tm endDate_) {

    // create and init singleton-DBInterface-object
    DBInterface& dbi = DBInterface::getInstance();
    // todo
    dbi.init();
    dbi.writeStatusOK(true);

    // define start date
    DataBuffer firstDataBuffer;
    firstDataBuffer.startDateTime = startDate_;
    firstDataBuffer.endDateTime = firstDataBuffer.startDateTime;
    firstDataBuffer.useDateTimes = true;
    firstDataBuffer.dataSource = dataSourceToAnalyze_;
    firstDataBuffer.useDataSource = true;
    firstDataBuffer.data[nameOfValueToAnalyze_] = 0;

    bool lastDateReached = false;
    int currentIterationNumber = 0;

    // iterate all data in 3-hour-steps until end date
    while (!lastDateReached) {
        if ( (firstDataBuffer.startDateTime.tm_year >= endDate_.tm_year) &&
             (firstDataBuffer.startDateTime.tm_mon  >= endDate_.tm_mon ) &&
             (firstDataBuffer.startDateTime.tm_mday >= endDate_.tm_mday) &&
             (firstDataBuffer.startDateTime.tm_hour >= endDate_.tm_hour) &&
             (firstDataBuffer.startDateTime.tm_min  >= endDate_.tm_min ) &&
             (firstDataBuffer.startDateTime.tm_sec  >= endDate_.tm_sec ) ){
            lastDateReached = true;
        }

        // get current value
        vector<DataBuffer> result = dbi.readFromDataBase(firstDataBuffer);
        double firstValue = result[0].data[nameOfValueToAnalyze_];

        // get value in three hours
        DataBuffer fourthDataBuffer = firstDataBuffer;
        fourthDataBuffer.startDateTime = incHour(firstDataBuffer.startDateTime,3);
        fourthDataBuffer.endDateTime = fourthDataBuffer.startDateTime;
        result = dbi.readFromDataBase(fourthDataBuffer);
        double fourthValue = result[0].data[nameOfValueToAnalyze_];

        // calculate values in between
        double slope = (fourthValue - firstValue) / 3.0;
        double secondValue = slope * 1 + firstValue;
        double thirdValue  = slope * 2 + firstValue;

        // insert second value
        DataBuffer secondDataBuffer = firstDataBuffer;
        secondDataBuffer.startDateTime = incHour(firstDataBuffer.startDateTime,1);
        secondDataBuffer.endDateTime = secondDataBuffer.startDateTime;
        secondDataBuffer.data[nameOfValueToAnalyze_] = secondValue;
        dbi.writeToDataBase(secondDataBuffer);

        // insert third value
        DataBuffer thirdDataBuffer = firstDataBuffer;
        thirdDataBuffer.startDateTime = incHour(firstDataBuffer.startDateTime,2);
        thirdDataBuffer.endDateTime = thirdDataBuffer.startDateTime;
        thirdDataBuffer.data[nameOfValueToAnalyze_] = thirdValue;
        dbi.writeToDataBase(thirdDataBuffer);

        //cout << "1 " <<  firstDataBuffer << endl;
        //cout << "2 " << secondDataBuffer << endl;
        //cout << "3 " <<  thirdDataBuffer << endl;
        //cout << "4 " << fourthDataBuffer << endl;

        firstDataBuffer.startDateTime = incHour(firstDataBuffer.startDateTime,3);
        firstDataBuffer.endDateTime   = firstDataBuffer.startDateTime;

        currentIterationNumber++;
        if (currentIterationNumber % 100 == 0) {
            cout << "iterated through " << currentIterationNumber << " datapoints." << endl;
        }
    }
}


void createZTransformMeasurement(string nameOfValueToAnalyze_, string dataSourceToAnalyze_, struct tm startDate_, struct tm endDate_) {

    // create and init singleton-DBInterface-object
    DBInterface& dbi = DBInterface::getInstance();
    // todo
    dbi.init();
    dbi.writeStatusOK(true);


    DataBuffer CurrentBuffer;
    CurrentBuffer.startDateTime = startDate_;
    CurrentBuffer.endDateTime = endDate_;
    CurrentBuffer.useDateTimes = true;
    CurrentBuffer.dataSource = dataSourceToAnalyze_;
    CurrentBuffer.useDataSource = true;
    CurrentBuffer.data[nameOfValueToAnalyze_] = 0;


    vector<DataBuffer> allDataBuffersOfValueToAnalyze = dbi.readFromDataBase(CurrentBuffer);
    vector<double>     allDataOfValueToAnalyze;
    for (int i = 0; i < allDataBuffersOfValueToAnalyze.size(); i++) {
        allDataOfValueToAnalyze.push_back(allDataBuffersOfValueToAnalyze[i].data[nameOfValueToAnalyze_]);
    }

    vector<double> zTransformedDataOfValueToAnalyze = zTransformVector(allDataOfValueToAnalyze);
    CurrentBuffer.dataSource = "zTransformed"+CurrentBuffer.dataSource;
    for (int i = 0 ; i < zTransformedDataOfValueToAnalyze.size(); i++) {
        CurrentBuffer.endDateTime = CurrentBuffer.startDateTime;
        CurrentBuffer.data[nameOfValueToAnalyze_] = zTransformedDataOfValueToAnalyze[i];
        dbi.writeToDataBase(CurrentBuffer);
        CurrentBuffer.startDateTime = incHour(CurrentBuffer.startDateTime,1);
        if (i % 100 == 0) {
            cout << "iterated through " << i << " datapoints." << endl;
        }
    }
}

vector<double> zTransformVector(const vector<double>& vectorToTransform_) {

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

void dataToCSV(const string& path_, string nameOfValueToAnalyze_, string dataSourceToAnalyze_, struct tm startDate_, struct tm endDate_) {
    ofstream file;
    file.open(path_);
    file << nameOfValueToAnalyze_ << endl;

    // create and init singleton-DBInterface-object
    DBInterface& dbi = DBInterface::getInstance();
    // todo
    dbi.init();
    dbi.writeStatusOK(true);


    DataBuffer CurrentBuffer;
    CurrentBuffer.startDateTime = startDate_;
    CurrentBuffer.endDateTime = endDate_;
    CurrentBuffer.useDateTimes = true;
    CurrentBuffer.dataSource = dataSourceToAnalyze_;
    CurrentBuffer.useDataSource = true;
    CurrentBuffer.data[nameOfValueToAnalyze_] = 0;


    vector<DataBuffer> allDataBuffersOfValueToAnalyze = dbi.readFromDataBase(CurrentBuffer);
    vector<double>     allDataOfValueToAnalyze;
    for (int i = 0; i < allDataBuffersOfValueToAnalyze.size(); i++) {
        file << allDataBuffersOfValueToAnalyze[i].data[nameOfValueToAnalyze_] << endl;
    }
}
