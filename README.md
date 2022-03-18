# SProto

C++ communication protocol.
Feaqtures: 
 - Easily expandable with inheritance.
 - Small packet lengths, minimal overhead.
 - Expandable crypto, with example (XOR), so can use over radio too.
 - Dynamically add data to packet.
 - Measurement data typed by content, eg Temperature, or Humidity, so content can be identified easily.
 - Helper functions to send data to MQTT broker.
 - Helper class ( SProtoServer ) to keep track of stations and sensors. New and already seen, and also expiration handling.
## This is a work in progress lib, with a lot of limitations! Use with cautions. If you want to improve it, send a PR, or ask for a feature.

## Usage:

See the example.
For now, you should edit the config.h if you want to change some parameters.
