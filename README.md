# DIY Sports Push Buttons - Based on ESP32-C3

![image](https://user-images.githubusercontent.com/65546290/203234603-6e5cd233-cc56-4582-bf34-83ca08bb1c30.png)

Push buttons, also known as tactile switches, can be found everywhere from simple lamps, doorbells to large industrial machines. There are so many kinds of push buttons, but I like the arcade push buttons most because they usually have a great tactile, "clicky” feel. I had several arcade buttons in my toolbox, and I always want to make something with them. Lately, I've been working out. One day an idea comes to my mind, can I turn my arcade buttons into a device that helps me do sports? Sounds not bad. I decided to give it a try.
I am going to make 4 sports push buttons, one as a master node with a screen that supports a menu display and the other three as salve nodes. They are all based on ESP32-C3, a micro Beetle controller from DFRobot. It integrates a battery charging management function, which allows us to connect a lithium battery to it as a mobile power supply for the project, or charge the battery from the controller's USB-C port when the battery is low. Also, to make them more interactive, I will add some sound and lighting to the push buttons. 

This is what it looks like in the end. 
![image](https://user-images.githubusercontent.com/65546290/203230928-a3bd57ae-2f2b-4b55-ab39-c5c1d633fb72.png)


[The relevant information is uploaded on our YouTube channel]()



# HARDWARE LIST
* 4x ESP32-C3
* OLED Display
* 4x Digital Buzzer
* WS2812 RGB LED strip
* 4x 3.7V battery
* 4x Arcade Push Button
* 4x Power switch
* 4x Case

# Connection 
The master node comes with a screen while the slave nodes do not. All parts should be connected as the diagram below.
* Buzzer -> Controller’s P6
* Button -> Controller’s P5
* WS2812 -> Controller’s P0
* Screen->I2C
![image](https://user-images.githubusercontent.com/65546290/203233723-237fa063-0c83-4304-bb07-6a98fe9f6231.png)


# Upload Program
 
Since the master node communicates with the slave nodes by ESP-NOW in this project, the only MAC address of each ESP32-C3 should be obtained so that the master knows which device the message is intended for. 
![image](https://user-images.githubusercontent.com/65546290/203233889-405fa662-9fe3-46ba-a65f-a03adeaad898.png)


Revise the MAC address to yours and upload the program.
![image](https://user-images.githubusercontent.com/65546290/203233622-48e2762e-c5f8-4fbe-9b16-464a67e1eab3.png)


# Start to Play
In the program, I set three modes for the buttons. Short press to select mode, press and hold for 2 seconds to confirm or enter the selection menu.
![image](https://user-images.githubusercontent.com/65546290/203233989-ef65266a-602b-45e6-8486-f3b6dfc41a50.png)


## Mode 1- Whac-A-Mole
In this mode, all 4 buttons show green light at first, then one of them changes to red randomly, and we need to hit the red one within a certain time to score. They can be played in various ways.

![image](https://user-images.githubusercontent.com/65546290/203234109-37bd4f0c-3304-4a8b-818c-4af7a4595fcc.png)
![image](https://user-images.githubusercontent.com/65546290/203234123-a973bb7f-ae09-4c57-af68-dc8b5c06c65c.png)
![image](https://user-images.githubusercontent.com/65546290/203234135-f96d0b91-76ec-4f8d-8794-9f9deb69d9f9.png)
![image](https://user-images.githubusercontent.com/65546290/203234141-b2d509f8-55b5-40c1-b4d6-28a8cb7d762c.png)


## Mode 2- Relay Race Mode
In this mode, hit the first button, then the second will light up green. Just like a relay race, the green light acts like a baton, which will be transferred one by one among four members repeatedly.
![image](https://user-images.githubusercontent.com/65546290/203234157-c2e63d0f-9de5-407e-9f12-3155112ebaf3.png)


## Mode 3- Random Picker Mode
When entering this mode, 4 buttons repeatedly show blue light in order, hit the main button(button with screen), and this time the button whose light turns on in blue will be the picked one. 


## Or you can only use one button to test how high you can jump. 
![image](https://user-images.githubusercontent.com/65546290/203234225-36cf0f9a-961f-47f3-8e82-68df7021ba0f.png)

# ESP-NOW delay test:
In the project, the master communicates with the salves by ESP-NOW. To ensure that the NOW protocol meets the communication requirement, here I completed a simple ESP-NOW delay test to record the time one node transmits a message to another and receives a replay. It can be seen that the total delay for transmitting and receiving is about 4ms, which means only a 2ms delay for one-direction transmission, equaling to 1/100s.

![image](https://user-images.githubusercontent.com/65546290/203234284-34f8f1fc-bd2d-4307-86b8-5980f0c1d546.png)
 

# Thank
Well, that's all for this project. There are so many possibilities for these sports push buttons. Come and try your sports buttons! Thanks for reading, feel free to leave your comments if you have any other good ideas!
