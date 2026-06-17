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

LIBRARY_IOC += Fastech

# xxxRecord.h will be created from xxxRecord.dbd
#DBDINC += xxxRecord
# install Fastech.dbd into <top>/dbd
DBD += Fastech.dbd

# specify all source files to be compiled and added to the library
#Fastech_SRCS += xxx

Fastech_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

