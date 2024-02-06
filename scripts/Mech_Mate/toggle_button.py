import tkinter as tk

class ToggleButton():
    def create(self, tkRootElement, on_image_path, off_image_path, func_callback):
        self.baseFrame = tk.Frame(tkRootElement)
        self.isOn = False
        
        self.on_image = tk.PhotoImage(on_image_path)
        self.off_image = tk.PhotoImage(off_image_path)
        

        self.label = tk.Label(self.baseFrame, text="Off")
        self.label.grid(column=0, row=0)
        self.button = tk.Button(self.baseFrame, bd=0, command=func_callback, background="green")
        self.button.grid(column=0, row=0)
        self._callbackFunc = func_callback

    def buttonClicked(self):
        self.switch()
        self._callbackFunc()

    def switch(self):
        if self.isOn:
            self.button.config(image=self.on_image)
            self.label.config(text="OFF", fg='grey')
            self.isOn = False
        else:
            self.button.config(image=self.off_image)
            self.label.config(text="ON", fg='green')
            self.isOn = True    

    def isActive(self):
        return self.isOn

    def getFrame(self):
        return self.baseFrame
