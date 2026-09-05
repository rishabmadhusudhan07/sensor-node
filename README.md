# ESP32 Low-Power Weather Node
A custom ESP32-based weather sensor node targeting six months of battery life and less than 50 µA system current during deep sleep. The project was motivated by gaps observed while mapping environmental data across the Pacific Northwest during a National Weather Service internship.

Planned firmware will use ESP-IDF and statically allocated FreeRTOS tasks for sensor sampling, local logging, wireless transmission, and deep-sleep control. Later revisions may add transmission integrity checks and adaptive sampling based on changing weather conditions.

Status: In development. The Rev A schematic and PCB layout are complete but have not yet been fabricated or electrically validated. BME280 breadboard testing is in progress. [Project Status](#project-status).

---
 
## Why this project
 
A low-cost, low-power sensor node could make denser local observations more practical. This project explores the engineering requirements for such a device.

## Hardware
The following values are preliminary estimates based on component datasheets and have not yet been verified on assembled hardware.

| Component | Role | Sleep current |
|---|---|---|
| ESP32-WROOM-32E-N4 | MCU + WiFi | ~10 µA (RTC timer + memory retention) |
| BME280 (through-hole breakout, soldered on) | Temp / humidity / pressure, forced mode | ≥0.1 µA sensor only; breakout overhead TBD |
| BH1750FVI-TR (bare IC, reflowed) | Ambient light / solar-charge proxy | 1 µA |
| TPB4056A20-DFGR | LiPo charge management | <2 µA |
| RT9080-33GJ5 | 3.3V LDO regulator, 600 mA max | ~2 µA |
| R1 (swappable 0 Ω sense resistor) | Bench current-characterization point | ~0 µA (passive) |
 
Target: **<50 µA average system sleep current.**

## Key design decisions
 
**ESP-IDF over Arduino.** ESP-IDF was selected to provide direct control over FreeRTOS task creation, static memory allocation, peripheral configuration, and ESP32 sleep/wake behavior.
 
**LDO instead of a buck-boost regulator.** An LDO offers a simpler design and lower quiescent current, at the cost of reduced usable battery voltage as the LiPo approaches the regulator’s dropout region. The RT9080-33GJ5 was selected for its approximately 2 µA quiescent current and 600 mA output rating. The MCP1700 was rejected because its 250 mA rating provided insufficient margin for ESP32 Wi-Fi current transients.
 
**No onboard current-sense IC.** . The system must measure microamp-level sleep current as well as much larger Wi-Fi current transients. Test points are therefore placed across a removable 0 Ω jumper. During characterization, the jumper can be replaced by a larger resistor for controlled sleep-current measurements or a low-value shunt for active-current measurements. The two operating states will be measured separately and combined using the measured duty cycle. An INA219 did not provide sufficient dynamic range for both measurements with a single shunt, while the INA228 added excessive quiescent current for this application.
 
**Adaptive sampling instead of a fixed interval.** The planned firmware will maintain a rolling window of pressure measurements, estimate the rate of change, and adjust the next sampling interval. Faster changes, such as an approaching front, will means more frequent sampling; stable conditions means longer sleep intervals.
 
 
**Fully static FreeRTOS allocation.** Every task and queue is created statically to keep predictable memory usage.


## Repo structure
 
```
├── firmware/       # ESP-IDF project (FreeRTOS tasks, drivers, adaptive sampling)
├── hardware/       # KiCad schematic + PCB layout, BOM
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
