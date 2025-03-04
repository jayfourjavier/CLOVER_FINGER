# Arduino Mega AS608 Fingerprint Sensor 

This project integrates an AS608 fingerprint sensor with an Arduino Mega 2560 for biometric authentication.

## Features
- Enroll, verify, and delete fingerprints
- Interface with LCD and LEDs for user feedback
- Serial communication for debugging

## Components
- **Arduino Mega 2560**
- **AS608 Fingerprint Sensor**
- **16x2 LCD Display**
- **LEDs and Buzzer**
- **Push Buttons**

## Installation
1. Clone this repository.
2. Install required libraries:
   - `Adafruit Fingerprint Sensor Library`
   - `LiquidCrystal_I2C`
3. Upload the code to Arduino Mega.
4. Connect components as per the wiring diagram.

## Usage
- Press `Enroll` to register a new fingerprint.
- Place your finger on the sensor for verification.
- Press `Delete` to remove a fingerprint.

## License
MIT License

``` Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```

