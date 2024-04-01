
import tkinter as tk
from tkinter import ttk

SensorVerificationPanel = "SENSORVERIFY"

# Pass in a reduced scope callbacks dictionary
class SensorVerificationPanel(ttk.Frame):
    def __init__(self, root : tk.Tk, callbacks : dict) -> None:
        self._root = root
        self._callbacks = callbacks
        callbacks[SensorVerificationPanel] = {}

    def create_window(self):
        baseFrame = ttk.LabelFrame(self._root, text="Sensor Verification")
        self._text = {}

        leftMotor = self.create_pid_entries(baseFrame, "Left Motor")
        leftMotor.grid(column=0, row=0)
        rightMotor = self.create_pid_entries(baseFrame, "Right Motor")
        rightMotor.grid(column=0, row=1)
        
        return baseFrame


    def create_pid_entries(self, root : tk.Tk, Text : str):
        # Create baseframe
        baseFrame = ttk.LabelFrame(root, text=Text)
        
        # Create variables
        self._text[Text] = {}
        self._text[Text]["Throttle"] = tk.StringVar()
        self._text[Text]["Time"] = tk.StringVar()
        self._text[Text]["Throttle"].set(0)
        self._text[Text]["Time"].set(0)

        # Create UI
        ttk.Label(baseFrame, text="Throttle").grid(column=0, row=0)
        ttk.Entry(baseFrame, textvariable=self._text[Text]["Throttle"]).grid(column=1, row=0, padx=1, pady=5)
        ttk.Label(baseFrame, text="Time").grid(column=2, row=0)
        ttk.Entry(baseFrame, textvariable=self._text[Text]["Time"]).grid(column=3, row=0, padx=1, pady=5)
        ttk.Button(baseFrame, text="Verify!", padding=10, command= lambda : self.button_pressed(Text)).grid(column=6, row=0)
        
        return baseFrame


    def button_pressed(self, whichButton : str):

        if whichButton == "Left Motor":
            controlIndex = 0
            self._callbacks[SensorVerificationPanel]["Send"](controlIndex, float(self._text[whichButton]["Throttle"].get()), float(self._text[whichButton]["Time"].get()))
        elif whichButton == "Right Motor": 
            controlIndex = 1
            self._callbacks[SensorVerificationPanel]["Send"](controlIndex, float(self._text[whichButton]["Time"].get()), float(self._text[whichButton]["Time"].get()))
        return 
            
        


