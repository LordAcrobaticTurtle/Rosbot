import tkinter as tk
from tkinter import ttk
from custom_errors import GUIError

import ui_elements.ui_command_sidebar as SideBar
import ui_elements.ui_record_settings as RecordSettings
import ui_elements.ui_serial_console as SerialConsole

class View(ttk.Frame):
    def __init__(self, root : tk.Tk):
        super().__init__(root)
        self._parent = root
        self._isControllerSet = False
        self._callbacks = {}
        self._sideBar = SideBar.CommandSideBar(self._parent, self._callbacks)
        self._recordingSettings = RecordSettings.RecordSettings(self._parent, self._callbacks)
        self._serialConsoleSettings = SerialConsole.SerialConsole(self._parent, self._callbacks)

        
    def close(self):
        self._parent.quit()
        self._parent.destroy()

    def create_window(self): 
        # if not self._isControllerSet:
        #     raise GUIError("Controller has not been set")

        numColums = 3
        numRows = 2

        colWeights = [1, 3, 3]
        rowWeights = [1, 1]

        for c in range(numColums):
            self._parent.columnconfigure(c, weight=colWeights[c])

        for r in range(numRows):
            self._parent.rowconfigure(r, weight=rowWeights[r])
            
        
        # Create title bar
        self.createTitleBar()
        commandFrame = self._sideBar.create_window()
        commandFrame.grid(column=0,row=0, sticky=(tk.N,tk.S), rowspan=numRows)        
        appSettingsFrame = self._recordingSettings.create_window()
        appSettingsFrame.grid(column=1, row=1)
        comSettingsFrame = self._serialConsoleSettings.create_window()
        comSettingsFrame.grid(column=2, row=1, sticky=(tk.W,tk.E))

    def createPlotWindow(self, tkRootElement :tk.Tk, plotIndex : str) -> (tk.Frame, dict):
    
        plotBaseFrame = tk.Frame(tkRootElement)

        dataGraph = tk.StringVar()
        plotType = ttk.OptionMenu(plotBaseFrame, dataGraph, "Select data to graph", "Control", "Localisation")
        plotType.grid()
        style.use('ggplot')

        plotVariables = {
            "plotIndex" : plotIndex,
            "t" : list(range(0, 200)),
            "x" : [0] * 200
        }
        fig = Figure(figsize=(8,8), dpi=100)
        axis = fig.add_subplot(111)
        axis.set_title(f"{plotIndex}")
        axis.set_xlabel("time (s)")
        axis.set_ylabel("Signal magnitude")
        axis.set_ylim([-2, 2])
        
        # plotting the graph
        linePlot, = axis.plot([], color='b')

        # creating the Tkinter canvas
        # containing the Matplotlib figure
        canvas = FigureCanvasTkAgg(fig, master = plotBaseFrame)  
        
        # Set all plots to update using the same function
        # ani = animation.FuncAnimation(fig, self.plotAnimate, interval=0, blit = True, repeat=False)
        
        canvas.draw()
        canvas.get_tk_widget().grid()
        
        plotVariables["figure"] = fig
        plotVariables["axis"] = axis
        plotVariables["lineplot"] = linePlot
        plotVariables["canvas"] = canvas
        plotVariables["dataGraph"] = dataGraph
        # plotVariables["animation"] = ani
  
        return plotBaseFrame, plotVariables
    
    def plotAnimatePlot1(self, i):
        plot1 = self.collectedPlots['Plot1']
        tdata1 = plot1["t"]
        xdata1 = plot1["x"]
        # set lineplot data to be equal to x and t
        # Remove oldest entry
        del tdata1[0] 
        tdata1.append(i)

        del xdata1[0]
        xdata1.append(math.sin(i/10))
        plot1["lineplot"].set_data(tdata1, xdata1)
        plot1["axis"].set_xlim(tdata1[0], tdata1[-1])
        
        return (plot1["lineplot"], )
        
    def plotAnimatePlot2(self, i):
        plot2 = self.collectedPlots['Plot2']
        tdata2 = plot2["t"]
        xdata2 = plot2["x"]
        # set lineplot data to be equal to x and t
        # Remove oldest entry
        del tdata2[0] 
        tdata2.append(i)

        del xdata2[0]
        xdata2.append(math.cos(i/20) + math.sin(i/15))

        plot2["lineplot"].set_data(tdata2, xdata2)
        plot2["axis"].set_xlim(tdata2[0], tdata2[-1])

        return (plot2["lineplot"], )


    def createTitleBar(self):
        menubar = tk.Menu(self._parent)
        filemenu = tk.Menu(menubar, tearoff=0)
        filemenu.add_command(label="New", command=self.donothing)
        filemenu.add_command(label="Open", command=self.donothing)
        filemenu.add_command(label="Save", command=self.donothing)
        filemenu.add_command(label="Save as...", command=self.donothing)
        filemenu.add_command(label="Close", command=self.donothing)

        filemenu.add_separator()

        filemenu.add_command(label="Exit", command=self.close)
        menubar.add_cascade(label="File", menu=filemenu)
        editmenu = tk.Menu(menubar, tearoff=0)
        editmenu.add_command(label="Undo", command=self.donothing)

        editmenu.add_separator()

        editmenu.add_command(label="Cut", command=self.donothing)
        editmenu.add_command(label="Copy", command=self.donothing)
        editmenu.add_command(label="Paste", command=self.donothing)
        editmenu.add_command(label="Delete", command=self.donothing)
        editmenu.add_command(label="Select All", command=self.donothing)

        menubar.add_cascade(label="Edit", menu=editmenu)
        helpmenu = tk.Menu(menubar, tearoff=0)
        helpmenu.add_command(label="Help Index", command=self.donothing)
        helpmenu.add_command(label="About...", command=self.donothing)
        menubar.add_cascade(label="Help", menu=helpmenu)
        menubar.add_separator()
        menubar.add_command(label = "Quit", command = self.close)

        self._parent.config(menu=menubar)


    def createSerialConsoleAndSettings(self, tkRootElement : tk.Tk) -> tk.Frame:
        comFrameBase = tk.Frame(tkRootElement)
        comFrameSettings = ttk.LabelFrame(comFrameBase, text="Connection Settings")
        comFrameSettings.grid(row=0, column=0)

        comConsole = ttk.Frame(comFrameBase, padding=10)
        comConsole.grid(column=1, row=0, sticky=(tk.W))
 
        # for x in range(10):
        #     commSettingsFrame.columnconfigure(x, weight=x)
        #     commSettingsFrame.rowconfigure(x,weight=x)

        serialConsoleLabel = ttk.Label(comFrameSettings, text="Serial console")
        serialConsoleLabel.grid(column=0, row=0)

        self._chosen_com_port = tk.StringVar()
        self._chosen_baud_rate = tk.StringVar()

        baud_menu = ttk.OptionMenu(comFrameSettings, self._chosen_baud_rate, "Select baud rate", "9600", "115200")
        self._chosen_baud_rate.set("115200")
        baud_menu.grid(column=0, row=1, sticky= (tk.E))

        self._com_ports = self.controller.getComPortList()
        com_menu = ttk.OptionMenu(comFrameSettings, self._chosen_com_port, *self._com_ports)
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
        self._serialInputBox.bind("<Return>", self.serial_input_keypress_enter )

        subBtn = ttk.Button(comFrameSettings, text="Connect",command = self.openComPort)
        subBtn.grid(column=4,row=1, sticky = (tk.E))

        RefreshBtn = ttk.Button(comFrameSettings, text="Get List",command = self.comPortListUpdate)
        RefreshBtn.grid(column=2,row=2, sticky = (tk.E))

        closeBtn = ttk.Button(comFrameSettings, text="Disconnect",command = self.closeComPort)
        closeBtn.grid(column=4,row=2, sticky = (tk.E))

        clearBtn = ttk.Button(comFrameSettings, text="Clear Messages",command = self.clearListbox)
        clearBtn.grid(column=3,row=2, sticky = (tk.E))

        return comFrameBase
    
    def button_pressed(self, whichButton : str): 
        print(whichButton)

    def openComPort(self):
        if (self._chosen_com_port.get() == "Mock Connection"):
            self.controller.openMockSerialPort(self._chosen_com_port.get(), self._chosen_baud_rate.get())
        else:
            self.controller.openSerialPort(self._chosen_com_port.get(), self._chosen_baud_rate.get())

    def updateSerialConsole(self, buffer : str):
        self._serialConsole.insert("end", buffer)
        # Why the magic numbers?
        self._serialConsole.select_clear(self._serialConsole.size() - 2)
        self._serialConsole.select_set("end")
        self._serialConsole.yview("end")

    def comPortListUpdate(self):
        pass

    def closeComPort(self):
        
        self.controller.closeMockSerialPort()
        self.controller.closeSerialPort()

    def clearListbox(self):
        pass

    def donothing(self):
        filewin = tk.Toplevel(self._parent)
        button = tk.Button(filewin, text="Do nothing button")
        button.pack()

    def serial_input_keypress_enter(self, event):
        self.controller.sendString(self._serialInputString.get() + "\n")
        self._serialInputString.set("")

    def clear_listBox(self):
        self._serialConsole.delete(0, tk.END)

    def set_controller(self, controller):
        """
        Set the controller
        :param controller:
        :return:
        """
        self._isControllerSet = True
        self.controller = controller

    def beginButtonClicked(self):
        self.controller.sendString("Begin\n")
        print("Begin clicked!")

    def standbyButtonClicked(self):
        self.controller.sendString("Standby\n")
        print("Standby clicked!")

    def calibrateButtonClicked(self):
        self.controller.sendString("Calibrate\n")
        print("Calibrated clicked!")

    def resetImuClicked(self):
        self.controller.sendString("Reset-IMU\n")
        print("Reset-IMU clicked!")

    def startRecordingClicked(self):
        print("Start recording clicked!")
        self.controller.activateRecording(self._recordingPath.get())

    def toggleRecording(self):
        print("Toggle recording clicked!")

        if self.isRecordingActive.get():
            print("Active recording!")
            self.controller.activateRecording(self._recordingPath.get())
        else:
            print("Deactive recording!")
            self.controller.deactivateRecording()

    def stopRecordingClicked(self):
        self.controller.deactivateRecording()
        print("Stop recording clicked!")

    def changeFilePathClicked(self):
        print(self._recordingPath.get())
        self._recordingPath.set(filedialog.askdirectory())
        print("Change file path clicked!")