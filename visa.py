# -*- coding: utf-8 -*-

# Python for Test and Measurement

# Requires VISA installed on controlling PC, 'http://pyvisa.sourceforge.net/pyvisa/'

# Keysight IO Libraries 18.1.22x 32-Bit Keysight VISA (as primary)

# Anaconda Python 4.4.0 32 bit

# pyvisa 3.6.x

# Keysight N9952A 50GHz FieldFox Handheld portable combination analyzer

# running A.10.17 application code

##"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

## Copyright © 2018 Keysight Technologies Inc. All rights reserved.

##

## You have a royalty-free right to use, modify, reproduce and distribute this

## example / files (and/or any modified version) in any way you find useful, provided

## that you agree that Keysight has no warranty, obligations or liability for any

## Sample Application / Files.

##

##"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

# Example Description:


# A python sample program utilizing pyvisa to connect and control a Keysight FieldFox

# Family Combination Analyzer.

#

# The application performs the following:


#   Imports the pyvisa libraries and operating system dependent functionality;

#   Establishes a visa resource manager;

#   Opens a connection to the FieldFox based on the instrument's VISA address as acquired via Keysight Connection Expert

#   Sets the visa time out (increasing the timeout as compared to the default).

#   Clears the event status register and thus clears the error queue;

#   Defines an error check function and checks the system error queue;

#   Presets the FieldFox unit; performs a *IDN?, sets the analyzer to Spectrum Analyzer mode,

#   then queries the number of points, start frequency and stop frequency.

#   Executes a synchronized single sweep.

#   Queries the spectrum analyzer trace data, builds a linear array to compute the stimulus array,

#   and plots the stimulus - response data as an X-Y trace.

#

# Import the visa libraries

import visa

import os

# The numpy is imported as it is helpful for a linear ramp creation for the stimulus array

import numpy as npStimulusArray

# import module for plotting

import matplotlib.pyplot as stimulusResponsePlot

# A variable to control various events and testing during development.

# by uncommenting the #debug True line, debug will occur, for efficiency, during development.

debug = False

# debug = True

print
"Debug flag set to " + str(debug)

# Set variables for ease of change - assumes 'debug is true.

# If debug is set to false then Spectrum Analyzer preset defaults for

# start frequency, stop frequency and number of points are utilized.

numPoints = 21

startFreq = 1.28579E9

stopFreq = 2.28579E9

# Open a VISA resource manager pointing to the installation folder for the Keysight Visa libraries.

rm = visa.ResourceManager('C:\\Program Files (x86)\\IVI Foundation\\VISA\\WinNT\\agvisa\\agbin\\visa32.dll')

# Based on the resource manager, open a session to a specific VISA resource string as provided via

# Keysight Connection Expert

# ALTER LINE BELOW - Updated VISA resource string to match your specific configuration

myFieldFox = rm.open_resource("TCPIP0::156.140.157.162::inst0::INSTR")

# Set Timeout - 10 seconds

myFieldFox.timeout = 10000

# Clear the event status registers and empty the error queue

myFieldFox.write("*CLS")

# Query identification string *IDN?

myFieldFox.write("*IDN?")

print(myFieldFox.read())


# Define Error Check Function

def Errcheck():
    myError = []

    ErrorList = myFieldFox.query("SYST:ERR?").split(',')

    Error = ErrorList[0]

    if int(Error) == 0:

        print("+0, No Error!")

    else:

        while int(Error) != 0:
            print("Error #: " + ErrorList[0])

            print("Error Description: " + ErrorList[1])

            myError.append(ErrorList[0])

            myError.append(ErrorList[1])

            ErrorList = myFieldFox.query("SYST:ERR?").split(',')

            Error = ErrorList[0]

            myError = list(myError)

    return myError


# Call and print error check results

print(Errcheck())

# Preset the FieldFox and wait for operation complete via the *OPC?, i.e.

# the operation complete query.

myFieldFox.write("SYST:PRES;*OPC?")

print
"Preset complete, *OPC? returned : " + myFieldFox.read()

# Set mode to Spectrum Analyzer and wait for operation complete via the *OPC?, i.e.

# the operation complete query.

myFieldFox.write("INST:SEL 'SA';*OPC?")

myFieldFox.read()

# If debug is true then user setting of start frequency, stop frequency and number of points

if debug:
    myFieldFox.write("SENS:SWE:POIN " + str(numPoints))

    myFieldFox.write("SENS:FREQ:START " + str(startFreq))

    myFieldFox.write("SENS:FREQ:STOP " + str(stopFreq))

# Determine, i.e. query, number of points in trace for ASCII transfer - query

myFieldFox.write("SENS:SWE:POIN?")

numPoints = myFieldFox.read()

print
"Number of trace points " + numPoints

# Determine, i.e. query, start and stop frequencies, i.e. stimulus begin and end points

myFieldFox.write("SENS:FREQ:START?")

startFreq = myFieldFox.read()

myFieldFox.write("SENS:FREQ:STOP?")

stopFreq = myFieldFox.read()

print
"FieldFox start frequency = " + startFreq + " stop frequency = " + stopFreq

# Set trigger mode to hold for trigger synchronization

myFieldFox.write("INIT:CONT OFF;*OPC?")

myFieldFox.read()

# Use of Python numpy import to comupte linear step size of stimulus array

# based on query of the start frequency - stop frequency and number of points.

# 'Other' analyzers support a SCPI "SENSe:X?" query which will provide the stimulus

#  array as a SCPI query.

stimulusArray = npStimulusArray.linspace(float(startFreq), float(stopFreq), int(numPoints))

print
stimulusArray

# Assert a single trigger and wait for trigger complete via *OPC? output of a 1

myFieldFox.write("INIT:IMM;*OPC?")

print
"Single Trigger complete, *OPC? returned : " + myFieldFox.read()

# Query the FieldFox response data

myFieldFox.write("TRACE:DATA?")

ff_SA_Trace_Data = myFieldFox.read()

print
ff_SA_Trace_Data  # This is one long comma separated string list of values.

# Use split to turn long string to an array of values

ff_SA_Trace_Data_Array = ff_SA_Trace_Data.split(",")

# Now plot the x - y data

maxResponseVal = max(ff_SA_Trace_Data_Array)

minResponseVal = min(ff_SA_Trace_Data_Array)

# if debug:

print
"Max value = " + maxResponseVal + " Min Value = " + minResponseVal

stimulusResponsePlot.title("Keysight FieldFox Spectrum Trace Data via Python - PyVisa - SCPI")

stimulusResponsePlot.xlabel("Frequency")

stimulusResponsePlot.ylabel("Amplitude (dBm)")

stimulusResponsePlot.plot(stimulusArray, ff_SA_Trace_Data_Array)

stimulusResponsePlot.autoscale(True, True, True)

stimulusResponsePlot.show()

# Return the FieldFox back to free run trigger mode

myFieldFox.write("INIT:CONT ON")

# Send a corrupt SCPI command end of application as a debug test

if debug:
    myFieldFox.write("INIT:CONT OOOOOOOOOO")

# Call the ErrCheck function and ensure no errors occurred between start of program

# (first Errcheck() call and end of program (last Errcheck() call.

print(Errcheck())

# On exit clean a few items up.

myFieldFox.clear()

myFieldFox.close()