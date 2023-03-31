
/*
    Name:       onlyRecording.ino
    Created:	4/5/2019 12:19:26 PM
    Author:     Justin Aronson

	!!!!PLEASE SAVE ALTERNATE VERSION OF THIS IF YOU MAKE ANY CHANGES!!!!
	 -----------------------------------------------------------------------------------------------
	 |    Script will wait for the trigger in from Spike, then simply record the time and position |
	 |    data until the trialTime is >= the totalTrialTime, in which case it will reset and wait  |
	 |    for the next trial to begin.                                                             |
	 -----------------------------------------------------------------------------------------------
*/


#include<Encoder.h>

Encoder enc(2, 3);

unsigned long totalTrialTime = 332000;
bool verbose = true; //Change this if you don't want to print the "Waiting for trigger..." message

long trialStartPos = 0;
unsigned long trialStartTime = 0;
unsigned long trialTime = 0;
long trialPos = 0;
int trialNum = 0;
bool run = false;
bool first = true;

unsigned long tSince(unsigned long t0) {
	return (unsigned long)millis() - t0;
}

long distMoved(long d0) {
	return (long)enc.read() - d0;
}


void printData() {
	//This function prints the data each time it is called in format:
	// Trial number,time,position,Number of rewards given

	trialTime = tSince(trialStartTime);
	trialPos = distMoved(trialStartPos);
	Serial.print(trialNum);
	Serial.print(",");
	Serial.print(trialTime);
	Serial.print(",");
	Serial.println(trialPos);


}
// The setup() function runs once each time the micro-controller starts
void setup()
{
	Serial.begin(9600);
	pinMode(A0, INPUT);

}

// Add the main program code into the continuous loop() function
void loop()
{
	if (first == true) {
		first = false;
		if (verbose) {
			Serial.println("Waiting for trigger...");
		}
	}

	if (digitalRead(A0) == HIGH) {
		run = true;
   trialNum = trialNum + 1;
		first = true;
	}

	while (run) {
		if (first = true) {
			trialStartTime = millis();
			trialStartPos = enc.read();
			first = false;
		}

		printData();

		while (trialTime < totalTrialTime) {
			printData();
		}

		if (trialTime >= totalTrialTime) {
			run = false;
			first = true;
			delay(5000);
		}
	}

}
