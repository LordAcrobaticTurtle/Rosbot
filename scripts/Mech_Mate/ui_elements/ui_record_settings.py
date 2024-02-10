import tkinter as tk
from tkinter import ttk
from tkinter import filedialog
import commands

class RecordSettings(ttk.Frame):
    def __init__(self, root : tk.Tk, callbacks : dict):
        self._root = root
        self._callbacks = callbacks

    def create_window (self) -> tk.Frame:
        appSettingsFrameBase = ttk.LabelFrame(self._root, text="AppSettingsWindow")
        self.isRecordingActive = tk.BooleanVar()
        ttk.Checkbutton(appSettingsFrameBase, text="Recording", padding=10, command=lambda : self.button_pressed("Recording"), variable=self.isRecordingActive).grid(column=0, row=0, padx=5, pady=5)
        
        self._recordingPath = tk.StringVar()
        ttk.Entry(appSettingsFrameBase, textvariable=self._recordingPath, state="readonly").grid(column=0, row=1, padx=5, pady=5)
        
        ttk.Button(appSettingsFrameBase, text="Change file path", padding=10, command=lambda : self.button_pressed("Change file path")).grid(column=1, row=1, padx=5, pady=5)
        
        # Drop down menu for all available packets
        self._chosen_packet = tk.StringVar()
        listOfPackets = []
        for i, string in enumerate(commands.commands):
            listOfPackets.append(f"{i} - {string}")

        ttk.OptionMenu(appSettingsFrameBase, self._chosen_packet, *listOfPackets).grid(column=1, row=0, padx=5, pady=5)

        return appSettingsFrameBase

    def button_pressed (self, whichButton : str):
        print(whichButton)
        if whichButton == "Recording":
            if self.isRecordingActive.get():
                # Grab first number in chosen packet
                packetId = int(self._chosen_packet.get().split(" ")[0])
                self._callbacks["start"](packetId, self._recordingPath.get())
            elif not self.isRecordingActive.get():
                self._callbacks["stop"]()

        elif whichButton == "Change file path":
            self._recordingPath.set(filedialog.askdirectory())
            print(self._recordingPath.get())


