#include <Adafruit_Fingerprint.h>
#define mySerial Serial1

Adafruit_Fingerprint fingerprint = Adafruit_Fingerprint(&mySerial);

enum Mode { ID_MODE, ENROLL_MODE, DELETE_ID_MODE, DELETE_ALL_MODE };
Mode currentMode = DELETE_ALL_MODE;
uint8_t id;
uint8_t fingerprintId = 0;
const uint8_t adminId = 1; 

void getFingerprintID(uint8_t & _gotId) {
  uint8_t p = fingerprint.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return;
    default:
      Serial.println("Unknown error");
      return;
  }

  // OK success!
  p = fingerprint.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return;
    case FINGERPRINT_FEATUREFAIL:
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return;
    default:
      Serial.println("Unknown error");
      return;
  }

  // OK converted!
  p = fingerprint.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return;
  } else {
    Serial.println("Unknown error");
    return;
  }

  // Found a match!
  Serial.print("Found ID #"); 
  Serial.print(fingerprint.fingerID);
  Serial.print(" with confidence of "); 
  Serial.println(fingerprint.confidence);
  
  _gotId = fingerprint.fingerID; // Update ID by reference
}


// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = fingerprint.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = fingerprint.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = fingerprint.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  Serial.print("Found ID #"); Serial.print(fingerprint.fingerID);
  Serial.print(" with confidence of "); Serial.println(fingerprint.confidence);
  return fingerprint.fingerID;
}


uint8_t deleteFingerprint(uint8_t _id) {
  Serial.print("\nDeleting ID #");
  Serial.println(_id);

  uint8_t p = -1;
  p = fingerprint.deleteModel(_id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
  } else {
    Serial.print("Unknown error: 0x"); 
    Serial.println(p, HEX);
  }

  return p;
}

uint8_t getFingerprintEnroll(uint8_t _id) {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); 
  Serial.println(_id);
  while (p != FINGERPRINT_OK) {
    p = fingerprint.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = fingerprint.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = fingerprint.getImage();
  }
  Serial.print("ID "); 
  Serial.println(_id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = fingerprint.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = fingerprint.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);

  p = fingerprint.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID "); Serial.println(_id);
  p = fingerprint.storeModel(_id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  return true;
}


void enrollFingerprint(uint8_t _id) {
  Serial.print("Enrolling ID #");
  Serial.println(_id);

  Serial.println("Waiting for a valid fingerprint...");

  while (!getFingerprintEnroll(_id)) {  // Loop until successful enrollment
      Serial.println("Enrollment failed, try again.");
  }

  Serial.print("Fingerprint enrolled successfully for ID: ");
  Serial.println(_id);
}



void fingerLed (bool _on){
  if (_on){
    fingerprint.LEDcontrol(true);
  } else {
    fingerprint.LEDcontrol(false);
  }
}

void setupFingerPrint (){
  // set the data rate for the sensor serial port
  fingerprint.begin(57600);
  delay(5);
  if (fingerprint.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  Serial.println(F("Reading sensor parameters"));
  fingerprint.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(fingerprint.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(fingerprint.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(fingerprint.capacity);
  Serial.print(F("Security level: ")); Serial.println(fingerprint.security_level);
  Serial.print(F("Device address: ")); Serial.println(fingerprint.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(fingerprint.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(fingerprint.baud_rate);

  fingerprint.getTemplateCount();

  if (fingerprint.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial.println("Waiting for valid fingerprint...");
    Serial.print("Sensor contains "); 
    Serial.print(fingerprint.templateCount); 
    Serial.println(" templates");
  }
}

void getFingerId(uint8_t &_id) {
  getFingerprintID(_id);
}

bool isAdmin() {
  getFingerId(fingerprintId);
  bool _adminDetected = (fingerprintId == adminId);
  _adminDetected && Serial.println("HELLO ADMIN");  // Print result for debugging
  return _adminDetected;
}

void enrollAdmin(uint8_t _adminId){
  Serial.print("Enrolling admin as # ");
  Serial.println(_adminId);
  enrollFingerprint(_adminId);
}

void eraseAllFingerprint (){
  fingerprint.emptyDatabase();
}

void processMode() {
  Serial.print("Current Mode: ");
  
  if (currentMode == ID_MODE) {
    Serial.println("ID_MODE");
    fingerprintId = 0;
    getFingerId(fingerprintId);
    Serial.print("Detected Fingerprint ID: ");
    Serial.println(fingerprintId);
  } 
  else if (currentMode == DELETE_ALL_MODE) {
    Serial.println("DELETE_ALL_MODE");
    Serial.println("Deleting all fingerprints...");

    eraseAllFingerprint();
    currentMode = ID_MODE;
    // Call delete all function here
  } 
  else if (currentMode == DELETE_ID_MODE) {
    Serial.println("DELETE_ID_MODE");
    Serial.print("Deleting Fingerprint ID: ");
    Serial.println(fingerprintId);
    // Call delete specific ID function here
  } 
  else if (currentMode == ENROLL_MODE) {
    Serial.println("ENROLL_MODE");
    Serial.print("Enrolling Fingerprint ID: ");
    Serial.println(fingerprintId);
    // Call enroll function here
  } 
  else {
    Serial.println("UNKNOWN_MODE");
  }
}




void setup() {
  Serial.begin(9600);
  delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  setupFingerPrint();
  //deleteFingerprint(13);
  //enrollFingerprint(1);
  enrollAdmin(adminId);
  fingerLed(true);
}

void loop() {
  //getFingerId(fingerprintId);
  //getFingerprintID();
  delay(1000);            //don't ned to run this at full speed.

  //isAdmin();
  processMode();
}
