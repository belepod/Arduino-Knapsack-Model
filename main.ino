#include <Servo.h>

Servo myServo;

struct Item {
  float weight;
  float value;
};

float fractionalKnapsack(Item items[], int n, float capacity) {
  float totalValue = 0.0;

  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i - 1; j++) {
      if ((items[j].value / items[j].weight) < (items[j + 1].value / items[j + 1].weight)) {
        Item temp = items[j];
        items[j] = items[j + 1];
        items[j + 1] = temp;
      }
    }
  }

  for (int i = 0; i < n; i++) {
    if (capacity >= items[i].weight) {
      capacity -= items[i].weight;
      totalValue += items[i].value;
    } else {
      totalValue += (items[i].value / items[i].weight) * capacity;
      break;
    }
  }

  return totalValue;
}

void setup() {
  Serial.begin(9600);

  myServo.attach(9);

  myServo.write(0);

  Serial.println("Enter number of items:");
}

void loop() {
  static int step = 0;
  static int numItems = 0;
  static Item items[10];
  static float capacity = 0.0;
  static int itemIndex = 0;

  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');

    if (step == 0) {
      numItems = input.toInt();
      if (numItems > 0 && numItems <= 10) {
        Serial.println("Enter weights and values for each item (weight value):");
        step = 1;
      } else {
        Serial.println("Invalid number of items, please enter again:");
      }
    }
    else if (step == 1) {
      if (itemIndex < numItems) {
        float weight = input.substring(0, input.indexOf(' ')).toFloat();
        float value = input.substring(input.indexOf(' ') + 1).toFloat();

        items[itemIndex] = {weight, value};
        itemIndex++;

        if (itemIndex == numItems) {
          Serial.println("Enter knapsack capacity:");
          step = 2;
        }
      }
    }
    else if (step == 2) {
      capacity = input.toFloat();
      float maxValue = fractionalKnapsack(items, numItems, capacity);

      Serial.print("Maximum Value: ");
      Serial.println(maxValue);

      int moves = (int)maxValue;
      for (int i = 0; i < moves; i++) {
        myServo.write(30);
        delay(150);
        myServo.write(0);
        delay(150);
      }

      Serial.println("Program completed.");
      step = 0;
      itemIndex = 0;
      Serial.println("Enter number of items:");
    }
  }
}

