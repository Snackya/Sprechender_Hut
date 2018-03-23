/*******TALKING HEAD************/
/*this programm controlls the interactive tangible interface 'Talking Hat'
  developed by Thomas Fischer and Jonas Jelinski in the lecture "Sketching with Hardware" from the University Regensburg in 2018*/

/*the code constists of different parts :*/

/*the SETUP PART */
//is responsible to setup the Arduino
//it starts when the sketch starts
//it initialize the pinModes which controll the sensors

/*the LOOP PART*/
//controlls the programm in a loop
//it starts when the setup is over
//it contains the state machine from the  STATE MACHINE PART

/*the GYROSENSOR PART */
//sets up the gyrosenor which is used to capture movement

/*the DFPlayer PART */
//sets up the DPlayer which is used to play mp3 files

/*the MUSIC PART */
//uses the DPLayer to play music

/*the SENSOR INPUT PART */
//captures the datainput of the sensors and gives informative feedback

/*the SENSOR UPDATE PART */
//updates sensors

/*the GAME PART */
//contains the functions and variables of the game

/*the STATE MACHINE PART */
//the state machine controlls the whole programm logic
//it uses the functions from the GAME PART
//it is used in the loop of the LOOP PART

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <CapacitiveSensor.h>

//**------Constants-----///
//setup constants
const static int DEBUG_LEVEL = 1; //set console debugging level (1=on, 0=off)
const static int TX_PIN = 11; //TXpin connected to the RXpin of the DFPlayer mini
const static int RX_PIN = 10; //RXpin connected to the TXpin of the DFPlayer mini
const static int VOLUME = 25; //volume (0-30)
const static int PIN_DFP_BUSY = 9;  //busyPin to check the playing state of the DFPlayer mini
const static int ANALOG_RANDOM = 0; //unused analog pin to seed the RNG

//music constants

const static int SOUNDS_FOLDER = 1; //folder where mp3-files are on the sd
const static int WELCOME_SOUND = 1; //tells the user to pick up the hat
const static int ASK_REPETITION_SOUND = 18; //tells user to answer question again
const static int SLEEPING_SOUND = 97; //hat goes to sleep
const static int SNORRING_SOUND = 98;

//sensor constants
//adjust to fit individual hat and head sizes
const static long MAX_TOUCH_VALUE = 350;  //capacity sensitivity for putting the head on
const static float MAX_HEAD_SHAKE = 3;    //min value to recognize a headshake
const static float MAX_HEAD_NODDING = 1;  //min value to recognize a nod

//game constants
const static int NUMBER_OF_HOUSES = 4;
//ids for each house
const static int HOUSE_GRY = 0;
const static int HOUSE_HUF = 1;
const static int HOUSE_RAV = 2;
const static int HOUSE_SLY = 3;

//if user has answered 'NO' to one house this house gets the id HOUSE_ANSWER_NO
const static int HOUSE_ANSWER_NO = 20;

//questions
const static bool YES = true;
const static bool NO = false;
const static int MAX_QUESTIONS = 4; //question max in one game
const static int NEW_QUESTION = 10; //const if question hasn't asked before


//**------SETUP PART-----///
//part of the arduiono structure

DFRobotDFPlayerMini myDFPlayer;
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);
SoftwareSerial mySoftwareSerial(RX_PIN, TX_PIN);
CapacitiveSensor cs_4_8 = CapacitiveSensor(4, 8);


//sets up all sensors
void setup() {
  cs_4_8.set_CS_AutocaL_Millis(0xFFFFFFFF); //turn off autocalibrate
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  delay(2000);
  if(DEBUG_LEVEL == 1) Serial.println("set up start");
  setupGyro();
  setupDFPlayer();
  if(DEBUG_LEVEL == 1) Serial.println("set up finished");
}


//**------LOOP PART-----///
//part of the arduiono structure

//runs the state machine in each loop
void loop() {
  if(DEBUG_LEVEL == 1) Serial.println("loop start");
  stateMachineRun();
  delay(500);
}

//**------GYROSENSOR  PART-----///
//**sets up gyrosensor to capture user movement**//

//sets up the gyro sensor
void setupGyro() {
  if(DEBUG_LEVEL == 1) Serial.println("setupGyro");
  gyro.enableAutoRange(true);
  if (!gyro.begin())
  {
    if(DEBUG_LEVEL == 1) Serial.println("No gyrosensor(L3GD20) detected. Check your wiring!");
    while (1);
  }
}

//**------DFPlayer PART-----///
//**sets up dfplayer to play mp3s**//
//source:https://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299, 22.03.18

void setupDFPlayer() {
  if(DEBUG_LEVEL == 1) Serial.println("setupDFPlayer"); 
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    while (true);
    }
  if(DEBUG_LEVEL == 1) Serial.println("setupDFPlayer begins");
  myDFPlayer.volume(VOLUME); 
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL); 
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD); 
  pinMode(PIN_DFP_BUSY, INPUT);
  if(DEBUG_LEVEL == 1) Serial.println("setupDFPlayer begins finished");
}

//**------MUSIC PART-----///
//**contains all sounds and their playtime**//

//plays sound from sd
//each case sets the time to play for each song
int playSongFromSD(int songname) {
  if(DEBUG_LEVEL == 1){
    Serial.print("playSongFromSD " );
    Serial.println(songname );
  }
  myDFPlayer.playFolder(SOUNDS_FOLDER, songname); //play song "songname" from folder "SOUNDS_FOLDER"

  int songtime10 = 10000;
  int songtime12 = 12000;
  int songtime15 = 15000;
  int songtime20 = 20000;
  int songtime25 = 25000;
  int songtime27 = 25000;
  int songtime30 = 30000;
  int songtimeSleep = 7000;

  switch (songname) {
    case 1: delay(songtime10);
      break;

    case 2: delay(songtime10);
      break;

    case 3: delay(songtime25);
      break;

    case 4: delay(songtime10);
      break;

    case 5: delay(songtime10);
      break;

    case 6: delay(songtime27);
      break;

    case 7: delay(songtime20);
      break;

    case 8: delay(songtime15);
      break;

    case 9: delay(songtime10);
      break;

    case 10: delay(songtime15);
      break;

    case 11: delay(songtime27);
      break;

    case 12: delay(songtime15);
      break;

    case 13: delay(songtime12);
      break;

    case 14: delay(songtime12);
      break;

    case 15: delay(songtime12);
      break;

    case 16: delay(songtime20);
      break;

    case 17: delay(songtime10);
      break;

    case 18: delay(songtime10);
      break;

    case 19: delay(songtime25);
      break;

    case 20: delay(songtime30);
      break;

    case 21: delay(songtime15);
      break;

    case 22: delay(songtime30);
      break;

    case 97: delay(songtimeSleep);
      break;

    case 98: delay(songtime10);
      break;

    case 99: delay(songtime20);
      break;

    default: delay (songtime20);
      break;
  }
}

//checks if dfplayer is busy
bool isFilePlaying() {
  if (digitalRead(PIN_DFP_BUSY) == HIGH) {
    return false;
  }
  else true;
}

//**------SENSOR INPUT PART-----///
//**functions to check sensor input**//

//checks if user touches the hat
//uses the capacitive sensor
bool isUserTouchingHat() {
  if(DEBUG_LEVEL == 1) Serial.print("is user touching hat running ");
  long touchvalue = (long) cs_4_8.capacitiveSensor(30);
  if(DEBUG_LEVEL == 1){
    Serial.print(touchvalue);
    Serial.print(" > ");
    Serial.println(MAX_TOUCH_VALUE);
  }

  if (touchvalue > MAX_TOUCH_VALUE) {
    if(DEBUG_LEVEL == 1) Serial.println("is user touching hat true");
    return true;
  }  
  else {
    if(DEBUG_LEVEL == 1) Serial.println("is user touching hat false");
    return false;
  }
}

//checks if user is shaking his head
//uses the gyro sensor
bool isUserShaking() {
  if(DEBUG_LEVEL == 1) Serial.println("isUserShaking() running");
  sensors_event_t gyro_event;
  gyro.getEvent(&gyro_event);
  float zValue = (float) gyro_event.gyro.z;
  float headShake = abs(zValue);

  if(DEBUG_LEVEL == 1){
    Serial.print("is user shaking hat ");
    Serial.print(headShake);
    Serial.print(" > ");
    Serial.print(MAX_HEAD_SHAKE);
    Serial.print(" real z: ");
    Serial.println(gyro_event.gyro.z);
  }
  if (headShake >= MAX_HEAD_SHAKE) {
    if(DEBUG_LEVEL == 1) Serial.println("is user shaking hat true");
    return true;
  }
  else {
    if(DEBUG_LEVEL == 1) Serial.println("is user shaking hat false");
    false;
  }
}

//checks if user is shaking his nodding
//uses the gyro sensor
bool isUserNodding() {
  if(DEBUG_LEVEL == 1) Serial.println("isUserNodding() running");
  sensors_event_t gyro_event;
  gyro.getEvent(&gyro_event);
  float yValue = (float) gyro_event.gyro.y;
  float headNod = abs(yValue);
  if(DEBUG_LEVEL == 1){
    Serial.print("is user nodding ");
    Serial.print(headNod);
    Serial.print(" > ");
    Serial.print(MAX_HEAD_NODDING);
    Serial.print(" real y: ");
    Serial.println(gyro_event.gyro.y);
  }
  if (headNod >= MAX_HEAD_NODDING) {
    if(DEBUG_LEVEL == 1) Serial.println("is user nodding true");
    return true;
  }
  else {
    if(DEBUG_LEVEL == 1) Serial.println("is user nodding false");
    return false;
  }
}

//**------SENSOR UPDATE PART-----///
//keeps sensors up to date///

//updates captive sensor
void updateCaptiveSensor() {
  if(DEBUG_LEVEL == 1) Serial.println("updateCaptiveSensor");
  delay(100);
}

//updates captive sensor
void updateGyroSensor() {
  if(DEBUG_LEVEL == 1) Serial.println("updateGyroSensor");
  delay(100);
}

//****////GAME-PART///******/////
//**contains game logic**//

//---variables---

//house array contains all houses
int houses[4] = {HOUSE_GRY, HOUSE_HUF, HOUSE_RAV, HOUSE_SLY};

//houses questions the user has to answer
int questions_gry[4] = {2, 3, 4, 5};
int questions_huf[4] = {6, 7, 8, 9};
int questions_rav[4] = {10, 11, 12, 13};
int questions_sly[4] = {14, 15, 16, 17};

//tells the user in which house he is
int answers[4] = {19, 20, 21, 22};

int q_asked_gry[4] = {NEW_QUESTION, NEW_QUESTION, NEW_QUESTION, NEW_QUESTION};
int q_asked_huf[4] = {NEW_QUESTION, NEW_QUESTION, NEW_QUESTION, NEW_QUESTION};
int q_asked_rav[4] = {NEW_QUESTION, NEW_QUESTION, NEW_QUESTION, NEW_QUESTION};
int q_asked_sly[4] = {NEW_QUESTION, NEW_QUESTION, NEW_QUESTION, NEW_QUESTION};

static int question_counter = 0;

//house which is asked in one state
//is set random for each loop
static int state_house = 0;

//---functions---

//**at start of the game**//

//gives user audio-feedback that hat is ready for the game
void hatawakes() {
  playSongFromSD(WELCOME_SOUND);
}

//**we ask user and save his answer yes or no**//

//ask a single question
void askquestion(int house) {
  if(DEBUG_LEVEL == 1) {
    Serial.println( "askquestion");
    Serial.println(house);
  }
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

//ask a random question which hasn't asked before
//sets the stae_house randomly
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
  if(DEBUG_LEVEL == 1) Serial.println("need new question");
  askuser();
}


//saves answer and marks it as 'old'
void saveAnswer(int house) {  
  bool answer = getUserAnswer();
  if (answer == NO) {
    if(DEBUG_LEVEL == 1){
      Serial.println("saveAnswer no");
      Serial.println(house);
    }
    houses[house] = HOUSE_ANSWER_NO;
  }
  if(DEBUG_LEVEL == 1){
    Serial.println("saveAnswer yes");
    Serial.println(house);
  }
}
//used in state machine
void listenToAnswer() {
  saveAnswer(state_house);
}

//returns if user answered YES or NO
bool getUserAnswer() {
  if(DEBUG_LEVEL == 1) Serial.println("getUserAnswer");
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
  //shouldn't be reached, because while is always true
  askForRepetition();
  return getUserAnswer();
}

//aks user to answer again
void askForRepetition() {  
  playSongFromSD(ASK_REPETITION_SOUND);
}

//plays question from a house
void playquestion(int house, int question_array[]) {
  randomSeed(analogRead(ANALOG_RANDOM));  //seeds the RNG using an unused analog pin
  int maxrandom = NUMBER_OF_HOUSES;
  int question = random(HOUSE_GRY, maxrandom);
  if(DEBUG_LEVEL == 1){
    Serial.println("playquestion() running");
    Serial.println(question);
  }
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
  if(DEBUG_LEVEL == 1) Serial.println("isQuestionOld");
  
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
  if(DEBUG_LEVEL == 1) Serial.println("rememberAskedQuestion");
  switch (house) {
    case HOUSE_GRY: q_asked_gry[askedquestion] = askedquestion;;
      break;
    case HOUSE_HUF: q_asked_huf[askedquestion] = askedquestion;
      break;
    case HOUSE_RAV: q_asked_rav[askedquestion] = askedquestion;
      break;
    case HOUSE_SLY: q_asked_sly[askedquestion] = askedquestion;
      break;
    default:        
      break;
  }
}

//**we need to know when the game is over**//

bool stillMoreThanOneHouseLeft() {
  if(DEBUG_LEVEL == 1) Serial.println("is user stillMoreThanOneHouseLeft");
  int counter = 0;
  int maxHouses = 1;
  for (int house = HOUSE_GRY; house <= HOUSE_SLY; house++) {   
    if (houses[house] != HOUSE_ANSWER_NO) {     
      counter++;
    }
   }
  if (counter > maxHouses) {
    return true;
  }
  else return false;
}

bool stillNoWinner() {
  if(DEBUG_LEVEL == 1) Serial.println("stillNoWinner");
  if (stillMoreThanOneHouseLeft() && MAX_QUESTIONS > question_counter) {
    question_counter = question_counter + 1;
    if(DEBUG_LEVEL == 1) Serial.println("stillNoWinner true");
    return true;
  }
  else return false;
  if(DEBUG_LEVEL == 1) Serial.println("stillNoWinner true");
}

//tells user his house if game is over
void tellUserHisHouse() {
   if(DEBUG_LEVEL == 1) Serial.println("tellUserHisHouse");
    int winner=answers[HOUSE_HUF];
    winner=getWinner();
    playSongFromSD(answers[winner]);
}

int getWinner(){
  int house_yes[]={0,0,0,0};
  int winner=HOUSE_GRY;
  for (int house = HOUSE_GRY; house <= HOUSE_SLY; house++) {
    int answer = houses[house];
    if (houses[house] != HOUSE_ANSWER_NO) {
      house_yes[house]=house_yes[house]+1;         
    }  
  }

   winner=getHighestValue(house_yes, );
  }
   return winner;
}

int getHighestValue(int house_yes[], int arraysize){
  int winner=HOUSE_GRY;
   for (int house = HOUSE_GRY; house <= HOUSE_SLY; house++) { 
    if (house_yes[house] >= winner) {
      winner=house;     
    }  
  
  }
}



//**if game is over everything will be reseted**//

void gameOver() {
  resetGame();
  playSongFromSD(SLEEPING_SOUND);
  playSongFromSD(SNORRING_SOUND);
}

void resetGame() {
  if(DEBUG_LEVEL == 1) Serial.println("resetGame");
  resetHouses();
  resetAskedQuestions();
  resetValues();
}


void resetHouses() {
  if(DEBUG_LEVEL == 1) Serial.println("resetHouses");
  houses[HOUSE_GRY] = HOUSE_GRY;
  houses[HOUSE_HUF] = HOUSE_HUF;
  houses[HOUSE_RAV] = HOUSE_RAV;
  houses[HOUSE_SLY] = HOUSE_SLY;
}


void resetAskedQuestions() {
  if(DEBUG_LEVEL == 1) Serial.println("resetAskedQuestions");
  resetAskedQuestionsPerHouse(q_asked_gry);
  resetAskedQuestionsPerHouse(q_asked_huf);
  resetAskedQuestionsPerHouse(q_asked_rav);
  resetAskedQuestionsPerHouse(q_asked_sly);
}


void resetAskedQuestionsPerHouse(int array[]) {
  int questionsPerHouse = 4;
  for (int i = 0; i < questionsPerHouse; i++) {
    if(DEBUG_LEVEL == 1){
      Serial.print("resetAskedQuestionsPerHouse" );
      Serial.print(i);
    }
    array[i] = NEW_QUESTION;
  }
}

void resetValues() {
  question_counter = 0;
  state_house = 0;
}


//**------STATE MACHINE PART-----///
//contains whole programm logic//
//controlls programm through 
//states of the GameState enum

enum class GameState {DEFAULT_STATE, HAT_AWAKES, GAME_START, ASKING, ANSWER, WAITING, CHECKING, GAMEOVER};
GameState state = GameState::DEFAULT_STATE;

void stateMachineRun() {
  switch (state) {
      if(DEBUG_LEVEL == 1) Serial.println("statemachine starts");

    //DEFAULT STATE
    case GameState::DEFAULT_STATE:
      if(DEBUG_LEVEL == 1) Serial.println("DEFAULT_STATE");
      //waits for sensor input
      while (true) {
        if(DEBUG_LEVEL == 1) Serial.println("user not touching");
        if (isUserTouchingHat()) {
          state = GameState::HAT_AWAKES;
          break;
        }
      }

    //HAT AWAKES
    case GameState::HAT_AWAKES:
      if(DEBUG_LEVEL == 1) Serial.println("HAT_AWAKES");
      hatawakes();
      state = GameState::GAME_START;
      break;

    //GAME START
    case GameState::GAME_START:
      if(DEBUG_LEVEL == 1) Serial.println("GAME_START");
      //waits for sensor input
      while (true) {
        if (isUserTouchingHat()) {
          state = GameState::ASKING;
          break;
        }
      }

    //ASKING
    case GameState::ASKING:
      if(DEBUG_LEVEL == 1) Serial.println("ASKING");
      askuser();
      state = GameState::ANSWER;
      break;

    //ANSWER
    case GameState::ANSWER:
      if(DEBUG_LEVEL == 1) Serial.println("ANSWER");
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
      if(DEBUG_LEVEL == 1) Serial.println("WAITING");
      updateCaptiveSensor();
      updateGyroSensor();
      state = GameState::ANSWER;
      return;

    //CHECKING
    case GameState::CHECKING:
      if(DEBUG_LEVEL == 1) Serial.println("CHECKING");
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
      if(DEBUG_LEVEL == 1) Serial.println("GAMEOVER");
      state = GameState::DEFAULT_STATE;
      gameOver();
      break;
  }
}
