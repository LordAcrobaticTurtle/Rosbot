
import view
import ui_elements.ui_command_sidebar
from controllers.serial_port_handler import SerialPortHandler

class SidebarHandler():
    def __init__(self, ui : view.View, serialConnection : SerialPortHandler) -> None:
        self._ui = ui
        self._serialConnection = serialConnection

    def populate_callbacks(self) -> None:
        self._ui._callbacks[ui_elements.ui_command_sidebar.CommandSideBarCallbackIndex]["Begin"] = self._serialConnection.send
        self._ui._callbacks[ui_elements.ui_command_sidebar.CommandSideBarCallbackIndex]["Standby"] = self._serialConnection.send
        self._ui._callbacks[ui_elements.ui_command_sidebar.CommandSideBarCallbackIndex]["Calibrate"] = self._serialConnection.send
        self._ui._callbacks[ui_elements.ui_command_sidebar.CommandSideBarCallbackIndex]["Reset-IMU"] = self._serialConnection.send
        self._ui._callbacks[ui_elements.ui_command_sidebar.CommandSideBarCallbackIndex]["Sensor-Verification"] = self._serialConnection.send