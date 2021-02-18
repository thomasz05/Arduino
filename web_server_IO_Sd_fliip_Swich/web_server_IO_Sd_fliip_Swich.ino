#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>
//dimensione del buffer utilizzato per acquisire le richieste HTTP
#define REQ_BUF_SZ   60

// Indirizzo MAC per lo Ethernet shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 59); // indirizzo IP dell'arduino
EthernetServer server(80);  // creare un server sulla porta 80
File webFile;               // il file della pagina web sulla scheda SD
char HTTP_req[REQ_BUF_SZ] = {0}; //richiesta HTTP memorizzata nel buffer come stringa terminata da null
char req_index = 0;              // index nel buffer HTTP_req
boolean LED_state[6] = {0};      // memorizza gli stati dei LED

void setup()
{
     // disabilitare il chip Ethernet
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
    
    Serial.begin(9600);       // per il debug
    
    // inizializza la scheda SD
    Serial.println("Inizializzazione della scheda SD...");
    if (!SD.begin(4)) {
        Serial.println("ERRORE - Inizializzazione della scheda SD non riuscita!");
        return;    // init fallito
    }
    Serial.println("SUCCESSO - Scheda SD inizializzata.");
    // check for index.htm file
    if (!SD.exists("index.htm")) {
        Serial.println("ERRORE - Impossibile trovare il file index.htm!");
        return;  // non riesco a trovare il file index
    }
    Serial.println("SUCCESS - Found index.htm file.");
    
    pinMode(3, OUTPUT);// LED1 
    pinMode(5, OUTPUT);// LED2
    pinMode(6, OUTPUT);// LED3
    pinMode(7, OUTPUT);// LED4
    pinMode(8, OUTPUT);// LED5
    pinMode(9, OUTPUT);// LED6
    
    Ethernet.begin(mac, ip);  //  inizializza il dispositivo Ethernet
    server.begin();           // inizia ad ascoltare i client
    Serial.print("il server è sul ip:   "); //da la ip su cui è il server
    Serial.println(Ethernet.localIP());
}

void loop()
{
    EthernetClient client = server.available();  // prova a ottenere il client
    if (client) {  // got client?
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // dati del client disponibili per la lettura
                char c = client.read(); // legge 1 byte (carattere) dal client
                // limitare la dimensione della richiesta HTTP ricevuta memorizzata
                // buffer della prima parte della richiesta HTTP nell'array HTTP_req (stringa)
                // lasciare l'ultimo elemento nell'array come 0 su null terminate string (REQ_BUF_SZ - 1)
                
                if (req_index < (REQ_BUF_SZ - 1)) {
                    HTTP_req[req_index] = c;          // salva il carattere di richiesta HTTP
                    req_index++;
                }
                // l'ultima riga della richiesta del client è vuota e termina con \ n
                // risponde al client solo dopo l'ultima riga ricevuta
                if (c == '\n' && currentLineIsBlank) {
                    // invia un'intestazione di risposta http standar
                    client.println("HTTP/1.1 200 OK");
                    // il resto dell'intestazione segue di seguito, a seconda di if
                    // è richiesta la pagina web o la pagina XML
                    // Richiesta Ajax - invia file XML
                    if (StrContains(HTTP_req, "ajax_inputs")) {
                        // invia il resto dell'intestazione HTTP
                        client.println("Content-Type: text/xml");
                        client.println("Connection: keep-alive");
                        client.println();
                        SetLEDs();
                        // invia file XML contenente gli stati di LEDs
                        XML_response(client);
                    }
                    else {  // richiesta della pagina web
                        // invia il resto dell'intestazione HTTP
                        client.println("Content-Type: text/html");
                        client.println("Connection: keep-alive");
                        client.println();
                        // invia pagina web
                        webFile = SD.open("index.htm");        // apri il file su Sd della pagina web
                        if (webFile) {
                            while(webFile.available()) {
                                client.write(webFile.read()); // invia la pagina web al client
                            }
                            webFile.close();
                        }
                    }
                    // visualizza la richiesta HTTP ricevuta sulla porta seriale
                    Serial.print(HTTP_req);
                    // reimposta l'indice del buffer e tutti gli elementi del buffer a 0
                    req_index = 0;
                    StrClear(HTTP_req, REQ_BUF_SZ);
                    break;
                }
                // ogni riga di testo ricevuta dal client termina con \ r \ n
                if (c == '\n') {
                    // ultimo carattere sulla riga del testo ricevuto
                    // inizia una nuova riga con il carattere successivo letto
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    // è stato ricevuto un carattere di testo dal client
                    currentLineIsBlank = false;
                }
            } // finine if (client.available())
        } // finine while (client.connected())
        delay(1);      // dare al browser web il tempo di ricevere i dati
        client.stop(); // chiudere la connessione
    } // fine if (client)
}

// controlla se la richiesta HTTP ricevuta sta accendendo / spegnendo i LED
// salva anche lo stato dei LED
void SetLEDs(void)
{
  // LED 1 (pin 3)
    if (StrContains(HTTP_req, "LED1=1")) {
        LED_state[0] = 1;  //salva lo stato del LED1
        digitalWrite(3, HIGH);
    }
    else if (StrContains(HTTP_req, "LED1=0")) {
        LED_state[0] = 0;  //salva lo stato del LED1
        digitalWrite(3, LOW);
    }
    // LED 2 (pin 5)
    if (StrContains(HTTP_req, "LED2=1")) {
        LED_state[1] = 1;  //salva lo stato del LED2
        digitalWrite(5, HIGH);
    }
    else if (StrContains(HTTP_req, "LED2=0")) {
        LED_state[1] = 0;  //salva lo stato del LED2
        digitalWrite(5, LOW);
    }
    // LED 3 (pin 6)
    if (StrContains(HTTP_req, "LED3=1")) {
        LED_state[2] = 1;  //salva lo stato del LED3
        digitalWrite(6, HIGH);
    }
    else if (StrContains(HTTP_req, "LED3=0")) {
        LED_state[2] = 0;  //salva lo stato del LED3
        digitalWrite(6, LOW);
    }
    // LED 4 (pin 7)
    if (StrContains(HTTP_req, "LED4=1")) {
        LED_state[3] = 1;  //salva lo stato del LED4
        digitalWrite(7, HIGH);
    }
    else if (StrContains(HTTP_req, "LED4=0")) {
        LED_state[3] = 0;  //salva lo stato del LED4
        digitalWrite(7, LOW);
    }
    // LED 5 (pin 8)
    if (StrContains(HTTP_req, "LED5=1")) {
        LED_state[4] = 1;  //salva lo stato del LED5
        digitalWrite(8, HIGH);
    }
    else if (StrContains(HTTP_req, "LED5=0")) {
        LED_state[4] = 0;  //salva lo stato del LED5
        digitalWrite(8, LOW);
    }
    // LED 6 (pin 9)
    if (StrContains(HTTP_req, "LED6=1")) {
        LED_state[5] = 1;  //salva lo stato del LED6
        digitalWrite(9, HIGH);
    }
    else if (StrContains(HTTP_req, "LED6=0")) {
        LED_state[5] = 0;  // salva lo stato del LED6
        digitalWrite(9, LOW);
    }
}

// inviare il file XML con lo stato del LED
void XML_response(EthernetClient cl)
{
    int count;                 //usato dai cicli "for"
    
    cl.print("<?xml version = \"1.0\" ?>");
    cl.print("<inputs>");
     // LED1
    cl.print("<LED>");
    if (LED_state[0]) {
        cl.print("checked");
    }
    else {
        cl.print("unchecked");
    }
    cl.println("</LED>");
    
    // LED2
    cl.print("<LED>");
    if (LED_state[1]) {
        cl.print("checked");
    }
    else {
        cl.print("unchecked");
    }
     cl.println("</LED>");
    
    // LED3
    cl.print("<LED>");
    if (LED_state[2]) {
        cl.print("checked");
    }
    else {
        cl.print("unchecked");
    }
    cl.println("</LED>");
    
    // LED4
    cl.print("<LED>");
    if (LED_state[3]) {
        cl.print("checked");
    }
    else {
        cl.print("unchecked");
    }
    cl.println("</LED>");
    
    // LED5
    cl.print("<LED>");
    if (LED_state[4]) {
        cl.print("checked");
    }
    else {
        cl.print("unchecked");
    }
    cl.println("</LED>");
    
    // LED6
    cl.print("<LED>");
    if (LED_state[5]) {
        cl.print("checked");
    }
    else {
        cl.print("unchecked");
    }
    cl.println("</LED>");
    cl.print("</inputs>");
}

// imposta ogni elemento di str a 0 (cancella l'array)
void StrClear(char *str, char length)
{
    for (int i = 0; i < length; i++) {
        str[i] = 0;
    }
}

// cerca la stringa sfind nella stringa str
// restituisce 1 se viene trovata una stringa
// restituisce 0 se la stringa non viene trovata
char StrContains(char *str, char *sfind)
{
    char found = 0;
    char index = 0;
    char len;

    len = strlen(str);
    
    if (strlen(sfind) > len) {
        return 0;
    }
    while (index < len) {
        if (str[index] == sfind[found]) {
            found++;
            if (strlen(sfind) == found) {
                return 1;
            }
        }
        else {
            found = 0;
        }
        index++;
    }

    return 0;
}
