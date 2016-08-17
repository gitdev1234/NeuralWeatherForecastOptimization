/**
 * DataBuffer.h
 * Purpose: defines constants, enums and structs
 *
 * @author Wall.Of.Death
 * @version 1.0 20160704
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <float.h>

using namespace std;

// constants
const string URL_OF_OPEN_WEATHER_MAP = "http://api.openweathermap.org/data/2.5/forecast?lat=52.9&lon=9.2&APPID=08854a5d6fe0754f2670f5fa5127a831";
const string URL_OF_DATABASE = "http://localhost:8086";
const string NAME_OF_DATBASE = "WeatherData";
const string PATH_OF_LOGFILE = "LogFile.txt";
const string PATH_OF_NET_STRUCTURE_WITHOUT_LOSS = "../NeuralWeatherForecastOptimization/prototxt/10_10_10_1_without_loss.prototxt";
const string PATH_OF_NET_STRUCTURE_WITH_LOSS    = "../NeuralWeatherForecastOptimization/prototxt/10_10_10_1_with_loss.prototxt";
const string PATH_OF_NET_SOLVER                 = "../NeuralWeatherForecastOptimization/prototxt/10_10_10_1_solver.prototxt";
const double INFLUXDB_MAX    =  1.79769e+308;
const double INFLUXDB_MIN    = -1.79769e+308;
const int    PIN_OF_DS18B20  = 21;

#endif // CONFIG_H
