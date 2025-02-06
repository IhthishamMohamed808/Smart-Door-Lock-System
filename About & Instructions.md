**Smart Door Lock System**

**Overview**
This project is a **Smart Door Lock System** utilizing an **ESP32**, multiple sensors, and actuators to provide secure access control. The system integrates with **Blynk**, enabling remote monitoring and control via a mobile app. It features:  

- **Keypad-based authentication** for unlocking the door.  
- **Blynk Dashboard integration** to remotely monitor and control door status.  
- **Ultrasonic sensor** for detecting obstacles near the door.  
- **PIR motion sensor** to detect unauthorized access attempts.  
- **LDR (Light Dependent Resistor)** to automate LED control based on ambient light.  
- **Buzzer alert** for wrong password attempts.  
- **Manual push button** for physical unlocking.  

---

**Components Used**  

| Component                  | Purpose |
|----------------------------|---------|
| ESP32                      | Microcontroller to handle all operations |
| 4x4 Keypad                 | User authentication through a password |
| Servo Motor                | Acts as the door lock mechanism |
| HC-SR04 Ultrasonic Sensor  | Measures distance to detect obstacles |
| PIR Motion Sensor          | Detects motion near the keypad |
| LDR (Light Sensor)         | Monitors ambient light levels for LED control |
| LED                        | Indicates low-light conditions |
| Buzzer                     | Alerts for incorrect password attempts |
| Push Button                | Allows manual unlocking |
| Blynk IoT Platform         | Remote monitoring and control via a smartphone |

---

**Pin Configuration**  

| Component               | ESP32 Pin |
|-------------------------|----------|
| Keypad Rows (1-4)       | 19, 18, 17, 16 |
| Keypad Columns (1-4)    | 23, 22, 21, 5 |
| Servo Motor             | 33 |
| PIR Motion Sensor       | 13 |
| Ultrasonic TRIG         | 25 |
| Ultrasonic ECHO         | 26 |
| LDR Sensor              | 34 (Analog) |
| LED                     | 4 |
| Buzzer                  | 2 |
| Push Button             | 12 |

---

**Working Principle**  

**1. Keypad Authentication**  
- The user enters a **4-digit password** using the keypad.  
- If the password matches the stored one, the **servo motor unlocks the door**.  
- If incorrect, the **buzzer rings** for one second as an alert.  

**2. Motion Detection (PIR Sensor)**  
- If motion is detected near the keypad, a **notification is sent to the Blynk app**.  

**3. Ultrasonic Sensor**  
- Measures the distance from the door to detect obstacles.  
- If an object is **too close** when attempting to lock the door, it prevents locking and alerts the user.  

**4. Light Sensor (LDR) & LED Control**  
- If ambient light is **too low** (LDR reading < 200), the **LED turns ON**.  
- If light is sufficient, the **LED remains OFF**.  

**5. Manual Push Button**  
- Pressing the button toggles the **servo motor to unlock/lock** the door.  
- Ensures **manual access** in case of an emergency.  

**6. Blynk IoT Integration**  
- Remote access and monitoring via the **Blynk mobile app**.  
- **Virtual Pins Mapping:**  

| Blynk Virtual Pin | Function |
|------------------|----------|
| V1 | Displays Ultrasonic Sensor Distance |
| V2 | Displays Motion Detection Status |
| V3 | Shows Door Lock Status |
| V4 | Displays Light Level (LDR Sensor) |
| V5 | Allows Manual Door Control from App |

---

**How to Set Up**  

1. **Upload the Code**  
   - Install the **Blynk** and **ESP32 libraries** in Arduino IDE.  
   - Replace **BLYNK_AUTH_TOKEN** with your own from the Blynk app.  
   - Set your **Wi-Fi credentials** in **ssid[]** and **pass[]**.  
   - Upload the **Smart Door Lock System.cpp** (C++) file to the ESP32 board.  

2. **Blynk App Configuration**  
   - Create a **Blynk project** and get the **Auth Token**.  
   - Add widgets:  
     - **Gauge** for Ultrasonic Distance (V1).  
     - **Label** for Motion Status (V2).  
     - **Label** for Lock Status (V3).  
     - **Gauge** for Light Level (V4).  
     - **Button** for Manual Control (V5).  

3. **Power and Connect**  
   - Connect components as per the **Pin Configuration** table.  
   - Power the ESP32 via **USB or an external 5V power supply**.  

4. **Testing**  
   - Enter the password on the keypad to unlock the door.  
   - Check **Blynk app updates** for motion, light, and distance status.  
   - Use the **manual button** or **Blynk app** to unlock/lock the door.  

---

**Future Improvements**  
- Add **RFID or Fingerprint Scanner** for more security.  
- Integrate **face recognition** with a camera module.  
- Implement **remote alerts via SMS or email**.  

---

This project is a **simple yet effective smart door lock system** that enhances security using **IoT and multiple sensors**. 
