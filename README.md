# Sharp GP2AP03VT00F Time-Of-Flight Sensor Arduino driver

## Purpose

The [Sharp GP2AP03VT00F](https://global.sharp/products/device/lineup/selection/opto/haca/diagram3.html)
Time-Of-Flight sensor seems to be a nice part for
makers and roboticists, focusing on good short distance sensing where other
parts sometimes have problems.

Alas, Sharp seems to be one of these companies that likes to make it
difficult for people that are not billion dollar corporations to use their
parts, so information on how to use these sensors from a programmer's
perspective is very hard to come by.

This Arduino driver is based on information found in
[this repo](https://github.com/sharpsensoruser/sharp-sensor-demos/tree/master)
for the related GP2AP02VT00F sensor.  I had to make some changes and do
a bunch of experiments to make it work for the GP2AP03VT00F, and having no
access to a GP2AP02VT00F I don't know if it will work for that part as well.

## Sensor operation

As far as I can glean from the sparse info in the
[datasheet](https://www.socle-tech.com/doc/IC%20Channel%20Product/SHARP_GP2AP03VT00F_Specification.pdf)
and the driver references above, the sensor returns two measurements:
a coarse and a fine reading.
* The coarse reading returns a value between 3 (object against the sensor)
and 39 (@ ~2.5m), step size is around 7 cm.
* The fine reading returns a value between ~12 (@ ~1 cm) and 115 (@ ~8 cm),
resolution is around 0.7 mm.  Accuracy is less than that according to the
datasheet and if you get closer than ~1 cm, the value starts to increase.

Anyway, I don't know if I'm using the sensor in the best way but this is
what I got working with the information available to me at the time.  If
you have useful insights or information, please open an issue!

## Usage

Please refer to the example code to see how to use this driver, it's
quite simple.

