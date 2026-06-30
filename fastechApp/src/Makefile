# SPDX-FileCopyrightText: 2003 Argonne National Laboratory
#
# SPDX-License-Identifier: EPICS

TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# build a support library

LIBRARY_IOC += fastechMotor

# xxxRecord.h will be created from xxxRecord.dbd
#DBDINC += xxxRecord
# install fastech.dbd into <top>/dbd
DBD += devEzS2PE.dbd

INC += EzS2PE_Driver.h

# specify all source files to be compiled and added to the library
#fastech_SRCS += xxx
fastechMotor_SRCS += EzS2PE_Driver.cpp

fastechMotor_LIBS += motor asyn
fastechMotor_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
