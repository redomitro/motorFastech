#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdfloat>

#include <iocsh.h>
#include <epicsThread.h>

#include <asynOctetSyncIO.h>

#include <epicsExport.h>
#include "EzS2PE_Driver.h"

#define NINT(f) (int)((f)>0 ? (f)+0.5 : (f)-0.5)

EzS2PEController::EzS2PEController(const char *portName, const char *EzS2PEPortName, int numAxes, 
                                 double movingPollPeriod, double idlePollPeriod)
  :  asynMotorController(portName, numAxes, NUM_EZS2PE_PARAMS,
                         0, // No additional interfaces beyond those in base class
                         0, // No additional callback interfaces beyond those in base class
                         ASYN_CANBLOCK | ASYN_MULTIDEVICE,
                         1, // autoconnect
                         0, 0)  // Default priority and stack size
{
  int axis;
  asynStatus status;
  EzS2PEAxis *pAxis;
  static const char *functionName = "EzS2PEController::EzS2PEController";

  /* Connect to EzS2PE controller */
  status = pasynOctetSyncIO->connect(EzS2PEPortName, 0, &pasynUserController_, NULL);
  if (status) {
    asynPrint(this->pasynUserSelf, ASYN_TRACE_ERROR, 
      "%s: cannot connect to EziS2-PE controller\n",
      functionName);
  }
  for (axis=0; axis<numAxes; axis++) {
    pAxis = new EzS2PEAxis(this, axis);
  }
  
  syncCounter = 0; //start the idempotence counter

  startPoller(movingPollPeriod, idlePollPeriod, 2);
}

/** Creates a new EzS2PEController object.
  * Configuration command, called directly or from iocsh
  * \param[in] portName          The name of the asyn port that will be created for this driver
  * \param[in] EzS2PEPortName    The name of the drvAsynIPPPort that was created previously to connect to the EzS2PE controller 
  * \param[in] numAxes           The number of axes that this controller supports
  * \param[in] movingPollPeriod  The time in ms between polls when any axis is moving
  * \param[in] idlePollPeriod    The time in ms between polls when no axis is moving
  */

extern "C" int EzS2PECreateController(const char *portName, const char *EzS2PEPortName, int numAxes, 
                                    int movingPollPeriod, int idlePollPeriod)
{
  EzS2PEController *pEzS2PEController
    = new EzS2PEController(portName, EzS2PEPortName, numAxes, movingPollPeriod/1000., idlePollPeriod/1000.);
  pEzS2PEController = NULL;

  return(asynSuccess);
}

/** Reports on status of the driver
  * \param[in] fp The file pointer on which report information will be written
  * \param[in] level The level of report detail desired
  *
  * If details > 0 then information is printed about each axis.
  * After printing controller-specific information it calls asynMotorController::report()
  */

void EzS2PEController::report(FILE *fp, int level)
{
  fprintf(fp, "EziS2-PE motor driver %s, numAxes=%d, moving poll period=%f, idle poll period=%f\n", 
  (double min_velocity, double max_velocity, double acceleration);
  this->portName, numAxes_, movingPollPeriod_, idlePollPeriod_);

  // Call the base class method
  asynMotorController::report(fp, level);
}

/** Returns a pointer to an EzS2PEAxis object.
  * Returns NULL if the axis number encoded in pasynUser is invalid.
  * \param[in] pasynUser asynUser structure that encodes the axis index number. */

EzS2PEAxis* EzS2PEController::getAxis(asynUser *pasynUser)
{
  return static_cast<EzS2PEAxis*>(asynMotorController::getAxis(pasynUser));
}

/** Returns a pointer to an EzS2PEAxis object.
  * Returns NULL if the axis number encoded in pasynUser is invalid.
  * \param[in] axisNo Axis index number. */

EzS2PEAxis* EzS2PEController::getAxis(int axisNo)
{
  return static_cast<EzS2PEAxis*>(asynMotorController::getAxis(axisNo));
}

// These are the EzS2PEAxis methods

/** Creates a new EzS2PEAxis object.
  * \param[in] pC Pointer to the EzS2PEController to which this axis belongs. 
  * \param[in] axisNo Index number of this axis, range 0 to pC->numAxes_-1.
  * 
  * Initializes register numbers, etc.
  */

EzS2PEAxis::EzS2PEAxis(EzS2PEController *pC, int axisNo)
  : asynMotorAxis(pC, axisNo),
    pC_(pC)
{  
}

/** Reports on status of the axis
  * \param[in] fp The file pointer on which report information will be written
  * \param[in] level The level of report detail desired
  *
  * After printing device-specific information calls asynMotorAxis::report()
  */

void EzS2PEAxis::report(FILE *fp, int level)
{
  if (level > 0) {
    fprintf(fp, "  axis %d\n",
            axisNo_);
  }

  // Call the base class method
  asynMotorAxis::report(fp, level);
}

/* Set acceleration for driver. Currently commented out because motor
 * expects integer acceleration in pps and asynMotor calls for float
 *

asynStatus EzS2PEAxis::sendAccel(double accel64){
  asynStatus status;
  float accel32 = (float)accel64;
  //set forward acceleration
  pC->syncCounter++;
  }
*/


asynStatus EzS2PEAxis::move(double position, int relative, double minVelocity, double maxVelocity, double acceleraton){
	//move to absolute position (RVAL) or relative position (TWF/TWR)
	
  asynStatus status;
  pC_->syncCounter++;

  unsigned char boiler[]={HEADER, 11, pC_->syncCounter, 0x00, 0x00}; //boilerplate string
  unsigned char buffer[256]; //placeholder for outstring

  if(relative){
    boiler[4] = TWEAK;
  } else {
    boiler[4] = MOVE;
  }

  int pos = NINT(position);
  int vel = NINT(maxVelocity);

  //build the outstring
  unsigned char* ptr = buffer;
  
  memcpy(ptr, boiler, sizeof(boiler)); //message header
  ptr += sizeof(boiler);
  
  memcpy(ptr, &pos, sizeof(int)); //position
  ptr += sizeof(int);

  memcpy(ptr, &vel, sizeof(int)); //speed
  ptr += sizeof(int);

	//write the outstring to controller
  memcpy(&pC_->outString_, buffer, 2+buffer[1]);
  status = pC_->writeReadController();
  
  return status;
}

asynStatus EzS2PEAxis::moveVelocity(double minVelocity, double maxVelocity, double acceleration){
	//jogging motion
	
	asynStatus status;
	pC_->syncCounter++;

	unsigned char boiler[]={HEADER, 8, pC_->syncCounter, 0x00, JOG};
	unsigned char buffer[256];

	int vel = NINT(abs(maxVelocity));

  //build the outstring
  unsigned char* ptr = buffer;
 
  memcpy(ptr, boiler, sizeof(boiler)); //message header
  ptr += sizeof(boiler);
  
  memcpy(ptr, &vel, sizeof(int)); //speed
  ptr += sizeof(int);

	if(maxVelocity > 0){ //direction
		*ptr = 1; //forward jog
	} else {
		*ptr = 0; //reverse jog
	}
	ptr++;

	//write the outstring to controller
  memcpy(&pC_->outString_, buffer, 2+buffer[1]);
  status = pC_->writeReadController();

  return status;
}

asynStatus EzS2PEAxis::home(double minVelocity, double maxVelocity, double acceleration, int forwards){
	/*	go to hardware home switch - motor will move at 5000 pps by default
		and forwards until it finds the limit or the origin, if it finds the limit first it will go back

		todo: implement custom speed
	*/
	
	asynStatus status;
	pC_->syncCounter++;

	unsigned char boiler[]={HEADER, 3, pC_->syncCounter, 0x00, ORIGIN};
	unsigned char buffer[256];

  //build the outstring
  unsigned char* ptr = buffer;
 
  memcpy(ptr, boiler, sizeof(boiler)); //message header
  ptr += sizeof(boiler);

  /*
  memcpy(ptr, &vel, sizeof(int)); //speed
  ptr += sizeof(int);
	*/
	
	//write the outstring to controller
  memcpy(&pC_->outString_, buffer, 2+buffer[1]);
  status = pC_->writeReadController();

  return status;
}

asynStatus EzS2PEAxis::stop(double acceleration){
	//stop the motor

	asynStatus status;
	pC_->syncCounter++;

	unsigned char boiler[]={HEADER, 3, pC_->syncCounter, 0x00, STOP};
	unsigned char buffer[256];

  //build the outstring
  unsigned char* ptr = buffer;
 
  memcpy(ptr, boiler, sizeof(boiler)); //message header
  ptr += sizeof(boiler);

  /*
  memcpy(ptr, &vel, sizeof(int)); //speed
  ptr += sizeof(int);
	*/
	
	//write the outstring to controller
  memcpy(&pC_->outString_, buffer, 2+buffer[1]);
  status = pC_->writeReadController();

  return status;
}
