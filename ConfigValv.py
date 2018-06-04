from tkinter import *
from tkinter.messagebox import showinfo
from tkinter.commondialog import Dialog
from tkinter.filedialog import *
import subprocess
import sys
import time
from subprocess import Popen
import serial

master = Tk();
master.title('Válvula Link')
con=0
master.iconbitmap('icon.ico')
##########################################Scan
def scan(puertos = 255, inicio=True):
 dispositivos_serie = []
 if inicio:
  print ("Escaneando "+str(puertos)+" puertos serie:")
  for i in range(puertos):
   if inicio:
    sys.stdout.write("puerto %d: " % i)
    sys.stdout.flush()
    f="COM"+str(i)
    print (f)
    try:
          s = serial.Serial(f, 9600)
          time.sleep(.3)
          if inicio: print ("OK --> %s" % s.portstr)
          dispositivos_serie.append((i, s.portstr))
          try:
              while s.in_waiting and con==0:
                det=s.read(1)
                print (det);
                if det==b'c':
                  con==1
                  global esp
                  esp=s.portstr
                  print(esp)
                  break;
          except:
                print("NOx2") 
                s.close()
    except:
          if inicio: print ("NO")
          pass
 return dispositivos_serie
 

##########################################Conectar
def cone():
    print(esp)
    global e
    e = serial.Serial(esp, 9600)
    e.write(b'c')
    det=e.read(30)
    print(det)
##########################################Enviar
def enviar():
    print("Información", showinfo("Información", "El envío puede tardar hasta 1 minuto"))
    scan();
    cone();
    nombre=ssid.get()
    contra=pasw.get()
    datos=nombre+"["+contra+"]" 
    print(datos)
    e.write(datos.encode())
    ex=e.read(25)
    print(ex)
    re=e.read(37)
    print(re)
    print("Información", showinfo("Información", "Envío exitoso desconecte el dispositivo"))
##########################################Interfaz
topf = Frame(master)
topf.pack()
label3 = Message(topf,width=150, text="          ")
label3.pack(side=TOP)
label15 = Message(master,width=150, text="          ")
label15.pack(side=TOP)
label16 = Message(topf,width=300, text="Digite antes de enviar")
label16.pack(side=TOP)
botf = Frame(master)
botf.pack()
midf = Frame(topf)
midf.pack(side=LEFT)
label1 = Message(midf,width=150, text="Digite el Nombre de Red")
label1.pack(side=TOP)
ssid = Entry(midf,width=25)
ssid.pack(side=TOP)
label2 = Message(midf,width=150, text="Digite la contraseña")
label2.pack(side=TOP)
pasw = Entry(midf,width=25)
pasw.pack(side=TOP)
label4 = Message(topf,width=150, text="  ")
label4.pack(side=LEFT)
botonenv = Button(topf,text="Enviar",width=25,height=3,command=enviar)
botonenv.pack(side=TOP)
botonsal = Button(topf,text="Salir",width=25,height=3,command=master.destroy)
botonsal.pack(side=RIGHT)
master.mainloop()
