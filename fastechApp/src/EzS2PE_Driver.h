#include <cstdint>
#include "asynMotorController.h"
#include "asynMotorAxis.h"

#define NUM_EZS2PE_PARAMS 0
#define HEAD_LENGTH   5

#define HEADER        0xAA
#define SET_PARAMETER 0x12
#define SERVO_ENABLE  0x2A
#define STOP          0x31
#define ORIGIN        0x33
#define MOVE          0x34
#define TWEAK         0x35
#define JOG           0x37
#define POLL          0x43

class epicsShareClass EzS2PEAxis : public asynMotorAxis
{
public:
    /* These are the methods we override from the base class */
    EzS2PEAxis(class EzS2PEController *pC, int axis);
    void report(FILE *fp, int level);
    asynStatus move(double position, int relative, double min_velocity, double max_velocity, double acceleration);
    asynStatus moveVelocity(double min_velocity, double max_velocity, double acceleration);
    asynStatus home(double min_velocity, double max_velocity, double acceleration, int forwards);
    asynStatus stop(double acceleration);
    asynStatus poll(bool *moving);
//  asynStatus setPosition(double position);
//  asynStatus setClosedLoop(bool closedLoop);

private:
    EzS2PEController *pC_;  /**< Pointer to the asynMotorController to which this axis belongs.
                             *   Abbreviated because it is used very frequently */
    asynStatus setParameter(uint8_t param, int32_t value);
    asynStatus servoPower(bool power);

friend class EzS2PEController;
};

class epicsShareClass EzS2PEController : public asynMotorController {
public:
    EzS2PEController(const char *portName, const char *EzS2PEPortName, int numAxes, double movingPollPeriod, double idlePollPeriod);

    void report(FILE *fp, int level);
    EzS2PEAxis* getAxis(asynUser *pasynUser);
    EzS2PEAxis* getAxis(int axisNozz);
    asynStatus writeReadController(const char *output, char *input, size_t maxChars, size_t *nread, double timeout);

private:
    uint8_t syncCounter;
    asynStatus writeReadFrame(uint8_t length, uint8_t frameType, uint8_t *payload);

friend class EzS2PEAxis;
};

