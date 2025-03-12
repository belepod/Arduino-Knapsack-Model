# Implementation of the Knapsack Algorithm in an Arduino-Based Vending Machine



## 1. Introduction
The knapsack problem is a fundamental optimization problem in computer science and operations research. It involves selecting a subset of items, each with a given weight and value, to maximize the total value without exceeding a weight constraint. This research explores the practical implementation of the knapsack algorithm using an Arduino-based hardware model. The model functions as a vending machine that selects and sorts items based on their weight and value, demonstrating how the algorithm can be applied in real-world scenarios.


## 2. Components Used
To build this hardware-based model, the following components are utilized:
·	Arduino Uno/Mega – The microcontroller unit for controlling operations.
·	Servo Motor – Used for sorting and placing selected items.
·	Ultrasonic Sensors (HC-SR04/US-100) – For distance measurement and item detection.
·	Load Cell with HX711 Amplifier – For precise weight measurement of items.
·	LCD Display (16x2 or OLED Display) – For displaying item information.
·	Push Buttons/Keypad – For user input and selecting constraints.
·	LED Indicators – For providing visual feedback.
·	Power Supply (5V-12V Adapter or Battery Pack) – To power the Arduino and connected components.
·	Chassis and Compartments – The physical structure of the vending machine.
·	Buzzer – Provides audio alerts for user feedback.
·	SD Card Module – Logs data for monitoring and optimization.


## 3. Working Principle

## 3.1. Algorithm Implementation

The project utilizes the 0/1 Knapsack Algorithm, which works as follows:
1.	Define the total weight capacity.
2.	Assign values and weights to available items.
3.	Sort items based on a value-to-weight ratio.
4.	Use dynamic programming to determine the optimal set of items to include.
5.	Select and dispense the chosen items accordingly.


![alt](https://github.com/belepod/Arduino-Knapsack-Model/blob/master/circuit%20diagram.png)



## 3.2. Hardware Functionality

1.	Item Detection: When a user places an item on the vending platform, the ultrasonic sensor detects its presence.
2.	Weight Measurement: The load cell measures the item's weight and sends data to the Arduino.
3.	Value Assignment: Pre-programmed item values are assigned based on weight and item type.
4.	Knapsack Calculation: The Arduino processes the input using the knapsack algorithm to determine whether the item should be selected.
5.	Sorting Mechanism: If the item is selected, the servo motor directs it to the accepted compartment; otherwise, it is moved to the rejected compartment.
6.	User Interaction: The LCD display and keypad allow users to input constraints such as total weight capacity.
7.	Data Logging: The SD card module logs transaction data for further analysis and optimization.
8.	Audio and Visual Feedback: LED indicators and a buzzer provide status updates.


## 4. Software Implementation

4.1. Arduino Code
The Arduino program is written in C++ using the Arduino IDE. The code consists of:
·	Sensor data collection (Ultrasonic, Load Cell)
·	Knapsack algorithm implementation
·	Motor control for sorting
·	Display handling for user interaction
·	Data logging using an SD card module
·	Feedback system control (LEDs and buzzer)


## Example Code for Knapsack Selection:
 ```
#include <Servo.h>
#include "HX711.h"
#include <SPI.h>
#include <SD.h>

#define TRIG_PIN 9
#define ECHO_PIN 10
#define SERVO_PIN 6
#define BUZZER_PIN 5
#define SD_CS 4

Servo sortingServo;
HX711 scale;
File logFile;

const int capacity = 500; // Maximum weight capacity in grams
struct Item {
    int weight;
    int value;
};
Item items[] = { {100, 60}, {200, 100}, {150, 90}, {120, 80} };
int itemCount = sizeof(items) / sizeof(items[0]);

bool knapsackSelection(int weight) {
    int dp[itemCount + 1][capacity + 1] = {0};
    for (int i = 1; i <= itemCount; i++) {
        for (int w = 0; w <= capacity; w++) {
            if (items[i - 1].weight <= w)
                dp[i][w] = max(items[i - 1].value + dp[i - 1][w - items[i - 1].weight], dp[i - 1][w]);
            else
                dp[i][w] = dp[i - 1][w];
        }
    }
    return dp[itemCount][capacity] >= weight;
}

void setup() {
    Serial.begin(9600);
    sortingServo.attach(SERVO_PIN);
    scale.begin(A1, A0);
    pinMode(BUZZER_PIN, OUTPUT);
    if (!SD.begin(SD_CS)) {
        Serial.println("SD Card initialization failed!");
        return;
    }
}

void loop() {
    float weight = scale.get_units();
    Serial.print("Weight: "); Serial.println(weight);
    if (knapsackSelection(weight)) {
        sortingServo.write(90);
        Serial.println("Item Accepted");
        digitalWrite(BUZZER_PIN, HIGH);
        delay(500);
        digitalWrite(BUZZER_PIN, LOW);
        logFile = SD.open("log.txt", FILE_WRITE);
        if (logFile) {
            logFile.println("Item Accepted");
            logFile.close();
        }
    } else {
        sortingServo.write(0);
        Serial.println("Item Rejected");
    }
    delay(2000);

}
```

## 5. Applications and Use Cases

### 5.1. Automated Inventory Sorting
The project can be extended to warehouses for automated sorting of packages based on priority, weight, and value constraints.

### 5.2. Smart Vending Machines
By implementing dynamic pricing and value-based selection, vending machines can offer cost-effective options based on availability and demand.

### 5.3. Waste Management Systems
This technology can be adapted to categorize recyclable and non-recyclable waste by assigning values to different materials.

### 5.4. Educational Purposes
This project serves as an excellent educational tool for teaching algorithms, optimization problems, and embedded systems.


## 6. Future Improvements

### 6.1. AI-Based Item Recognition
Using image processing and AI, items can be identified visually, reducing reliance on predefined values.


### 6.2. IoT Integration
An IoT-enabled system can allow remote monitoring and control of the vending machine via a web dashboard.

### 6.3. Battery Optimization
Enhancing the power efficiency of the system to support long-term deployment in remote locations.

### 6.4. Real-Time Data Analytics
Developing a cloud-based analytics system to optimize vending machine restocking and decision-making based on item selection trends.


## 7. Conclusion
This research successfully demonstrates the practical implementation of the knapsack algorithm using an Arduino-based vending machine. The integration of sensors and actuators with the algorithm allows for effective sorting and selection of items based on predefined constraints. Future developments can refine this model for broader applications in automation and smart retail systems, making it an ideal solution for efficient inventory management and decision-making processes.

![alt](https://github.com/belepod/Arduino-Knapsack-Model/blob/master/model_photo1.jpeg)
