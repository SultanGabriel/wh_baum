#define PIN_SHIFT 12 // connected to SHCP
#define PIN_STORE 8  // connected to STCP
#define PIN_DATA 11  // connected to DS

int ledPattern[8] = {0, 1, 0, 0, 1, 0, 1, 0};

void setup()
{
    pinMode(PIN_STORE, OUTPUT);
    pinMode(PIN_SHIFT, OUTPUT);
    pinMode(PIN_DATA, OUTPUT);

    for (int i = 0; i < 8; i++)
    {
        // set shift pin to "wait"
        digitalWrite(PIN_SHIFT, LOW);

        // writing to data pin
        digitalWrite(PIN_DATA, ledPattern[i]);

        // rising slope -> shifting data in the register
        digitalWrite(PIN_SHIFT, HIGH);
    }
}

void loop()
{
}