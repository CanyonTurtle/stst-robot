/* ======== robotAPI.h ========
the purpose of this file is to provide subsystem movement functions.
ex: powering drive, lift, ect.
*/

// top function declarations.
void setDrivePower(int leftPower, int rightPower);
void setLiftPower(int power);
void setClawPower(int power);
void moveLiftTowardsEncoderTarget(int encoderTarget);
void holdLiftAtEncoderTarget(int encoderTarget);

// low-level way to set the drive motors.
void setDrivePower(int leftPower, int rightPower) {
		motor[LeftDrive1] = leftPower;
		motor[LeftDrive2] = leftPower;
		motor[RightDrive1] = rightPower;
		motor[RightDrive2] = rightPower;
}

// low-level way to power the lift
void setLiftPower(int power) {
	motor[LeftNotY] = power;
	motor[LeftY] = power;
	motor[RightNotY] = power;
	motor[RightY] = power;
}

// easiest way to move the claws.
void setClawPower(int power) {
	motor[Claw1] = power;
	motor[Claw2] = power;
}

// used for correcting lift speed to go towards a lift angle.
void moveLiftTowardsEncoderTarget(int encoderTarget) {
	int error = SensorValue[LiftEncoder] - encoderTarget;

	// do not power at all, if lift error is small, or if the lift lowered.
	if(abs(error) < 2 || SensorValue[LiftEncoder] < 15){
		error = 0;
	}
	int power = -3 * error;// + pastError;
	setLiftPower(power);
}

// raises the lift to a specific angle using an encoder target to adjust to.
void holdLiftAtEncoderTarget(int encoderTarget) {
	moveLiftTowardsEncoderTarget(encoderTarget);
	wait1Msec(25);
}

// move drivebase forward until encoder delta is reached.
void moveDriveByEncoderChange(int power, int encoderChange) {
	int drivePower = (encoderChange > 0) ? power : -1 * power;
	int initialEncoderValue = SensorValue[RightEncoder];
	int targetEncoderValue = initialEncoderValue + encoderChange;

	setDrivePower(drivePower, drivePower);

	// go until error less than tolerance, scenarios for front or backwards.
	if(encoderChange > 0) {
		waitUntil(targetEncoderValue - SensorValue[RightEncoder] < 0);
	}
	else {
		waitUntil(targetEncoderValue - SensorValue[RightEncoder] > 0);
	}
	setDrivePower(0, 0);
}

// timebased turning as temporary substitute for encoder-based turn functions.
void turnDriveByTime(int power, int timeInMsec) {
	if(autonOrientation == LEFT_ORIENTATION) {
		power = -1 * power;
	}
	setDrivePower(power, -1 * power);
	wait1Msec(timeInMsec);
	setDrivePower(0,0);
}

// turn, input angle in tenths of a degree, angle is always positive, dir either CLOCKWISE or COUTNERCLOCKWISE
void turnByGyro(int power, unsigned int angleChange, int dir) {
	bool flippedAngle = (dir == CLOCKWISE && autonOrientation == RIGHT_ORIENTATION) ? true : false;
	int initialAngle = SensorValue[gyro];
	angleChange = (flippedAngle) ? -1 * angleChange : angleChange;
	int targetAngle = initialAngle + angleChange;

	int drivePower = (autonOrientation == LEFT_ORIENTATION) ? -1 * power : power;

	//ramp up drive speed to reduce gyro drift
	setDrivePower(0.5 * drivePower, -0.5 * drivePower);
	wait10Msec(30);
	setDrivePower(drivePower, -1 * drivePower);

	if(flippedAngle) {
		waitUntil(targetAngle - SensorValue(gyro) > 0);
	}
	else {
		waitUntil(targetAngle - SensorValue(gyro) < 0);
	}
	setDrivePower(-0.5 * drivePower, 0.5 * drivePower);
	wait10Msec(10);
	setDrivePower(0,0);
}
