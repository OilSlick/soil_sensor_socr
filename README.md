# Soil Sensor
An Arduino-based soil sensor to measure the temperature and moisture content of two different samples of soil; one covered by a broad-leaf carpet grass and the other fully exposed. 

The sensor is part of a project for an intro to soil sciences class from Colorado State University. 

I gratuitously copied-and-pasted large chunks of code from example pages of each of the devices and I relied heavily on Adafruit documentation as well. I buy a lot of components from them for various other projects and I also pay for the Plus-level of service from [io.Adafruit](https://io.adafruit.com/). I highly recommend them.

There is more detail about the project on the [wiki page](https://github.com/OilSlick/soil_sensor_socr/wiki/Home/).

### Basic Unit Operation

Collect the following data from two distinct soil locations:

1. Temperature of soil at 15cm
2. Percent soil moisture (a crude measure using a cheap sensor)

The sensor itself is functionally complete. Data can be viewed at the following link:

https://io.adafruit.com/mcwresearch/dashboards/soil-sensor-socr

Without an account with Adafruit, you will have to manually refresh the page to see the very latest data. But visitors without accounts can still see a snapshot of the most recent (and all historic) data. 

### Hardware To-Do
* [x] Complete Perma-Proto board
* [ ] Connect Adafruit TPL5110 Low Power Timer Breakout (may opt not to...)
* [x] Connect proper antenna

### Software To-Do
* [ ] [#1](https://github.com/OilSlick/soil_sensor_socr/issues/1#issue-584777977) Reduce number of transmissions