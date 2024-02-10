
from model import Model
from view import View
from controllers.serial_port_handler import SerialPortHandler
from ui_elements.ui_pid_tuning import PIDTuningCallbackIndex

class PIDTuningHandler():

    def __init__(self, ui : View, data : Model, robotConnection : SerialPortHandler) -> None:
        self._ui = ui
        self._model = data
        self._serialConnection = robotConnection

    def populate_callbacks(self):
        self._ui._pidTuner._callbacks[PIDTuningCallbackIndex]["Send"] = self.send
        
    def send(self, p : float, i : float, d : float):
        buffer = f"Set-pid [{p},{i},{d}]"
        self._serialConnection.send(buffer)

    
    