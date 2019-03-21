unsigned long int mean1 = 0;
unsigned long int val1 = 0;

unsigned long int mean2 = 0;
unsigned long int val2 = 0;

unsigned long int mean3 = 0;
unsigned long int val3 = 0;

unsigned long int mean4 = 0;
unsigned long int val4 = 0;

int positionvar = 0;
int prevpositon = 1;
int a = 1;


void setup()

{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(9, OUTPUT);
}


void loop()

{
  for (int i = 0; i < 1000; i++)
  {
    int sensorValue1 = analogRead(A0);
    int sensorValue2 = analogRead(A1);
    int sensorValue3 = analogRead(A2);
    int sensorValue4 = analogRead(A3);

    mean1 = mean1 + sensorValue1;
    mean2 = mean2 + sensorValue2;
    mean3 = mean3 + sensorValue3;
    mean4 = mean4 + sensorValue4;
    delay(1);
  }

  val1 = (mean1 / 1000);
  val2 = (mean2 / 1000);
  val3 = (mean3 / 1000);
  val4 = (mean4 / 1000);

  if (val1 >= 900)
  {
    positionvar = 2;
  }


  if (val2 >= 900)
  {

    positionvar = 1;
  }


  if (val3 >= 900)
  {
    positionvar = 4;

  }


  if (val4 >= 900)
  {
    positionvar = 3;
  }

  if (prevpositon != positionvar)
  {
    Serial.println("event trigger");
    digitalWrite(13, HIGH);
    digitalWrite(9, HIGH);
    delay(3500);
    digitalWrite(13, LOW);
    digitalWrite(9, LOW);

  }






  Serial.print("val1 : ");
  Serial.println(val1);
  Serial.print("val2 : ");
  Serial.println(val2);
  Serial.print("val3 : ");
  Serial.println(val3);
  Serial.print("val4 : ");
  Serial.println(val4);

  Serial.print("prevpositon : ");
  Serial.println(prevpositon);
  Serial.print("positionvar : ");
  Serial.println(positionvar);
  Serial.println("======================================");

  mean1 = 0;
  mean2 = 0;
  mean3 = 0;
  mean4 = 0;
  prevpositon = positionvar;
  delay(1000);

}
