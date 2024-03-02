import tkinter as tk

from model import Model
from view import View
from controller import Controller


class App(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title('MechMate 🤖')
        print("Application start")
        # create a model
        self.model = Model()

        # create a view and place it on the root window
        self.view = View(self)
        self.protocol("WM_DELETE_WINDOW", self.view.close)
        self.view.grid(row=0, column=0, padx=10, pady=10)        
        # Make full screen
        # self.state('zoomed')

        # create a controller
        self.controller = Controller(self.model, self.view)

        # set the controller to view
        
        self.view.create_window()
        
if __name__ == '__main__':
    app = App()
    app.mainloop() 
    