#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// --- Fuer MAAS KAS-3-PL
// --- Optimal: MH-ET LIVE ESP32MiniKit
// --- WLAN KONFIGURATION (In Ihr bestehendes Heim-WLAN eintragen) ---
const char* ssid = "SSID";
const char* password = "PASSWD";
unsigned long letzteWlanPruefung = 0;
const unsigned long wlanIntervall = 10000;

// --- Webserver Port) ---
WebServer server(5006); 
Servo meinServo;

const int servoPin = 18; 

// --- WINKEL-EINSTELLUNGEN ---
const int winkelAnt1 = 50;   // Position Links
const int winkelAnt2 = 91;   // Position Mitte
const int winkelAnt3 = 143;  // Position Rechts

// Globale Variable speichert den aktuellen Status (1 = Ant1, 2 = Ant2, 3 = Ant3)
int aktuellePosition = 2; // Startet standardmäßig in der Mitte (Antenne 2)

// Funktion zur Generierung der Webseite mit dynamischem Status und Auto-Refresh-Skript
String generiereWebseite() {
  String html = "<!DOCTYPE html><html><head><meta charset='utf-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Antennen Switch Live-Status</title>";
  html += "<style>body{font-family:Arial, sans-serif; text-align:center; margin-top:40px; background:#1e1e24; color:#fff;}";
  html += ".status-box{background:#2a2a35; padding:15px; border-radius:8px; display:inline-block; margin-bottom:20px; border:1px solid #444; font-size:18px;}";
  html += "button{width:240px; padding:20px; margin:10px; font-size:18px; font-weight:bold; cursor:pointer; border:none; border-radius:8px; transition: 0.2s;}";
  html += "button:hover{transform: scale(1.03);}";
  html += ".btn-off{background:#444; color:#aaa;}";
  html += ".btn-ant1-on{background:#e74c3c; color:white; box-shadow: 0 0 15px #e74c3c;}";
  html += ".btn-ant2-on{background:#f39c12; color:white; box-shadow: 0 0 15px #f39c12;}";
  html += ".btn-ant3-on{background:#2ecc71; color:white; box-shadow: 0 0 15px #2ecc71;}";
  html += "</style>";
  
  // JavaScript für das automatische Live-Update (AJAX) ohne Seiten-Reload
  html += "<script>";
  html += "setInterval(function() {";
  html += "  fetch('/status').then(response => response.text()).then(pos => {";
  html += "    pos = parseInt(pos);";
  // Aktualisiere Textbox
  html += "    var text = 'Mitte: ANTENNE 2 (Aktiv)'; color = '#f39c12';";
  html += "    if(pos == 1) { text = 'Links: ANTENNE 1 (Aktiv)'; color = '#e74c3c'; }";
  html += "    if(pos == 3) { text = 'Rechts: ANTENNE 3 (Aktiv)'; color = '#2ecc71'; }";
  html += "    document.getElementById('statustext').innerHTML = text;";
  html += "    document.getElementById('statustext').style.color = color;";
  // Aktualisiere Buttons
  html += "    document.getElementById('b1').className = (pos == 1) ? 'btn-ant1-on' : 'btn-off';";
  html += "    document.getElementById('b2').className = (pos == 2) ? 'btn-ant2-on' : 'btn-off';";
  html += "    document.getElementById('b3').className = (pos == 3) ? 'btn-ant3-on' : 'btn-off';";
  html += "  });";
  html += "}, 1000);"; // Intervall: Jede Sekunde (1000ms) prüfen
  html += "</script>";
  
  html += "</head><body>";
  html += "<h1>SRAS KAS-3-PL</h1>";
  
  // Dynamischer Status-Container
  html += "<div class='status-box'>Aktueller Status: <strong><span id='statustext'>";
  if(aktuellePosition == 1)      html += "Links: ANTENNE 1 (Aktiv)";
  else if(aktuellePosition == 3) html += "Rechts: ANTENNE 3 (Aktiv)";
  else                           html += "Mitte: ANTENNE 2 (Aktiv)";
  html += "</span></strong></div><br>";

  // Buttons mit IDs für das JavaScript-Live-Update
  html += "<button id='b1' class='" + String(aktuellePosition == 1 ? "btn-ant1-on" : "btn-off") + "' onclick=\"fetch('/set?pos=1')\">ANTENNE 1 (Links)</button><br>";
  html += "<button id='b2' class='" + String(aktuellePosition == 2 ? "btn-ant2-on" : "btn-off") + "' onclick=\"fetch('/set?pos=2')\">ANTENNE 2 (Mitte)</button><br>";
  html += "<button id='b3' class='" + String(aktuellePosition == 3 ? "btn-ant3-on" : "btn-off") + "' onclick=\"fetch('/set?pos=3')\">ANTENNE 3 (Rechts)</button>";
  
  html += "</body></html>";
  return html;
}


void schaltePosition(int position) {
  aktuellePosition = position; 
  
  // 1. Servo elektronisch aktivieren
  meinServo.attach(servoPin);
  delay(50); // Kurze Pause, damit das Signal stabil anliegt
  
  // 2. Zielposition anfahren
  if (position == 1) {
    meinServo.write(winkelAnt1);
    Serial.println("Schalter auf ANTENNE 1 (Links) gedreht.");
  } else if (position == 3) {
    meinServo.write(winkelAnt3);
    Serial.println("Schalter auf ANTENNE 3 (Rechts) gedreht.");
  } else {
    meinServo.write(winkelAnt2);
    Serial.println("Schalter auf ANTENNE 2 (Mitte) gedreht.");
  }
  
  // 3. Dem Servo Zeit geben, um die Position physisch zu erreichen
  delay(800); // 800ms reichen für den DT996 locker aus
  
  // 4. Servo komplett abschalten (Signal trennen) -> Absolut lautlos & zitterfrei!
  meinServo.detach();
  Serial.println("Servo abgeschaltet (schläft).");
}

void setup() {
  Serial.begin(115200);
  
  meinServo.attach(servoPin);
  meinServo.write(winkelAnt2); // Start auf Antenne 2 (Mitte)

  // ====================================================================
  // STATISCHE IP-ADRESSE (Bei Bedarf die "//" entfernen)
  // ====================================================================
  // IPAddress statischeIP(192, 168, 178, 200); 
  // IPAddress gateway(192, 168, 178, 1);       
  // IPAddress subnetz(255, 255, 255, 0);       
  // IPAddress dns(192, 168, 178, 1);           
  // if (!WiFi.config(statischeIP, gateway, subnetz, dns)) {
  //   Serial.println("Statische IP-Konfiguration fehlgeschlagen!");
  // }
  // ====================================================================

  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  Serial.print("Verbinde mit WLAN");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nErfolgreich verbunden!");
  Serial.print("IP-Adresse des Antennenschalters: ");
  Serial.println(WiFi.localIP());

  // ROUTE 1: Weboberfläche im Browser
  server.on("/", []() {
    server.send(200, "text/html", generiereWebseite());
  });

  // ROUTE 2: Hintergrund-Abfrage für den Live-Status (wird vom JavaScript genutzt)
  server.on("/status", []() {
    server.send(200, "text/plain", String(aktuellePosition));
  });

  // ROUTE 3: API-Schnittstelle für curl und AE5900_Remote_V2
  server.on("/set", []() {
    if (server.hasArg("pos")) {
      int pos = server.arg("pos").toInt();
      schaltePosition(pos);
      server.send(200, "text/plain", "OK - Position geaendert auf Antenne " + String(pos));
    } else {
      server.send(400, "text/plain", "Fehler: Parameter 'pos' fehlt (1, 2 oder 3 nutzen)");
    }
  });

  server.begin();
}

void loop() {
  server.handleClient();

  unsigned long aktuelleZeit = millis();

  if (aktuelleZeit - letzteWlanPruefung >= wlanIntervall) {
    letzteWlanPruefung = aktuelleZeit;
    
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("[WLAN] Verbindung verloren! Versuche automatischen Reconnect...");

      WiFi.disconnect();
      WiFi.begin();

    }
  }
  
  delay(2);
}
