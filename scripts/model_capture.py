# Read pendulum file + localisation packet file.
# Read the correct columns 
# Find peaks + troughs, generate pendulum model 
# Find scale error of gyroscope.
import sys
import csv


WINDOW_RADIUS = 4
TIMESTAMP_INDEX = 0
POSITION_INDEX = 1
VELOCITY_INDEX = 2

# Data[] contains is a list of lists.
def collectCSVData(pathToJigData : str):
    
    # Open file

    file = open(pathToJigData, 'r')
    data = []
    for index, line in enumerate(file):
        raw_entries = line.split(",")
        # First must be int
        # Everything else floats
        entries = []
        try:
            timestamp = int(raw_entries[0])
        except:
            continue

        entries.append(timestamp)

        for entry in raw_entries[1:len(raw_entries)]:
            entries.append(float(entry))

        data.append(entries)

    file.close()

    return data

# findPeaks returns a list of dictionaries containing (index, entry[timestamp, pos, vel])
def findPeaksInDataset(rawData : list):
    peaks = []
    # I currently only care about angular position. (index 1 and 2)
    numberOfPeaks = 0
    for i, entry in enumerate(rawData):
        # Check the ith entry is a maximum amongst its peers AND the window meets a threshold of variance
        # Create window list
        
        # if (i-windowRadius) > 0 and (i+windowRadius) <= len(rawData):
            # print()
        windowList = rawData[i-WINDOW_RADIUS:i+WINDOW_RADIUS]
        trimmedWindowList = [x[POSITION_INDEX] for x in windowList]

        if len(trimmedWindowList) <= 0:
            continue

        if entry[POSITION_INDEX] == max(trimmedWindowList):
            print(entry[POSITION_INDEX])
            numberOfPeaks += 1
            peaks.append((i, entry))

    print(numberOfPeaks)
    return peaks

def findTroughsInDataset(rawData : list):
    troughs = []
    # I currently only care about angular position. (index 1 and 2)
    numberOfTroughs = 0
    for i, entry in enumerate(rawData):
        windowList = rawData[i-WINDOW_RADIUS:i+WINDOW_RADIUS]
        trimmedWindowList = [x[POSITION_INDEX] for x in windowList]

        if len(trimmedWindowList) <= 0:
            continue

        if entry[POSITION_INDEX] == min(trimmedWindowList):
            print(entry[POSITION_INDEX])
            numberOfTroughs += 1
            troughs.append((i, entry))

    print(numberOfTroughs)
    return troughs

def main():
    if (len(sys.argv) < 3):
        print ("Usage - python model_capture.py <path-to-pendulum-jig-data> <path-to-IMU-collected-data>")
        return

    # Read the entire file into memory
    jigDataFromFile = collectCSVData(sys.argv[1])
    # jigDataFromFile = collectCSVData("nuts")

    imuDataFromFile = collectCSVData(sys.argv[2])
    # imuDataFromFile = collectCSVData("Nuts")

    # Using jig data set
    # Find peaks - Sliding window?
    listOfPeaks = findPeaksInDataset(jigDataFromFile)
    listOfTroughs = findTroughsInDataset(jigDataFromFile)
    # Find troughs

    # Find initial and final timestamp of dataset
    initialTimestamp = min(listOfPeaks[0][1][TIMESTAMP_INDEX], listOfTroughs[0][1][TIMESTAMP_INDEX])
    print(listOfPeaks[0])
    print(listOfTroughs[0])
    print("Initial Timestamp: " + str(initialTimestamp))

    # Trim first portion of dataset to remove stillness/moving to position
    

    # Remove duplicate values

    # Now we have true data that we can actually work into a model

    # Choose the first n peaks. 
    # Find the corresponding amplitude for those peaks
    # use logarithmic decrement formula
    # Calculate average time between peaks to determine pendulum period
    # Calculate damped natural frequency from pendulum period
    # = 2* PI / T
    # Calculate natural frequnecy
    # Calculate pendulum length

    # Then for the hard part. 
    # Calculating gyro scale error



    

main()