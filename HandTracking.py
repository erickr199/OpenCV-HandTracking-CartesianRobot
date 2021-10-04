########################################
#Name: Erick Osvaldo Castelazo Guerra  #
#Id: A01273870                         #
#Computer Vision                       #
#June 20201                            #
#Hand Tracking                         #
########################################
import cv2
import mediapipe as mp #computer vison library
import numpy as np
import time
import math
import serial
import HandTracking_module as htm

#fps
pTime = 0
cTime = 0
#timeCount 
time1= time.time()
handOpen = 1
#roi area
x_1  = 160
y_1  = 80
x_2  = 480 + 40
y_2  = 400 + 40


#Open serial port
serialCom = serial.Serial("COM5",9600)

#Create handDetector object
detector = htm.handDetector(maxHands=1)
         #thumb, index, middle, ring, pinky
tipIds = [4,8,12,16,20]


#camera
#################################
wCam, hCam = 640, 480
cap = cv2.VideoCapture(1)
cap.set(3, wCam)
cap.set(4, hCam)
################################
while True:
    success, img = cap.read()
        
    #h,w,c = overlayList[0].shape
    #img[0:h,0:w] = ovarlayList[0]
        
        
    img = detector.findHands(img,isolate=False) #Detect hand and draw hand wireframe
    lmList = detector.findPosition(img)         #Get list of coordinates of landmarks of the hand
    
    if len(lmList) != 0:#if the hand was detected
        
        #Get hand coordinates    x, y
        handCoords = [lmList[9][1], lmList[9][2]+50]
        #draw coordinates location
        cv2.circle(img, (lmList[9][1], lmList[9][2]+50), 5, (255,0,0), -1)
        
        
        #check if within roi bounding box
        if handCoords[0] >x_1 and handCoords[0]<x_2 and handCoords[1]>y_1 and handCoords[1]<y_2:
            handCoords[0]-=x_1
            handCoords[1]-=y_1
            cv2.putText(img, "x: " + str(handCoords[0]) ,(10,200), cv2.FONT_HERSHEY_PLAIN, 1.5, (255,255,0),2)
            cv2.putText(img, "y: " + str(handCoords[1]) ,(10,220), cv2.FONT_HERSHEY_PLAIN, 1.5, (255,255,0),2)
            
             
            #Serial Comunication, send a command every 0.5 seconds
            time2 =  time.time()
            timeCount = time2-time1
            if timeCount > 0.5:
                command = str(handCoords[0]) + ',' + str(handCoords[1]) + ',' + str(handOpen)
                cv2.putText(img, command,(550,40), cv2.FONT_HERSHEY_PLAIN, 1, (255,0,255),2)
                serialCom.write(command.encode('ascii'))
                time1 = time.time();
        
        
        
        #Finger Count for gesture recognition
        fingers = []
        
        #Thumb
        #if lmList[tipIds[0]][1] > lmList[tipIds[0]-1][1]:
        #    fingers.append(1)
        #else:
        #    fingers.append(0)
        
        #other 4 fingers
        for id in range(1,5):
            if lmList[tipIds[id]][2] < lmList[tipIds[id]-2][2]:
                fingers.append(1)#if the finger is opened
            else:
                fingers.append(0)#if the finger is clossed
                
        
          
        totalFingers = fingers.count(1)
        if totalFingers >= 1:
            cv2.putText(img, "Hand is open",(10,420), cv2.FONT_HERSHEY_PLAIN, 1.5, (0,255,0),2)
            handOpen = 1
        else:
            cv2.putText(img, "Hand is closed",(10,420), cv2.FONT_HERSHEY_PLAIN, 1.5, (0,0,255),2)
            handOpen=0;
        
        cv2.putText(img, str(fingers) ,(10,440), cv2.FONT_HERSHEY_PLAIN, 1.5, (0,0,255),2)
        #cv2.putText(img, str(fingers.count(1)) ,(10,400), cv2.FONT_HERSHEY_PLAIN, 4, (255,0,0),2)
    
    
    
    
    cv2.rectangle(img,(x_1,y_1),(x_2,y_2),(0,255,0),thickness=2) #draw roi    
    #fps
    cTime = time.time()
    fps = 1/(cTime-pTime)
    pTime = cTime
    cv2.putText(img, str(int(fps)),(10,70), cv2.FONT_HERSHEY_PLAIN, 1.5, (255,0,0),2)
         
    cv2.imshow("Image", img) #display in window
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break            
    
    
# When everything done, release the capture
cap.release()
cv2.destroyAllWindows()

#Close serial Port
serialCom.close()

#########################################
#Notes
#wrist: 0
#Finger tips:
#thumb   4
#index   8
#middel  12
#ring    16
#pinky   20 
#
#lmList = [id, cx, cy]
#           0   1   2
#Example
#lmList[0,2] = y coord of landmark 0
#image starts at top left