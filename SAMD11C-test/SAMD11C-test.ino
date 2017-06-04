#undef LED_BUILTIN
#define LED_BUILTIN 2

unsigned long *NVM_REG = (unsigned long*)0x41004000;
#define NVM_REG_CTRLA NVM_REG[0]    // 0x00
#define NVM_REG_CTRLB NVM_REG[1]    // 0x04
#define NVM_REG_PARAM NVM_REG[2]    // 0x08
#define NVM_REG_INTENCLR NVM_REG[3] // 0x0C
#define NVM_REG_INTENSET NVM_REG[4] // 0x10
#define NVM_REG_INTFLAG NVM_REG[5]  // 0x14
#define NVM_REG_STATUS NVM_REG[6]   // 0x18
#define NVM_REG_ADDR NVM_REG[7]     // 0x1C
#define NVM_REG_LOCK NVM_REG[8]     // 0x20

char buf[128];

void show_nvm_stat(void)
{
  sprintf(buf, "    CTRLA: 0x%08x\n", NVM_REG_CTRLA);
  Serial.print(buf);
  sprintf(buf, "    CTRLB: 0x%08x\n", NVM_REG_CTRLB);
  Serial.print(buf);
  sprintf(buf, "    PARAM: 0x%08x\n", NVM_REG_PARAM);
  Serial.print(buf);
  sprintf(buf, " INTENCLR: 0x%08x\n", NVM_REG_INTENCLR);
  Serial.print(buf);
  sprintf(buf, " INTENSET: 0x%08x\n", NVM_REG_INTENSET);
  Serial.print(buf);
  sprintf(buf, "  INTFLAG: 0x%08x\n", NVM_REG_INTFLAG);
  Serial.print(buf);
  sprintf(buf, "   STATUS: 0x%08x\n", NVM_REG_STATUS);
  Serial.print(buf);
  sprintf(buf, "     ADDR: 0x%08x\n", NVM_REG_ADDR);
  Serial.print(buf);
  sprintf(buf, "     LOCK: 0x%08x\n", NVM_REG_LOCK);
  Serial.print(buf);
}

void setup() {
  Serial.begin(9600);
  //Serial.begin();
  while (!Serial);
  Serial.println("Hello!");
  Serial.print("LED_BUILTIN=");
  Serial.println(LED_BUILTIN);

  show_nvm_stat();

  // Page Buffer Clear
  Serial.println("issue page buffer clear command...");
  NVM_REG_CTRLA = 0xa544;
  delay(500);

  show_nvm_stat();

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
