
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20_U.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <CapacitiveSensor.h>


#define MAX_VALUES 20
const static int RXpin = 11;
const static int TXpin = 10;
const static int volume = 25;
const static int pinDfpBusy = 9;


DFRobotDFPlayerMini myDFPlayer;
Adafruit_L3GD20_Unified gyro = Adafruit_L3GD20_Unified(20);
SoftwareSerial mySoftwareSerial(TXpin, RXpin);
CapacitiveSensor cs_4_8 = CapacitiveSensor(4,8);

long touchvalues[MAX_VALUES];
long gyroXvalues[MAX_VALUES];
long gyroYvalues[MAX_VALUES];
enum class GameState {DEFAULT_STATE, HAT_AWAKES,GAME_START, ASKING, ANSWER, WAITING, CHECKING, GAMEOVER};
GameState state=GameState::DEFAULT_STATE;
static int state_house=0;

unsigned long previousMillis = 0;
const  long interval = 1000;


//**setup part**///


void setupDFPlayer(){
  Serial.println("setupDFPlayer");
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    while(true);
  }
  Serial.println("setupDFPlayer begins");

  //myDFPlayer.setTimeOut(500);
  myDFPlayer.volume(volume); //set volume (0-30)
  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  //----Set device we use SD as default----
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  //----Mp3 play----
 // myDFPlayer.play(1);  //Play next mp3
//busy pin sends low if file is playing; high: no file is playing
  pinMode(pinDfpBusy, INPUT);
  Serial.println("setupDFPlayer begins finished");
}

void setupGyro(){
  
   Serial.println("setupGyro");
  gyro.enableAutoRange(true);
  if(!gyro.begin())
  {
   Serial.println("Ooops, no L3GD20 detected ... Check your wiring!");
    while(1);
  }
}

//sets up single array
  void setupArray(long array[]){
    long start_Value=1;
    for(int i=0; i<sizeof(array);i++){
        array[i]=start_Value;
    }

  }



//sets up the arrays for the measured values
 void setUpArrays(){
  Serial.println("setUpArrays");
    setupArray(touchvalues);
    setupArray(gyroXvalues);
    setupArray(gyroYvalues);
  }




void setup() {
  cs_4_8.set_CS_AutocaL_Millis(0xFFFFFFFF);
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  delay(2000);
  Serial.println("ich beginne");
  setupGyro();
  setupDFPlayer();
  setUpArrays();
}


//**------STATE MACHINE-----///

void loop() {
  
  Serial.println("loop start");
updateCaptiveSensor();
 Serial.println("cs updated");
updateGyroSensor();
 Serial.println("gyro updated");
switch(state){
   Serial.println("state");
  case GameState::DEFAULT_STATE:
   Serial.println("defauklt");
      //bool hatIsWaiting=true;
       while(true){
        Serial.println("usee not touching");
         if(isUserTouchingHat()){
           state=GameState::HAT_AWAKES;
           break;
         }
       }

  case GameState::HAT_AWAKES:
   Serial.println("awakes");
      hatawakes();
      state=GameState::GAME_START;
      break;

  case GameState::GAME_START:
   Serial.println("start");
      //bool headisofhead=true;
      while(true){
        if(isUserTouchingHat()){
          state=GameState::ASKING;
          break;
          }
      }

  case GameState::ASKING:
   Serial.println("asking");
               askuser();
               state=GameState::ANSWER;
               break;
  case GameState::ANSWER:
   Serial.println("ANSWER");
               if(isUserShaking()||isUserNodding()){
                  listenToAnswer();
                  state=GameState::CHECKING;
                  break;
               }
               else{
                  state=GameState::WAITING;
                  break;
                }
   case GameState::WAITING:
     Serial.println("wating");
             updateCaptiveSensor();
             updateGyroSensor();
             state=GameState::ANSWER;
             return;
   case GameState::CHECKING:
     Serial.println("ckecking");
             if(stillNoWinner()){
                  state=GameState::ASKING;
                  break;
              }
              else{
                  tellUserHisHouse();
                   state=GameState::GAMEOVER;
                   break;
               }
   case GameState::GAMEOVER:
     Serial.println("gameover");
              gameOver();
              state=GameState::DEFAULT_STATE;
              break;

}

delay(500);

}

//**------Sensor Part-----///

//saves new values in array  of captive sensor and prints them
  void updateCaptiveSensor(){
       Serial.println("updateCaptiveSensor");
    /* Get a new sensor gyro_event */
    /*long start = millis();
    long capacitive_value =  cs_4_8.capacitiveSensor(30);

     Serial.println(millis() - start);        // check on performance in milliseconds
     Serial.println("\t");                    // tab character for debug window spacing
     Serial.println(capacitive_value);                  // print sensor output 1
     updateArray(capacitive_value, touchvalues);
    Serial.println();
    */
  }

//saves new values in array  of gyro sensor and prints them
  void updateGyroSensor(){
    Serial.println("updateGyroSensor");
    /*
    sensors_event_t gyro_event;
    gyro.getEvent(&gyro_event);
    Serial.println("X: "); Serial.println(gyro_event.gyro.x); Serial.println("  ");
    long zvalue=long(gyro_event.gyro.z);
    updateArray(zvalue, gyroXvalues);
    Serial.println("Y: "); Serial.println(gyro_event.gyro.y); Serial.println("  ");
    long yvalue=long(gyro_event.gyro.y);
    updateArray(yvalue, gyroYvalues);
    Serial.println("Z: "); Serial.println(gyro_event.gyro.z); Serial.println("  ");
   Serial.println("rad/s ");
   Serial.println();
   */

  }

//updates a single array, works like a stack(push and pull)
void updateArray(long newValue, long arr[]){
  for(int i=MAX_VALUES;i>=0;i--){
    if(i>0&&i<MAX_VALUES){
      arr[i+1]=arr[i];
    }
    if(i==0){
      arr[0]=newValue;
    }
  }
}

//checks if user touches the hat
bool isUserTouchingHat(){
  Serial.println("is user touching hat");
  //sensors_event_t gyro_event;
  //gyro.getEvent(&gyro_event);
  long touchvalue=cs_4_8.capacitiveSensor(30);
  long maxTouchValue=2000;
/*
  for(int j=0; j <20;j++){
      long xvalue=long(gyro_event.gyro.x);
      touchvalues[j]=abs(xvalue);
  }

  long average=getAverage(touchvalues);
*/
  if(touchvalue>maxTouchValue){
    Serial.println("is user touching hat true");
    return true;
  }

  

  else {
  Serial.println("is user touching hat false");
  return false;}

}

bool isUserShaking(){

  Serial.println("is user shaking hat");
  sensors_event_t gyro_event;
  gyro.getEvent(&gyro_event);
  long maxXValue=1;
  long headShake=abs(long(gyro_event.gyro.x));

  /*

  for(int j=0; j <MAX_VALUES;j++){
      long xvalue= (long)gyro_event.gyro.x;
      gyroXvalues[j]=abs(xvalue);
  }*/

 // long average=getAverage(gyroXvalues);

  if(headShake>maxXValue){
    Serial.println("is user shaking hat true");
    return true;
  }
 
  else {
     Serial.println("is user shaking hat false");
    false;
  }

}

bool isUserNodding(){
  Serial.println("is user nodding");
  sensors_event_t gyro_event;
    gyro.getEvent(&gyro_event);
  float maxYValue=2;
   long yvalue= long( gyro.getEvent(&gyro_event));
  /*for(int i=0; i<MAX_VALUES;i++){
      yvalue=long (gyro_event.gyro.y);
      gyroYvalues[i]=abs(yvalue);
  }*/
  //long average=getAverage(gyroYvalues);

  if(maxYValue>maxYValue){
     Serial.println("is user nodding true");
    return true;
  }
   
  else{ 
  Serial.println("is user nodding false"); 
  return false;}
}

//returns average values of a long array with size of 20
long getAverage(long values[]){
  long average=0;
  for(int i=0; i<MAX_VALUES;i++){
      average+=values[i];
  }
  long divider=long(MAX_VALUES);
  average=average/MAX_VALUES;
  return average;

}

int playSongFromSD(int songname){
    Serial.print("playSongFromSD " );
     Serial.println(songname );
  int SOUNDS_FOLDER=1;
  myDFPlayer.play(songname);
  int waitingtime1=10000;
  int waitingtime2=20000;
  int waitingtime3=30000;
  
  switch(songname){
      case 1: delay(waitingtime1); 
      break;
      
      case 3:delay(waitingtime3); 
      break;

      case 6: delay(waitingtime3); 
      break;

      case 11: delay(waitingtime3); 
      break;

      case 20: delay(waitingtime3);
      break;

      case 22: delay(waitingtime3);
      break;

      default: delay (waitingtime2);
      break;
      
    }
  }

bool isFilePlaying(){
  if (digitalRead(pinDfpBusy) == HIGH) {
      return false;
  }

  else true;

}




//****////GAME-PART///******/////

//ids for each house
const int HOUSE_GRY = 0;

const int HOUSE_HUF = 1;

const int HOUSE_RAV = 2;

const int HOUSE_SLY = 3;

const int HOUSE_ANSWER_NO = 20;

//house array contains all houses

int houses[4] = {HOUSE_GRY, HOUSE_HUF, HOUSE_RAV, HOUSE_SLY};

//Folder where sounds are on SD
const int SOUNDS_FOLDER = 1;

    //Welcome sound: tells the use to pick up the hat
int  welcome_sound= 1;

//houses questions the user has to answer
int questions_gry[4] = {2, 3, 4, 5};

int questions_huf[4] = {6,7,8,9};

int questions_rav[4] = {10,11,12,13};

int questions_sly[4] = {14,15,16,17};

   //tells user to answer question if user doesn't shake his head properly
int ask_repetition_sound = 18;

//hat goes to sleep
int sleeping_sound = 100;
int snorring_sound=101;

//tells the user in which house he is
int answers[4] = {19, 20, 21,22};


 //houses questions asked
const int NEW_QUESTION = 10;

int q_asked_gry[4] = {NEW_QUESTION, NEW_QUESTION, NEW_QUESTION, NEW_QUESTION};

int q_asked_huf[4] = {NEW_QUESTION, NEW_QUESTION, NEW_QUESTION, NEW_QUESTION};

int q_asked_rav[4] = {NEW_QUESTION, NEW_QUESTION, NEW_QUESTION, NEW_QUESTION};

int  q_asked_sly[4] = {NEW_QUESTION, NEW_QUESTION, NEW_QUESTION, NEW_QUESTION};

 //answers 'yes' and 'no'
const bool YES = true;

const bool NO = false;

const int MAX_QUESTIONS=4;

static int question_counter=0;

void hatawakes(){
 playSongFromSD(welcome_sound);
}

void startgame(){
  bool headisofhead=true;
  while(headisofhead){
       if(isUserTouchingHat()){
          playgame();
          break;
        }
    }
}

//starts a single game
void playgame(){
        int roundnumber=0;
        int maxRounds=1;
        while(roundnumber<maxRounds){
            playSingleRound();
            roundnumber++;
        }
        tellUserHisHouse();
        resetGame();
    }

void playSingleRound(){

        for(int house=HOUSE_GRY;house <= HOUSE_SLY; house++){
         Serial.println("singleround");
         Serial.println(house);
            askquestion(house);
            saveAnswer(house);
        }
    }

void askquestion(int house){
           Serial.println( "askquestion");
           Serial.println(house);
            switch(house){
                     case HOUSE_GRY:  playquestion(house, questions_gry);
                                    break;
                     case HOUSE_HUF:  playquestion(house, questions_huf);
                                    break;
                     case HOUSE_RAV: playquestion(house, questions_rav);
                                    break;
                     case HOUSE_SLY: playquestion(house, questions_sly);
                                    break;
                     case HOUSE_ANSWER_NO:break;
                     default:break;
            }

    }


//used in state machine
 void askuser(){
  state_house=random(HOUSE_GRY, HOUSE_SLY+1);
  for(int i=0; i<sizeof(houses);i++){
      if(state_house==houses[i]){
          askquestion(state_house);
          return;
        }
    }
    //as long questions if one house is found!
   Serial.println("need new question");
    askuser();
  }


void saveAnswer(int house){

        bool answer=getUserAnswer();
        if(answer==NO){
           Serial.println("saveAnswer no");
           Serial.println(house);
            houses[house]=HOUSE_ANSWER_NO;
        }
           Serial.println("saveAnswer yes");
           Serial.println(house);
    }

//usedin state machine
void listenToAnswer(){
  saveAnswer(state_house);
  }


bool getUserAnswer(){
 Serial.println("getUserAnswer");
        bool noanswer=true;
        //while durch timer ersetzen
        while(noanswer){
            if(isUserNodding()){
                bool answer=YES;
                return answer;
            }

            if(isUserShaking()){
                bool answer=NO;
                return answer;
            }
        }
        //fals Zeit abgelaufen ist, wird der Nutzer noch einmal gefragt
        askForRepetition();
        return getUserAnswer();
    }

void askForRepetition(){
    //passenden sound abspielen
    playSongFromSD(ask_repetition_sound);

    }


void playquestion(int house, int question_array[]){
        int maxrandom=HOUSE_SLY+1;
        int question = random(HOUSE_GRY,maxrandom);
        //Serial.printlnln("playquestion");
       Serial.println(question);

        //plays questions if question is new
        if (!isQuestionOld(house, question)) {
          playSongFromSD(question_array[question]);
          rememberAskedQuestion(question, house);
        }
        //trys again if question is old
        else{
                playquestion(house, question_array);
        }
    }
bool isQuestionOld(int house, int question){
  Serial.println("isQuestionOld");
        switch(house){
        case HOUSE_GRY:
        if (q_asked_gry[question] != NEW_QUESTION) {
          return true;
        }

        else return false;

        break;

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


void rememberAskedQuestion(int askedquestion, int house) {
  Serial.println("rememberAskedQuestion");

      switch(house){
        case HOUSE_GRY: q_asked_gry[askedquestion]=askedquestion;;
                        break;
        case HOUSE_HUF: q_asked_huf[askedquestion]=askedquestion;
                        break;
        case HOUSE_RAV: q_asked_rav[askedquestion]=askedquestion;
                        break;
        case HOUSE_SLY: q_asked_sly[askedquestion]=askedquestion;
                        break;
        default:        break;

        }


    }


void tellUserHisHouse(){

         for(int house=HOUSE_GRY;house<=HOUSE_SLY; house++){
           int answer=houses[house];
            Serial.println("tellUserHisHouse");
            Serial.println(answer);
            if(houses[house] != HOUSE_ANSWER_NO){
              Serial.println("tellUserHisHouse");
                playSongFromSD(answers[house]);
                return;
            }

        }
        playSongFromSD(answers[HOUSE_HUF]);
    }

void resetHouses(){
  Serial.println("resetHouses");
            houses[HOUSE_GRY]=HOUSE_GRY;
            houses[HOUSE_HUF]=HOUSE_HUF;
            houses[HOUSE_RAV]=HOUSE_RAV;
            houses[HOUSE_SLY]=HOUSE_SLY;
    }

bool stillMoreThanOneHouseLeft(){
  int counter=0;
  int maxHouses=1;
  for(int house=HOUSE_GRY;house<=HOUSE_SLY; house++){
     Serial.println("is user stillMoreThanOneHouseLeft");
   
     if(houses[house] != HOUSE_ANSWER_NO){
       counter++;
     }
   }
    Serial.print("is user stillMoreThanOneHouseLeft counter ");
    Serial.println(counter);
    Serial.print("is user stillMoreThanOneHouseLeft maxhouse ");
    Serial.println(maxHouses);

   if(counter>maxHouses){
     return true;
   }

   else return false;
}

bool stillNoWinner(){
      Serial.println("stillNoWinner");
      if(stillMoreThanOneHouseLeft()||MAX_QUESTIONS>question_counter){
        question_counter=question_counter+1;
        Serial.println("stillNoWinner true");
        return true;
      }
      else return false;
 Serial.println("stillNoWinner true");
}




void gameOver(){

  resetGame();
  playSongFromSD(sleeping_sound);
  playSongFromSD(snorring_sound);
}

void resetGame(){
   Serial.println("resetGame");
        resetHouses();
        resetAskedQuestions();
    }


void resetAskedQuestions(){
  Serial.println("resetAskedQuestions");
        resetAskedQuestionsPerHouse(questions_gry);
        resetAskedQuestionsPerHouse(questions_huf);
        resetAskedQuestionsPerHouse(questions_rav);
        resetAskedQuestionsPerHouse(questions_sly);
    }





void resetAskedQuestionsPerHouse(int array[]){
        int questionsPerHouse=4;
        for(int i=0; i<questionsPerHouse;i++){
            array[i]=NEW_QUESTION;
        }

    }

