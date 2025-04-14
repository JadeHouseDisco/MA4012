#pragma config(Motor,  port2,           rollerMotor,   tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port3,           leftMotor,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           rightMotor,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           releaseMotor,    tmotorServoContinuousRotation, openLoop)
#pragma config(Sensor, in1,    frontDistanceSensor,        sensorAnalog)
#pragma config(Sensor, in2,    backDistanceSensor,        sensorAnalog)
#pragma config(Sensor, in3,    enemyDistanceSensor,        sensorAnalog)
#pragma config(Sensor, in4,    angledDistanceSensor,        sensorAnalog)
#pragma config(Sensor, dgtl1, pickUpSensor, sensorDigitalIn)
#pragma config(Sensor, dgtl2, backLeftSensor, sensorDigitalIn)
#pragma config(Sensor, dgtl3, frontRightSensor, sensorDigitalIn)
#pragma config(Sensor, dgtl4, frontLeftSensor, sensorDigitalIn)
#pragma config(Sensor, dgtl5, backRightSensor, sensorDigitalIn)
#pragma config(Sensor, dgtl7,  northSensor,      sensorDigitalIn)
#pragma config(Sensor, dgtl8,  eastSensor,      sensorDigitalIn)
#pragma config(Sensor, dgtl9,  southSensor,      sensorDigitalIn)
#pragma config(Sensor, dgtl10,  westSensor,      sensorDigitalIn)
#pragma config(Sensor, dgtl11, rampSensor, sensorDigitalIn)
#pragma config(Sensor, dgtl6, backLeftBumperSensor, sensorDigitalIn)
#pragma config(Sensor, dgtl12, backRightBumperSensor, sensorDigitalIn)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

int frontDistance;
int backDistance;
int enemyDistance;
int angledDistance;
int ramp = 1;
int pickUp = 1;
int backLeft;
int frontRight;
int frontLeft;
int backRight;
int north;
int east;
int south;
int west;
int freshRun = 1;
int backLeftBumper;
int backRightBumper;
bool returnMode = false;
float distanceThresh;
int repositionCount = 1;
bool atLeft = true;

void releaseBall() {
	motor[leftMotor] = 0;
	motor[rightMotor] = 0;
	motor[releaseMotor] = -60;
	wait1Msec(1250);
	while (pickUp == 0){
		pickUp = SensorValue[pickUpSensor];
		motor[releaseMotor] = 0;
	}
	motor[releaseMotor] = 60;
	wait1Msec(1250);
	motor[releaseMotor] = 0;
}

void moveForward(int speed) {
	motor[leftMotor] = -speed - 7;
	motor[rightMotor] = speed;
    clearTimer(T4);
}

void moveBackward(int speed) {
	motor[leftMotor] = speed + 7;
	motor[rightMotor] = -speed;
	clearTimer(T2);
}

void rotateAntiClockwise(int speed) {
	motor[leftMotor] = speed;
	motor[rightMotor] = speed;
	clearTimer(T2);
    clearTimer(T4);
}

void rotateClockwise(int speed) {
	motor[leftMotor] = -speed;
	motor[rightMotor] = -speed;
	clearTimer(T2);
	clearTimer(T4);
}

void stopMovement() {
	motor[leftMotor] = 0;
	motor[rightMotor] = 0;
	clearTimer(T2);
    clearTimer(T4);
}

void reorient(int speed) {
	north = SensorValue(northSensor);
	east = SensorValue(eastSensor);
	west = SensorValue(westSensor);
	south = SensorValue(southSensor);
	if (((north == 1) && (east == 1)  && (south == 0) && (west == 1))
		|| ((north == 1) && (east == 0)  && (south == 0) && (west == 1))
		|| ((north == 1) && (east == 0)  && (south == 1) && (west == 1))
		|| ((north == 0) && (east == 0)  && (south == 1) && (west == 1))) {
		while (true) {
			north = SensorValue(northSensor);
			east = SensorValue(eastSensor);
			west = SensorValue(westSensor);
			south = SensorValue(southSensor);
			if ((north == 1) && (east == 1)  && (south == 0) && (west == 0)) {
				break;
			}
			else {
				rotateClockwise(speed);
			}
		}
		moveBackward(60);
	}
	else {
		while (true) {
			north = SensorValue(northSensor);
			east = SensorValue(eastSensor);
			west = SensorValue(westSensor);
			south = SensorValue(southSensor);
			if ((north == 1) && (east == 1)  && (south == 0) && (west == 1)) {
				break;
			}
			else {
				rotateAntiClockwise(speed);
			}
		}
		moveBackward(60);
	}
}

task main() {
	while (true) {

		if (freshRun == 1) {
			clearTimer(T1);
			freshRun = 0;
			motor[rollerMotor] = 127;
			moveForward(60);
			while (time1[T1] < 3000) {
				backLeft = SensorValue(backLeftSensor);
				frontRight = SensorValue(frontRightSensor);
				frontLeft = SensorValue(frontLeftSensor);
				backRight = SensorValue(backRightSensor);
				pickUp = SensorValue[pickUpSensor];
				if (frontLeft == 0 || frontRight == 0) {
					moveBackward(60);
					wait1Msec(500);
					reorient(45);
				}
				else if (pickUp == 0) {
					returnMode = true;
					reorient(45);
					break;
				}
				else {
					moveForward(60);
				}
			}
			clearTimer(T2); // for infinite wall pushing
			clearTimer(T3); // for infinite searching
      clearTimer(T4); // for the infininte returningmode
		}

		frontDistance = SensorValue(frontDistanceSensor);
		backDistance = SensorValue(backDistanceSensor);
		enemyDistance = SensorValue(enemyDistanceSensor);
		angledDistance = SensorValue(angledDistanceSensor);
		backLeft = SensorValue(backLeftSensor);
		frontRight = SensorValue(frontRightSensor);
		frontLeft = SensorValue(frontLeftSensor);
		backRight = SensorValue(backRightSensor);

		//Convert analog value to distance(m)
		if (frontDistance > 0) {
			frontDistance = 1 / ((frontDistance + 149.1) / 24339);
		}
		else {
			frontDistance = 100.0;
		}

		//Convert analog value to distance(m)
		if (angledDistance > 0) {
			angledDistance = 1 / ((angledDistance + 149.1) / 24339);
		}
		else {
			angledDistance = 100.0;
		}

		if (enemyDistance > 200) {
			enemyDistance = 1 / ((enemyDistance + 149.1) / 24339);
		}
		else {
			enemyDistance = 100.0;
		}

		north = SensorValue(northSensor);
		if (north == 0) {
			distanceThresh = 35.0;
		}
		else {
			distanceThresh = 70.0;
		}

		if (time1[T3] > 20000) {
			reorient(45);
			if (repositionCount % 2 == 1) {
				moveBackward(60);
				clearTimer(T3);
				while (time1[T3] < 1000) {
					backLeft = SensorValue(backLeftSensor);
					backRight = SensorValue(backRightSensor);
					pickUp = SensorValue[pickUpSensor];
					if (backLeft == 0 || backRight == 0) {
						moveForward(60);
						wait1Msec(500);
						reorient(45);
					}
					else if (pickUp == 0) {
					reorient(45);
					returnMode = true;
						break;
					}
					else {
						moveBackward(60);
					}
				}
				clearTimer(T3);
			}
			else {
				moveForward(60);
				clearTimer(T3);
				while (time1[T3] < 1000) {
					frontRight = SensorValue(frontRightSensor);
					frontLeft = SensorValue(frontLeftSensor);
					pickUp = SensorValue[pickUpSensor];
					if (frontLeft == 0 || frontRight == 0) {
						moveBackward(60);
						wait1Msec(500);
						reorient(45);
					}
					else if (pickUp == 0) {
					reorient(45);
					returnMode = true;
						break;
					}
					else {
						moveForward(60);
					}
				}
				clearTimer(T3);
			}
		}
		else if (frontLeft == 0 || frontRight == 0) {
			moveBackward(60);
			wait1Msec(500);
		}
		else if (backLeft == 0 || backRight == 0) {
			if (time1[T4] > 5000) {
				moveForward(60);
				wait1Msec(500);
				reorient(45);
				clearTimer(T1);
				while (time1[T1] < 1500) {
					backLeft = SensorValue(backLeftSensor);
					frontRight = SensorValue(frontRightSensor);
					frontLeft = SensorValue(frontLeftSensor);
					backRight = SensorValue(backRightSensor);
					pickUp = SensorValue[pickUpSensor];
					if (frontLeft == 0 || frontRight == 0) {
						moveBackward(60);
						wait1Msec(500);
						reorient(45);
					}
					else {
						moveForward(60);
					}
				}
				reorient(45);
				moveBackward(60);
				clearTimer(T3);
				clearTimer(T4);
      		}
			else if (pickUp == 0) {
				wait1Msec(300);
				backLeftBumper = SensorValue(backLeftBumperSensor);
				backRightBumper = SensorValue(backRightBumperSensor);
				if ((backLeft == 0) && (backRight == 0) && (backLeftBumper == 0) && (backRightBumper == 0)) {
					releaseBall();
					freshRun = 1;
					ramp = 1;
					returnMode = false;
					clearTimer(T3);
                    clearTimer(T4);
				}
				else if ((backLeftBumper == 0) || (backRightBumper == 0)) {
					moveBackward(40);
				}
				else {
					moveForward(60);
					wait1Msec(800);
					reorient(45);
				}
			}
			else {
				moveForward(60);
				wait1Msec(500);
			}
		}
		else if (pickUp == 0 && returnMode == false) {
			moveBackward(60);
			returnMode = true;
			clearTimer(T3);
		}
		else if (pickUp == 1) {
			if (frontDistance < distanceThresh && enemyDistance >= 55.0) {
				if (angledDistance < 35.0) {
					rotateAntiClockwise(40);
					wait1Msec(100);
					while (true) {
						frontRight = SensorValue(frontRightSensor);
						frontLeft = SensorValue(frontLeftSensor);
						if (ramp == 0) {
							stopMovement();
							pickUp = SensorValue[pickUpSensor];
							if (pickUp == 0) {
								reorient(45);
								moveBackward(60);
								returnMode = true;
								clearTimer(T3);
                clearTimer(T4);
								break;
							}
							else if (SensorValue[rampSensor] == 1) {
								wait1Msec(400);
								motor[rollerMotor] = -127;
							}
						}
						else if (frontRight == 0 || frontLeft == 0) {
							moveBackward(60);
							wait1Msec(500);
							break;
						}
						else {
							moveForward(60);
							ramp = SensorValue(rampSensor);
							if (time1[T2] > 5000) {
								moveBackward(60);
								wait1Msec(500);
								reorient(45);
								clearTimer(T2);
								while (time1[T2] < 2500) {
									backLeft = SensorValue(backLeftSensor);
									frontRight = SensorValue(frontRightSensor);
									frontLeft = SensorValue(frontLeftSensor);
									backRight = SensorValue(backRightSensor);
									if (frontLeft == 0 || frontRight == 0) {
										moveBackward(60);
										wait1Msec(500);
										reorient(45);
									}
									else {
										moveForward(60);
									}
									pickUp = SensorValue[pickUpSensor];
								}
								clearTimer(T2);
								clearTimer(T3);
								break;
							}
						}
					}
				}
				else {
					moveForward(60);
				}
			}
			else {
				pickUp = SensorValue[pickUpSensor];
				if (atLeft) {
					rotateClockwise(40);
				}
				else {
					rotateAntiClockwise(40);
				}
			}
		}
		else {
			clearTimer(T3);
			if (time1[T4] > 5000) {
				moveForward(60);
				wait1Msec(500);
				reorient(45);
				clearTimer(T4);
				while (time1[T4] < 1500) {
					backLeft = SensorValue(backLeftSensor);
					frontRight = SensorValue(frontRightSensor);
					frontLeft = SensorValue(frontLeftSensor);
					backRight = SensorValue(backRightSensor);
					pickUp = SensorValue[pickUpSensor];
					if (frontLeft == 0 || frontRight == 0) {
						moveBackward(60);
						wait1Msec(500);
						reorient(45);
					}
					else {
						moveForward(60);
					}
				}
				reorient(45);
				moveBackward(60);
				clearTimer(T3);
				clearTimer(T4);
			}
			else {
				moveBackward(60);
			}
		}

		writeDebugStreamLine("frontDistance : %d", frontDistance);
		writeDebugStreamLine("angledDistance: %d", angledDistance);
		clearDebugStream();
	}
}
