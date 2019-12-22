# 2d-xmas-Tree-art
A DIY flat christmas decoration with a ATtiny25, 20 led´s and a coin cell battery 

## Differencies from the original project
- Converted to [PlatformIo](https://platformio.org/) project
- More convinient for building and setting fuses
- Added animation compressing script
- Significantly reduced memory footprint
- More default animations
- Reduced power consumption

## Overview

![Image from the 2D xmas tree](img/2dxmastree_gif.gif)

This is a only 2 dimensional pcb with all the components embedded into the 1,6mm thick pcb itself.
The components are placed with the help of castellation into the pcb. 

![Castellation with component](img/castellation_0805.PNG)

## PCB

The PCB design is made with KiCad 5.0.1

The PCB should be 1,6mm thick to hide the components inside. last time I ordered them from [DirtyPCbs](https://dirtypcbs.com/) as suggesed [@designer2k2](https://github.com/designer2k2). They handle castellation but you still need to clean metal contacts from bended metalization debris. Next time going to order with [JLCPCB](https://jlcpcb.com/) as I like more their bright solder mask color. To order PCB use prepared gerber archive [2D_xmas_tree01_gerber_complete.zip](https://raw.githubusercontent.com/kasedy/2d-xmas-tree-art/master/pcb-kicad/gerb/2D_xmas_tree01_gerber_complete.zip)

Most of the PCB is to give it the look of a tree or house, it uses the different color achievable by blending the different layers. Here is a good article on possible colors: https://hackaday.com/2018/06/11/ask-hackaday-what-color-are-your-pcbs/

## Partlist

|ID|#|Ref|Value|Footprint|RS|Digikey|Aliexpress|Img|
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
|BT|2|BT1-BT2|Battery Cell|CR2016|-|-|[CR2016](https://www.aliexpress.com/item/32865072812.html)|<img src="./img/partlist/battery.jpg" height="50">|
|C|1|C1|22µF|0805|790-0682|1276-6786-1-ND|[0805 22uF capacitor](https://www.aliexpress.com/item/32374775517.html)|<img src="./img/partlist/capacitor.jpg" height="50">|
|D|20|D1-D20|LED|0805|176-6117|516-1431-1-ND|[0805 LED](https://www.aliexpress.com/item/1206476683.html)|<img src="./img/partlist/led.jpg" height="50">|
|R|5|R1-R5|75|0805|-|RR12Q75DCT-ND|[0805 Resistor](https://www.aliexpress.com/item/32865947306.html)|<img src="./img/partlist/resistor.jpg" height="50">|
|R|1|R6|10k|0805|125-1189|RR12P10.0KDCT-ND|[0805 Resistor](https://www.aliexpress.com/item/32865947306.html)|<img src="./img/partlist/resistor.jpg" height="50">|
|SW|1|SW1|SW_SPDT|PCM12SMTR|793-1721|401-2016-1-ND|[MSK-12C02](https://www.aliexpress.com/item/32863424888.html)|<img src="./img/partlist/switch.jpg" height="50">|
|U|1|U1|ATTINY25-20SU|SOIC8|133-1611|ATTINY25-20SU-ND|-|<img src="./img/partlist/attiny.jpg" height="50">|

ATiny-45 and ATiny-85 will work as well as ATtiny-25 but ATtiny-25 is cheaper and has enough memory to fit the sketch. To program ATtiny microcontroller I suggest to get ISP programmer. For example and open-source [USB ASP](https://www.fischl.de/usbasp/).

## Create your own animation!

a helper to get the patterns graphically made: https://designer2k2.github.io/2d-xmas-tree/

![GUI demonstration](https://github.com/designer2k2/2d-xmas-tree/raw/master/img/animation.gif)

## make your own

see the wiki page on how to make it: https://github.com/designer2k2/2d-xmas-tree/wiki
