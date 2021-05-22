#include <LCD.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//button area  $$$$$$$$$
const int button_1_position = A0; // 宣告0鍵接腳(低態動作)
const int button_2_position = A1;
const int button_3_position = A2;
const int button_4_position = A3;
int button_1 = 1, button_2 = 1, button_3 = 1, button_4 = 1;
//button       $$$$$$$$$$$$

int targetNumber = 0; //系統選定的 目標號碼

int numLeft = 0;  //選定的十位數字
int numRight = 0; //選定的個位數字

int minNumber = 0;  //用於顯示的範圍數字 (最小數)
int MaxNumber = 99; //用於顯示的範圍數字 (最大數)

int tryChance = 0; //紀錄猜了幾次
int tryTime = 7;   //遊戲設定 (可以猜的次數)

#define Do 523
#define Re 587
#define Mi 659
#define Fa 698
#define So 784
#define La 880
#define Si 988
int melody[7] = {Do, Re, Mi, Fa, So, La, Si};
const int buzzer = 13;

void setup()
{
    lcd.begin(16, 2);    //LCD 初始化
    lcd.setCursor(0, 0); // 設定游標位置在第一行 行首
    lcd.print("opening");

    delay(1000);                  //Wait rest of 1000ms recommended delay before
    lcd.clear();                  //清除LCD畫面
    lcd.print("Game Start 0-99"); //LCD Print

    lcd.setCursor(0, 1);           // 設定游標位置在第二行 行首
    lcd.print("press btn4 start"); //LCD Print

    pinMode(buzzer, OUTPUT);    //設定 蜂鳴器 腳位 為OUTPUT
    Serial.begin(9600);         //設定電腦連線平率
    randomSeed(analogRead(A0)); //random number

    Serial.println(targetNumber); //電腦端print 出 targetNumber

    tone(buzzer, melody[0]); //蜂鳴器響
    delay(100);
    noTone(buzzer); //蜂鳴停止
}

void loop()
{

    button_1 = digitalRead(button_1_position); // 讀取button是否按下 (button 1)
    button_2 = digitalRead(button_2_position); // 讀取button是否按下 (button 2)
    button_3 = digitalRead(button_3_position); // 讀取button是否按下 (button 3)
    button_4 = digitalRead(button_4_position); // 讀取button是否按下 (button 4)

    //button1   選擇十位數 加上10
    if (button_1)
    {
        lcd.clear();  //清除LCD Board畫面
        numLeft += 1; //十位數字加10
        if (numLeft == 10)
        {
            numLeft = 0;
        }
        //用來顯示 target number可能的範圍，與顯示使用者目前選定的數字
        lcd.print(minNumber);
        lcd.print("-");
        lcd.print(MaxNumber);
        lcd.print(" try again");
        lcd.setCursor(0, 1);
        lcd.print(numLeft * 10 + numRight);
        delay(100); //等待使用者放開按鈕(以免誤觸)
    }

    //button2   選擇個位數 加上1
    if (button_2)
    {
        lcd.clear();   //清除LCD Board畫面
        numRight += 1; //個位數字加1
        if (numRight == 10)
        {
            numRight = 0;
        }
        //用來顯示 target number可能的範圍，與顯示使用者目前選定的數字
        lcd.print(minNumber);
        lcd.print("-");
        lcd.print(MaxNumber);
        lcd.print(" try again");
        lcd.setCursor(0, 1);
        lcd.print(numLeft * 10 + numRight);
        delay(100); //等待使用者放開按鈕(以免誤觸)
    }

    //button3  確定按鈕
    if (button_3)
    {
        lcd.clear(); //清除LCD Board畫面

        //判斷如果選擇到對的number ->如果對了
        if (numLeft * 10 + numRight == targetNumber)
        {
            lcd.print("you win");    //LCD Board print
            tone(buzzer, melody[3]); //播放音樂
            delay(100);
            noTone(buzzer);
        }
        else
        {
            //->沒猜到
            //檢查範圍並設定，LCD Board 更新數字範圍
            if ((numLeft * 10 + numRight > targetNumber) && (numLeft * 10 + numRight < MaxNumber))
            {
                MaxNumber = numLeft * 10 + numRight;
            }
            if ((numLeft * 10 + numRight < targetNumber) && (numLeft * 10 + numRight > minNumber))
            {
                minNumber = numLeft * 10 + numRight;
            }
            lcd.print(minNumber);
            lcd.print("-");
            lcd.print(MaxNumber);
            lcd.print(" try again");
        }
        //輸入次數記錄
        tryChance += 1;
        if (tryChance > tryTime) //如果大於猜過次數 判斷為輸
        {
            lcd.clear();             //清除LCD Board畫面
            lcd.print("you lose");   //LCD Board print you lose
            tone(buzzer, melody[5]); //播放音樂
            delay(100);
            noTone(buzzer);
        }

        numLeft = 0;  //初始化使用者輸入數  設為0
        numRight = 0; //初始化使用者輸入數  設為0
        delay(100);   //等待使用者放開按鈕(以免誤觸)
    }

    //button4 開始遊戲  初始化必要data
    if (button_4)
    {
        lcd.clear();         //清除LCD board 畫面
        lcd.print("start");  //LCD board print start

        minNumber = 0;
        MaxNumber = 99;
        tryChance = 0;
        targetNumber = random(100);    //random number
        Serial.println(targetNumber);  //電腦端print 出 targetNumber
    }

    delay(200); // 每0.2秒檢查所有按鈕是否按下
}
