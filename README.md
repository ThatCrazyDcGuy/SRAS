## SRAS (simple remote antenna switch)
Ein sehr einfacher Fern-Antennenumschalter, basierend auf einem Servo, einem ESP32-Modul und einem mechanischen UHF-Antennenschalter

![SRAS](/picnvid/SRAS_VID1.GIF)
![SRAS](/picnvid/overview.png)
![SRAS](/picnvid/parts.png)

Dieses Projekt ermöglicht es, einen mechanischen 3-Stellungs-Antennenumschalter vollautomatisch über einen Modellbau-Servo zu steuern. Die Bedienung erfolgt wahlweise über eine responsive Weboberfläche mit Live-Status (AJAX) oder über Terminal-Befehle (`curl`), wodurch sich das System perfekt in Remote-Software wie *AE5900_Remote_V2* integrieren lässt.

## Projektbilder

 - Der Prototyp:
 
![SRAS](/picnvid/AECX05_proto.jpg)

 - Die Hassliebe:
 
Oder auch: Warum ich den CX05 nicht mag, aber für Bastelprojekte immer wieder kaufen würde.
So sieht das Ding original von innen aus und es macht mich traurig.

![SRAS](/picnvid/AXCX05_hate.jpg)

Wir räumen also mal auf:

![SRAS](/picnvid/AXCX05_clean.jpg)

Wir montieren:

![SRAS](/picnvid/AECX05_mount.jpg)

Und jetzt kennt Ihr den Plan:

![SRAS](/picnvid/AECX05_uknownow.jpg)

 - Der CX201:
 
Solltet Ihr den CX201 verwenden, kürzt im inneren die Feder für den Umschaltwiderstand. So spart Ihr euch auch einen stärkeren Servo.
Ihr könnt dem Umbau von oben aber auch direkt im CX201 übernehmen. Da ist es tatsächlich noch einfacher.

![SRAS](/picnvid/AECX201.jpg)

 - Der MAAS KAS-3-PL // Diamond CX310A :
 
Auch für dieses Modell habe ich einen passenden Arduino Sketch hochgeladen.

![SRAS](/picnvid/KAS-3-PL.jpg)


## ❓ DAS "WARUM"
Warum habe ich nicht ein Boardlayout mit Relays, Optokopplern, PL Steckern, einem HF-sicheren Gehäuse, Entstörfilter, einer Steuereinheit und all dem anderen Gedöns gebaut?
Weil ich das immer und immer wieder gemacht habe. Rein aus Spass am Basteln. 
Das ist aber eigentlich absoluter Schwachsinn, da es viel zu kompliziert gedacht ist.

Ein ESP32 an einen Servo kleben und die Achse des Servo mit einem bereits gut geschirmten, mechanischem Antennenswitch zu verbinden, bekommt jeder hin und es entsehen keinerlei Störsignale im und am Antennenkabel.

## 🛠 Features
* **Drei Schaltstufen:** Unterstützt Antenne 1 (Links), Antenne 2 (Mitte) und Antenne 3 (Rechts).
* **Live-Status-Webseite:** Die Weboberfläche aktualisiert sich im Hintergrund jede Sekunde automatisch (kein manueller Seiten-Reload nötig).
* **REST-API für Terminal-Befehle:** Volle Kompatibilität mit `curl` zur einfachen Einbindung in Custom-Buttons von Drittsoftware.
* **Optimiert für ESP32 Mini:** Exakt abgestimmt auf das platzsparendere Profil (z. B. MH-ET LIVE D1 Mini).

## 🔌 Hardware-Verkabelung

> ⚠️ **WICHTIG:** Ein starker Servo (z.B. mit Metallgetriebe) zieht beim Schalten viel Strom. Schließe ihn **niemals** direkt an die 5V-Versorgung des ESP32 an, da dies den Mikrocontroller beschädigen oder zum Absturz bringen kann! Nutze immer eine externe Stromquelle.

* **Servo Rot (VCC):** An den Pluspol (+) einer externen 5V/6V Stromquelle (mind. 2A).
* **Servo Braun/Schwarz (GND):** An den Minuspol (-) der externen Stromquelle **UND** an einen `GND`-Pin des ESP32 (gemeinsame Masse!).
* **Servo Orange/Gelb (Signal):** An den Pin `GPIO 18` (oder den entsprechenden PWM-Pin deines ESP32).

## 💻 Software-Einrichtung (Arduino IDE)

1. Installiere die Board-Unterstützung für den ESP32 über den Boardverwalter.
2. Installiere die Bibliothek **ESP32Servo** über den Bibliotheksverwalter.
3. Wähle als Board-Modell **"MH-ET LIVE ESP32MiniKit"** (oder alternativ "ESP32 Dev Module").
4. Trage deine WLAN-Zugangsdaten im Code ein (`ssid` und `password`).
5. Passe ggf. die Servowinkel (`winkelAnt1`, `winkelAnt2`, `winkelAnt3`) im Code an deine mechanische Rastung an.
6. Flashe den Code auf den ESP32.

## ⌨️ Steuerung via Terminal (Remote-Software / curl)

Ersetze `<IP>` durch die tatsächliche IP-Adresse deines ESP32 (wird beim Start im Seriellen Monitor angezeigt). Der verwendete Netzwerk-Port ist im Code auf `5006` eingestellt.

* **Antenne 1 (Links):**
  ```bash
  curl -s "http://<IP>:5006/set?pos=1"
  ```
* **Antenne 2 (Mitte):**
  ```bash
  curl -s "http://<IP>:5006/set?pos=2"
  ```
* **Antenne 3 (Rechts):**
  ```bash
  curl -s "http://<IP>:5006/set?pos=3"
  ```
