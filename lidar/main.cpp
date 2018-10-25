/*
10/24/18

*/
#include <cstdlib>
#include <iostream>

#include <sweep/sweep.hpp>

void processData(const sweep::sample& sample);

static int startingAngle = 0;
static int endingAngle = 180000;
static int warningDistance = 15; // cm
static int minSignalQuality = 50;

int main(int argc, char* argv[]) try {

	if (argc != 2) {
		std::cerr << "Usage: ./lidar device\n";
		return EXIT_FAILURE;
	}

	std::cout << "Constructing sweep device..." << std::endl;
	sweep::sweep device{argv[1]};

	std::cout << "Motor Speed Setting: " << device.get_motor_speed() << " Hz" << std::endl;
	std::cout << "Sample Rate Setting: " << device.get_sample_rate() << " Hz" << std::endl;

	std::cout << "Beginning data acquisition as soon as motor speed stabilizes..." << std::endl;
	device.start_scanning();

	std::cout << "BEGIN SCAN" << std::endl;
	for (auto n = 0; n < 100; ++n) {
	//while(1) {

		//preform one full scan
		const sweep::scan scan = device.get_scan();

		//std::cout << "Scan #" << n << ":" << std::endl;

		for (const sweep::sample& sample : scan.samples) {
			processData(sample);
		}

	}

  device.stop_scanning();

} catch (const sweep::device_error& e) {
  std::cerr << "Error: " << e.what() << std::endl;
}
// END MAIN

//processes the data for each data point
void processData(const sweep::sample& sample){

	// verify if it is the angle that we care about
	if(sample.angle >= startingAngle && sample.angle <= endingAngle){
		// check the distance
		if(sample.distance <= warningDistance){
			// check signal strength
			if(sample.signal_strength >= minSignalQuality){

				std::cout << "distance: " << sample.distance << " angle: " << sample.angle << " strength: " << sample.signal_strength << "\n";
			}
		}
	}

	//std::cout << "distance: " << sample.distance << " angle: " << sample.angle << " strength: " << sample.signal_strength << "\n";
}
