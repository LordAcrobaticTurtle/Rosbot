
from controllers.serial_port_handler import SerialPortHandler
from ui_elements.ui_verification_panel import SensorVerificationPanel

class SensorVerification():

    def __init__(self, ui : SensorVerificationPanel, robotConnection : SerialPortHandler) -> None:
        self._ui = ui
        self._serialConnection = robotConnection

    def populate_callbacks(self):
        self._ui._callbacks[SensorVerificationPanel]["Send"] = self.send
        
    def send(self, controlIndex : int,  throttle : float, time : float):
        buffer = f"Verify [{controlIndex},{throttle},{time}]"
        self._serialConnection.send(buffer)

    
    