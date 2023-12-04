# HW12 - WiFi

Maze game from week09 ([code](https://github.com/DM-GY-6063-2023F-D/week09/tree/main/play-maze), [original game](https://dm-gy-6063-2023f-d.github.io/week09/play-maze/)), but updated to use an Arduino with a potentiometer to control it.

## POST request: p5js -> Arduino

The p5js sketch sends the following object as a JSON to the Arduino to indicate the successful completion of a level:

```
data: {
  success: 1
}
```

## GET request: Arduino -> p5js

The object that is created by the Arduino and sent to p5js looks like this:

```
data: {
  A0: {
    raw: integer,
    previous: integer,
    average: integer
  }
}
```

```A0.average``` is used to update the rotation of the maze: the values from the ```A0``` pin are mapped from [0, 4095] to [-180, 180] to determine the angle of rotation of the board.

```A0.raw``` and ```A0.previous``` are also sent in case a different averaging method is desired.

## Schematic

![](./imgs/HW12-WiFi_sch.jpg)

## Board

![](./imgs/HW12-WiFi_bb.jpg)

## Actual Board

![](./imgs/HW12-WiFi_00.jpg)
![](./imgs/HW12-WiFi_01.jpg)

## Video

https://github.com/DM-GY-6063-2023F-D/HW12-WiFi/assets/850815/604e8fa5-3a14-4a9e-a8f3-fcd77c8119bd
