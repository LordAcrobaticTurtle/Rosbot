import tkinter as tk
from tkinter import ttk

SerialConsoleCallbackIndex = "SerialConsole"

class SerialConsole(ttk.Frame):
    def __init__(self, root : tk.Tk, callbacks : dict):
        # super.__init__(root)
        self._root = root
        self._callbacks = callbacks

    def create_window (self):
        # Create a base frame for all serial console elements.
        comFrameBase = tk.Frame(self._root)
        comFrameSettings = ttk.LabelFrame(comFrameBase, text="Connection Settings")
        comFrameSettings.grid(row=0, column=0)

        comConsole = ttk.Frame(comFrameBase, padding=10)
        comConsole.grid(column=1, row=0, sticky=(tk.W))
 
        serialConsoleLabel = ttk.Label(comFrameSettings, text="Serial console")
        serialConsoleLabel.grid(column=0, row=0)

        self._chosen_com_port = tk.StringVar()
        self._chosen_baud_rate = tk.StringVar()

        baud_menu = ttk.OptionMenu(comFrameSettings, self._chosen_baud_rate, "Select baud rate", "9600", "115200")
        self._chosen_baud_rate.set("115200")
        baud_menu.grid(column=0, row=1, sticky= (tk.E))

        #self._com_ports = self.controller.getComPortList()
        # Have a separate button responsible for updating com port list.
        self._com_ports = []
        com_menu = ttk.OptionMenu(comFrameSettings, self._chosen_com_port, self._com_ports)
        com_menu.grid(column=1, row=1, sticky=(tk.E))

        # Create serial console
        self._serialConsole = tk.Listbox(comConsole, width=100, xscrollcommand=1)
        self._serialConsole.grid(column=1, row=1, stick=tk.W+tk.E)
        serialConsoleScrollV = ttk.Scrollbar(comConsole, orient='vertical')
        serialConsoleScrollV.grid(column=2, row=1, sticky=tk.N + tk.S)
        serialConsoleScrollV.config(command=self._serialConsole.yview)
        serialConsoleScrollH = ttk.Scrollbar(comConsole, orient='horizontal')
        serialConsoleScrollH.config(command=self._serialConsole.xview)
        self._serialConsole.configure(xscrollcommand=serialConsoleScrollH.set, 
                                yscrollcommand=serialConsoleScrollV.set)
        
        self._serialInputString = tk.StringVar()
        self._serialInputBox = ttk.Entry(comConsole, textvariable=self._serialInputString)
        self._serialInputBox.grid(column=1, row=2, sticky= tk.W + tk.E)
        self._serialInputBox.bind("<Return>", lambda: self.button_pressed("SerialEnter"))

        subBtn = ttk.Button(comFrameSettings, text="Connect", command = lambda: self.button_pressed("Connect"))
        subBtn.grid(column=4,row=1, sticky = (tk.E))

        RefreshBtn = ttk.Button(comFrameSettings, text="Get List", command = lambda: self.button_pressed("Get list"))
        RefreshBtn.grid(column=2,row=2, sticky = (tk.E))

        closeBtn = ttk.Button(comFrameSettings, text="Disconnect", command = lambda: self.button_pressed("Disconnect"))
        closeBtn.grid(column=4,row=2, sticky = (tk.E))

        clearBtn = ttk.Button(comFrameSettings, text="Clear Messages", command = lambda: self.button_pressed("Clear Messages"))
        clearBtn.grid(column=3,row=2, sticky = (tk.E))

        return comFrameBase

    def button_pressed(self, whichButton : str):
        print(whichButton)
        
        try:
            if whichButton == "Connect":
                # Send the right parameters
                self._callbacks[SerialConsoleCallbackIndex]["Connect"](self._chosen_com_port, self._chosen_baud_rate)
                pass
            elif whichButton == "Disconnect":
                self._callbacks[SerialConsoleCallbackIndex]["Disconnect"]()
                pass
            elif whichButton == "Get List":
                self._callbacks[SerialConsoleCallbackIndex]["Get list"]()
                pass
            elif whichButton == "Clear Messages":
                self._serialConsole.delete(0, tk.END)
                pass
            # self._callbacks[SerialConsoleCallbackIndex]["Clear messages"]()
            elif whichButton == "SerialInput":
                # Send string
                self._callbacks[SerialConsoleCallbackIndex]["SerialInput"](self._serialInputString.get())
                pass
        except: 
            print("Button callback not implemented correctly")
        # self._callbacks[SerialConsoleCallbackIndex]["SerialInput"]()

    def updateSerialConsole(self, buffer : str):
        self._serialConsole.insert("end", buffer)
        # Why the magic numbers?
        self._serialConsole.select_clear(self._serialConsole.size() - 2)
        self._serialConsole.select_set("end")
        self._serialConsole.yview("end")

    def clear_listBox(self):
        self._serialConsole.delete(0, tk.END)
