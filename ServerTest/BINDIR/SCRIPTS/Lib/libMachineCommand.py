import sys, traceback
import datetime
import random
import socket
sys.path.append('../')
import IcePy
import Ice
import UVServerAppServer

#####################################
#Fill in the classname of the object#
#####################################
class libMachineCommand:
    #####################################
    #Constructor class (stay's this way)#
    #####################################
    def __init__(self):
            print ('libMachineCommand::__init__->start')
            self.meIceCommunicator = None
            self.initData = None
            self.base = None
            self.meInterfaceConnection = None
            self.argv = [1]
            self.argv[0] = ''
            self.host = None
            self.port = None
            #Set up the interface communication
            try:
                self.initData = Ice.InitializationData()
                self.initData.properties = Ice.createProperties()
                self.initData.properties.load('config.client.Server')
                
                self.meIceCommunicator = Ice.initialize(self.argv,self.initData)

                self.base = self.meIceCommunicator.propertyToProxy('ServerAppServerAdaptor.Proxy')

                self.meInterfaceConnection = UVServerAppServer.ServerAppServerPrx.checkedCast(self.base)
                if not self.meInterfaceConnection:
                    raise RuntimeError("Invalid proxy")
            except:
                traceback.print_exc()
    ####################################
    #Destructor class (stay's this way)#
    ####################################
    def __del__(self):
            print ('libMachineCommand::__del__->start')
            #Close the interface communication
            if self.meIceCommunicator:
                #Clean up
                try:
                    print ('before self.meIceCommunicator -> destroying')
                    #self.meIceCommunicator.destroy()
                    print ('self.meIceCommunicator -> destroyed')
                except:
                    pass
                    #traceback.print_exc()
    #####################################
    # METHODS                           #
    #####################################
    #Getting the host parameters
    def getHostPlusPort(self, paLivingObject):
        print ('libMachineCommand::getHostPlusPort->start')
        status = 0
        try:
            loReturnProps = ['SOCKET_PORT','SOCKET_IP']
            loReturn = self.meInterfaceConnection.getFromTableDatabaseById( 'LIVING_OBJ_MACH',paLivingObject,loReturnProps)
            loReturnValues = loReturn[1]
            loReturnProps_lenght = len(loReturnProps)
            for i in range(0, loReturnProps_lenght):
                if str(loReturnProps[i]) == 'SOCKET_PORT':
                    self.port = loReturnValues[i]
                elif str(loReturnProps[i]) == 'SOCKET_IP':
                    self.host = loReturnValues[i]            
            status = 1
        except:
            traceback.print_exc()
            status = -1
        print ('libMachineCommand::getHostPlusPort->end')           
        return status
    #Perform the command over the socket
    def doCommand(self,paCommand):
        try:
            host = self.host
            port = int(self.port)

            client_socket = socket.socket()  # instantiate
            client_socket.settimeout(10) # max 10 seconds for response WVA to check not working          
            client_socket.connect((host, port))  # connect to the server
            client_socket.send(paCommand.encode())  # send message
            

            data = client_socket.recv(1024).decode()  # receive response
            client_socket.close()  # close the connection
            return data 
        except:
            return '<returnHardwareDevice><id>0</id><state>W0R0P0D0C0T0</state><datas></datas><error>Connection Socket Error on host:[' + self.host + '] port:[' + self.port + ']...</error></returnHardwareDevice>'
               
    #Run command
    def runMachine(self, paLivingObject, arg_mach):
        print ('libMachineCommand::runMachine->start')
        status = ''
        try:
            #Get the host and port from the object
            self.getHostPlusPort(paLivingObject)
            
            #create arguments string                                            
            loObjects = ''
            for loObject in arg_mach:
                loObjects = loObjects + '<data>' + str(loObject) + '</data>'
            
            #build sending command string
            loSendString =  '<hardwareDevice>' + '<id>' + str(random.randint(1, 999999)) + '</id>' \
                            '<connect do=\'false\'></connect>' \
                            '<disconnect do=\'false\'></disconnect>' \
                            '<state do=\'false\'></state>' \
                            '<run do=\'true\'>' + loObjects + '</run>' \
                            '<abort do=\'false\'></abort>' \
                            '<hold do=\'false\'></hold>' \
                            '<continue do=\'false\'></continue>' \
                            '<sendFile do=\'false\'></sendFile>' \
                            '<receiveFile do=\'false\'></receiveFile>' \
                            '<scriptCommand do=\'false\'></scriptCommand>' \
                            '<optionalCommand do=\'false\' name=\'SendDataCommand2\'>' \
                            '</optionalCommand>' \
                            '</hardwareDevice>'
                                
            loRecievedData = self.doCommand(loSendString)                        
            print (loRecievedData)
            
            #Get the recieved params TODO WVA
            status = loRecievedData
        except:
            traceback.print_exc()
            status = 'libCreateOperation::runMachine->Error'
        print ('libCreateOperation::runMachine->end')           
        return status
    #Pause command
    def holdMachine(self, paLivingObject, arg_mach):
        print ('libMachineCommand::holdMachine->start')
        status = ''
        try:
            #Get the host and port from the object
            self.getHostPlusPort(paLivingObject)
            
            #create arguments string                                            
            loObjects = ''
            for loObject in arg_mach:
                loObjects = loObjects + '<data>' + str(loObject) + '</data>'
            
            #build sending command string
            loSendString =  '<hardwareDevice>' + '<id>' + str(random.randint(1, 999999)) + '</id>' \
                            '<connect do=\'false\'></connect>' \
                            '<disconnect do=\'false\'></disconnect>' \
                            '<state do=\'false\'></state>' \
                            '<run do=\'false\'></run>' \
                            '<abort do=\'false\'></abort>' \
                            '<hold do=\'true\'>' + loObjects + '</hold>' \
                            '<continue do=\'false\'></continue>' \
                            '<sendFile do=\'false\'></sendFile>' \
                            '<receiveFile do=\'false\'></receiveFile>' \
                            '<scriptCommand do=\'false\'></scriptCommand>' \
                            '<optionalCommand do=\'false\' name=\'SendDataCommand2\'>' \
                            '</optionalCommand>' \
                            '</hardwareDevice>'
                                
            loRecievedData = self.doCommand(loSendString)                        
            print (loRecievedData)
            
            #Get the recieved params TODO WVA
            status = loRecievedData
        except:
            traceback.print_exc()
            status = 'libCreateOperation::holdMachine->Error'
        print ('libCreateOperation::holdMachine->end')           
        return status        
    #Continue command
    def continueMachine(self, paLivingObject, arg_mach):
        print ('libMachineCommand::continueMachine->start')
        status = ''
        try:
            #Get the host and port from the object
            self.getHostPlusPort(paLivingObject)
            
            #create arguments string                                            
            loObjects = ''
            for loObject in arg_mach:
                loObjects = loObjects + '<data>' + str(loObject) + '</data>'
            
            #build sending command string
            loSendString =  '<hardwareDevice>' + '<id>' + str(random.randint(1, 999999)) + '</id>' \
                            '<connect do=\'false\'></connect>' \
                            '<disconnect do=\'false\'></disconnect>' \
                            '<state do=\'false\'></state>' \
                            '<run do=\'false\'></run>' \
                            '<abort do=\'false\'></abort>' \
                            '<hold do=\'false\'></hold>' \
                            '<continue do=\'true\'>' + loObjects + '</continue>' \
                            '<sendFile do=\'false\'></sendFile>' \
                            '<receiveFile do=\'false\'></receiveFile>' \
                            '<scriptCommand do=\'false\'></scriptCommand>' \
                            '<optionalCommand do=\'false\' name=\'SendDataCommand2\'>' \
                            '</optionalCommand>' \
                            '</hardwareDevice>'
                                
            loRecievedData = self.doCommand(loSendString)                        
            print (loRecievedData)
            
            #Get the recieved params TODO WVA
            status = loRecievedData
        except:
            traceback.print_exc()
            status = 'libCreateOperation::continueMachine->Error'
        print ('libCreateOperation::continueMachine->end')           
        return status 
    #State command
    def stateMachine(self, paLivingObject, arg_mach):
        print ('libMachineCommand::stateMachine->start')
        status = ''
        try:
            #Get the host and port from the object
            self.getHostPlusPort(paLivingObject)
            
            #create arguments string                                            
            loObjects = ''
            for loObject in arg_mach:
                loObjects = loObjects + '<data>' + str(loObject) + '</data>'
            
            #build sending command string
            loSendString =  '<hardwareDevice>' + '<id>' + str(random.randint(1, 999999)) + '</id>' \
                            '<connect do=\'false\'></connect>' \
                            '<disconnect do=\'false\'></disconnect>' \
                            '<state do=\'true\'>' + loObjects + '</state>' \
                            '<run do=\'false\'></run>' \
                            '<abort do=\'false\'></abort>' \
                            '<hold do=\'false\'></hold>' \
                            '<continue do=\'false\'></continue>' \
                            '<sendFile do=\'false\'></sendFile>' \
                            '<receiveFile do=\'false\'></receiveFile>' \
                            '<scriptCommand do=\'false\'></scriptCommand>' \
                            '<optionalCommand do=\'false\' name=\'SendDataCommand2\'>' \
                            '</optionalCommand>' \
                            '</hardwareDevice>'
                                
            loRecievedData = self.doCommand(loSendString)                        
            print (loRecievedData)
            
            #Get the recieved params TODO WVA
            status = loRecievedData
        except:
            traceback.print_exc()
            status = 'libCreateOperation::stateMachine->Error'
        print ('libCreateOperation::stateMachine->end')           
        return status  
    #Abort command
    def abortMachine(self, paLivingObject, arg_mach):
        print ('libMachineCommand::abortMachine->start')
        status = ''
        try:
            #Get the host and port from the object
            self.getHostPlusPort(paLivingObject)
            
            #create arguments string                                            
            loObjects = ''
            for loObject in arg_mach:
                loObjects = loObjects + '<data>' + str(loObject) + '</data>'
            
            #build sending command string
            loSendString =  '<hardwareDevice>' + '<id>' + str(random.randint(1, 999999)) + '</id>' \
                            '<connect do=\'false\'></connect>' \
                            '<disconnect do=\'false\'></disconnect>' \
                            '<state do=\'false\'></state>' \
                            '<run do=\'false\'></run>' \
                            '<abort do=\'true\'>' + loObjects + '</abort>' \
                            '<hold do=\'false\'></hold>' \
                            '<continue do=\'false\'></continue>' \
                            '<sendFile do=\'false\'></sendFile>' \
                            '<receiveFile do=\'false\'></receiveFile>' \
                            '<scriptCommand do=\'false\'></scriptCommand>' \
                            '<optionalCommand do=\'false\' name=\'SendDataCommand2\'>' \
                            '</optionalCommand>' \
                            '</hardwareDevice>'
                                
            loRecievedData = self.doCommand(loSendString)                        
            print (loRecievedData)
            
            #Get the recieved params TODO WVA
            status = loRecievedData
        except:
            traceback.print_exc()
            status = 'libCreateOperation::abortMachine->Error'
        print ('libCreateOperation::abortMachine->end')           
        return status  
    #Connect command
    def connectMachine(self, paLivingObject, arg_mach):
        print ('libMachineCommand::connectMachine->start')
        status = ''
        try:
            #Get the host and port from the object
            self.getHostPlusPort(paLivingObject)
            
            #create arguments string                                            
            loObjects = ''
            for loObject in arg_mach:
                loObjects = loObjects + '<data>' + str(loObject) + '</data>'
            
            #build sending command string
            loSendString =  '<hardwareDevice>' + '<id>' + str(random.randint(1, 999999)) + '</id>' \
                            '<connect do=\'true\'>' + loObjects + '</connect>' \
                            '<disconnect do=\'false\'></disconnect>' \
                            '<state do=\'false\'></state>' \
                            '<run do=\'false\'></run>' \
                            '<abort do=\'false\'></abort>' \
                            '<hold do=\'false\'></hold>' \
                            '<continue do=\'false\'></continue>' \
                            '<sendFile do=\'false\'></sendFile>' \
                            '<receiveFile do=\'false\'></receiveFile>' \
                            '<scriptCommand do=\'false\'></scriptCommand>' \
                            '<optionalCommand do=\'false\' name=\'SendDataCommand2\'>' \
                            '</optionalCommand>' \
                            '</hardwareDevice>'
                                
            loRecievedData = self.doCommand(loSendString)                        
            print (loRecievedData)
            
            #Get the recieved params TODO WVA
            status = loRecievedData
        except:
            traceback.print_exc()
            status = 'libCreateOperation::connectMachine->Error'
        print ('libCreateOperation::connectMachine->end')           
        return status 
    #Disconnect command
    def disconnectMachine(self, paLivingObject, arg_mach):
        print ('libMachineCommand::disconnectMachine->start')
        status = ''
        try:
            #Get the host and port from the object
            self.getHostPlusPort(paLivingObject)
            
            #create arguments string                                            
            loObjects = ''
            for loObject in arg_mach:
                loObjects = loObjects + '<data>' + str(loObject) + '</data>'
            
            #build sending command string
            loSendString =  '<hardwareDevice>' + '<id>' + str(random.randint(1, 999999)) + '</id>' \
                            '<connect do=\'false\'></connect>' \
                            '<disconnect do=\'true\'>' + loObjects + '</disconnect>' \
                            '<state do=\'false\'></state>' \
                            '<run do=\'false\'></run>' \
                            '<abort do=\'false\'></abort>' \
                            '<hold do=\'false\'></hold>' \
                            '<continue do=\'false\'></continue>' \
                            '<sendFile do=\'false\'></sendFile>' \
                            '<receiveFile do=\'false\'></receiveFile>' \
                            '<scriptCommand do=\'false\'></scriptCommand>' \
                            '<optionalCommand do=\'false\' name=\'SendDataCommand2\'>' \
                            '</optionalCommand>' \
                            '</hardwareDevice>'
                                
            loRecievedData = self.doCommand(loSendString)                        
            print (loRecievedData)
            
            #Get the recieved params TODO WVA
            status = loRecievedData
        except:
            traceback.print_exc()
            status = 'libCreateOperation::disconnectMachine->Error'
        print ('libCreateOperation::disconnectMachine->end')           
        return status 
    #SendFile command
    def sendFileMachine(self, paLivingObject, arg_mach):
        print ('libMachineCommand::sendFileMachine->start')
        status = ''
        try:
            #Get the host and port from the object
            self.getHostPlusPort(paLivingObject)
            
            #create arguments string                                            
            loObjects = ''
            for loObject in arg_mach:
                loObjects = loObjects + '<data>' + str(loObject) + '</data>'
            
            #build sending command string
            loSendString =  '<hardwareDevice>' + '<id>' + str(random.randint(1, 999999)) + '</id>' \
                            '<connect do=\'false\'></connect>' \
                            '<disconnect do=\'false\'></disconnect>' \
                            '<state do=\'false\'></state>' \
                            '<run do=\'false\'></run>' \
                            '<abort do=\'false\'></abort>' \
                            '<hold do=\'false\'></hold>' \
                            '<continue do=\'false\'></continue>' \
                            '<sendFile do=\'true\'>' + loObjects + '</sendFile>' \
                            '<receiveFile do=\'false\'></receiveFile>' \
                            '<scriptCommand do=\'false\'></scriptCommand>' \
                            '<optionalCommand do=\'false\' name=\'SendDataCommand2\'>' \
                            '</optionalCommand>' \
                            '</hardwareDevice>'
                                
            loRecievedData = self.doCommand(loSendString)                        
            print (loRecievedData)
            
            #Get the recieved params TODO WVA
            status = loRecievedData
        except:
            traceback.print_exc()
            status = 'libCreateOperation::sendFileMachine->Error'
        print ('libCreateOperation::sendFileMachine->end')           
        return status  
    #RecieveFile command
    def recieveFileMachine(self, paLivingObject, arg_mach):
        print ('libMachineCommand::recieveFileMachine->start')
        status = ''
        try:
            #Get the host and port from the object
            self.getHostPlusPort(paLivingObject)
            
            #create arguments string                                            
            loObjects = ''
            for loObject in arg_mach:
                loObjects = loObjects + '<data>' + str(loObject) + '</data>'
            
            #build sending command string
            loSendString =  '<hardwareDevice>' + '<id>' + str(random.randint(1, 999999)) + '</id>' \
                            '<connect do=\'false\'></connect>' \
                            '<disconnect do=\'false\'></disconnect>' \
                            '<state do=\'false\'></state>' \
                            '<run do=\'false\'></run>' \
                            '<abort do=\'false\'></abort>' \
                            '<hold do=\'false\'></hold>' \
                            '<continue do=\'false\'></continue>' \
                            '<sendFile do=\'false\'></sendFile>' \
                            '<receiveFile do=\'true\'>' + loObjects + '</receiveFile>' \
                            '<scriptCommand do=\'false\'></scriptCommand>' \
                            '<optionalCommand do=\'false\' name=\'SendDataCommand2\'>' \
                            '</optionalCommand>' \
                            '</hardwareDevice>'
                                
            loRecievedData = self.doCommand(loSendString)                        
            print (loRecievedData)
            
            #Get the recieved params TODO WVA
            status = loRecievedData
        except:
            traceback.print_exc()
            status = 'libCreateOperation::recieveFileMachine->Error'
        print ('libCreateOperation::recieveFileMachine->end')           
        return status  
    #ScriptCommand command
    def scriptCommandMachine(self, paLivingObject, arg_mach):
        print ('libMachineCommand::scriptCommandMachine->start')
        status = ''
        try:
            #Get the host and port from the object
            self.getHostPlusPort(paLivingObject)
            
            #create arguments string                                            
            loObjects = ''
            for loObject in arg_mach:
                loObjects = loObjects + '<data>' + str(loObject) + '</data>'
            
            #build sending command string
            loSendString =  '<hardwareDevice>' + '<id>' + str(random.randint(1, 999999)) + '</id>' \
                            '<connect do=\'false\'></connect>' \
                            '<disconnect do=\'false\'></disconnect>' \
                            '<state do=\'false\'></state>' \
                            '<run do=\'false\'></run>' \
                            '<abort do=\'false\'></abort>' \
                            '<hold do=\'false\'></hold>' \
                            '<continue do=\'false\'></continue>' \
                            '<sendFile do=\'false\'></sendFile>' \
                            '<receiveFile do=\'false\'></receiveFile>' \
                            '<scriptCommand do=\'true\'>' + loObjects + '</scriptCommand>' \
                            '<optionalCommand do=\'false\' name=\'SendDataCommand2\'>' \
                            '</optionalCommand>' \
                            '</hardwareDevice>'
                                
            loRecievedData = self.doCommand(loSendString)                        
            print (loRecievedData)
            
            #Get the recieved params TODO WVA
            status = loRecievedData
        except:
            traceback.print_exc()
            status = 'libCreateOperation::scriptCommandMachine->Error'
        print ('libCreateOperation::scriptCommandMachine->end')           
        return status 
    #Optional command
    def optionalCommandMachine(self, paLivingObject, scriptName,  arg_mach):
        print ('libMachineCommand::optionalCommandMachine->start')
        status = ''
        try:
            #Get the host and port from the object
            self.getHostPlusPort(paLivingObject)
            
            #create arguments string                                            
            loObjects = ''
            for loObject in arg_mach:
                loObjects = loObjects + '<data>' + str(loObject) + '</data>'
            
            #build sending command string
            loSendString =  '<hardwareDevice>' + '<id>' + str(random.randint(1, 999999)) + '</id>' \
                            '<connect do=\'false\'></connect>' \
                            '<disconnect do=\'false\'></disconnect>' \
                            '<state do=\'false\'></state>' \
                            '<run do=\'false\'></run>' \
                            '<abort do=\'false\'></abort>' \
                            '<hold do=\'false\'></hold>' \
                            '<continue do=\'false\'></continue>' \
                            '<sendFile do=\'false\'></sendFile>' \
                            '<receiveFile do=\'false\'></receiveFile>' \
                            '<scriptCommand do=\'false\'></scriptCommand>' \
                            '<optionalCommand do=\'true\' name=\'' + scriptName + '\'>' + loObjects + '</optionalCommand>' \
                            '</hardwareDevice>'
                                
            loRecievedData = self.doCommand(loSendString)                        
            print (loRecievedData)
            
            #Get the recieved params TODO WVA
            status = loRecievedData
        except:
            traceback.print_exc()
            status = 'libCreateOperation::optionalCommandMachine->Error'
        print ('libCreateOperation::optionalCommandMachine->end')           
        return status         
##########################################################
#Created for manual testing of the methods on the class  #
##########################################################
def main():
    #Testing purpose
    loMachineCommand = libMachineCommand()
    loMachineCommand.runMachine('f90152f0-8b27-11ef-bac9-2cf05d44ba6c',['test','test'])
    loMachineCommand.connectMachine('f90152f0-8b27-11ef-bac9-2cf05d44ba6c',[])
    loMachineCommand.disconnectMachine('f90152f0-8b27-11ef-bac9-2cf05d44ba6c',[])
    loMachineCommand.abortMachine('f90152f0-8b27-11ef-bac9-2cf05d44ba6c',[])
    loMachineCommand.holdMachine('f90152f0-8b27-11ef-bac9-2cf05d44ba6c',[])
    loMachineCommand.continueMachine('f90152f0-8b27-11ef-bac9-2cf05d44ba6c',[])
    loMachineCommand.sendFileMachine('f90152f0-8b27-11ef-bac9-2cf05d44ba6c',[])
    loMachineCommand.recieveFileMachine('f90152f0-8b27-11ef-bac9-2cf05d44ba6c',[])
    loMachineCommand.scriptCommandMachine('f90152f0-8b27-11ef-bac9-2cf05d44ba6c',[])
    loMachineCommand.optionalCommandMachine('f90152f0-8b27-11ef-bac9-2cf05d44ba6c','scriptNameToInput',[])    
if __name__ == "__main__":
    main()
