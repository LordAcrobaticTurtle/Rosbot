import tkinter as tk
from tkinter import ttk
from custom_errors import GUIError
from controller import Controller
from toggle_button import ToggleButton

from comms.packetID import PacketIDs

# Import dependenues for LIVE plotting
from matplotlib import style
import matplotlib.animation as animation
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
from matplotlib import style

# App is the parent widget. The View is the frame that sits on top
# It should contain minimal logic for displaying data. 
# It will call functions in the controller layer to retrieve the data 

class View(ttk.Frame):
    def __init__(self, parent : tk.Tk):
        super().__init__(parent)
        self._parent = parent
        self._isControllerSet = False

    def create_window(self): 
        if not self._isControllerSet:
            raise GUIError("Controller has not been set")

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
        
        # Create command window
        commandFrame = self.createCommandWindow(self._parent)
        commandFrame.grid(column=0,row=0, sticky=(tk.N,tk.S), rowspan=numRows)        

        self.collectedPlots = {}

        # Create graphing windows
        self.collectedPlots["Plot1"] = False
        plot1, plotVariables = self.createPlotWindow(self._parent, "Plot1")
        self.collectedPlots["Plot1"] = plotVariables
        plot1.grid(column=1, row=0, sticky=(tk.E), padx=10)

        # print(plotVariables)
        
        # Create app settings panel
        appSettingsFrame = self.createAppSettingsWindow(self._parent)
        appSettingsFrame.grid(column=1, row=1)
        
        #  Create serial console
        comSettingsFrame = self.createSerialConsoleAndSettings(self._parent)
        comSettingsFrame.grid(column=2, row=1, sticky=(tk.W,tk.E))


    def createPlotWindow(self, tkRootElement :tk.Tk, plotIndex : str) -> (tk.Frame, dict):
    
        plotBaseFrame = tk.Frame(tkRootElement)

        style.use('ggplot')

        plotVariables = {
            "plotIndex" : plotIndex,
            "t" : list(range(0, 200)),
            "x" : [0] * 200
        }
        fig = Figure(figsize=(8,8), dpi=100)
        axis = fig.add_subplot(111)
        axis.set_title("Plot")
        axis.set_xlabel("time (s)")
        axis.set_ylabel("magnitude of signal")
        axis.set_ylim([-1, 1])
        
        # plotting the graph
        linePlot, = axis.plot([])


        # creating the Tkinter canvas
        # containing the Matplotlib figure
        canvas = FigureCanvasTkAgg(fig, master = plotBaseFrame)  
        # ani = animation.FuncAnimation(fig, self.plotAnimate, fargs=(plotIndex,),
        #                                          interval=50, blit=True, repeat=True)
        canvas.draw()
        canvas.get_tk_widget().grid()
        
        plotVariables["figure"] = fig
        plotVariables["axis"] = axis
        plotVariables["lineplot"] = linePlot
        plotVariables["canvas"] = canvas
        # plotVariables["animation"] = ani
  
        return plotBaseFrame, plotVariables
    
    def plotAnimate(self, i, plotIndex : str):
        (t, xS) = self.controller.generateSineWaveDataPoint(i)
        # print(self.collectedPlots[plotIndex])
        if (self.collectedPlots[plotIndex]):
            self.collectedPlots[plotIndex]["x"].append(xS)
            print(t, xS)
            return self.collectedPlots[plotIndex]["lineplot"]
        return self.linePlot,

        
    def createAppSettingsWindow(self, tkRootElement : tk.Tk) -> tk.Frame:
        appSettingsFrameBase = ttk.LabelFrame(tkRootElement, text="AppSettingsWindow")
        
        ttk.Button(appSettingsFrameBase, text="Start recording", padding=10).grid(column=0, row=0, padx=5, pady=5)
        ttk.Button(appSettingsFrameBase, text="Stop recording", padding=10).grid(column=1, row=0, padx=5, pady=5)
        ttk.Button(appSettingsFrameBase, text="File path for recording", padding=10).grid(column=0, row=1, padx=5, pady=5)
        ttk.Button(appSettingsFrameBase, text="Change file path", padding=10).grid(column=1, row=1, padx=5, pady=5)
        # rowC, colC = tkRootElement.grid_size()
        # for r in range(rowC):
        #     for c in range(colC):
        #         tk.Label(appSettingsFrameBase, text='R%s/C%s'%(r,c),borderwidth=1 ).grid(row=r,column=c)

        return appSettingsFrameBase

    def createCommandWindow(self, tkRootElement : tk.Tk) -> tk.Frame:
        commandFrameBase = ttk.LabelFrame(tkRootElement, text="Robot commands", width=20)

        commands = ["Begin", "Standby" ]
        self.commandButtons = [self.beginButtonClicked, self.standbyButtonClicked]
        for counter, c in enumerate(commands):
            button = ttk.Button(commandFrameBase, text=c, padding=15, command=self.commandButtons[counter])
            button.grid(column=0, row=counter, pady=10)
            self.commandButtons[counter] = button
    
        # toggle is WIP
        toggleButton = ToggleButton()
        toggleButton.create(commandFrameBase)
        toggleButton.getFrame().grid(column=0, row=5, pady=10)
        return commandFrameBase

    def beginButtonClicked(self):
        
        print("Begin clicked!")


    def standbyButtonClicked(self):
        print("Standby clicked!")


    def createTitleBar(self):
        menubar = tk.Menu(self._parent)
        filemenu = tk.Menu(menubar, tearoff=0)
        filemenu.add_command(label="New", command=self.donothing)
        filemenu.add_command(label="Open", command=self.donothing)
        filemenu.add_command(label="Save", command=self.donothing)
        filemenu.add_command(label="Save as...", command=self.donothing)
        filemenu.add_command(label="Close", command=self.donothing)

        filemenu.add_separator()

        filemenu.add_command(label="Exit", command=self._parent.quit)
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
        menubar.add_command(label = "Quit", command = self._parent.destroy)

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


    def openComPort(self):
        self.controller.openSerialPort(self._chosen_com_port.get(), self._chosen_baud_rate.get())

    def updateSerialConsole(self, buffer : str):
        self._serialConsole.insert("end", buffer)
        self._serialConsole.select_clear(self._serialConsole.size() - 2)
        self._serialConsole.select_set("end")
        self._serialConsole.yview("end")

    def comPortListUpdate(self):
        pass

    def closeComPort(self):
        self.controller.closeSerialPort(self._chosen_com_port.get())

    def clearListbox(self):
        pass

    def donothing(self):
        filewin = tk.Toplevel(self._parent)
        button = tk.Button(filewin, text="Do nothing button")
        button.pack()

    def About_me(self):
        filewin = tk.Toplevel(self._parent)
        button = tk.Button(filewin, text="Quit", command = filewin.destroy)
        button.pack()


    def serial_input_keypress_enter(self, event):
        self._serialInputString.set("")

    def clear_listBox(self):
        self._serialConsole.delete(0, tk.END)

    def set_controller(self, controller : Controller):
        """
        Set the controller
        :param controller:
        :return:
        """
        self._isControllerSet = True
        self.controller = controller

    def save_button_clicked(self):
        """
        Handle button click event
        :return:
        """
        if self.controller:
            self.controller.save(self.email_var.get())

    def show_error(self, message):
        """
        Show an error message
        :param message:
        :return:
        """
        self.message_label['text'] = message
        self.message_label['foreground'] = 'red'
        self.message_label.after(3000, self.hide_message)
        self.email_entry['foreground'] = 'red'

    def show_success(self, message):
        """
        Show a success message
        :param message:
        :return:
        """
        self.message_label['text'] = message
        self.message_label['foreground'] = 'green'
        self.message_label.after(3000, self.hide_message)

        # reset the form
        self.email_entry['foreground'] = 'black'
        self.email_var.set('')

    def hide_message(self):
        """
        Hide the message
        :return:
        """
        self.message_label['text'] = ''