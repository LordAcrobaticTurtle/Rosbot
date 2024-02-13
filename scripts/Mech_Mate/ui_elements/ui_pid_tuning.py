
import tkinter as tk
from tkinter import ttk

PIDTuningCallbackIndex = "PID"

# Pass in a reduced scope callbacks dictionary
class PidTuning(ttk.Frame):
    def __init__(self, root : tk.Tk, callbacks : dict) -> None:
        self._root = root
        self._callbacks = callbacks
        callbacks[PIDTuningCallbackIndex] = {}

    def create_window(self):
        PidTuningBase = ttk.LabelFrame(self._root, text="PID tuner")
        self._text = {}

        angleTuner = self.create_pid_entries(PidTuningBase, "PID Tuner - Angle")
        angleTuner.grid(column=0, row=0)
        positionTuner = self.create_pid_entries(PidTuningBase, "PID Tuner - Position")
        positionTuner.grid(column=0, row=1)
        
        return PidTuningBase


    def create_pid_entries(self, root : tk.Tk, Text : str):
        # Create baseframe
        baseFrame = ttk.LabelFrame(root, text=Text)
        
        # Create variables
        self._text[Text] = {}
        self._text[Text]["p"] = tk.StringVar()
        self._text[Text]["i"] = tk.StringVar()
        self._text[Text]["d"] = tk.StringVar()
        self._text[Text]["p"].set(0)
        self._text[Text]["i"].set(0)
        self._text[Text]["d"].set(0)

        # Create UI
        ttk.Label(baseFrame, text="P").grid(column=0, row=0)
        ttk.Entry(baseFrame, textvariable=self._text[Text]["p"]).grid(column=1, row=0, padx=1, pady=5)
        ttk.Label(baseFrame, text="I").grid(column=2, row=0)
        ttk.Entry(baseFrame, textvariable=self._text[Text]["i"]).grid(column=3, row=0, padx=1, pady=5)
        ttk.Label(baseFrame, text="D").grid(column=4, row=0)
        ttk.Entry(baseFrame, textvariable=self._text[Text]["d"]).grid(column=5, row=0, padx=1, pady=5)
        ttk.Button(baseFrame, text="Send!", padding=10, command= lambda : self.button_pressed(Text)).grid(column=6, row=0)
        
        return baseFrame


    def button_pressed(self, whichButton : str):

        if whichButton == "PID Tuner - Angle":
            controlIndex = 0
            self._callbacks[PIDTuningCallbackIndex]["Send"](controlIndex, float(self._text[whichButton]["p"].get()), float(self._text[whichButton]["i"].get()), float(self._text[whichButton]["d"].get()))
        elif whichButton == "PID Tuner - Position": 
            controlIndex = 1
            self._callbacks[PIDTuningCallbackIndex]["Send"](controlIndex, float(self._text[whichButton]["p"].get()), float(self._text[whichButton]["i"].get()), float(self._text[whichButton]["d"].get()))

        return 
            
        


