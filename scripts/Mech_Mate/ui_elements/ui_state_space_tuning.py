
import tkinter as tk
from tkinter import ttk

StateSpaceTuningCallbackIndex = "STATESPACE"

# Pass in a reduced scope callbacks dictionary
class StateSpaceTuning(ttk.Frame):
    def __init__(self, root : tk.Tk, callbacks : dict) -> None:
        self._root = root
        self._callbacks = callbacks
        callbacks[StateSpaceTuningCallbackIndex] = {}

    def create_window(self):
        StateSpaceTuningBase = ttk.LabelFrame(self._root, text="State-space tuner")
        self._text = {}

        angleTuner = self.create_pid_entries(StateSpaceTuningBase, "State-space-gain-matrix")
        angleTuner.grid(column=0, row=0)
        # positionTuner = self.create_pid_entries(StateSpaceTuningBase, "PID Tuner - Position")
        # positionTuner.grid(column=0, row=1)
        
        return StateSpaceTuningBase


    def create_pid_entries(self, root : tk.Tk, Text : str):
        # Create baseframe
        baseFrame = ttk.LabelFrame(root, text=Text)
        
        # Create variables
        self._text[Text] = {}
        self._text[Text]["pos"] = tk.StringVar()
        self._text[Text]["vel"] = tk.StringVar()
        self._text[Text]["angpos"] = tk.StringVar()
        self._text[Text]["angvel"] = tk.StringVar()
        self._text[Text]["pos"].set(0)
        self._text[Text]["vel"].set(0)
        self._text[Text]["angpos"].set(0)
        self._text[Text]["angvel"].set(0)

        # Create UI
        ttk.Label(baseFrame, text="pos").grid(column=0, row=0)
        ttk.Entry(baseFrame, textvariable=self._text[Text]["pos"]).grid(column=1, row=0, padx=1, pady=5)
        
        ttk.Label(baseFrame, text="vel").grid(column=2, row=0)
        ttk.Entry(baseFrame, textvariable=self._text[Text]["vel"]).grid(column=3, row=0, padx=1, pady=5)
        
        ttk.Label(baseFrame, text="angpos").grid(column=4, row=0)
        ttk.Entry(baseFrame, textvariable=self._text[Text]["angpos"]).grid(column=5, row=0, padx=1, pady=5)

        ttk.Label(baseFrame, text="angvel").grid(column=6, row=0)
        ttk.Entry(baseFrame, textvariable=self._text[Text]["angvel"]).grid(column=7, row=0, padx=1, pady=5)
        
        ttk.Button(baseFrame, text="Send!", padding=10, command= lambda : self.button_pressed(Text)).grid(column=8, row=0)
        
        return baseFrame


    def button_pressed(self, whichButton : str):

        if whichButton == "State-space-gain-matrix":
            posGain = float(self._text[whichButton]["pos"].get())
            velGain = float(self._text[whichButton]["vel"].get())
            angPosGain = float(self._text[whichButton]["angpos"].get())
            angVelGain = float(self._text[whichButton]["angvel"].get())
    
            self._callbacks[StateSpaceTuningCallbackIndex]["Send"](posGain,velGain,angPosGain,angVelGain)

        return 
            
        


