import socket
import xml.etree.ElementTree as ET
import datetime
from threading import Thread

#####################################
#Fill in the classname of the object#
#####################################
class clSocketBoxSimulator:
    #####################################
    #Constructor class (stay's this way)#
    #####################################
    def __init__(self):
            print ('clSocketBoxSimulator::__init__->start')
            self.commandID = ""
            self.timerSendFile = 0
            self.timerRecieveFile = 0
            self.timerRunFile = 0
            self.paused = 0
            self.error = ''
            self.objectID = ''
            self.counter = 50

            #Set up the interface communication
            try:
                self.commandID = ""
                self.timerSendFile = 0
                self.timerRecieveFile = 0
                self.timerRunFile = 0
                self.error = ''
            except:
                traceback.print_exc()
    ####################################
    #Destructor class (stay's this way)#
    ####################################
    def __del__(self):
            print ('clSocketBoxSimulator::__del__->start')

    ###########
    #Functions#
    ###########
    def run_server(self):
        # create a socket object
        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        server_ip = "127.0.0.1"
        port = 4002

        # bind the socket to a specific address and port
        server.bind((server_ip, port))
        # listen for incoming connections
        server.listen(10)
        print('Listening on ' + server_ip + ':' + str(port))


        while True:
            client_socket, addr = server.accept()
            print('Accepted connection from ' + str(addr))
            client_handler = Thread(target=self.handle_client, args=(client_socket,))
            client_handler.start()
            # close server socket
        server.close()
        
    def handle_client(self,client_socket):
        while True:
            try:
                message = client_socket.recv(1024).decode('utf-8')
                if not message:
                    print('Nothing received')
                else:    
                    print('Received: ' + message)
                    ######################### parse the input #######################################
                    self.parseXML(message)
                    ######################### send the response #######################################
                    #W0R0D0P0C0T5648779879
                    #Waiting
                    #Running
                    #DataTransfer
                    #Pauzed
                    #Connected
                    #Timestamp
                    returnState = ""
                    
                    if self.timerRunFile > 0:
                        returnState = returnState + 'W0R1'
                    else:
                        returnState = returnState + 'W1R0'
                    if self.paused == 0:
                        returnState = returnState + 'P0'
                    else:
                        returnState = returnState + 'P1'
                    if self.timerSendFile == 0 and self.timerRecieveFile == 0:
                        returnState = returnState + 'D0'
                    else:
                        returnState = returnState + 'D1'
                    #Connected
                    returnState = returnState + 'C1'
                    loTimeStamp = datetime.datetime.now()
                    returnState = returnState + 'T' + loTimeStamp.strftime("%f")
                    #<object name=\"Magazijn_1001\"><x value=\"30.5\"/><y value=\"30.5\"/><z value=\"1.0\"/></object><object name=\"Employee_200\"><x value=\"60.5\"/><y value=\"50.5\"/><z value=\"0.5\"/></object>
                    returnString = ('<returnHardwareDevice><id>' + self.commandID + '</id><state>' + returnState + '</state><datas>' + self.objectID + '</datas><error>' + self.error + '</error></returnHardwareDevice>')

                    print('Response: ' + returnString)
                    print('sendall start')
                    client_socket.sendall(returnString.encode('utf-8'))
                    print('sendall not handeled')
            except:
                print('Exception')
                break
        client_socket.close()
        
    def parseXML(self,request):
        print('START parseXML(requestXML)')
        loRoot = ET.fromstring(request)
        loSelectedChild = None
        loSelectedChildren = []
        for loChild in loRoot:
            print(loChild.tag, loChild.attrib)
            if loChild.tag == 'id':
                self.commandID = loChild.text
                
            loPerform = loChild.get('do')
            if (loPerform == 'true'):
                loSelectedChild = loChild
                for loChildParams in loSelectedChild:
                    loSelectedChildren.append(loChildParams.text)
                break
        
        if loSelectedChild.tag == "connect":
            self.commandConnect(loSelectedChildren)
        elif loSelectedChild.tag == "disconnect":
            self.commandDisconnect(loSelectedChildren)
        elif loSelectedChild.tag == "state":
            self.commandState(loSelectedChildren)
        elif loSelectedChild.tag == "run":
            self.commandRun(loSelectedChildren)
        elif loSelectedChild.tag == "abort":
            self.commandAbort(loSelectedChildren)
        elif loSelectedChild.tag == "hold":
            self.commandHold(loSelectedChildren)
        elif loSelectedChild.tag == "continue":
            self.commandContinue(loSelectedChildren)
        elif loSelectedChild.tag == "sendFile":
            self.commandSendFile(loSelectedChildren)
        elif loSelectedChild.tag == "recieveFile":
            self.commandRecieveFile(loSelectedChildren)
        elif loSelectedChild.tag == "scriptCommand":
            self.commandScriptCommand(loSelectedChildren)
        elif loSelectedChild.tag == "optionalCommand":
            self.commandOptionalCommand(loSelectedChildren)
        print('STOP parseXML(requestXML)')

    def commandConnect(self,paParams):
        print('START commandConnect(paParams')
        print('STOP commandConnect(paParams')
        
    def commandDisconnect(self,paParams):
        print('START commandDisconnect(paParams)')
        print('STOP commandDisconnect(paParams)')
        
    def commandState(self,paParams):
        print('START commandState(paParams)')
        
        ##################################################
        #Moving of 2 objects into the field
        ##################################################
        self.counter = self.counter - 5
        if self.counter < 1:
            self.counter = 50
            
        if self.counter >= 40:
            self.objectID = '<object name=\"Magazijn_1001\"><x value=\"70.5\"/><y value=\"70.5\"/><z value=\"1.0\"/></object><object name=\"Employee_200\"><x value=\"80.5\"/><y value=\"50.5\"/><z value=\"0.5\"/></object>'
        elif self.counter >= 30:
            self.objectID = '<object name=\"Magazijn_1001\"><x value=\"60.5\"/><y value=\"60.5\"/><z value=\"1.0\"/></object><object name=\"Employee_200\"><x value=\"65.5\"/><y value=\"40.5\"/><z value=\"0.5\"/></object>'
        elif self.counter >= 20:
            self.objectID = '<object name=\"Magazijn_1001\"><x value=\"40.5\"/><y value=\"40.5\"/><z value=\"1.0\"/></object><object name=\"Employee_200\"><x value=\"50.5\"/><y value=\"30.5\"/><z value=\"0.5\"/></object>'
        elif self.counter >= 10:
            self.objectID = '<object name=\"Magazijn_1001\"><x value=\"30.5\"/><y value=\"30.5\"/><z value=\"1.0\"/></object><object name=\"Employee_200\"><x value=\"35.5\"/><y value=\"20.5\"/><z value=\"0.5\"/></object>'
        elif self.counter >= 0:
            self.objectID = '<object name=\"Magazijn_1001\"><x value=\"15.5\"/><y value=\"15.5\"/><z value=\"1.0\"/></object><object name=\"Employee_200\"><x value=\"10.5\"/><y value=\"10.5\"/><z value=\"0.5\"/></object>'
        ##################################################
        
        if self.timerRecieveFile > 0:
            self.timerRecieveFile = self.timerRecieveFile - 1
        
        if self.timerRunFile > 0:
            self.timerRunFile = self.timerRunFile - 1
        
        if self.timerRunFile < 5:
            self.error = ''
        
        if self.timerSendFile > 0:
            self.timerSendFile = self.timerSendFile -1
        
        print('STOP commandState(paParams)')
        
    def commandRun(self,paParams):
        print('START commandRun(paParams)')
        self.timerRunFile = 10
        self.error = 'approx 10 seconds running'
        print('STOP commandRun(paParams)')
        
    def commandAbort(self,paParams):
        print('START commandAbort(paParams)')
        print('STOP commandAbort(paParams)')
        
    def commandHold(self,paParams):
        print('START commandHold(paParams)')
        self.paused = 1
        print('STOP commandHold(paParams)')
        
    def commandContinue(self,paParams):
        print('START commandContinue(paParams)')
        self.paused = 0
        print('STOP commandContinue(paParams)')
        
    def commandSendFile(self,paParams):
        print('START commandSendFile(paParams)')
        self.timerSendFile = 10
        print('STOP commandSendFile(paParams)')
        
    def commandRecieveFile(self,paParams):
        print('START commandRecieveFile(paParams)')
        self.timerReceiveFile = 10
        print('STOP commandRecieveFile(paParams)')
        
    def commandScriptCommand(self,paParams):
        print('START commandScriptCommand(paParams)')
        print('STOP commandScriptCommand(paParams)')
        
    def commandOptionalCommand(self,paParams):
        print('START optionalCommand(paParams)')
        print('STOP optionalCommand(paParams)')
##########################################################
#Created for manual testing of the methods on the class  #
##########################################################
def main():

    meSocketBoxSimulator = clSocketBoxSimulator()
    #Test the function manually
    #while True:
    #    try:
    meSocketBoxSimulator.run_server()
    #    except:
    #        print('Exception') 
            

if __name__ == "__main__":
    main()
