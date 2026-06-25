# SPDX-FileCopyrightText: 2003 Argonne National Laboratory
#
# SPDX-License-Identifier: EPICS

TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = fastech
# fastech.dbd will be created and installed
DBD += fastech.dbd

# fastech.dbd will be made up from these files:
fastech_DBD += base.dbd
fastech_DBD += asyn.dbd
fastech_DBD += drvAsynIPPort.dbd
fastech_DBD += motorSupport.dbd
fastech_DBD += devEzS2PE.dbd

# Include dbd files from all support applications:
#fastech_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#fastech_LIBS += xxx
fastech_LIBS += asyn motor fastechMotor

# fastech_registerRecordDeviceDriver.cpp derives from fastech.dbd
fastech_SRCS += fastech_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
fastech_SRCS_DEFAULT += fastechMain.cpp
fastech_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#fastech_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
fastech_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

