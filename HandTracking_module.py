import cv2
import mediapipe as mp #computer vison library
import numpy as np
import time


#class handDetector
class handDetector():
    def __init__(self, mode=False, maxHands=2, detectionCon=0.5, trackCon=0.5 ):
        self.mode = mode
        self.maxHands = maxHands
        self.detectionCon = detectionCon
        self.trackCon = trackCon
        
        self.mpHands = mp.solutions.hands 
        self.mpDraw  = mp.solutions.drawing_utils
        #create a hands object
        self.hands   = self.mpHands.Hands(self.mode, self.maxHands, self.detectionCon, self.trackCon)

        
        
    #Method findhads  
    #finds for hands and draws the wireframe of the hand dtected
    def findHands(self, img, draw=True, isolate=False):
        imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        self.results = self.hands.process(imgRGB)#results contains the information generated by hands.process() 
        
        
        if isolate == True:
            img = np.zeros(img.shape,dtype='uint8')


        if self.results.multi_hand_landmarks:                #if a hand is detected
            for handLms in self.results.multi_hand_landmarks:#for each detected hand
                if draw:
                    self.mpDraw.draw_landmarks(img, handLms, self.mpHands.HAND_CONNECTIONS)#draw points and conections over img


        return img
    
    
    
    
    #Method findPosition    
    #returns lmList[], a list which stores the coordinates of each landmark of the hand number handNo
    def findPosition(self, img, handNo=0, draw= True):
                              #hand number
        lmList = [] #landmark list
        
        if self.results.multi_hand_landmarks:                  #if a hand is detected
            myHand = self.results.multi_hand_landmarks[handNo] #myhand contains the data of the specified hand
        
            for id, lm in enumerate(myHand.landmark):          #for each landmark in the specified hand
                h, w, c = img.shape
                cx, cy = int(lm.x * w), int(lm.y * h)          #convert from decimal to pixel numbers
                #print(id, cx, cy)
                lmList.append([id, cx, cy])                     #append the data of each of the landmarks of the specified hand to lmlist[] (lanmarks list) 
                #if draw:
                #    cv2.circle(img, (cx, cy), 10, (255,0,0), -1)
            
                
        return lmList

