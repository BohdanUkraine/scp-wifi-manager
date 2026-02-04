# SCP-WIFI-MANAGER: Foundation Personnel Network Utility

## Welcome, New Foundation Personnel!

Congratulations on securing your position within the SCP Foundation. Your security and operational efficiency are paramount. This SCP-WIFI-MANAGER utility is an essential tool provided to secure your workstation from potential cognitive hazards, data breaches, and unauthorized network infiltrations that may be encountered through unsecured wireless vector sources.

**Installation of this utility is mandatory for all Foundation-issued laptops.**

## Operational Guide

### 1. Network Discovery
Scan the environment for local wireless anomalies. The UI utilizes a **proprietary signal font** (`scp-wifi-strength-font.ttf`) to render unique signal fidelity icons.
![Scan](readmeGifs/refreshWifiList.gif)

### 2. Secure Connection
Standard WPA2/RSN handshake protocols. 
![Connect](readmeGifs/connectToWifi.gif) ![Password](readmeGifs/tryConnectingWithPassword.gif)

### 3. Tactical Hotspot (SCP_Hotspot_Profile)
Deployment of a localized secure access point. The system automatically wipes the connection profile upon termination to ensure a zero-trace footprint.
![Settings](readmeGifs/hotspotSettings.gif)

### 4. Rapid Authentication (QR)
Bypass manual credential entry using encrypted visual handshakes.
![QR](readmeGifs/shareViaQrCode.gif)

---

### Technical Stack
* **Language:** C++ / Qt 6
* **Communication:** Asynchronous D-Bus (NetworkManager API)
* **Design:** Custom typography for telemetry visualization
