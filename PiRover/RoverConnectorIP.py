#!python
#encoding: utf-8

import RoverConnector

# Just a placeholder for right now
class RoverConnectorIP(RoverConnector.RoverConnector):
	def __init__(self, hostMode):
		pass

	def listen(self):
		return True

	def accept(self, maxTries):
		return True

	def connect(self):
		return True

	def send(self, data):
		print(">>[RoverConnector.send] Dummy data sent: '%s'" % data)

	def recv(self, maxLength):
		return ""

	def setBlocking(self, blocking):
		pass

	def disconnect(self):
		pass
