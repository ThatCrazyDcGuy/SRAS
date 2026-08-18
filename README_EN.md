## SRAS (Simple Remote Antenna Switch)

A very simple remote antenna switch based on a servo, an ESP32 module, and a mechanical UHF antenna switch.

![SRAS](/picnvid/SRAS_VID1.GIF)
![SRAS](/picnvid/overview.png)
![SRAS](/picnvid/parts.png)

This project allows you to fully automatically control a mechanical 3-position antenna switch using a model servo. Operation is possible either via a responsive web interface with live status (AJAX) or via terminal commands (`curl`), making the system perfectly compatible with remote software such as *AE5900_Remote_V2*.

## ❓ THE "WHY"

Why didn't I build a board layout with relays, optocouplers, PL connectors, an RF-shielded enclosure, a filter, a control unit, and all that other stuff?
Because I've done it over and over again. Purely for the fun of tinkering.
But that's actually complete nonsense, as it's far too complicated.

Anyone can glue an ESP32 to a servo and connect the servo's shaft to a well-shielded, mechanical antenna switch, and there won't be any interference signals in or on the antenna cable.

## 🛠 Features
* **Three Switching Positions:** Supports Antenna 1 (Left), Antenna 2 (Center), and Antenna 3 (Right).

* **Live Status Website:** The web interface updates automatically in the background every second (no manual page reload required).

* **REST API for Terminal Commands:** Full compatibility with `curl` for easy integration into custom buttons in third-party software.

* **Optimized for ESP32 Mini:** Precisely tailored to the more compact profile (e.g., MH-ET LIVE D1 Mini).

## 🔌 Hardware Wiring

> ⚠️ **IMPORTANT:** A more powerful servo (e.g., with metal gears) draws a lot of current when switching. **Never** connect it directly to the ESP32's 5V supply, as this can damage or crash the microcontroller! Always use an external power source.

* **Servo Red (VCC):** Connect to the positive (+) terminal of an external 5V/6V power source (min. 2A).

* **Servo Brown/Black (GND):** Connect to the negative (-) terminal of the external power source **AND** to a `GND` pin of the ESP32 (common ground!).

* **Servo Orange/Yellow (Signal):** Connect to pin “GPIO 18” (or the corresponding PWM pin on your ESP32).

## 💻 Software Setup (Arduino IDE)

1. Install board support for the ESP32 using the Boards Manager.

2. Install the **ESP32Servo** library using the Library Manager.

3. Select **“MH-ET LIVE ESP32MiniKit”** (or alternatively, “ESP32 Dev Module”) as the board model.

4. Enter your Wi-Fi credentials in the code (“ssid” and “password”).

5. If necessary, adjust the servo angles (`angleAnt1`, `angleAnt2`, `angleAnt3`) in the code to match your mechanical detents.

6. Flash the code to the ESP32.


## ⌨️ Control via Terminal (Remote Software / curl)

Replace "<IP>" with the actual IP address of your ESP32 (displayed in the serial monitor at startup). The network port used is set to "5006" in the code.

* **Antenne 1 (left):**
  ```bash
  curl -s "http://<IP>:5006/set?pos=1"
  ```
* **Antenne 2 (middle):**
  ```bash
  curl -s "http://<IP>:5006/set?pos=2"
  ```
* **Antenne 3 (right):**
  ```bash
  curl -s "http://<IP>:5006/set?pos=3"
  ```
