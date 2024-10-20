import cv2
from cvzone.HandTrackingModule import HandDetector
import serial  # Importing pyserial

# Initialize video capture and hand detector
cap = cv2.VideoCapture(0)
detector = HandDetector(maxHands=1, detectionCon=0.7)

# Initialize serial communication (adjust "COM8" and baud rate as needed)
mySerial = serial.Serial("COM3", 9600)  # Use pyserial for serial communication

while True:
    success, img = cap.read()
    
    if not success:
        print("Error: Could not read frame from camera")
        break
    
    # Detect hands and landmarks
    hands, img = detector.findHands(img)

    if hands:
        lmList = hands[0]['lmList']  # Get landmark list of the first hand
        bbox = hands[0]['bbox']  # Get bounding box of the first hand
        
        fingers = detector.fingersUp(hands[0])  # Get the state of fingers
        # Send finger data via serial communication
        mySerial.write(f"${''.join(map(str, fingers))}".encode())
 # Send the data via serial
        print(fingers)
    # Display the image with hand landmarks
    cv2.imshow("Image", img)

    # Exit loop when 'q' is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
