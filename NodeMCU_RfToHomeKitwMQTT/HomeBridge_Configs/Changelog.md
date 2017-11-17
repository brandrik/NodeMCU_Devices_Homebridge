# CHANGELOG:


- config.json
- package.json

for homebridge running in a docker container on rapsberry pi 1

# Changes
20171117:
- raspberry currently running in Podest Hemmerde
- added dyson 360 packae and config
{
  "accessory": "Dyson360EyeRobotVacuum",
  "name": "Cleano",
  "host": "192.168.1.48",
  "port": 1883,
  "username": "JJ5-UK-HEA1285A",
  "password": "",
  "refresh": 0
},


Password of dyson has to be first converted to SHA-12 and then to Base64. Carried out via

https://caligatio.github.io/jsSHA/

This can be done via a tool at https://caligatio.github.io/jsSHA/ (client side javascript only). Put password into 'Input text', input type 'TEXT', SHA Variant 'SHA-512', Number of Rounds "1", Output Type "Base64":

kkszw87D+XQHg3nLvS4PiWnlLdGYHd3wRfXOkO6r0ksT3sQ6/cMhvO3CachwZDPk+rFs+qTmSZBxpmLoaJiZPQ==





{
  "accessory": "Dyson360EyeRobotVacuum",
  "name": "Molli",
  "host": "192.168.1.76",
  "port": 1883,
  "username": "JG8-EU-HKA2172A",
  "password": "kkszw87D+XQHg3nLvS4PiWnlLdGYHd3wRfXOkO6r0ksT3sQ6/cMhvO3CachwZDPk+rFs+qTmSZBxpmLoaJiZPQ==",
  "refresh": 0
}
