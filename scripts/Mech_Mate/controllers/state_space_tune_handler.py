
from controllers.serial_port_handler import SerialPortHandler
from ui_elements.ui_state_space_tuning import StateSpaceTuning, StateSpaceTuningCallbackIndex

class StateSpaceTuningHandler():

    def __init__(self, ui : StateSpaceTuning, robotConnection : SerialPortHandler) -> None:
        self._ui = ui
        self._serialConnection = robotConnection

    def populate_callbacks(self):
        self._ui._callbacks[StateSpaceTuningCallbackIndex]["Send"] = self.send
        
    def send(self, pos : float,  vel : float, angpos : float, angvel : float):
        buffer = f"Set-ss-gain [{pos},{vel},{angpos},{angvel}]"
        self._serialConnection.send(buffer)

    
    