


import tkinter as tk
from tkinter import ttk

from matplotlib import style
import matplotlib.animation as animation
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
from matplotlib import style
from matplotlib.backend_bases import key_press_handler

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

        # listOfPackets.append(f"{len(listOfPackets)} - Sine")
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
        canvas.get_tk_widget().grid(columnspan=2)

        self.toolbar = NavigationToolbar2Tk(canvas, plotBaseFrame, pack_toolbar=False)
        self.toolbar.update()
        self.toolbar.grid(row=0, column=1)

        canvas.mpl_connect(
        "key_press_event", lambda event: print(f"you pressed {event.key}"))
        canvas.mpl_connect("key_press_event", key_press_handler)

        self.figure = fig
        self.axis = axis
        self.lineplot = linePlot
        self.canvas = canvas
        self.dataGraph = dataGraph

        # Set all plots to update using the same function
        ani = animation.FuncAnimation(fig, self.animate, interval=0, blit = True, repeat=False)
        self.animation = ani
        
        return plotBaseFrame
    
    def animate(self, event):
        # Read cache and update plot
        # print("Ui-Plot.animate")
        # print(len(self.dataCache["x"]))
        # self.lineplot.set_xdata(self.dataCache["x"])
        # self.lineplot.set_ydata(self.dataCache["y"])
        self.lineplot.set_data(self.dataCache["x"], self.dataCache["y"])
        if len(self.dataCache["x"]) > 0:
            self.axis.set_xlim(self.dataCache["x"][0], self.dataCache["x"][-1])

        # if len(self.dataCache["y"]) > 0:
        #     self.axis.set_ylim(self.dataCache["y"][0], self.dataCache["y"][-1])
        self.axis.set_xlabel("time (s)")
        self.axis.set_ylabel("Signal magnitude")
        # self.axis.set_ylim([-2, 2])
        # self.axis.set_xticks()
        
        return (self.lineplot, self.axis, )


    def ui_packetIDChanged(self, chosenPacket : str):
        # print(chosenPacket)
        packetId = int(chosenPacket.split(" ")[0])
        # This should update the controller with whatever the new packet ID is.
        self._callbacks[PLOT_WINDOW_CALLBACK_INDEX]["packetIDHasChanged"](packetId)
        # print(packetId)
        
    


