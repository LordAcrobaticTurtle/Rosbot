


import tkinter as tk
from tkinter import ttk


class PlotWindow(ttk.Frame):
    def __init__(self, root : tk.Tk, callbacks : dict):
        self._root = root
        self._callbacks = callbacks

    
    def create_window(self) -> tk.Frame:
        plotBaseFrame = tk.Frame(self._root)

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

