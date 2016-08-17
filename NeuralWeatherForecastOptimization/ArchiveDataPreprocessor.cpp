#include <iostream>
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

void   fillMissingForecastDataWithWeatherStationPlusStdDevOfError(string nameOfValueToAnalyze_);
double analyzeMissingData(string nameOfValueToAnalyze_);

/**
 * main of ArchiveDataPreprocessor
 * @brief iterates through all data-points in influxDB and creates missing datapoints, using linear lines of best fit
 *
 */
//int main4() {
int main() {
    cout << "Hello World!" << endl;

    DataBuffer firstDataBuffer;
    LogWriter log("ArchiveDataPreprocessor", PATH_OF_LOGFILE+"x");


    fillMissingForecastDataWithWeatherStationPlusStdDevOfError("Lufttemperatur_2m");

    return 0;
}

void fillMissingForecastDataWithWeatherStationPlusStdDevOfError(string nameOfValueToAnalyze_) {
    //double meanError = analyzeMissingData(nameOfValueToAnalyze_);
    double meanError = -1.03303;

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

    // iterate through data and generate missing
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

        if ( (resultWeatherStation.size() > 0) && (resultForecast.size() == 0 ) ) {
            double valueWeatherStation = resultWeatherStation.data()->data[nameOfValueToAnalyze_];
            DataBuffer newForecastValueForMissingDate = WeatherStationBuffer;
            newForecastValueForMissingDate.dataSource = "Forecast";
            newForecastValueForMissingDate.data[nameOfValueToAnalyze_] = valueWeatherStation + meanError;
            dbi.writeToDataBase(newForecastValueForMissingDate);
            if (dbi.getDBFailure()) {
                cout << "Error during inserting" << endl;
            }

        }

        WeatherStationBuffer.startDateTime = incHour(WeatherStationBuffer.startDateTime,3);
        WeatherStationBuffer.endDateTime   = WeatherStationBuffer.startDateTime;
    }
}

/**
 * @brief analyzeMissingData
 * @return returns mean error
 */
double analyzeMissingData(string nameOfValueToAnalyze_) {

    // create and init singleton-DBInterface-object
    DBInterface& dbi = DBInterface::getInstance();
    // todo
    dbi.init();
    dbi.writeStatusOK(true);

    DataBuffer normalValueBuffer;
    normalValueBuffer.startDateTime.tm_year = 2014;
    normalValueBuffer.startDateTime.tm_mon  = 8;
    normalValueBuffer.startDateTime.tm_mday = 19;
    normalValueBuffer.startDateTime.tm_hour = 0;
    normalValueBuffer.startDateTime.tm_min  = 0;
    normalValueBuffer.startDateTime.tm_sec  = 0;
    normalValueBuffer.endDateTime = normalValueBuffer.startDateTime;
    normalValueBuffer.useDateTimes = true;
    normalValueBuffer.dataSource = "WeatherStation";
    normalValueBuffer.useDataSource = true;
    normalValueBuffer.data[nameOfValueToAnalyze_] = 0;

    bool lastDateReached = false;
    vector<double> ErrorValues;
    int dataCountTotal                      = 0;
    int dataCountMissingFromWeatherStation  = 0;
    int dataCountMissingFromForecast        = 0;
    int dataCountMissingFromBoth            = 0;

    // get standard deviation
    while (!lastDateReached) {
        DataBuffer ForecastBuffer = normalValueBuffer;
        ForecastBuffer.dataSource = "Forecast";
        if ( (normalValueBuffer.startDateTime.tm_year >= 2016) &&
             (normalValueBuffer.startDateTime.tm_mon  >= 7   ) &&
             (normalValueBuffer.startDateTime.tm_mday >= 16  ) &&
             (normalValueBuffer.startDateTime.tm_hour >= 21  ) &&
             (normalValueBuffer.startDateTime.tm_min  >= 0   ) &&
             (normalValueBuffer.startDateTime.tm_sec  >= 0   ) ){
            lastDateReached = true;
        }

        vector<DataBuffer> resultWeatherStation = dbi.readFromDataBase(normalValueBuffer);
        vector<DataBuffer> resultForecast       = dbi.readFromDataBase(ForecastBuffer);



        if ( (resultWeatherStation.size() > 0) && (resultForecast.size() > 0 ) ) {
            double valueWeatherStation = resultWeatherStation.data()->data[nameOfValueToAnalyze_];
            double valueForecast       =       resultForecast.data()->data[nameOfValueToAnalyze_];
            double Error = valueWeatherStation - valueForecast;
            ErrorValues.push_back(Error);

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

        dataCountTotal++;
        if (dataCountTotal % 100 == 0) {
            cout << "iterated " << dataCountTotal << " dates" << endl;
        }

        normalValueBuffer.startDateTime = incHour(normalValueBuffer.startDateTime,3);
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
            WeatherStationBuffer.dataSource = "Error";
            ErrorBuffer.data[nameOfValueToAnalyze_] = Error;
            dbi.writeToDataBase(ErrorBuffer);
            if (dbi.getDBFailure()) {
                cout << "Error during insert" << endl;
            }
        }
        WeatherStationBuffer.startDateTime = incHour(WeatherStationBuffer.startDateTime,1);
        WeatherStationBuffer.endDateTime   = WeatherStationBuffer.startDateTime;
    }
}

void createSlopeMeasurement(string nameOfValueToAnalyze_, string dataSource_) {

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
    int dataCountTotal                      = 0;
    int dataCountMissingFromWeatherStation  = 0;
    int dataCountMissingFromForecast        = 0;
    int dataCountMissingFromBoth            = 0;

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
            WeatherStationBuffer.dataSource = "Error";
            dbi.writeToDataBase(ErrorBuffer);
            if (dbi.getDBFailure()) {
                cout << "Error during insert" << endl;
            }
        }
    }
}

/*
    firstDataBuffer.startDateTime.tm_year = 2014;
    firstDataBuffer.startDateTime.tm_mon  = 8;
    firstDataBuffer.startDateTime.tm_mday = 19;
    firstDataBuffer.startDateTime.tm_hour = 0;
    firstDataBuffer.startDateTime.tm_min  = 0;
    firstDataBuffer.startDateTime.tm_sec  = 0;
    firstDataBuffer.endDateTime = firstDataBuffer.startDateTime;
    firstDataBuffer.useDateTimes = true;
    firstDataBuffer.dataSource = "Forecast";
    firstDataBuffer.useDataSource = true;
    firstDataBuffer.data[name] = 0;

    bool lastDateReached = false;


    while (!lastDateReached) {
        if ( (firstDataBuffer.startDateTime.tm_year >= 2016) &&
             (firstDataBuffer.startDateTime.tm_mon  >= 7   ) &&
             (firstDataBuffer.startDateTime.tm_mday >= 16  ) &&
             (firstDataBuffer.startDateTime.tm_hour >= 21  ) &&
             (firstDataBuffer.startDateTime.tm_min  >= 0   ) &&
             (firstDataBuffer.startDateTime.tm_sec  >= 0   ) ){
            lastDateReached = true;
        }

        if ( (firstDataBuffer.startDateTime.tm_year == 2014) &&
             (firstDataBuffer.startDateTime.tm_mon  == 9   ) &&
             (firstDataBuffer.startDateTime.tm_mday == 23  ) &&
             (firstDataBuffer.startDateTime.tm_hour == 0   ) &&
             (firstDataBuffer.startDateTime.tm_min  == 0   ) &&
             (firstDataBuffer.startDateTime.tm_sec  == 0   ) ){
            cout << "got it " << endl;
        }

        vector<DataBuffer> result = dbi.readFromDataBase(firstDataBuffer);
        if (result.size() == 0) {
            log << SLevel(ERROR) << "Data missing at "
                << firstDataBuffer.startDateTime.tm_year << "-"
                << firstDataBuffer.startDateTime.tm_mon  << "-"
                << firstDataBuffer.startDateTime.tm_mday << "T"
                << firstDataBuffer.startDateTime.tm_hour << ":"
                << firstDataBuffer.startDateTime.tm_min  << ":"
                << firstDataBuffer.startDateTime.tm_sec  << "Z" << endl;
            vector<DataBuffer> result2 = dbi.readFromDataBase(firstDataBuffer);
        } else {

        }
        if (firstDataBuffer.startDateTime.tm_mday == 25) {
            cout << "tuut" << endl;
        }
        firstDataBuffer.startDateTime = incHour(firstDataBuffer.startDateTime,3);
        firstDataBuffer.endDateTime   = firstDataBuffer.startDateTime;

    }

    /*
    vector<DataBuffer> result = dbi.readFromDataBase(firstDataBuffer);
    double firstValue = result[0].data[name];

    DataBuffer fourthDataBuffer = firstDataBuffer;
    fourthDataBuffer.startDateTime = incHour(firstDataBuffer.startDateTime,3);
    fourthDataBuffer.endDateTime = fourthDataBuffer.startDateTime;
    result = dbi.readFromDataBase(fourthDataBuffer);
    double fourthValue = result[0].data[name];

    double slope = (fourthValue - firstValue) / 3.0;
    double secondValue = slope * 1 + firstValue;
    double thirdValue  = slope * 2 + firstValue;

    DataBuffer secondDataBuffer = firstDataBuffer;
    secondDataBuffer.startDateTime = incHour(firstDataBuffer.startDateTime,1);
    secondDataBuffer.endDateTime = secondDataBuffer.startDateTime;
    secondDataBuffer.data[name] = secondValue;
    dbi.writeToDataBase(secondDataBuffer);

    DataBuffer thirdDataBuffer = firstDataBuffer;
    thirdDataBuffer.startDateTime = incHour(firstDataBuffer.startDateTime,2);
    thirdDataBuffer.endDateTime = thirdDataBuffer.startDateTime;
    thirdDataBuffer.data[name] = thirdValue;
    dbi.writeToDataBase(thirdDataBuffer);

    cout << "1 " <<  firstDataBuffer << endl;
    cout << "2 " << secondDataBuffer << endl;
    cout << "3 " <<  thirdDataBuffer << endl;
    cout << "4 " << fourthDataBuffer << endl;

    return 0;
} */

