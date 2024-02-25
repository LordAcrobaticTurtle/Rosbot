import math
from time import sleep
from datetime import datetime
import commands 

import view
import model

import ui_elements.ui_command_sidebar
import controllers.serial_port_handler
import controllers.sidebar_handler
import controllers.recording_handler
import controllers.pid_tuning_handler
from controllers.plotting_handler import PlottingHandler

class Controller:
    def __init__(self, model : model.Model, ui : view.View):
        self.model = model
        self.view = ui
        self.isAppRunning = True
        self._isPortOpen = False
        self._isMockPortOpen = False
        self._isRecordingActive = False

        self._robotConnection = controllers.serial_port_handler.SerialPortHandler(self.view, self.model)
        self._robotConnection.populate_callbacks()
        self._sidebarHandler = controllers.sidebar_handler.SidebarHandler(self.view, self._robotConnection)
        self._sidebarHandler.populate_callbacks()
        self._recordingHandler = controllers.recording_handler.RecordingHandler(self.view, self.model)
        self._recordingHandler.populate_callbacks()
        self._pidTunerHandler = controllers.pid_tuning_handler.PIDTuningHandler(self.view._pidTuner, self._robotConnection)
        self._pidTunerHandler.populate_callbacks()
        self._plotHandler = PlottingHandler(self.view._plot1, self.model)
        self._plotHandler.populate_callbacks()
        
        ui._callbacks["close"] = self.close
    
    def close(self): 
        print("Controller close")
        self._robotConnection.close()
        
        
    def generateSineWaveDataPoint(self, i):
        x = math.sin(i)
        return (i, x)
    
    # Returns a tuple for x AND y data
    def generateCosWaveDataPoint(self, i):
        x = math.cos(i)
        return (i, x)


def main():
    from model import Model
    from view import View
    import tkinter as tk
    view = View(tk.Tk())
    model = Model()
    controller = Controller(model, view)
    # controller.sendPacket(PacketIDs.BEGIN)
    # controller.sendPacket(PacketIDs.STANDBY)

if __name__ == "__main__":
    main()