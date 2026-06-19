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

  syncCounter = 0;

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

asynStatus EzS2PEAxis::sendAccel(double accel64){
  asynStatus status;
  float accel32 = (float)accel64;
  //set forward acceleration
  syncCounter++;
  }
