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

const static int DEBUG_LEVEL = 1; //turns console debuggin on (1) or off (0)
const static int TX_PIN = 11; //TXpin connected to the RXpin of the DFPlayer
const static int RX_PIN = 10; //RXpin connected to the TXpin of the DFPlayer
const static int VOLUME = 25;
const static int ANALOG_RANDOM=0; //unused analog pin to seed the RNG

DFRobotDFPlayerMini myDFPlayer;
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);
SoftwareSerial mySoftwareSerial(RX_PIN, TX_PIN);
CapacitiveSensor cs_4_8 = CapacitiveSensor(4, 8);


//**------SETUP PART-----///

void setup() {
  cs_4_8.set_CS_AutocaL_Millis(0xFFFFFFFF); //turn off autocalibrate
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  delay(2000);
  if(DEBUGGING_LEVEL == 1) Serial.println("set up start");
  setupGyro();
  setupDFPlayer();
  setUpArrays();
  if(DEBUGGING_LEVEL == 1) Serial.println("set up finished");
}


//**------LOOP PART-----///

void loop() {
  stateMachineRun();
  delay(500);
}

//**------GYROSENSOR  PART-----///
//**sets up gyrosensor to capture user movement**//

void setupGyro() {

  if(DEBUGGING_LEVEL == 1) Serial.println("setupGyro");
  gyro.enableAutoRange(true);
  if (!gyro.begin())
  {
    if(DEBUGGING_LEVEL == 1) Serial.println("Ooops, no L3GD20 detected ... Check your wiring!");
    while (1);
  }
}

//**------DFPlayer PART-----///
//**sets up dfplayer to play mp3s**//
//source:https://www.dfrobot.com/wiki/index.php/DFPlayer_Mini_SKU:DFR0299, 22.03.18


void setupDFPlayer() {
  if(DEBUGGING_LEVEL == 1) Serial.println("setupDFPlayer");
  //Use softwareSerial to communicate with mp3.
  if(!myDFPlayer.begin(mySoftwareSerial)) {  
    while (true);
  }
  if(DEBUGGING_LEVEL == 1) Serial.println("setupDFPlayer begins");

  //set VOLUME (0-30)
  myDFPlayer.volume(VOLUME); 
  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  //----Set device we use SD as default----
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  //----Mp3 play----
  //busy pin sends low if file is playing; high: no file is playing
  pinMode(PIN_DFP_BUSY, INPUT);
  if(DEBUGGING_LEVEL == 1) Serial.println("setupDFPlayer begins finished");
}

//**------MUSIC PART-----///
//**contains all sounds and their playtime**//
const int SOUNDS_FOLDER = 1;


int playSongFromSD(int songname) {
  if(DEBUGGING_LEVEL == 1){
    Serial.print("playSongFromSD " );
    Serial.println(songname );  
  }
  myDFPlayer.playFolder(SOUNDS_FOLDER, songname); 
    int waitingtime1=10000;
    int waitingtime2=20000;
    int waitingtime3=30000;
    int waitingtimeSleep=7000;

    switch(songname){
      case 1: delay(waitingtime1);
      break;

      case 2: delay(waitingtime1);
      break;

      case 3:delay(24000);
      break;

      case 4: delay(8000);
      break;

      case 5: delay(waitingtime1);
      break;

      case 6: delay(26000);
      break;

      case 7: delay(waitingtime2);
      break;

      case 8: delay(13000);
      break;

      case 9: delay(waitingtime1);
      break;

      case 10: delay(15000);
      break;

      case 11: delay(26000);
      break;

      case 12: delay(12000);
      break;

      case 13: delay(11000);
      break;

      case 14: delay(11000);
      break;

      case 15: delay(11000);
      break;

      case 16: delay(18000);
      break;

      case 17: delay(waitingtime1);
      break;

      case 18: delay(waitingtime1);
      break;

      case 19: delay(23000);
      break;

      case 20: delay(waitingtime3);
      break;

      case 21: delay(15000);
      break;

      case 22: delay(waitingtime3);
      break;

      case 97: delay(waitingtimeSleep);
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

const static long MAX_TOUCH_VALUE=350;
const static float MAX_HEAD_SHAKE=1;
const static float MAX_HEAD_NODDING=1;
const static int MAX_VALUES =20;


const static long TOUCH_FILTER = 3;
const static float SHAKE_FILTER= 3;
const static float NOD_FILTER = 3;

//checks if user touches the hat
bool isUserTouchingHat() {
   if(DEBUGGING_LEVEL == 1) Serial.print("is user touching hat ");
     
  long touchvalue = (long) cs_4_8.capacitiveSensor(30);  
   
  /*
    for(int j=0; j <TOUCH_FILTER;j++){
        touchvalue += (long) cs_4_8.capacitiveSensor(30);
            }

    touchvalue=touchvalue/TOUCH_FILTER;  */

   
  if(DEBUGGING_LEVEL == 1){
    Serial.print(touchvalue);
    Serial.print(" > ");
    Serial.println(MAX_TOUCH_VALUE);
  }

  if (touchvalue > MAX_TOUCH_VALUE) {
    if(DEBUGGING_LEVEL == 1) Serial.println("is user touching hat true");
    return true;
  }

  else {
    if(DEBUGGING_LEVEL == 1) Serial.println("is user touching hat false");
    return false;
  }

}

bool isUserShaking() {

  if(DEBUGGING_LEVEL == 1) Serial.println("is user shaking");
  sensors_event_t gyro_event;
  gyro.getEvent(&gyro_event); 
  float zValue=(float) gyro_event.gyro.z;
  float headShake = abs(zValue); 

  if(DEBUGGING_LEVEL == 1){
    Serial.print("is user shaking hat ");
    Serial.print(headShake);
    Serial.print(" > ");
    Serial.print(MAX_HEAD_SHAKE);
    Serial.print(" real z: ");
    Serial.println(gyro_event.gyro.z);
  }

  if (headShake >= SHAKE_FILTER) {
    if(DEBUGGING_LEVEL == 1) Serial.println("is user shaking hat true");
    return true;
  }

  else {
    if(DEBUGGING_LEVEL == 1) Serial.println("is user shaking hat false");
    false;
  }

}

bool isUserNodding() {
  if(DEBUGGING_LEVEL == 1) Serial.println("is user nodding");
  sensors_event_t gyro_event;
  gyro.getEvent(&gyro_event);
  float yValue = (float) gyro_event.gyro.y;
  float headNod = abs(yValue);
  
  if(DEBUGGING_LEVEL == 1){
  Serial.print("is user nodding ");
  Serial.print(headNod);
  Serial.print(" > ");
  Serial.print(MAX_HEAD_NODDING);
  Serial.print(" real y: ");
  Serial.println(gyro_event.gyro.y);
  }
  
  if (headNod >= MAX_HEAD_NODDING) {
    if(DEBUGGING_LEVEL == 1) Serial.println("is user nodding true");
    return true;
  }

  else {
    if(DEBUGGING_LEVEL == 1) Serial.println("is user nodding false");
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

void testquetions(){
  int sizes=4;
 
  for(int i=0;i<sizes;i++){
    if(DEBUGGING_LEVEL == 1) Serial.println(question_counter);
    int questionvalue=666;
    questionvalue=q_asked_gry[i];
    if(DEBUGGING_LEVEL == 1) Serial.println( "questionvalue gry");
    if(DEBUGGING_LEVEL == 1) Serial.println(questionvalue);  
      
    questionvalue=q_asked_huf[i];
    if(DEBUGGING_LEVEL == 1){
      Serial.println( "questionvalue huf");
      Serial.println(questionvalue);
    }
      
    questionvalue=q_asked_rav[i];
    if(DEBUGGING_LEVEL == 1) {
      Serial.println( "questionvalue rav");
      Serial.println(questionvalue);
    }
       
    questionvalue=q_asked_sly[i];
    if(DEBUGGING_LEVEL == 1){
      Serial.println( "questionvalue sly");
      Serial.println(questionvalue);
    }  
  }
}


//**we ask user and are handling his answer yes or no**//

//ask a single question
void askquestion(int house) {
  if(DEBUGGING_LEVEL == 1){
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


//used in state machine
void askuser() {
  randomSeed(analogRead(ANALOG_RANDOM));  //seed the RNG using the unused analog pin
  state_house = random(HOUSE_GRY, HOUSE_SLY + 1);
  for (int i = 0; i < NUMBER_OF_HOUSES; i++) {
    if (state_house == houses[i]) {
      askquestion(state_house);
      return;
    }
  }
  //as long questions if one house is found!
  if(DEBUGGING_LEVEL == 1) Serial.println("need new question");
  askuser();
}


//saves answer and marks it as 'old'
void saveAnswer(int house) {

  bool answer = getUserAnswer();
  if (answer == NO) {
    if(DEBUGGING_LEVEL == 1){
      Serial.println("saveAnswer no");
      Serial.println(house);
    }
    houses[house] = HOUSE_ANSWER_NO;
  }
  if(DEBUGGING_LEVEL == 1){
    Serial.println("saveAnswer yes");
    Serial.println(house);
  }
}

//used in state machine
void listenToAnswer() {
  saveAnswer(state_house);
}


bool getUserAnswer() {
  if(DEBUGGING_LEVEL == 1) Serial.println("getUserAnswer");
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
  if(DEBUGGING_LEVEL == 1) Serial.println(question);

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
  if(DEBUGGING_LEVEL == 1) Serial.println("isQuestionOld");
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
  if(DEBUGGING_LEVEL == 1) Serial.println("rememberAskedQuestion");

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
    if(DEBUGGING_LEVEL == 1) Serial.println("is user stillMoreThanOneHouseLeft");

    if (houses[house] != HOUSE_ANSWER_NO) {
      if(DEBUGGING_LEVEL == 1){
         Serial.print("is user stillMoreThanOneHouseLeft  houses[house] != HOUSE_ANSWER_NO ");
         Serial.print("house nr ");
         Serial.println(house);
         Serial.print("HOUSE_ANSWER_NO" );
         Serial.println(HOUSE_ANSWER_NO);
         Serial.print("houses[house]" );
         Serial.println(houses[house]);
      }
      counter++;
    }
  }
  if(DEBUGGING_LEVEL == 1){
    Serial.print("is user stillMoreThanOneHouseLeft counter ");
    Serial.println(counter);
    Serial.print("is user stillMoreThanOneHouseLeft maxhouse ");
    Serial.println(maxHouses);
  }

  if (counter > maxHouses) {
    return true;
  }
  else return false;
}

bool stillNoWinner() {
  if(DEBUGGING_LEVEL == 1) Serial.println("stillNoWinner");
  if (stillMoreThanOneHouseLeft() && MAX_QUESTIONS > question_counter) {
    question_counter = question_counter + 1;
    if(DEBUGGING_LEVEL == 1) Serial.println("stillNoWinner true");
    return true;
  }
  else return false;
  if(DEBUGGING_LEVEL == 1) Serial.println("stillNoWinner true");
}



//tells user his house if game is over
void tellUserHisHouse() {

  for (int house = HOUSE_GRY; house <= HOUSE_SLY; house++) {
    int answer = houses[house];
    if(DEBUGGING_LEVEL == 1){
      Serial.println("tellUserHisHouse");
      Serial.println(answer);
    }
    if (houses[house] != HOUSE_ANSWER_NO) {
      if(DEBUGGING_LEVEL == 1) Serial.println("tellUserHisHouse");
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
  if(DEBUGGING_LEVEL == 1) Serial.println("resetGame");
  resetHouses();
  resetAskedQuestions();
  resetValues();
}


void resetHouses() {
  if(DEBUGGING_LEVEL == 1) Serial.println("resetHouses");
  houses[HOUSE_GRY] = HOUSE_GRY;
  houses[HOUSE_HUF] = HOUSE_HUF;
  houses[HOUSE_RAV] = HOUSE_RAV;
  houses[HOUSE_SLY] = HOUSE_SLY;
}


void resetAskedQuestions() {
  if(DEBUGGING_LEVEL == 1) Serial.println("resetAskedQuestions");
  resetAskedQuestionsPerHouse(q_asked_gry);
  resetAskedQuestionsPerHouse(q_asked_huf);
  resetAskedQuestionsPerHouse(q_asked_rav);
  resetAskedQuestionsPerHouse(q_asked_sly);
}



void resetAskedQuestionsPerHouse(int array[]) {
  int questionsPerHouse = 4;
  for (int i = 0; i < questionsPerHouse; i++) {
    if(DEBUGGING_LEVEL == 1){
      Serial.print("resetAskedQuestionsPerHouse" );
      Serial.print(i);
    }
    array[i] = NEW_QUESTION;
  }
}

void resetValues(){
  question_counter=0;
  state_house=0;
}

//**------STATE MACHINE PART-----///
//contains whole logic//
//controlls programm

enum class GameState {DEFAULT_STATE, HAT_AWAKES, GAME_START, ASKING, ANSWER, WAITING, CHECKING, GAMEOVER};
GameState state = GameState::DEFAULT_STATE;

void stateMachineRun() {
  switch (state) {
    if(DEBUGGING_LEVEL == 1) Serial.println("statemachine starts");

    //DEFAULT STATE
    case GameState::DEFAULT_STATE:
      if(DEBUGGING_LEVEL == 1) Serial.println("DEFAULT_STATE");
      //waits for sensor input
      while (true) {
        if(DEBUGGING_LEVEL == 1) Serial.println("user not touching");
        if (isUserTouchingHat()) {
          state = GameState::HAT_AWAKES;
          break;
        }
      }

    //HAT AWAKES
    case GameState::HAT_AWAKES:
      if(DEBUGGING_LEVEL == 1) Serial.println("HAT_AWAKES");
      hatawakes();
      state = GameState::GAME_START;
      break;

    //GAME START
    case GameState::GAME_START:
      if(DEBUGGING_LEVEL == 1) Serial.println("GAME_START");
      //waits for sensor input
      while (true) {
        if (isUserTouchingHat()) {
          state = GameState::ASKING;
          break;
        }
      }
    //ASKING
    case GameState::ASKING:
      if(DEBUGGING_LEVEL == 1) Serial.println("ASKING");
      askuser();
      state = GameState::ANSWER;
      break;

    //ANSWER
    case GameState::ANSWER:
      if(DEBUGGING_LEVEL == 1) Serial.println("ANSWER");
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
      if(DEBUGGING_LEVEL == 1) Serial.println("WAITING");
      updateCaptiveSensor();
      updateGyroSensor();
      state = GameState::ANSWER;
      return;

    //CHECKING
    case GameState::CHECKING:
      if(DEBUGGING_LEVEL == 1) Serial.println("CHECKING");
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
      if(DEBUGGING_LEVEL == 1) Serial.println("GAMEOVER");
      state = GameState::DEFAULT_STATE;
      gameOver();
      break;
  }
}
