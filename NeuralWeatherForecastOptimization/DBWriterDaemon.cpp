#include <iostream>
#include <ctime>
// ----
#include "DataBuffer.h"
#include "Sensor.h"
#include "TemperatureSensor.h"
#include "AirPressureSensor.h"
#include "DBInterface.h"
#include "LogWriter.h"
#include "WebContent.h"
#include "OpenWeatherMap.h"


/**
 * main of DBWriterDaemon
 * @brief reads sensor-values and web-forecast and writes them to DB
 *
 */
int main1() {
//int main() {
    // create Log-Writer Object
    LogWriter log("DBWriterDaemon", PATH_OF_LOGFILE);
    log << SLevel(INFO) << "Started writing sensor/forecast-data to database." << endl;
    // measure time-start
    clock_t begin = clock();

    // create and init singleton-DBInterface-object
    DBInterface& dbi = DBInterface::getInstance();
    dbi.init();
    // todo
    dbi.writeStatusOK(true);

    // create sensor objects
    Sensor AirTemperature2MSensor;
    AirTemperature2MSensor.setSensorType(new TemperatureSensor());
    Sensor AirPressure2MSensor;
    AirPressure2MSensor.setSensorType(new AirPressureSensor());

    // read sensor values
    DataBuffer SensorValuesBuffer;
    SensorValuesBuffer.dataSource = "WeatherStation";
    SensorValuesBuffer.useDataSource = true;
    SensorValuesBuffer.data["Lufttemperatur_2m"] = AirTemperature2MSensor.readSensor();
    SensorValuesBuffer.data["Luftdruck_2m"]      = AirPressure2MSensor.readSensor();

    // create webcontent - objects
    WebContent OpenWeatherMapForecast;
    OpenWeatherMapForecast.setWebContentType(new OpenWeatherMap());

    // read forecast values
    DataBuffer ForecastValuesBuffer;
    ForecastValuesBuffer.dataSource = "Forecast";
    ForecastValuesBuffer = OpenWeatherMapForecast.readWebContent();

    // check status
    bool statusOK = dbi.readStatusOK();
    if (!statusOK) {
        log << SLevel(ERROR) << "Aborted writing sensor-data and forecast-data to" <<
                                " database, because of status is not okay." << endl;
    } else {
        // write sensor values to database
        dbi.writeToDataBase(SensorValuesBuffer);
        bool dbFailure = dbi.getDBFailure();
        if (!dbFailure) {
            log << SLevel(INFO) << "Succesfully wrote sensor-data to database." << endl;
        } else {
            log << SLevel(ERROR) << "DB-Failure occured during writing sensor-data to database." << endl;
        }

        // write forecast / webcontent values to database
        dbi.writeToDataBase(ForecastValuesBuffer);
        dbFailure = dbi.getDBFailure();
        if (!dbFailure) {
            log << SLevel(INFO) << "Succesfully wrote forecast-data to database." << endl;
        } else {
            log << SLevel(ERROR) << "DB-Failure occured during writing forecast-data to database." << endl;
        }

    }

    // measure time-end
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    log << SLevel(INFO) << "Finished writing sensor/forecast-data to database. Elapsed Time : " << elapsed_secs << " seconds." << endl;

    return 0;
}
