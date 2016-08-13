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
    qtDate = qtDate.addSecs(3600*hourDelta_);
    time_.tm_year = qtDate.date().year();
    time_.tm_mon  = qtDate.date().month();
    time_.tm_mday = qtDate.date().day();
    time_.tm_hour = qtDate.time().hour();
    time_.tm_min  = qtDate.time().minute();
    time_.tm_sec  = qtDate.time().second();
    return time_;
}


/**
 * main of ArchiveDataPreprocessor
 * @brief iterates through all data-points in influxDB and creates missing datapoints, using linear lines of best fit
 *
 */
//int main4() {
int main() {
    cout << "Hello World!" << endl;

    DataBuffer firstDataBuffer;

    // create and init singleton-DBInterface-object
    DBInterface& dbi = DBInterface::getInstance();
    // todo
    dbi.init();
    dbi.writeStatusOK(true);

    string name = "Lufttemperatur_2m";

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
}
