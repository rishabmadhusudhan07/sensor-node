# ESP32 Low-Power Weather Node
A custom ESP32-based weather sensor node targeting six months of battery life and less than 50 µA system current during deep sleep. The project was motivated by gaps observed while mapping environmental data across the Pacific Northwest during a National Weather Service internship.

Planned firmware will use ESP-IDF and statically allocated FreeRTOS tasks for sensor sampling, local logging, wireless transmission, and deep-sleep control. Later revisions may add transmission integrity checks and adaptive sampling based on changing weather conditions.

Status: In development. The initial schematic and PCB layout are complete but not yet been assembled. BME280 I2C communication is being tested. Chip ID was verified. Compensated temperature/pressure/humidity values are in progress. Currently being tested on a XIAO ESP32-S3. [Project Status](#project-status).

---
 
## Why this project
 
A low-cost, low-power sensor node could make denser local observations more practical. This project explores the engineering requirements for that type of device.

## Hardware
The following values are estimates based on component datasheets and have not yet been verified on assembled hardware.

| Component | Role | Sleep current |
|---|---|---|
| ESP32-WROOM-32E-N4 | MCU + WiFi | ~10 µA (RTC timer + memory retention) |
| BME280 (through-hole breakout, soldered on) | Temp / humidity / pressure, forced mode | ≥0.1 µA sensor only; breakout overhead TBD |
| BH1750FVI-TR (bare IC, reflowed) | Ambient light / solar-charge proxy | 1 µA |
| TPB4056A20-DFGR | LiPo charge management | <2 µA |
| RT9080-33GJ5 | 3.3V LDO regulator, 600 mA max | ~2 µA |
 
Target: **<50 µA average system sleep current.**

## Key design decisions
 
**ESP-IDF over Arduino.** ESP-IDF was selected to provide direct control over FreeRTOS task creation, static memory allocation, peripheral configuration, and ESP32 sleep/wake behavior.
 
**LDO instead of a buck-boost regulator.** An LDO offers a simpler design and lower quiescent current, though reduced usable battery voltage as the LiPo approaches the regulator’s dropout region. The RT9080-33GJ5 was selected for its approximately 2 µA quiescent current and 600 mA output rating. Did not use MCP1700 because of its 250 mA rating, insufficient margin for high current draw during WiFi transmission..
 
**No onboard current-sense IC.** The system must measure sleep current in the microamps as well as much larger Wi-Fi transmission currents. Instead, test points are placed across a removable jumper. The jumper can be replaced by a larger resistor for controlled sleep-current measurements or a low-value one for active-current measurements. An INA219 did not provide sufficient dynamic range for both measurements with a single shunt, while the INA228 drew too much current.
 
**Adaptive sampling instead of a fixed interval.** The planned firmware will have  a rolling window of pressure measurements, estimate the rate of change, and adjust the next sampling interval. Faster changes, such as a cold front or wind shift, will mean more frequent sampling; stable conditions mean longer sleep intervals.
 
 
**Fully static FreeRTOS allocation.** Every task and queue will be created statically to keep predictable memory usage.


## Repo structure
 
```
├── firmware/       # ESP-IDF project
├── hardware/       # KiCad schematic + PCB layout
└── README.md
```

## Project status
 
| Phase                                   | Status      |
| --------------------------------------- | ----------- |
| Rev A schematic and PCB layout          | Complete    |
| BME280 breadboard bring-up              | In progress |
| PCB fabrication and bring-up            | Not started |
| FreeRTOS task implementation            | Not started |
| Data integrity and Wi-Fi retry pipeline | Not started |
| Power characterization                  | Not started |
| Field testing                           | Not started |

## License
 
MIT — see [LICENSE](LICENSE).
