import tkinter as tk
from tkinter import ttk
from custom_errors import GUIError

import ui_elements.ui_command_sidebar as SideBar
import ui_elements.ui_record_settings as RecordSettings
import ui_elements.ui_serial_console as SerialConsole
from ui_elements.ui_pid_tuning import PidTuning
from ui_elements.ui_plot import PlotWindow

APP_CALLBACK_INDEX = "APP_CALLBACK_INDEX"

class View(ttk.Frame):
    def __init__(self, root : tk.Tk):
        super().__init__(root)
        self._parent = root
        self._isControllerSet = False
        self._callbacks = {}
        self._sideBar = SideBar.CommandSideBar(self._parent, self._callbacks)
        self._recordingSettings = RecordSettings.RecordSettings(self._parent, self._callbacks)
        self._serialConsoleSettings = SerialConsole.SerialConsole(self._parent, self._callbacks)
        self._callbacks["AngleTuner"] = {}
        self._callbacks["PositionTuner"] = {}
        self._pidTuner = PidTuning(self._parent, self._callbacks)
        self._plot1 = PlotWindow(self._parent, self._callbacks)
        
   
    def close(self):
        self._parent.quit()
        self._parent.destroy()
        self._callbacks["close"]()

    def create_window(self): 
        # if not self._isControllerSet:
        #     raise GUIError("Controller has not been set")

        numColums = 3
        numRows = 2

        colWeights = [1, 3, 3]
        rowWeights = [1, 2]

        for c in range(numColums):
            self._parent.columnconfigure(c, weight=colWeights[c])

        for r in range(numRows):
            self._parent.rowconfigure(r, weight=rowWeights[r])
            
        
        # Create control windows
        self.createTitleBar()
        commandFrame = self._sideBar.create_window()
        commandFrame.grid(column=0,row=0, sticky=(tk.N,tk.S), rowspan=numRows)        
        appSettingsFrame = self._recordingSettings.create_window()
        appSettingsFrame.grid(column=1, row=1)
        comSettingsFrame = self._serialConsoleSettings.create_window()
        comSettingsFrame.grid(column=2, row=1, sticky=(tk.W+tk.N))
        pidTuningFrame = self._pidTuner.create_window()
        pidTuningFrame.grid(column=2, row=0)
        # plot1Frame = self._plot1.create_window()
        # plot1Frame.grid(column=1, row=0)
        # PositionPidTUningFrame = self._positionTuner.create_window()
        # PositionPidTUningFrame.grid(column=1, row=1, sticky=(tk.E + tk.N))

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

    def donothing(self):
        print("NOTHING")