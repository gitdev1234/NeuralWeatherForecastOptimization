/**
 * OpenWeatherMap.cpp
 * Purpose: implements class OpenWeatherMap
 *
 * @author Wall.Of.Death
 * @version 1.0 20160705
 */

#include "../include/OpenWeatherMap.h"

/**
 * OpenWeatherMap::readWebContent
 * @brief readWebContent() returns the current forecast-data read from http://openweathermap.org/
 * @return returns DataBuffer-object which contains the current forecast-data of the webcontent http://openweathermap.org/
 */
DataBuffer OpenWeatherMap::readWebContent() {
    // --- TODO -- dummy code ---
    DataBuffer temp;
    temp.dataSource = "Forecast";
    temp.useDataSource = true;
    temp.data["Lufttemperatur_2m"] = rand() % 60;
    temp.data["Luftdruck_2m"] = (rand() % 400) + 800;

    cout << "read WebContent OpenWeatherMap" << endl;
    return temp;
    // --- TODO -- dummy code ---
}
