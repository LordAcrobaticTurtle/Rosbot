# Read pendulum file + localisation packet file.
# Read the correct columns 
# Find peaks + troughs, generate pendulum model 
# Find scale error of gyroscope.
import sys
import csv

# Data[] contains is a list of lists.
def collectCSVData(pathToJigData : str):
    data = [
        ["timestamp1", 0.0],
        ["timestamp2", 0.1],
        ["timestamp3", 0.2],
        ["timestamp4", 0.3],
        ["timestamp5", 0.4],
    ]


    return data

# findPeaks returns a list of dictionaries containing (timestamp, amplitude)
def findPeaksInDataset(rawData : list):
    peaks = []

    # I currently only care about angular position. (index 1 and 2)

    windowRadius = 4
    for i, entry in enumerate(rawData):
        # Check the ith entry is a maximum amongst its peers AND the window meets a threshold of variance
        
        if entry[2] == max(rawData[i-windowRadius:i+windowRadius][2]) and  
        
        # print(i)
        # print(entry[1])


    return peaks

def main():
    print(sys.argv)

    # if (len(sys.argv) < 3):
    #     print ("Usage - python model_capture.py <path-to-pendulum-jig-data> <path-to-IMU-collected-data>")
    #     return

    # Read the entire file into memory
    # jigDataFromFile = collectCSVData(sys.argv[1])
    jigDataFromFile = collectCSVData("nuts")

    # imuDataFromFile = collectCSVData(sys.argv[2])
    imuDataFromFile = collectCSVData("Nuts")


    # Using jig data set
    # Find peaks - Sliding window?
    listOfPeaks = findPeaksInDataset(jigDataFromFile)

    # Find troughs

    # Sort them both

    # Find initial and final timestamp of dataset


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