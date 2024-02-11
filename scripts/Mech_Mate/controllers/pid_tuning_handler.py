
from controllers.serial_port_handler import SerialPortHandler
from ui_elements.ui_pid_tuning import PIDTuningCallbackIndex, PidTuning

class PIDTuningHandler():

    def __init__(self, ui : PidTuning, robotConnection : SerialPortHandler) -> None:
        self._ui = ui
        self._serialConnection = robotConnection

    def populate_callbacks(self):
        self._ui._callbacks[PIDTuningCallbackIndex]["Send"] = self.send
        
    def send(self, p : float, i : float, d : float):
        buffer = f"Set-pid [{p},{i},{d}]"
        self._serialConnection.send(buffer)

    
    