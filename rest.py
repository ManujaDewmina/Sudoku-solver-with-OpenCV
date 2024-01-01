import easyocr
import cv2
import re

def easyocr_model_load():
    text_reader = easyocr.Reader(["en"])  # Initialzing the ocr
    return text_reader

text_reader = easyocr_model_load()
img = cv2.imread("cells\cell_13_5.jpg")  
resized_img = cv2.resize(img, (500,500), interpolation=cv2.INTER_LINEAR)
cv2.imshow("image", resized_img)
cv2.waitKey(0)
resulteasyocr = text_reader.readtext(resized_img)
numeric_text = re.sub(r'[^0-9]', '', resulteasyocr[0][1])
print(numeric_text) 