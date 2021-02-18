# Arduino
Questo progetto permette Di controllare 6 pin Digitali di Arduino Uno con una semplice intefccia web HTTP

HTML Client


![Image](https://www.thomas05.tk/Arduino/html.PNG)


Ogni volta che vieni premuto lo swich viene inviato un GET (LED1=1&LED2=0&LED3=0&LED4=0&LED5=0&LED6=0&) con le richieste.
Poi per aggiornare lo status degli swich per ogni client vieni ogni secondo mandato da arduino un file XML con tutti gli status che poi sul cliet vengono tradotti da uno script nei vari status.


Questo codice è completamnete libero 

