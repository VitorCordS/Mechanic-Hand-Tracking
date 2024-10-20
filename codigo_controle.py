import cv2
from cvzone.HandTrackingModule import HandDetector
import serial  # Importando pyserial

# Inicializa a captura de vídeo e o detector de mãos
cap = cv2.VideoCapture(0)
detector = HandDetector(maxHands=1, detectionCon=0.7)

# Inicializa a comunicação serial (ajuste "COM3" e a taxa de baud conforme necessário)
mySerial = serial.Serial("COM3", 9600)  # Usando pyserial para comunicação serial

while True:
    success, img = cap.read()
    
    if not success:
        print("Erro: Não foi possível ler o quadro da câmera")
        break
    
    # Detecta as mãos e os marcos
    hands, img = detector.findHands(img)

    if hands:
        lmList = hands[0]['lmList']  # Obtém a lista de marcos da primeira mão
        bbox = hands[0]['bbox']  # Obtém a caixa delimitadora da primeira mão
        
        fingers = detector.fingersUp(hands[0])  # Obtém o estado dos dedos
        # Envia os dados dos dedos via comunicação serial
        mySerial.write(f"${''.join(map(str, fingers))}".encode())  # Envia os dados via serial
        print(fingers)  # Exibe o estado dos dedos
    
    # Exibe a imagem com os marcos da mão
    cv2.imshow("Imagem", img)

    # Sai do loop quando 'q' é pressionado
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
