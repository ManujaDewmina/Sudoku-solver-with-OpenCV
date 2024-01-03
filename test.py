import cv2
import numpy as np
from keras.models import load_model

classifier = load_model("./digit_model.h5")


def recognize_digit(image_path):
    image = cv2.imread(image_path, cv2.IMREAD_COLOR)

    # # Resize the image to 100x100
    # resized_image = cv2.resize(image, (50, 50), interpolation=cv2.INTER_LINEAR)

    # # Define the border size
    # border_size = 10

    # # Add a black border around the image
    # image_with_border = cv2.copyMakeBorder(resized_image, border_size, border_size, border_size, border_size, cv2.BORDER_CONSTANT, value=[255, 255, 255])

    gray = cv2.cvtColor(image.copy(), cv2.COLOR_BGR2GRAY)
    ret, th = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)

    contours = cv2.findContours(th, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[0]
    final_number = ''
    for cnt in contours:
        x, y, w, h = cv2.boundingRect(cnt)
        # make a rectangle box around each curve
        cv2.rectangle(image, (x, y), (x + w, y + h), (255, 0, 0), 1)

        # Cropping out the digit from the image corresponding to the current contours in the for loop
        digit = th[y:y + h, x:x + w]

        # Resize the image to 28x28
        img = cv2.resize(digit, (28, 28))

        cv2.imshow('image', img)
        cv2.waitKey(0)

        # Reshape the image to (1, 28, 28, 1)
        img = img.reshape(1, 28, 28, 1)

        # Normalize the pixel values to be between 0 and 1
        img = img.astype('float32') / 255.0

        # Make a prediction
        predict = classifier.predict(img)
        final_pred = np.argmax(predict[0])

        final_number += str(final_pred)

        # data = str(final_pred) + ' ' + str(int(max(pred) * 100)) + '%'

        font = cv2.FONT_HERSHEY_SIMPLEX
        fontScale = 0.5
        color = (255, 0, 0)
        thickness = 1
        # cv2.putText(image, data, (x, y - 5), font, fontScale, color, thickness)

    # cv2.imshow('image', image)
    # cv2.waitKey(0)
    return final_number


number = recognize_digit("cell_0_15.jpg")
print(number)