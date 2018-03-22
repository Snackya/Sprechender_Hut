/*******TALKING HEAD************/
/*this programm controlls the interacftive tangible interface Talking Head
developed by Thomas Fischer and Jonas Jelinski in the lecture "Sketching with Hardware" from the University Regensburg in 2018*/

/*the code constists of different parts */
/*the SETUP PART is responsible to setup the Arduino*/
/*the LOOP PART controlls the programm in a loop*/
/*the GYROSENSOR PART sets up the gyrosenor which is used to capture movement*/
/*the DFPlayer PART sets up the DPlayer which is used to play mp3 files*/
/*the MUSIC PART uses the DPLayer to play music*/
/*the SENSOR INPUT PART captures the datainput of the sensors and gives inforamtive feedback*/
/*the SENSOR UPDATE PART updates sensors*/
/*the SENSOR DATA ARRAYS PART handels the arrays which are used to store sensor data in arrays like in a stack*/
/*the GAME PART contains the logic of the game*/
/*the STATE MACHINE PART the state machine controlls the whole programm logic
it is used in the loop of the LOOP PART*/



#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <CapacitiveSensor.h>

const static int RX_PIN = 11;
const static int TX_PIN = 10;
const static int VOLUME = 25;
const static int PIN_DFP_BUSY = 9;
const static int ANALOG_RANDOM=0; 

DFRobotDFPlayerMini myDFPlayer;
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);
SoftwareSerial mySoftwareSerial(TX_PIN, RX_PIN);
CapacitiveSensor cs_4_8 = CapacitiveSensor(4, 8);


//**------SETUP PART-----///

void setup() {
  cs_4_8.set_CS_AutocaL_Millis(0xFFFFFFFF);
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  delay(2000);
  Serial.println("set up start");
  setupGyro();
  setupDFPlayer();
  setUpArrays();
  Serial.println("set up finished");
}


//**------LOOP PART-----///

void loop() {

  Serial.println("loop start");
  //updateCaptiveSensor();
  Serial.println("cs updated");
 // updateGyroSensor();
  Serial.println("gyro updated");
  stateMachineRun();
  delay(500);

}

//**------GYROSENSOR  PART-----///
//**sets up gyrosensor to capture user movement**//

void setupGyro() {

  Serial.println("setupGyro");
  gyro.enableAutoRange(true);
  if (!gyro.begin())
  {
    Serial.println("Ooops, no L3GD20 detected ... Check your wiring!");
    while (1);
  }
}

//**------DFPlayer PART-----///
//**sets up dfplayer to play mp3s**//
//source:https://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299, 22.03.18


void setupDFPlayer() {
  Serial.println("setupDFPlayer");
  //Use softwareSerial to communicate with mp3.
  if(!myDFPlayer.begin(mySoftwareSerial)) {  
    while (true);
  }
  Serial.println("setupDFPlayer begins");

  //set VOLUME (0-30)
  myDFPlayer.volume(VOLUME); 
  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  //----Set device we use SD as default----
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  //----Mp3 play----
  //busy pin sends low if file is playing; high: no file is playing
  pinMode(PIN_DFP_BUSY, INPUT);
  Serial.println("setupDFPlayer begins finished");
}

//**------MUSIC PART-----///
//**contains all sounds and their playtime**//
const int SOUNDS_FOLDER = 1;


int playSongFromSD(int songname) {
  Serial.print("playSongFromSD " );
  Serial.println(songname );  
  myDFPlayer.playFolder(SOUNDS_FOLDER, songname);
  
    int waitingtime1=10000;
    int waitingtime2=20000;
    int waitingtime3=30000;

    switch(songname){
      case 1: delay(waitingtime1);
      break;

      case 2: delay(waitingtime1);
      break;

      case 3:delay(waitingtime3);
      break;

      case 4: delay(waitingtime1);
      break;

      case 5: delay(waitingtime1);
      break;

      case 6: delay(waitingtime3);
      break;

      case 7: delay(waitingtime2);
      break;

      case 8: delay(waitingtime1);
      break;

      case 9: delay(waitingtime1);
      break;

      case 10: delay(waitingtime2);
      break;

      case 11: delay(waitingtime3);
      break;

      case 12: delay(waitingtime1);
      break;

      case 13: delay(waitingtime1);
      break;

      case 14: delay(waitingtime1);
      break;

      case 15: delay(waitingtime1);
      break;

      case 16: delay(waitingtime2);
      break;

      case 17: delay(waitingtime1);
      break;

      case 18: delay(waitingtime1);
      break;

      case 19: delay(waitingtime3);
      break;

      case 20: delay(waitingtime3);
      break;

      case 21: delay(waitingtime2);
      break;

      case 22: delay(waitingtime3);
      break;

      case 97: delay(waitingtime1);
      break;

      case 98: delay(waitingtime1);
      break;

      case 99: delay(waitingtime2);
      break;

      default: delay (waitingtime2);
      break;

    }
}

bool isFilePlaying() {
  if (digitalRead(PIN_DFP_BUSY) == HIGH) {
    return false;
  }

  else true;

}

//**------SENSOR INPUT PART-----///
//**functions to check sensor input**//

const static long MAX_TOUCH_VALUE=240;
const static float MAX_HEAD_SHAKE=1;
const static float MAX_HEAD_NODDING=1;
const static int MAX_VALUES =20;


const static long TOUCH_FILTER = 3;
const static float SHAKE_FILTER= 3;
const static float NOD_FILTER = 3;

//checks if user touches the hat
bool isUserTouchingHat() {
   Serial.print("is user touching hat ");
     
  long touchvalue = (long) cs_4_8.capacitiveSensor(30);  
   
  /*
    for(int j=0; j <TOUCH_FILTER;j++){
        touchvalue += (long) cs_4_8.capacitiveSensor(30);
            }

    touchvalue=touchvalue/TOUCH_FILTER;  */

   
  Serial.print(touchvalue);
  Serial.print(" > ");
  Serial.println(MAX_TOUCH_VALUE);
  
  if (touchvalue > MAX_TOUCH_VALUE) {
    Serial.println("is user touching hat true");
    return true;
  }

  else {
    Serial.println("is user touching hat false");
    return false;
  }

}

bool isUserShaking() {

  Serial.println("is user shaking");
  sensors_event_t gyro_event;
  gyro.getEvent(&gyro_event); 
  float zValue=(float) gyro_event.gyro.z;
  float headShake = abs(zValue); 

   /*for(int i=0; i<SHAKE_FILTER;i++){
      zValue =(float) gyro_event.gyro.z;
      headShake + =abs(zValue);
    }*/

 //headShake=headShake/NOD_FILTER;

  Serial.print("is user shaking hat ");
  Serial.print(headShake);
  Serial.print(" > ");
  Serial.print(MAX_HEAD_SHAKE);
  Serial.print(" real z: ");
  Serial.println(gyro_event.gyro.z);

  if (headShake >= SHAKE_FILTER) {
    Serial.println("is user shaking hat true");
    return true;
  }

  else {
    Serial.println("is user shaking hat false");
    false;
  }

}

bool isUserNodding() {
  Serial.println("is user nodding");
  sensors_event_t gyro_event;
  gyro.getEvent(&gyro_event);
  float yValue = (float) gyro_event.gyro.y;
  float headNod = abs(yValue);
  
  /*for(int i=0; i<NOD_FILTER;i++){
      yvalue =(float) gyro_event.gyro.y;
      headNod + =abs(yvalue);
    }*/
    
  //headNod=headNod/NOD_FILTER;
  
  Serial.print("is user nodding ");
  Serial.print(headNod);
  Serial.print(" > ");
  Serial.print(MAX_HEAD_NODDING);
  Serial.print(" real y: ");
  Serial.println(gyro_event.gyro.y);

  if (headNod >= MAX_HEAD_NODDING) {
    Serial.println("is user nodding true");
    return true;
  }

  else {
    Serial.println("is user nodding false");
    return false;
  }
}

//returns average values of a long array with size of MAX_VALUES
long getAverage(long values[]) {
  long average = 0;
  for (int i = 0; i < MAX_VALUES; i++) {
    average += values[i];
  }
  long divider = long(MAX_VALUES);
  average = average / MAX_VALUES;
  return average;

}


//**------SENSOR UPDATE PART-----///
//keeps sensor arrays up to date///

//saves new values in array  of captive sensor and prints them
void updateCaptiveSensor() {
  Serial.println("updateCaptiveSensor");
  /* Get a new sensor gyro_event */
  /*long start = millis();
    long capacitive_value =  cs_4_8.capacitiveSensor(30);

    Serial.println(millis() - start);        // check on performance in milliseconds
    Serial.println("\t");                    // tab character for debug window spacing
    Serial.println(capacitive_value);                  // print sensor output 1
    //updateArray(capacitive_value, touchvalues);
    Serial.println();
  */
}

//saves new values in array  of gyro sensor and prints them
void updateGyroSensor() {
  Serial.println("updateGyroSensor");
  /*
    sensors_event_t gyro_event;
    gyro.getEvent(&gyro_event);
    Serial.println("got event");
    Serial.println("X: "); Serial.println(gyro_event.gyro.x); Serial.println("  ");

   // long zvalue=(long) gyro_event.gyro.z;
   // updateArray(zvalue, gyroZvalues);
    Serial.println("Y: "); Serial.println(gyro_event.gyro.y); Serial.println("  ");
    //long yvalue=(long) gyro_event.gyro.y;
   // updateArray(yvalue, gyroYvalues);
    Serial.println("Z: "); Serial.println(gyro_event.gyro.z); Serial.println("  ");
    Serial.println("rad/s ");
    Serial.println();
  */

}

//**------SENSOR DATA ARRAYS PART-----///
//**works with values of sensordata**//

long gyroYvalues[MAX_VALUES];
long gyroZvalues[MAX_VALUES];


//sets up single array
void setupArray(long array[]) {
  long start_Value = 1;
  for (int i = 0; i < MAX_VALUES; i++) {
    array[i] = start_Value;
  }
}


//updates a single array, works like a stack(push and pull)
void updateArray(long newValue, long arr[]) {
  for (int i = MAX_VALUES; i >= 0; i--) {
    if (i > 0 && i < MAX_VALUES) {
      arr[i + 1] = arr[i];
    }
    if (i == 0) {
      arr[0] = newValue;
    }
  }
}


//sets up the arrays for the measured values
void setUpArrays() {
  Serial.println("setUpArrays");
  //setupArray(touchvalues);
  //setupArray(gyroYvalues);
  //setupArray(gyroZvalues);
}


//****////GAME-PART///******/////
//**contains game logic**//

//ids for each house
const static int HOUSE_GRY = 0;

const static int HOUSE_HUF = 1;

const static int HOUSE_RAV = 2;

const int HOUSE_SLY = 3;

const int HOUSE_ANSWER_NO = 20;

//house array contains all houses

int houses[4] = {HOUSE_GRY, HOUSE_HUF, HOUSE_RAV, HOUSE_SLY};

const int NUMBER_OF_HOUSES = 4;

//Welcome sound: tells the use to pick up the hat
const int  WELCOME_SOUND = 1;

//houses questions the user has to answer
int questions_gry[4] = {2, 3, 4, 5};

int questions_huf[4] = {6, 7, 8, 9};

int questions_rav[4] = {10, 11, 12, 13};

int questions_sly[4] = {14, 15, 16, 17};

//tells user to answer question if user doesn't shake his head properly
const static int ASK_REPETITION_SOUND = 18;

//hat goes to sleep
const static int SLEEPING_SOUND = 97;
const static int SNORRING_SOUND = 98;

//tells the user in which house he is
int answers[4] = {19, 20, 21, 22};


//houses questions asked
const int NEW_QUESTION = 10;

int q_asked_gry[4] = {NEW_QUESTION, NEW_QUESTION, NEW_QUESTION, NEW_QUESTION};

int q_asked_huf[4] = {NEW_QUESTION, NEW_QUESTION, NEW_QUESTION, NEW_QUESTION};

int q_asked_rav[4] = {NEW_QUESTION, NEW_QUESTION, NEW_QUESTION, NEW_QUESTION};

int  q_asked_sly[4] = {NEW_QUESTION, NEW_QUESTION, NEW_QUESTION, NEW_QUESTION};

//answers 'yes' and 'no'
const bool YES = true;

const bool NO = false;

//question max in one game
const int MAX_QUESTIONS = 4;

static int question_counter = 0;

//house which is asked in one state
//is set random for each loop
static int state_house = 0;


//first thing that happens
//gives user audio-feedback that hat is ready for the game
void hatawakes() {
  playSongFromSD(WELCOME_SOUND);
}


//**we ask user and are handling his answer yes or no**//

//ask a single question
void askquestion(int house) {
  Serial.println( "askquestion");
  Serial.println(house);
  switch (house) {
    case HOUSE_GRY: playquestion(house, questions_gry);
      break;
    case HOUSE_HUF: playquestion(house, questions_huf);
      break;
    case HOUSE_RAV: playquestion(house, questions_rav);
      break;
    case HOUSE_SLY: playquestion(house, questions_sly);
      break;
    case HOUSE_ANSWER_NO: 
      break;
    default: 
      break;
  }

}


//used in state machine
void askuser() {
  randomSeed(analogRead(ANALOG_RANDOM));
  state_house = random(HOUSE_GRY, HOUSE_SLY + 1);
  for (int i = 0; i < NUMBER_OF_HOUSES; i++) {
    if (state_house == houses[i]) {
      askquestion(state_house);
      return;
    }
  }
  //as long questions if one house is found!
  Serial.println("need new question");
  askuser();
}


//saves answer and marks it as 'old'
void saveAnswer(int house) {

  bool answer = getUserAnswer();
  if (answer == NO) {
    Serial.println("saveAnswer no");
    Serial.println(house);
    houses[house] = HOUSE_ANSWER_NO;
  }
  Serial.println("saveAnswer yes");
  Serial.println(house);
}

//used in state machine
void listenToAnswer() {
  saveAnswer(state_house);
}


bool getUserAnswer() {
  Serial.println("getUserAnswer");
  bool noanswer = true;
  //while durch timer ersetzen
  while (noanswer) {
    if (isUserNodding()) {
      bool answer = YES;
      return answer;
    }

    if (isUserShaking()) {
      bool answer = NO;
      return answer;
    }
  }
  //if sth goes wrong this saves the function
  //shouldnt be reached, because while is always true
  askForRepetition();
  return getUserAnswer();
}

//aks user to anser again
void askForRepetition() {
 
  playSongFromSD(ASK_REPETITION_SOUND);

}


//plays question from a house
void playquestion(int house, int question_array[]) {
  randomSeed(analogRead(0));
  int maxrandom = NUMBER_OF_HOUSES;
  int question = random(HOUSE_GRY, maxrandom);
  //Serial.printlnln("playquestion");
  Serial.println(question);

  //plays questions if question is new
  if (!isQuestionOld(house, question)) {
    playSongFromSD(question_array[question]);
    rememberAskedQuestion(question, house);
  }
  //trys again if question is old
  else {
    playquestion(house, question_array);
  }
}

//checks if question has been asked before
bool isQuestionOld(int house, int question) {
  Serial.println("isQuestionOld");
  switch (house) {
    case HOUSE_GRY:
      if (q_asked_gry[question] != NEW_QUESTION) {
        return true;
      }
      else return false;
     

    case   HOUSE_HUF:
      if (q_asked_huf[question] != NEW_QUESTION) {
        return true;
      }   
      else return false;
      
    case  HOUSE_RAV:
      if (q_asked_rav[question] != NEW_QUESTION) {
        return true;
      }     
      else return false;
      
    case   HOUSE_SLY:
      if (q_asked_sly[question] != NEW_QUESTION) {
        return true;
      } 
      else return false;

    default: return true;
  }
}


//saves asked question as old
void rememberAskedQuestion(int askedquestion, int house) {
  Serial.println("rememberAskedQuestion");

  switch (house) {
    case HOUSE_GRY: q_asked_gry[askedquestion] = askedquestion;;
      break;
    case HOUSE_HUF: q_asked_huf[askedquestion] = askedquestion;
      break;
    case HOUSE_RAV: q_asked_rav[askedquestion] = askedquestion;
      break;
    case HOUSE_SLY: q_asked_sly[askedquestion] = askedquestion;
      break;
    default:        break;

  }
}



//**we need to know when the game is over**//

bool stillMoreThanOneHouseLeft() {
  int counter = 0;
  int maxHouses = 1;
  for (int house = HOUSE_GRY; house <= HOUSE_SLY; house++) {
    Serial.println("is user stillMoreThanOneHouseLeft");

    if (houses[house] != HOUSE_ANSWER_NO) {
      counter++;
    }
  }
  Serial.print("is user stillMoreThanOneHouseLeft counter ");
  Serial.println(counter);
  Serial.print("is user stillMoreThanOneHouseLeft maxhouse ");
  Serial.println(maxHouses);

  if (counter > maxHouses) {
    return true;
  }

  else return false;
}

bool stillNoWinner() {
  Serial.println("stillNoWinner");
  if (stillMoreThanOneHouseLeft() && MAX_QUESTIONS > question_counter) {
    question_counter = question_counter + 1;
    Serial.println("stillNoWinner true");
    return true;
  }
  else return false;
  Serial.println("stillNoWinner true");
}



//tells user his house if game is over
void tellUserHisHouse() {

  for (int house = HOUSE_GRY; house <= HOUSE_SLY; house++) {
    int answer = houses[house];
    Serial.println("tellUserHisHouse");
    Serial.println(answer);
    if (houses[house] != HOUSE_ANSWER_NO) {
      Serial.println("tellUserHisHouse");
      playSongFromSD(answers[house]);
      return;
    }

  }
  playSongFromSD(answers[HOUSE_HUF]);
}




//**if game is over everything will be reseted**//

void gameOver() {

  resetGame();
  playSongFromSD(SLEEPING_SOUND);
  playSongFromSD(SNORRING_SOUND);
}

void resetGame() {
  Serial.println("resetGame");
  resetHouses();
  resetAskedQuestions();
}


void resetHouses() {
  Serial.println("resetHouses");
  houses[HOUSE_GRY] = HOUSE_GRY;
  houses[HOUSE_HUF] = HOUSE_HUF;
  houses[HOUSE_RAV] = HOUSE_RAV;
  houses[HOUSE_SLY] = HOUSE_SLY;
}


void resetAskedQuestions() {
  Serial.println("resetAskedQuestions");
  resetAskedQuestionsPerHouse(questions_gry);
  resetAskedQuestionsPerHouse(questions_huf);
  resetAskedQuestionsPerHouse(questions_rav);
  resetAskedQuestionsPerHouse(questions_sly);
}



void resetAskedQuestionsPerHouse(int array[]) {
  int questionsPerHouse = 4;
  for (int i = 0; i < questionsPerHouse; i++) {
    array[i] = NEW_QUESTION;
  }

}


//**------STATE MACHINE PART-----///
//contains whole logic//
//controlls programm


enum class GameState {DEFAULT_STATE, HAT_AWAKES, GAME_START, ASKING, ANSWER, WAITING, CHECKING, GAMEOVER};
GameState state = GameState::DEFAULT_STATE;

void stateMachineRun() {

  switch (state) {
      Serial.println("statemachine starts");

    //DEFAULT STATE
    case GameState::DEFAULT_STATE:
      Serial.println("DEFAULT_STATE");
      //waits for sensor input
      while (true) {
        Serial.println("user not touching");
        if (isUserTouchingHat()) {
          state = GameState::HAT_AWAKES;
          break;
        }
      }

    //HAT AWAKES
    case GameState::HAT_AWAKES:
      Serial.println("HAT_AWAKES");
      hatawakes();
      state = GameState::GAME_START;
      break;

    //GAME START
    case GameState::GAME_START:
      Serial.println("GAME_START");
      //waits for sensor input
      while (true) {
        if (isUserTouchingHat()) {
          state = GameState::ASKING;
          break;
        }
      }

    //ASKING
    case GameState::ASKING:
      Serial.println("ASKING");
      askuser();
      state = GameState::ANSWER;
      break;

    //ANSWER
    case GameState::ANSWER:
      Serial.println("ANSWER");
      if (isUserShaking() || isUserNodding()) {
        listenToAnswer();
        state = GameState::CHECKING;
        break;
      }
      else {
        state = GameState::WAITING;
        break;
      }

    //WAITING
    case GameState::WAITING:
      Serial.println("WAITING");
      updateCaptiveSensor();
      updateGyroSensor();
      state = GameState::ANSWER;
      return;

    //CHECKING
    case GameState::CHECKING:
      Serial.println("CHECKING");
      if (stillNoWinner()) {
        state = GameState::ASKING;
        break;
      }
      else {
        tellUserHisHouse();
        state = GameState::GAMEOVER;
        break;
      }

    //GAMEOVER
    case GameState::GAMEOVER:
      Serial.println("GAMEOVER");
      gameOver();
      state = GameState::DEFAULT_STATE;
      break;

  }

}






