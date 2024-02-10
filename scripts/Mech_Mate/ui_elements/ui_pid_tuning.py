
import tkinter as tk
from tkinter import ttk

PIDTuningCallbackIndex = "PID"

class PidTuning(ttk.Frame):
    def __init__(self, root : tk.Tk, callbacks : dict) -> None:
        self._root = root
        self._callbacks = callbacks
        callbacks[PIDTuningCallbackIndex] = {}


    def create_window(self):
        PidTuningBase = ttk.LabelFrame(self._root, text="PID tuner")
        self._pText = tk.StringVar()
        self._iText = tk.StringVar()
        self._dText = tk.StringVar()

        ttk.Label(PidTuningBase, text="P").grid(column=0, row=0)
        ttk.Entry(PidTuningBase, textvariable=self._pText).grid(column=1, row=0, padx=1, pady=5)
        ttk.Label(PidTuningBase, text="I").grid(column=2, row=0)
        ttk.Entry(PidTuningBase, textvariable=self._iText).grid(column=3, row=0, padx=1, pady=5)
        ttk.Label(PidTuningBase, text="D").grid(column=4, row=0)
        ttk.Entry(PidTuningBase, textvariable=self._dText).grid(column=5, row=0, padx=1, pady=5)
        ttk.Button(PidTuningBase, text="Send!", padding=10, command= lambda : self.button_pressed("Send")).grid(column=6, row=0)
        
        return PidTuningBase


    def button_pressed(self, whichButton : str):
        print(whichButton)
        if whichButton == "Send":
            self._callbacks[PIDTuningCallbackIndex]["Send"](float(self._pText.get()), float(self._iText.get()), float(self._dText.get()))
        


