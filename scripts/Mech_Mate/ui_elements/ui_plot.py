


import tkinter as tk
from tkinter import ttk

from matplotlib import style
import matplotlib.animation as animation
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
from matplotlib import style
import commands

PLOT_WINDOW_CALLBACK_INDEX = "PlotWindow"

class PlotWindow(ttk.Frame):
    def __init__(self, root : tk.Tk, callbacks : dict):
        self._root = root
        self._callbacks = callbacks
        self._callbacks[PLOT_WINDOW_CALLBACK_INDEX] = {}
        self.dataCache = {}
        self.dataCache["x"] = []
        self.dataCache["y"] = []

    
    def create_window(self) -> tk.Frame:
        plotBaseFrame = tk.Frame(self._root)

        dataGraph = tk.StringVar()

        # Create drop down box that lists all packets you can subscribe too.
        self._chosen_packet = tk.StringVar()
        listOfPackets = []
        for i, string in enumerate(commands.commands):
            listOfPackets.append(f"{i} - {string}")

        ttk.OptionMenu(plotBaseFrame, self._chosen_packet, *listOfPackets, command=self.ui_packetIDChanged).grid(column=0, row=0, padx=5, pady=5)

        style.use('ggplot')

        fig = Figure(figsize=(8,8), dpi=100)
        axis = fig.add_subplot(111)
        # axis.set_title(f"{plotIndex}")
        axis.set_xlabel("time (s)")
        axis.set_ylabel("Signal magnitude")
        axis.set_ylim([-2, 2])
        
        # plotting the graph
        linePlot, = axis.plot([], color='b')

        # creating the Tkinter canvas
        # containing the Matplotlib figure
        canvas = FigureCanvasTkAgg(fig, master = plotBaseFrame)  
        
        canvas.draw()
        canvas.get_tk_widget().grid()

        
        self.figure = fig
        self.axis = axis
        self.lineplot = linePlot
        self.canvas = canvas
        self.dataGraph = dataGraph

        # Set all plots to update using the same function
        ani = animation.FuncAnimation(fig, self.animate, interval=100)
        self.animation = ani
        
        return plotBaseFrame
    
    def animate(self, i):
        # Read cache and update plot
        self.lineplot.set_data(self.dataCache["x"], self.dataCache["y"])


    def ui_packetIDChanged(self, chosenPacket : str):
        print(chosenPacket)
        packetId = int(chosenPacket.split(" ")[0])
        print(packetId)
        



