from tkinter import *
from tkinter import ttk
import serial
import serial.tools.list_ports
import datetime
import threading




isAppRunning = True

#for printing debugging messages in console
dbg = 0

gRoot = Tk()
gRoot.geometry("1280x720")
gRoot.title("Serial Console")
gRoot.columnconfigure(0,weight=1)
gRoot.rowconfigure(0,weight=1)
commSettingsFrame = ttk.LabelFrame(gRoot,text="Connection Setting",padding=10)
commSettingsFrame.grid(column=1,row=1, sticky=(W,E))

#Frame for COM messages
gFrame21 = ttk.Frame(gRoot,padding=10)
gFrame21.grid(column=2,row=1, sticky=(W))



for x in range(10):
    commSettingsFrame.columnconfigure(x,weight = x)
    commSettingsFrame.rowconfigure(x,weight = x)
    
serialConsoleLabel=ttk.Label(commSettingsFrame, text = "Serial Console")
serialConsoleLabel.grid(column=2,row=0)
commSettingsFrame.rowconfigure(0,weight=2)

"""
Com Port List
"""
#Start
ports = serial.tools.list_ports.comports()
com_port_list = [com[0] for com in ports]
com_port_list.insert(0,"Select an Option")
if dbg == 1:
    print(com_port_list)

#END
com_value_inside = StringVar()
baud_value_inside = StringVar()
baud_menu = ttk.OptionMenu(commSettingsFrame,baud_value_inside,"select baud rate","9600",
                           '19200','28800','38400','57600','76800',
                           '115200','128000','153600','230400','256000','460800','921600')
baud_value_inside.set('9600')
baud_menu.grid(column=3, row=1, sticky = (E))

def com_port_list_update():
    global ports
    global com_port_list
    ports = serial.tools.list_ports.comports()
    com_port_list = [com[0] for com in ports]
    com_port_list.insert(0,"Select an Option")
    if dbg == 1:
        print(com_port_list)
    com_menu = ttk.OptionMenu(commSettingsFrame,com_value_inside,*com_port_list)
    com_menu.grid(column=2, row=1, sticky = (E))
    

def serial_print():
    global serFlag
    global serialPort
    global counter1
    x =""
    #print("Task 1 assigned to thread: {}".format(threading.current_thread().name))
    #print("ID of process running task 1: {}".format(os.getpid()))
    if(serFlag):
        if(serialPort.in_waiting>0):
            try:
                x = serialPort.readline(serialPort.in_waiting)
                y = f"{counter1}: {datetime.datetime.now()} -> {x.decode()}"
                # y = str(counter1)+": "+str(datetime.datetime.now())+" -> "+str(x.decode())
                Lb2.insert(counter1, str(y))
                Lb2.see("end")
                #print (counter1)
                counter1 += 1
                #gFrame.after(100,serial_print)
            except:
                pass
        serialPort.flush()
        commSettingsFrame.after(100,serial_print)


serialPort = serial.Serial()
serFlag = 0
def serial_connect(com_port,baud_rate):
    global serialPort
    serialPort.baudrate = baud_rate
    serialPort.port = com_port
    serialPort.timeout = 1
    serialPort._xonxoff=1
    serialPort.bytesize=serial.EIGHTBITS
    serialPort.parity=serial.PARITY_NONE
    serialPort.stopbits=serial.STOPBITS_ONE
    serialPort.open()
    global serFlag
    serFlag = 1
    
    t1 = threading.Thread(target = serial_print, args = (), daemon=1)
    t1.start()
    #t1.join()
    """
    P1 = multiprocessing.Process(target = serial_print, args=())
    P1.start()
    P1.join()
    """
    #serial_print()
counter1 = 0
    
def serial_close():
    global serialPort
    global serFlag
    serFlag = 0
    serialPort.close()
    
def submit_value():
    if dbg == 1:
        print("selected option: {}".format(com_value_inside.get()))
        print(" Baud Rate {}".format(baud_value_inside.get()))
    serial_connect(com_value_inside.get(),baud_value_inside.get())

# Populate gFrame21
Lb2 = Listbox(gFrame21, width = 100, xscrollcommand = 1)
Lb2.grid(column=1, row = 1, sticky = W+E)
Sb2 = ttk.Scrollbar(gFrame21,orient = 'vertical')
Sb2.config(command=Lb2.yview)
Sb2.grid(column = 2,row =1, sticky=N+S)
Sb2v = ttk.Scrollbar(gFrame21,orient = 'horizontal')
Sb2v.grid(column = 1,row =2, sticky=W+E)
Sb2v.config(command = Lb2.xview)
Lb2.configure(xscrollcommand = Sb2v.set, yscrollcommand = Sb2.set)

# Add serial input box
def serial_input_keypress_enter(event):
    inputString = serialInputString.get() + "\r\n"
    encodedString = inputString.encode('utf')
    serialPort.write(encodedString)
    serialInputString.set("")

serialInputString = StringVar()
serialInputBox = ttk.Entry(gFrame21, textvariable=serialInputString)
serialInputBox.grid(column=1, row=2, sticky= W+E)
serialInputBox.bind("<Return>", serial_input_keypress_enter)


def clear_listbox():
    Lb2.delete(0,END)
    
subBtn = ttk.Button(commSettingsFrame,text="Connect",command = submit_value)
subBtn.grid(column=4,row=1, sticky = (E))

RefreshBtn = ttk.Button(commSettingsFrame,text="Get List",command = com_port_list_update)
RefreshBtn.grid(column=2,row=2, sticky = (E))

closeBtn = ttk.Button(commSettingsFrame,text="Disconnect",command = serial_close)
closeBtn.grid(column=4,row=2, sticky = (E))

clearBtn = ttk.Button(commSettingsFrame,text="Clear Messages",command = clear_listbox)
clearBtn.grid(column=3,row=2, sticky = (E))

def donothing():
   filewin = Toplevel(gRoot)
   button = Button(filewin, text="Do nothing button")
   button.pack()

def About_me():
   filewin = Toplevel(gRoot)
   button = Button(filewin, text="Quit", command = filewin.destroy)
   button.pack()

menubar = Menu(gRoot)
filemenu = Menu(menubar, tearoff=0)
filemenu.add_command(label="New", command=donothing)
filemenu.add_command(label="Open", command=donothing)
filemenu.add_command(label="Save", command=donothing)
filemenu.add_command(label="Save as...", command=donothing)
filemenu.add_command(label="Close", command=donothing)

filemenu.add_separator()

filemenu.add_command(label="Exit", command=gRoot.quit)
menubar.add_cascade(label="File", menu=filemenu)
editmenu = Menu(menubar, tearoff=0)
editmenu.add_command(label="Undo", command=donothing)

editmenu.add_separator()

editmenu.add_command(label="Cut", command=donothing)
editmenu.add_command(label="Copy", command=donothing)
editmenu.add_command(label="Paste", command=donothing)
editmenu.add_command(label="Delete", command=donothing)
editmenu.add_command(label="Select All", command=donothing)

menubar.add_cascade(label="Edit", menu=editmenu)
helpmenu = Menu(menubar, tearoff=0)
helpmenu.add_command(label="Help Index", command=donothing)
helpmenu.add_command(label="About...", command=donothing)
menubar.add_cascade(label="Help", menu=helpmenu)
menubar.add_separator()
menubar.add_command(label = "Quit", command = gRoot.destroy)

gRoot.config(menu=menubar)
gRoot.mainloop()
