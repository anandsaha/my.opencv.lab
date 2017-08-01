#g++ -o bs   tutorial_background_subtraction.cpp     `pkg-config --cflags --libs opencv`
#g++ -o ftt  loc_featurestotrack.cpp                 `pkg-config --cflags --libs opencv`
#g++ -o fd   frame_differencing.cpp                  `pkg-config --cflags --libs opencv`
#g++ -o hc   harris_corner.cpp                       `pkg-config --cflags --libs opencv`
#g++ -o st   shi-tomasi.cpp                          `pkg-config --cflags --libs opencv`
g++ -o pipeline   pipeline.cpp                          `pkg-config --cflags --libs opencv`


