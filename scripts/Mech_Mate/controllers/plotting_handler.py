
from ui_elements.ui_plot import PlotWindow, PLOT_WINDOW_CALLBACK_INDEX
from model import Model, MAX_QUEUE_LENGTH


class PlottingHandler():
    def __init__(self, view : PlotWindow, data : Model) -> None:
        
        self._ui = view
        self._model = data
        
        self.structure = {
            "unique_id" : "PlotHandler",
            "packetId" : -1,
            "func" : self.modelObserver
        }

    def populate_callbacks(self):
        self._ui._callbacks[PLOT_WINDOW_CALLBACK_INDEX]["packetIDHasChanged"] = self.packetIDHasChanged

    def modelObserver(self, timestamp : int, data : list):
        # print("plot.modelObserver: New data")
        # Append new element in list
        self._ui.dataCache["x"].append(timestamp)
        self._ui.dataCache["y"].append(data[0])

        # Remove first element in list if over a certain number of elements 
        if len(self._ui.dataCache["x"]) > MAX_QUEUE_LENGTH:
            del self._ui.dataCache["x"][0]
            del self._ui.dataCache["y"][0]

    # def subscribeToPacketStream(self, packetId : int):
    #     self._model.registerCallbackFunction(self.structure)

    def packetIDHasChanged (self, packetId : int):
        # First, unsubscribe from any existing callbacks
        if self.structure["packetId"] > -1:
            self._model.deRegisterCallbackFunction(self.structure["unique_id"], self.structure["packetId"])
        
        # Next, create new subscription 
        self.structure["packetId"] = packetId
        self._model.registerCallbackFunction(self.structure)
        print("Successful change")



    