# ![Image](https://raw.githubusercontent.com/thomasz05/site/main/Arduino/arduino_22429.ico) Progetto Web server con sd e ajax
Questo progetto permette Di controllare 6 pin Digitali di Arduino Uno con una semplice intefccia web HTTP

HTML Client


![Image](https://raw.githubusercontent.com/thomasz05/site/main/Arduino/html.PNG)


Ogni volta che vieni premuto lo swich viene inviato un GET (LED1=1&LED2=0&LED3=0&LED4=0&LED5=0&LED6=0&) con le richieste.
Poi per aggiornare lo status degli swich per ogni client viene ogni secondo mandato da arduino un file XML con tutti gli status che poi sul cliet vengono tradotti da uno script nei vari status.


Questo codice è completamnete libero 

