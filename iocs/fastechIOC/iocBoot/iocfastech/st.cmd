#!../../bin/linux-x86_64/fastech

#- SPDX-FileCopyrightText: 2003 Argonne National Laboratory
#-
#- SPDX-License-Identifier: EPICS

#- You may have to change fastech to something else
#- everywhere it appears in this file

< envPaths

cd "${TOP}"

## Register all support components
dbLoadDatabase "dbd/fastech.dbd"
fastech_registerRecordDeviceDriver pdbbase

drvAsynIPPortConfigure("L0", "192.168.0.204:3001 UDP", 0, 0, 1)
EzS2PECreateController("motorExit", "L0", 1, 50, 1000, 0)

## Load record instances
dbLoadRecords("db/motor.db","SYS=EXIT, SUB=zpos")

cd "${TOP}/iocBoot/${IOC}"

asynSetTraceIOMask("L0", 0, 2)
asynSetTraceMask("L0", 0, 9)
asynSetTraceIOMask("motorExit", 0, 2)
asynSetTraceMask("motorExit", 0, 255)

iocInit

## Start any sequence programs
#seq sncxxx,"user=rea"
