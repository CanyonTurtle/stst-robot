// ======= userControl.h =======
// this function gives the driver controls.
// =============================

//start the user's controls, this composes the drive, lift, and claw control.
void runUserControl()
{
	// drive variables
	int joystickSmallTolerance = 10;
	int leftDrivePower;
	int rightDrivePower;

	// lift variables
	int liftEncoderTarget = SensorValue[LiftEncoder];

	while(true){
		// driving control
		leftDrivePower = vexRT(Ch3);
		rightDrivePower = vexRT(Ch2);
		leftDrivePower = (abs(leftDrivePower) < joystickSmallTolerance) ? leftDrivePower : 0;
		rightDrivePower = (abs(rightDrivePower) < joystickSmallTolerance) ? rightDrivePower : 0;
		setDrivePower(leftDrivePower, rightDrivePower);

		// lift control, which siphons to tasks if needed.
		if(vexRT(Btn6U) == 1) {
			setLiftPower(127);
			liftEncoderTarget = SensorValue[LiftEncoder];
		}
		else if (vexRT(Btn6D) == 1) {
			setLiftPower(-127);
			liftEncoderTarget = SensorValue[LiftEncoder];
		}
		else {
			//setLiftPower(0);
			moveLiftTowardsEncoderTarget(liftEncoderTarget);
		}

		// claw control
		if(vexRT(Btn5U) == 1) {
			setClawPower(127);
		}
		else if (vexRT(Btn5D) == 1) {
			setClawPower(-127);
		}
		else {
			setClawPower(0);
		}

		wait1Msec(25);
	}
}
