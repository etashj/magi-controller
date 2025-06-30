int BUT1 = D0; 
int BUT2 = D1; 
int BUT3 = D2; 
int BUT4 = D3; 

int LED1 = D4; 
int LED2 = D5;
int LED3 = D6; 
int LED4 = D7; 

bool ledCombos[8][3] = {
  {true, true, true, }, 
  {true, true, false, }, 
  {true, false, true, }, 
  {true, false, false, }, 
  {false, true, true, }, 
  {false, true, false, }, 
  {false, false, true, }, 
  {false, false, false, }, 
};

int comboIndex = 0;

struct Node {
  int data;
  Node* next; 

  Node(int d) {
    data = d; 
    next = nullptr; 
  }
  
  Node(int d, Node* n) {
    data = d; 
    next = n; 
  }
};

Node* listHead = nullptr; 
Node* listEnd = nullptr; 

int but3Time = 0; 
int but4On = 0; 
int but4Off = 0; 
int countdown =0;  
bool ledState = false; 
Node* current = listHead; 
bool playing = false; 

bool shuffler=false; 

void setup() {
  // put your setup code here, to run once:
  pinMode(BUT1, INPUT); 
  pinMode(BUT2, INPUT); 
  pinMode(BUT3, INPUT); 
  pinMode(BUT4, INPUT); 

  pinMode(LED1, OUTPUT); 
  pinMode(LED2, OUTPUT); 
  pinMode(LED3, OUTPUT); 
  pinMode(LED4, OUTPUT); 

  long startTime = millis(); 
}

void setLEDs(bool config[3]) {
  digitalWrite(LED1, config[0] ? HIGH : LOW); 
  digitalWrite(LED2, config[1] ? HIGH : LOW); 
  digitalWrite(LED3, config[2] ? HIGH : LOW); 
}

void loop() {
  // Edit lighting configuration
  if ( digitalRead(BUT1) == HIGH ) {
    comboIndex++; 
    if (comboIndex > 7) comboIndex=0; 
    setLEDs(ledCombos[comboIndex]); 
    delay(250); 
  }
  else if ( digitalRead(BUT2) == HIGH ) {
    shuffler = !shuffler; 
    delay(250); 
  }
  if (shuffler) {
    digitalWrite(LED1, random(2) ? HIGH : LOW);
    digitalWrite(LED2, random(2) ? HIGH : LOW);
    digitalWrite(LED3, random(2) ? HIGH : LOW);
    delay(250); 
  }

  // Morse Code Recorder
  // On Hold of switch 3, we can record morse code with switch 4. 
  // When switch 3 is pressed, play back the recording. 
  if ( digitalRead(BUT3) == HIGH ) {
    but3Time++;
    if (but3Time == 400) { 
      current = listHead;
      while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
      }
      listHead = nullptr;
      listEnd = nullptr; 
      current = nullptr;
    }
    if (but3Time>400) {
      // RECORD
      if (digitalRead(BUT4) == HIGH) {
        digitalWrite(LED4, HIGH); 
        if (but4Off>0) {
          // State Change => write prev value
          if (listHead==nullptr) { listHead = new Node{but4Off, nullptr}; listEnd = listHead; current = listHead; }
          else {
            listEnd->next = new Node{but4Off, nullptr}; 
            listEnd = (*listEnd).next; 
          }
        }
        but4On++; 
        but4Off=0; 
      }
      else {
        digitalWrite(LED4, LOW); 
        if (but4On>0) {
          // State Change => write prev value
          if (listHead==nullptr) { listHead = new Node{but4On, nullptr}; listEnd = listHead; }
          else {
            listEnd->next = new Node{but4On, nullptr}; 
            listEnd = (*listEnd).next; 
          }
        }
        but4On = 0; 
        but4Off++; 
      }
    } 
  } else {
    playing = false; 
    if (but3Time<400 && but3Time>0) {
      if (current!=nullptr) {
        if (countdown <= 0) {
          countdown = current->data;
          ledState = !ledState;
          digitalWrite(LED4, ledState ? HIGH : LOW); 
          playing = true;
          if (current->next != nullptr) current = current->next;
        }
        else {
          countdown--; 
          digitalWrite(LED4, ledState ? HIGH : LOW); 
          playing = true; 
        }
      } else { current=listHead; }
    }
    but3Time = 0; 
    if (!playing) {
      (digitalRead(BUT4) == HIGH) ? digitalWrite(LED4, HIGH) : digitalWrite(LED4, LOW); 
    }
  }
}

wake wake wake wake wake wake wake wake wake wake 