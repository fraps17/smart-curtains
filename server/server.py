import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import time
import spidev
import flask

from flask import request, jsonify
try:
    GPIO.setmode(GPIO.BCM)

    pipes = [[0xE8, 0xE8, 0xF0, 0xF0, 0xE1], [0xF0, 0xF0, 0xF0, 0xF0, 0xE1]]

    radio = NRF24(GPIO, spidev.SpiDev())
    radio.begin(0, 17)

    radio.setPayloadSize(32)
    radio.setChannel(0x76)
    radio.setDataRate(NRF24.BR_1MBPS)
    radio.setPALevel(NRF24.PA_MIN)

    radio.setAutoAck(True)
    radio.enableDynamicPayloads()
    radio.enableAckPayload()

    radio.openWritingPipe(pipes[0])
    radio.openReadingPipe(1, pipes[1])
    radio.printDetails()
    # radio.startListening()


    app = flask.Flask(__name__)
    app.config["DEBUG"] = True

    @app.route('/status', methods = ['GET'])
    def status():
        message = [0, 0]
        start = time.time()
        radio.write(message)
        print("Sent the message: {}".format(message))
        radio.startListening()

        while not radio.available(0):
            time.sleep(1 / 100)
            if time.time() - start > 2:
                print("Timed out.")
                break

        receivedMessage = []
        radio.read(receivedMessage, radio.getDynamicPayloadSize())
        print("Received: {}".format(receivedMessage))
        radio.stopListening()
        return jsonify(receivedMessage)

    @app.route('/command', methods = ['POST'])
    def command():
        message = [1, 1]
        start = time.time()
        radio.write(message)
        print("Sent the message: {}".format(message))
        radio.startListening()

        while not radio.available(0):
            time.sleep(1 / 100)
            if time.time() - start > 2:
                print("Timed out.")
                break

        receivedMessage = []
        radio.read(receivedMessage, radio.getDynamicPayloadSize())
        print("Received: {}".format(receivedMessage))
        radio.stopListening()
        return jsonify(receivedMessage)


    app.run(host='0.0.0.0', port=8080)

finally:
    GPIO.cleanup()
