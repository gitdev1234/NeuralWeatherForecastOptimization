#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <float.h>
#include "DataBuffer.h"
#include "DBInterface.h"
#include "Sensor.h"
#include "TemperatureSensor.h"
#include "AirPressureSensor.h"
#include "WebContent.h"
#include "OpenWeatherMap.h"
#include "ANNWrapper.h"


// --- Tests for HTTP-REQUEST ---

TEST_CASE( "HTTPRequest.post works") {
    HTTPRequest req;
    string url;
    string postFields;

    SECTION("both parameters valid") {
        url = "http://localhost:8086/write?db=test3";
        postFields = "forecast,datatype=temperature value=50";
        REQUIRE(req.post(url,postFields));
    }

    SECTION("url_ valid, postFields_ empty") {
        url = "http://localhost:8086/query?q=create+database+WeatherData2&db=_internal";
        postFields = "";
        REQUIRE(req.post(url,postFields));
    }

    SECTION("url_ is not valid") {
        url = "http://localhost:86/write?db=test3";
        postFields = "forecast,datatype=temperature value=50";
        REQUIRE_FALSE(req.post(url,postFields));
    }

    SECTION("url_ is empty") {
        url = "";
        postFields = "forecast,datatype=temperature value=50";
        REQUIRE_FALSE(req.post(url,postFields));
    }

}

TEST_CASE( "HTTPRequest.get works") {
    HTTPRequest req;
    string url;

    SECTION("url_ valid") {
        url = "http://localhost:8086/query?pretty=true&db=WeatherData&q=SELECT+Lufttemperatur_2m+FROM+point";
        string result = req.get(url);
        string sub = result.substr(0,20);
        REQUIRE(sub == "{\n    \"results\": [\n ");
    }


    SECTION("url_ is not valid") {
        url = "http://localhost:8086/Lufttemperatur_2m+FROM+point";
        REQUIRE(req.get(url) == "");
    }

    SECTION("url_ is empty") {
        url = "";
        REQUIRE(req.get(url) == "");
    }

}


TEST_CASE("DBInterface, write/read to/from database, getDBFailure") {
    DataBuffer dataBuffer, dataBuffer2;

    DBInterface& dbi = DBInterface::getInstance();
    dbi.init();

    dbi.writeStatusOK(true);

    SECTION ("write / read valid databuffer") {
        //2015-06-11T20:46:02
        dataBuffer.startDateTime.tm_sec  = 2;    // seconds
        dataBuffer.startDateTime.tm_min  = 46;   // minutes
        dataBuffer.startDateTime.tm_hour = 20;   // hours
        dataBuffer.startDateTime.tm_mday = 11;   // day
        dataBuffer.startDateTime.tm_mon  = 6;    // month
        dataBuffer.startDateTime.tm_year = 2015; // Year

        dataBuffer.endDateTime = dataBuffer.startDateTime;
        dataBuffer.useDateTimes = true;

        dataBuffer.dataSource = "Forecast";
        dataBuffer.useDataSource = true;

        dataBuffer.data["Lufttemperatur_2m"] = 14.5;
        dataBuffer.data["RelativeLuftfeuchte_2m"] = 1200;
        dbi.writeToDataBase(dataBuffer);

        dataBuffer2 = dataBuffer;
        dataBuffer2.data["Lufttemperatur_2m"] = 0;
        dataBuffer2.data["RelativeLuftfeuchte_2m"] = 0;
        vector<DataBuffer> dataBufferVec = dbi.readFromDataBase(dataBuffer2);
        REQUIRE(dataBufferVec[0] == dataBuffer);
        REQUIRE(dbi.getDBFailure() == false);
    }

    SECTION ("write / read date = 01.01.1972") {
        dataBuffer.startDateTime.tm_sec  = 0;   // seconds
        dataBuffer.startDateTime.tm_min  = 0;   // minutes
        dataBuffer.startDateTime.tm_hour = 0;   // hours
        dataBuffer.startDateTime.tm_mday = 1;   // day
        dataBuffer.startDateTime.tm_mon  = 1;    // month
        dataBuffer.startDateTime.tm_year = 1972; // Year

        dataBuffer.endDateTime = dataBuffer.startDateTime;
        dataBuffer.useDateTimes = true;

        dataBuffer.dataSource = "Forecast";
        dataBuffer.useDataSource = true;

        dataBuffer.data["Lufttemperatur_2m"] = 14.5;
        dataBuffer.data["RelativeLuftfeuchte_2m"] = 1200;
        dbi.writeToDataBase(dataBuffer);

        dataBuffer2 = dataBuffer;
        dataBuffer2.data["Lufttemperatur_2m"] = 0;
        dataBuffer2.data["RelativeLuftfeuchte_2m"] = 0;
        vector<DataBuffer> dataBufferVec = dbi.readFromDataBase(dataBuffer2);
        REQUIRE(dataBufferVec[0] == dataBuffer);
        REQUIRE(dbi.getDBFailure() == false);
    }

    SECTION ("write / read date < 01.01.1972") {
        dataBuffer.startDateTime.tm_sec  = 0;   // seconds
        dataBuffer.startDateTime.tm_min  = 0;   // minutes
        dataBuffer.startDateTime.tm_hour = 0;   // hours
        dataBuffer.startDateTime.tm_mday = 31;   // day
        dataBuffer.startDateTime.tm_mon  = 12;    // month
        dataBuffer.startDateTime.tm_year = 1971; // Year

        dataBuffer.endDateTime = dataBuffer.startDateTime;
        dataBuffer.useDateTimes = true;

        dataBuffer.dataSource = "Forecast";
        dataBuffer.useDataSource = true;

        dataBuffer.data["Lufttemperatur_2m"] = 14.5;
        dataBuffer.data["RelativeLuftfeuchte_2m"] = 1200;
        dbi.writeToDataBase(dataBuffer);

        dataBuffer2 = dataBuffer;
        dataBuffer2.data["Lufttemperatur_2m"] = 0;
        dataBuffer2.data["RelativeLuftfeuchte_2m"] = 0;
        vector<DataBuffer> dataBufferVec = dbi.readFromDataBase(dataBuffer2);
        REQUIRE(dataBufferVec.size() == 0);
        REQUIRE(dbi.getDBFailure() == true);
    }

    /* TODO Influx Error

    SECTION ("write / read with current local time") {
        dataBuffer.useDateTimes = false;

        dataBuffer.dataSource = "Forecast";
        dataBuffer.useDataSource = true;

        dataBuffer.data["Lufttemperatur_2m"] = -10.5;
        dataBuffer.data["RelativeLuftfeuchte_2m"] = 90.4;
        dataBuffer.data["Windgeschwindigkeit_Stundenmittel"] = 10.345;

        dbi.writeToDataBase(dataBuffer);

        dataBuffer2 = dataBuffer;
        dataBuffer2.data["Lufttemperatur_2m"] = 0;
        dataBuffer2.data["RelativeLuftfeuchte_2m"] = 0;
        dataBuffer2.data["Windgeschwindigkeit_Stundenmittel"] = 0;
        vector<DataBuffer> dataBufferVec = dbi.readFromDataBase(dataBuffer2);
        REQUIRE(dataBufferVec[0] == dataBuffer);
        REQUIRE(dbi.getDBFailure() == false);
    }*/


    SECTION ("write / read maximum / minimum values") {
        dataBuffer.startDateTime.tm_sec  = 0;   // seconds
        dataBuffer.startDateTime.tm_min  = 0;   // minutes
        dataBuffer.startDateTime.tm_hour = 0;   // hours
        dataBuffer.startDateTime.tm_mday = 31;   // day
        dataBuffer.startDateTime.tm_mon  = 12;   // month
        dataBuffer.startDateTime.tm_year = 1972; // Year

        dataBuffer.endDateTime = dataBuffer.startDateTime;
        dataBuffer.useDateTimes = true;

        dataBuffer.dataSource = "Forecast";
        dataBuffer.useDataSource = true;

        dataBuffer.data["Lufttemperatur_2m"] = INFLUXDB_MAX;
        dataBuffer.data["RelativeLuftfeuchte_2m"] = INFLUXDB_MIN;
        dataBuffer.data["Windgeschwindigkeit_Stundenmittel"] = 0;

        dbi.writeToDataBase(dataBuffer);

        dataBuffer2 = dataBuffer;
        dataBuffer2.data["Lufttemperatur_2m"] = 0;
        dataBuffer2.data["RelativeLuftfeuchte_2m"] = 0;
        dataBuffer2.data["Windgeschwindigkeit_Stundenmittel"] = 0;
        vector<DataBuffer> dataBufferVec = dbi.readFromDataBase(dataBuffer2);
        REQUIRE(dataBufferVec[0] == dataBuffer);
        REQUIRE(dbi.getDBFailure() == false);
    }

    SECTION ("write / read invalid values and strings") {
        // invalid values get cut to range
        // invalid extra signs get removed
        // (alphabetical chars, numbers and '_' are allowed)
        dataBuffer.startDateTime.tm_sec  = 59;   // seconds
        dataBuffer.startDateTime.tm_min  = 59;   // minutes
        dataBuffer.startDateTime.tm_hour = 1;   // hours
        dataBuffer.startDateTime.tm_mday = 10;   // day
        dataBuffer.startDateTime.tm_mon  = 8;   // month
        dataBuffer.startDateTime.tm_year = 2016; // Year

        dataBuffer.endDateTime = dataBuffer.startDateTime;
        dataBuffer.useDateTimes = true;

        dataBuffer.dataSource = "Foreca%st";
        dataBuffer.useDataSource = true;

        dataBuffer.data["Luft%&te2mperatur_2m"] = INFLUXDB_MAX+1;
        dataBuffer.data["RelativeL34uf'tfeuchte_2m"] = INFLUXDB_MIN-1;
        dataBuffer.data["Windgeschwindi()gk?eit_Stundenmittel"] = 0;

        dbi.writeToDataBase(dataBuffer);

        dataBuffer2 = dataBuffer;
        dataBuffer2.data["Luft%&te2mperatur_2m"] = 0;
        dataBuffer2.data["RelativeL34uf'tfeuchte_2m"] = 0;
        dataBuffer2.data["Windgeschwindi()gk?eit_Stundenmittel"] = 0;
        vector<DataBuffer> dataBufferVec = dbi.readFromDataBase(dataBuffer2);

        DataBuffer dataBufferWithoutExtraSigns;

        dataBufferWithoutExtraSigns.startDateTime = dataBuffer.startDateTime;
        dataBufferWithoutExtraSigns.endDateTime   = dataBuffer.endDateTime;
        dataBufferWithoutExtraSigns.useDateTimes = true;

        dataBufferWithoutExtraSigns.dataSource = "Forecast";
        dataBufferWithoutExtraSigns.useDataSource = true;

        dataBufferWithoutExtraSigns.data["Luftte2mperatur_2m"] = INFLUXDB_MAX+1;
        dataBufferWithoutExtraSigns.data["RelativeL34uftfeuchte_2m"] = INFLUXDB_MIN-1;
        dataBufferWithoutExtraSigns.data["Windgeschwindigkeit_Stundenmittel"] = 0;

        REQUIRE(dataBufferVec[0] == dataBufferWithoutExtraSigns);
        REQUIRE(dbi.getDBFailure() == false);
    }

}

TEST_CASE ("Write / Read Status") {
    DBInterface& dbi = DBInterface::getInstance();
    dbi.init();

    SECTION("Write/Read of true works") {
        dbi.writeStatusOK(true);
        REQUIRE(dbi.readStatusOK());
    }

    SECTION("Write/Read of false works") {
        dbi.writeStatusOK(false);
        REQUIRE_FALSE(dbi.readStatusOK());
    }
}

// --- Tests for Sensor

TEST_CASE("readSensor works") {
    // create and init singleton-DBInterface-object
    DBInterface& dbi = DBInterface::getInstance();
    dbi.init();
    // todo
    dbi.writeStatusOK(true);

    // --- TODO -- dummy code ---
    // randomize
    srand(time(NULL));
    // --- TODO -- dummy code ---

    SECTION("readSensor works for temperature-sensor") {
        Sensor Sensor1;
        Sensor1.setSensorType(new TemperatureSensor());
        DataBuffer temperatureSensorBuffer = Sensor1.readSensor();
        REQUIRE(temperatureSensorBuffer.data.size() > 0);
        REQUIRE(temperatureSensorBuffer.data["Lufttemperatur_2m"] > -2000);
        REQUIRE(temperatureSensorBuffer.data["Lufttemperatur_2m"] <  2000);
        REQUIRE(dbi.readStatusOK());
    }

    SECTION("readSensor works for airpressure-sensor") {
        Sensor Sensor1;
        Sensor1.setSensorType(new AirPressureSensor());
        DataBuffer airPressureSensorBuffer = Sensor1.readSensor();
        REQUIRE(airPressureSensorBuffer.data.size() > 0);
        REQUIRE(airPressureSensorBuffer.data["Luftdruck_2m"] > -2000);
        REQUIRE(airPressureSensorBuffer.data["Luftdruck_2m"] <  2000);
        REQUIRE(dbi.readStatusOK());
    }

}


// --- Tests for WebContent

TEST_CASE("readWebContent works") {
    // create and init singleton-DBInterface-object
    DBInterface& dbi = DBInterface::getInstance();
    dbi.init();
    // todo
    dbi.writeStatusOK(true);

    SECTION("readWebContent works for OpenWeatherMap") {
        WebContent WebContent1;
        WebContent1.setWebContentType(new OpenWeatherMap());
        DataBuffer openWeatherMapBuffer = WebContent1.readWebContent();
        REQUIRE(openWeatherMapBuffer.data.size() > 0);
        REQUIRE(openWeatherMapBuffer.data["Lufttemperatur_2m"] > -2000);
        REQUIRE(openWeatherMapBuffer.data["Lufttemperatur_2m"] <  2000);
        REQUIRE(openWeatherMapBuffer.data["Luftdruck_2m"] > -2000);
        REQUIRE(openWeatherMapBuffer.data["Luftdruck_2m"] <  2000);
        REQUIRE(openWeatherMapBuffer.data["RelativeLuftfeuchte_2m"] > -2000);
        REQUIRE(openWeatherMapBuffer.data["RelativeLuftfeuchte_2m"] <  2000);
        REQUIRE(openWeatherMapBuffer.data["Windgeschwindigkeit"] > -2000);
        REQUIRE(openWeatherMapBuffer.data["Windgeschwindigkeit"] <  2000);
        REQUIRE(openWeatherMapBuffer.data["Windrichtung"] > -2000);
        REQUIRE(openWeatherMapBuffer.data["Windrichtung"] <  2000);
        REQUIRE(dbi.readStatusOK());
    }
}

// --- Tests for ANNWrapper

TEST_CASE("ANNWrapper works") {
    // create and init singleton-DBInterface-object
    DBInterface& dbi = DBInterface::getInstance();
    dbi.init();
    dbi.writeStatusOK(true);

    // --- TODO -- dummy code ---
    // randomize
    srand(time(NULL));
    // --- TODO -- dummy code ---

    SECTION("calculateOutput works") {
        ANNWrapper annWrapper(&dbi);

        DataBuffer forecast = annWrapper.calculateOutput();
        REQUIRE(forecast.data.size() > 0);
        REQUIRE(forecast.data["ANNTemperature"] > -2000);
        REQUIRE(forecast.data["ANNTemperature"] <  2000);
        REQUIRE(forecast.data["ANNAirPressure"] > -2000);
        REQUIRE(forecast.data["ANNAirPressure"] <  2000);
        REQUIRE(dbi.readStatusOK());
    }

    SECTION("trainArtificialNeuroNets works") {
        ANNWrapper annWrapper(&dbi);
        annWrapper.trainArtificialNeuroNets();
        REQUIRE(dbi.readStatusOK());
    }

}

// --- Tests for ArtificialNeuroNet

TEST_CASE("ArtificialNeuroNet forward works") {
    // create and init singleton-DBInterface-object
    DBInterface& dbi = DBInterface::getInstance();
    dbi.init();
    dbi.writeStatusOK(true);

    // --- TODO -- dummy code ---
    // randomize
    srand(time(NULL));
    // --- TODO -- dummy code ---

    ArtificialNeuroNet ann;
    ann.init("test ann",&dbi);

    SECTION("empty inputValues_ works") {
        REQUIRE(ann.forward({}) == 0);
        REQUIRE_FALSE(dbi.readStatusOK());
    }

    SECTION("inputValues containing max/min/0 values works") {
        vector<double> inputValues = {INFLUXDB_MAX,INFLUXDB_MIN,0};
        REQUIRE(ann.forward(inputValues) >= -2000);
        REQUIRE(ann.forward(inputValues) <=  2000);
        REQUIRE(dbi.readStatusOK());
    }

/*
    SECTION("inputValues containing max+1/min-1/0 values works") {
        REQUIRE(INFLUXDB_MAX==INFLUXDB_MAX+1);
        vector<double> inputValues = {INFLUXDB_MAX+1,INFLUXDB_MIN-1,0};
        REQUIRE(ann.forward(inputValues) == 0);
        REQUIRE_FALSE(dbi.readStatusOK());
    }*/

}

TEST_CASE("ArtificialNeuroNet train works") {
    // create and init singleton-DBInterface-object
    DBInterface& dbi = DBInterface::getInstance();
    dbi.init();
    dbi.writeStatusOK(true);

    // --- TODO -- dummy code ---
    // randomize
    srand(time(NULL));
    // --- TODO -- dummy code ---

    ArtificialNeuroNet ann;
    ann.init("test ann",&dbi);

    SECTION("empty inputValues_ works") {
        ann.train({},{});
        REQUIRE_FALSE(dbi.readStatusOK());
    }

    SECTION("input und expected output containing max/min/0 values works") {
        vector<vector<double>> inputValues;
        inputValues.push_back({INFLUXDB_MAX+1,INFLUXDB_MIN-1,0});
        vector<double> expectedOutPutValues = {INFLUXDB_MAX,INFLUXDB_MIN,0};
        ann.train(inputValues,expectedOutPutValues);
        REQUIRE(dbi.readStatusOK());
    }

/*
    SECTION("input und expected output values containing max+1/min-1/0 values works") {
        vector<vector<double>> inputValues;
        inputValues.push_back({INFLUXDB_MAX+1,INFLUXDB_MIN-1,0});
        inputValues.push_back({INFLUXDB_MAX+1,INFLUXDB_MIN-1,0});
        vector<double> expectedOutPutValues = {INFLUXDB_MAX+1,INFLUXDB_MIN-1,0};
        ann.train(inputValues,expectedOutPutValues);
        REQUIRE_FALSE(dbi.readStatusOK());
    }
*/
}
