# Progetto Sistemi Operativi - Oscilloscopio
The project aims to create a small oscilloscope using analog pins on the arduino to then print its value on-screen using the GNUPlot library

 - [Linux](#Linux-project)
 - [Arduino](#arduino-project)

### Prerequisites
Install all prerequisites with
```bash
sudo apt install arduino arduino-mk gnuplot cmake make
```

## Linux project
The linux part of the project can be found in `linux/` folder.
you need to have cmake installed

```bash
mkdir build && cd build && cmake ..
make
```

Uso gnuplot per plottare la linea dell'oscilloscopio

USO plot '-' w l per tirare una linea 
poi chiudo con e e riapro

## Arduino project
Still a big huge TODO, it's inside `arduino/`

build it with
```
make main.hex
```

:3

 - Ricky