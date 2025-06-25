# Arduino_OnOffStress-Test-Tool

## OnOffStress Box Featrues
- control AC/DC power automatically
- control ACPI power state: S0, S5, G3, powerbutton-trigger
- integrated with GUI App for user input testing parameters
- high stability for testing 9999 cycles
- modulize componets for maitain easily
- high scalability for add new features
- low cost under NTD 1000

## Program Structure
- lib/Grove_4-Digit_Display: Library for 4-Digit Display
- Arduino_ONOFFTOOL.ino: the code for MCU to run main()
- OnOffTool.cpp: actual implemented functions for OnOffTool Class
- OnOffTool.h: header file for OnOffTool Class
- config.h: define MCU pin configuration
- debugLog.h: debug printf()
