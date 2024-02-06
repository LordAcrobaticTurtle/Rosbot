# Import tkinter in the notebook
from tkinter import *

# Create an instance of window of frame
win = Tk()

# set Title
win.title('Toggle Button Demonstration')

# Set the Geometry
win.geometry("700x400")
win.resizable(0, 0)

# Create a variable to turn on the button initially
is_on = True

# Create Label to display the message
label = Label(win, text="Night Mode is On", bg="white", fg="black", font=("Poppins bold", 22))
label.pack(pady=20)

# Define our switch function
def button_mode():
   global is_on

   # Determine it is on or off
   if is_on:
      on_.config(image=off)
      label.config(text="Day Mode is On", bg="white", fg="black")
      is_on = False
   else:
      on_.config(image=on)
      label.config(text="Night Mode is On", fg="black")
      is_on = True

# Define Our Images
on = PhotoImage(file="record-button.png")
off = PhotoImage(file="stop-button.png")

# Create A Button
on_ = Button(win, bd=0, command=button_mode)
on_.pack(pady=50)

# Keep Running the window
win.mainloop()