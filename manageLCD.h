//return the index of the autonomous indicated by the dial.
int getAutonSelectorIndex() {
	int index = 0;
	int autonNamePotRanges[6] = {3500, 2500, 1900, 1000, 300, -300};
	while(SensorValue[autonSelectorPot] < autonNamePotRanges) {
		index++;
	}
	return index;
}

task LCD()
{
	//battery string
	string batteryString = "";
	// auton name variables, must 12 chars at most!!
	char * autonNames[6];
	autonNames[CUBE_MIDFIELD_AUTON] = "cube-midfld";
	autonNames[CUBE_SHORT_AUTON] = "cube-short";
	autonNames[SAME_SIDE_AUTON] = "same-side";
	autonNames[3] = "default-3";
	autonNames[4] = "defalut-4";
	autonNames[5] = "defalut-5";
	char autonOrientationChar;

	// control variables
	int newAutonSelectorIndex;
	int oldAutonSelectorIndex = getAutonSelectorIndex();

	while(true) {
		//display voltage
		clearLCDLine(0);
		batteryString = "";
		sprintf(batteryString, "MA: %1.2%fV, BK: %1.2%fV", nAvgBatteryLevel/1000.0, BackupBatteryLevel/1000.0);
		displayLCDString(0, 0, batteryString);

		// display auton name.
		newAutonSelectorIndex = getAutonSelectorIndex();
		if(oldAutonSelectorIndex != newAutonSelectorIndex) {
			clearLCDLine(1);
			displayLCDCenteredString(1, autonNames[newAutonSelectorIndex]);
		}

		// flip orientation if button pressed. Pauses task execution briefly during button press.
		if(nLCDButtons != 0) {
			autonOrientation = (autonOrientation == RIGHT_ORIENTATION) ? LEFT_ORIENTATION : RIGHT_ORIENTATION;
			waitUntil(nLCDButtons == 0);
		}

		//display orientation
		autonOrientationChar = (autonOrientation == RIGHT_ORIENTATION) ? 'R' : 'L';
		displayLCDChar(0, 13, '<');
		displayLCDChar(0, 14, autonOrientationChar);
		displayLCDChar(0, 15, '>');

		wait1Msec(25);
	}

}
