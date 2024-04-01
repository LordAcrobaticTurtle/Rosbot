import tkinter as tk
from tkinter import ttk 


CommandSideBarCallbackIndex = "SidebarCallbacks"

class CommandSideBar(ttk.Frame):
    def __init__(self, root : tk.Tk, callbacks : dict):
        # super.__init__(root)
        self._root = root
        self._callbacks = callbacks
        self._callbacks[CommandSideBarCallbackIndex] = {}

    # The callbacks dict must be initialized BEFORE calling create window
    def create_window(self):
        commandFrameBase = ttk.LabelFrame(self._root, text="Robot commands", width=20)

        commands = ["Begin", "Standby", "Calibrate", "Reset-IMU", "Sensor-Verification" ]
        
        # for counter, c in enumerate(commands):
        button = ttk.Button(commandFrameBase, text="Begin", padding=15, command = lambda: self.button_pressed("Begin"))
        button.grid(column=0, row=0, pady=10)

        button = ttk.Button(commandFrameBase, text="Standby", padding=15, command = lambda: self.button_pressed("Standby"))
        button.grid(column=0, row=1, pady=10)
    
        button = ttk.Button(commandFrameBase, text="Calibrate", padding=15, command = lambda: self.button_pressed("Calibrate"))
        button.grid(column=0, row=2, pady=10)
    
        button = ttk.Button(commandFrameBase, text="Reset-IMU", padding=15, command = lambda: self.button_pressed("Reset-IMU"))
        button.grid(column=0, row=3, pady=10)

        button = ttk.Button(commandFrameBase, text="Verify", padding=15, command = lambda: self.button_pressed("Sensor-Verification"))
        button.grid(column=0, row=4, pady=10)
    
        return commandFrameBase
    
    def button_pressed(self, whichButton : str):
        print(whichButton) 
        # Based on which button I push, I can call the appropriate callback through the callbacks dict
        # None of these sidebar functions should have parameters
        self._callbacks[CommandSideBarCallbackIndex][whichButton](whichButton) 

