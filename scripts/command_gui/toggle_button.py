import tkinter as tk

class ToggleButton():
    def create(self, tkRootElement):
        self.baseFrame = tk.Frame(tkRootElement)
        self.isOn = False
        self.label = tk.Label(self.baseFrame, text="Demo switch off", fg='grey')
        self.label.grid(column=0, row=0)
        self.button = tk.Button(self.baseFrame, bd=0, command=self.switch)
        self.button.grid(column=0, row=0)

    def switch(self):
        if self.isOn:
            self.label.config(text="OFF", fg='grey')
            self.isOn = False
        else:
            self.label.config(text="ON", fg='green')
            self.isOn = True    

    def getFrame(self):
        return self.baseFrame
