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
EzS2PECreateController("motorExit", "L0", 1, 10, 1000, 0)

## Load record instances
dbLoadRecords("db/motor.db","SYS=EXIT, SUB=zpos, DESC=z-axis control, CONT=motorExit, AXIS=0, MRES=2e-4, TWV=2")

cd "${TOP}/iocBoot/${IOC}"

asynSetTraceIOMask("L0", 0, 4)
asynSetTraceMask("L0", 0, 0x12)
asynSetTraceIOMask("motorExit", 0, 4)
asynSetTraceMask("motorExit", 0, 0x12)

iocInit

## Start any sequence programs
#seq sncxxx,"user=rea"
