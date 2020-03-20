# Soil Sensor
An Arduino-based soil sensor to measure the temperature and moisture content of two different samples of soil; one covered by a broad-leaf carpet grass and the other fully exposed. 

The sensor is part of a project for an intro to soil sciences class from Colorado State University. 

### Basic Unit Operation

Collect the following data from two distinct soil locations:

1. Temperature of soil at 15cm
2. Percent soil moisture (a crude measure using a cheap sensor)

The sensor itself is functionally complete. Data can be viewed at the following link:

https://io.adafruit.com/mcwresearch/dashboards/soil-sensor-socr

Without an account with Adafruit, you will have to manually refresh the page to see the very latest data. But visitors without accounts can still see a snapshot of the most recent (and all historic) data. 

As of March 19, 2020, the data is not real data. The sensor is sitting on my desk. My next step is to take it from the breadboard to a water-proof home and get it deployed and collecting real data. 

### Hardware To-Do
* [ ] Complete Perma-Proto board
* [ ] Connect Adafruit TPL5110 Low Power Timer Breakout
* [ ] Connect proper antenna

### Software To-Do
* [ ] [#1](https://github.com/OilSlick/soil_sensor_socr/issues/1#issue-584777977) Reduce number of transmissions