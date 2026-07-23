# MpvLogParser
Utility to convert non-standard XML logs (*.log) from MPVnet dispaching system to CSV and .SQLITE databases.
## Changelog
- 20260723_1735
  - Soubor::csvZapisJedenRadek
    - reworked to allow faster import 
  - Soubor::slotSouborNaRadky2
    - transaction enforcement
  
- 20240509_1535
    - added icon
- 20240314_1744
    - ability to load logs from Telematika channel
        - Soubor::logZpracujRadekHledejHlavicky
        - Soubor::logZpracujRadekStream
- 20240314_1744
    - ability to load metro log format (m/vlak tags)
    - new version of XmlRopidImportStream
- 20240104_1323
    - version label format change
- 20240104
    - XmlRopidImportStream 
        - new version
        - Qt6 compatibility
    - import time measurement
    - soubor.cpp
        - import of row by xmlStream
    - ZaznamMpvLogu
        - removal of unused variables
