import cv2
import numpy as np
import base64
import requests
import pymongo
import tensorflow as tf
from pymongo import MongoClient

# Setup MongoDB Atlas connection
mongo_uri = 'mongodb+srv://codeirawan:<password>@lazora-cluster.1avlnts.mongodb.net/mydatabase?retryWrites=true&w=majority'
client = MongoClient(mongo_uri)
db = client['mydatabase']
collection = db['images']

# Load the trained model
model = tf.keras.models.load_model('cabai_model.h5')

# Print model summary to understand the expected input shape
model.summary()

# Function to preprocess the frame before feeding it into the model
def preprocess_frame(frame):
    resized_frame = cv2.resize(frame, (150, 150))
    normalized_frame = resized_frame / 255.0
    input_frame = np.expand_dims(normalized_frame, axis=0)
    return input_frame

# Function to save the frame and prediction result to MongoDB
def save_to_mongo(frame, label, confidence):
    # Ensure all data is converted to standard types
    _, buffer = cv2.imencode('.jpg', frame)
    encoded_image = base64.b64encode(buffer).decode('utf-8')

    document = {
        "image": encoded_image,
        "label": label,
        "confidence": float(confidence),  # Convert numpy.float32 to Python float
        "description": "Gambar dari sumber dengan prediksi model"
    }

    collection.insert_one(document)

# Function to fetch image from URL
def fetch_image_from_url(url):
    response = requests.get(url)
    if response.status_code == 200:
        img_arr = np.array(bytearray(response.content), dtype=np.uint8)
        return cv2.imdecode(img_arr, -1)
    else:
        print("Error: Could not retrieve image from URL.")
        return None

# Function to fetch image from ESP32
def fetch_image_from_esp32(url):
    response = requests.get(url)
    if response.status_code == 200:
        img_arr = np.array(bytearray(response.content), dtype=np.uint8)
        return cv2.imdecode(img_arr, -1)
    else:
        print("Error: Could not retrieve image from ESP32.")
        return None

# Function to capture image from camera
def capture_image_from_camera():
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("Error: Could not open camera.")
        return None

    ret, frame = cap.read()
    cap.release()
    if ret:
        return frame
    else:
        print("Failed to capture image from camera.")
        return None

# Choose source of image
source = "esp32"  # Options: "url", "esp32", "camera"

# Image URL for testing
image_url = "https://cdns.klimg.com/merdeka.com/i/w/news/2020/10/05/1228037/540x270/cara-menanam-cabai-dari-biji-keringnya.jpg"
esp32_url = "http://192.168.94.19/capture"  # Replace with actual ESP32 IP address

while True:
    if source == "url":
        frame = fetch_image_from_url(image_url)
    elif source == "esp32":
        frame = fetch_image_from_esp32(esp32_url)
    elif source == "camera":
        frame = capture_image_from_camera()
    else:
        print("Invalid source.")
        break

    if frame is None:
        break

    input_frame = preprocess_frame(frame)

    # Make prediction
    predictions = model.predict(input_frame)

    class_idx = np.argmax(predictions[0])
    confidence = float(predictions[0][class_idx])  # Convert numpy.float32 to Python float

    if class_idx == 0:
        label = "Cabai Baik"
    else:
        label = "Cabai Busuk"

    # Save the frame and prediction result to MongoDB
    save_to_mongo(frame, label, confidence)

    # Display the result on the frame
    cv2.putText(frame, f"{label} ({confidence * 100:.2f}%)",
                (10, 30),
                cv2.FONT_HERSHEY_SIMPLEX,
                1,
                (0, 255, 0) if label == "Cabai Baik" else (0, 0, 255),
                2,
                cv2.LINE_AA)

    # Show the frame with the prediction
    cv2.imshow('Cabai Detection', frame)

    # Press 'q' to quit
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cv2.destroyAllWindows()
