
from ui_elements.ui_plot import PlotWindow
from model import Model


class PlottingHandler():
    def __init__(self, view : PlotWindow, data : Model) -> None:
        
        self._ui = view
        self._model = data


    def modelObserver(self, timestamp : int, data : list):
        print("New data")
        # Append new element in list
        self._ui.dataCache["x"].append(timestamp)
        self._ui.dataCache["y"].append(data[0])

        # Remove first element in list
        del self._ui.dataCache["x"][0]
        del self._ui.dataCache["y"][0]

    def subscribeToPacketStream(self, packetId : int):

        self.structure = {
            "unique_id" : "PlotHandler",
            "packetId" : packetId,
            "func" : self.modelObserver
        }
        
        self._model.registerCallbackFunction(self.structure)


    