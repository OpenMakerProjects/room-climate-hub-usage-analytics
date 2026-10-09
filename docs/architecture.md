# Architecture
A0 peak-to-peak amplitude -> five-second activity hold; optional MQTT on/off request -> active output. D5 relay and D9 servo follow active output. Pure Usage state accounts prior output time and splits intervals at uptime-day/week boundaries. Serial and LAN MQTT publish summaries each second. No calendar time, energy estimate, speech recognition or cloud dependency.
