#password='mnsbuldog'
#import rpdb2; rpdb2.start_embedded_debugger(password)

import sys, traceback, os
import IcePy
import Ice
import UVServerAppServer


from libCreateOperation import *


#####################################
#Fill in the classname of the object#
#####################################
class RTI_ADD_PHONE:
    #####################################
    #Constructor class (stay's this way)#
    #####################################
    def __init__(self):
            print ('RTI_ADD_PHONE::__init__->start')
            self.meIceCommunicator = None
            self.initData = None
            self.base = None
            self.meInterfaceConnection = None
            self.argv = [1]
            self.argv[0] = ''
            #Set up the interface communication
            try:
                self.initData = Ice.InitializationData()
                self.initData.properties = Ice.createProperties()
                self.initData.properties.load('config.client.Server')

                #self.meIceCommunicator = Ice.initialize(sys.argv,self.initData)

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
            print ('RTI_ADD_PHONE::__del__->start')
            #Close the interface communication
            if self.meIceCommunicator:
                #Clean up
                try:
                    print ('before self.meIceCommunicator -> destroying')
                    self.meIceCommunicator.destroy()
                    print ('self.meIceCommunicator -> destroyed')
                except:
                    pass
                    #traceback.print_exc()
    #####################################
    # METHODS                           #
    #####################################
    #Fill in the method name of the class
    def messageToAddPhoneNumber (self, a, b):
            print ('RTI_ADD_PHONE::messageToAddPhoneNumber->start')
            status = 1
            try:
                loOperation = libCreateOperation()
                loArguments = [a,b]
                #loOperation.openOperation('RTI_ADD_PHONE::messageToAddPhoneNumber',loArguments,'')
            
                #code comes here to communicate with interfance
                #self.meInterfaceConnection.insertItem(0)
                print ('Function with no arguments -> ' + a + ' -> ' + b)
                
                returnIds = None
                returnNames = None
                
                returnNames = self.meInterfaceConnection.getFromTableDatabaseGeneral('customer','0','0',None)
                
                
                print (returnNames[0])
                print (returnNames[1])
                print (returnNames[2])
                print (returnNames[3])
                loOperation.closeOperation('RTI_ADD_PHONE::messageToAddPhoneNumber',loArguments,'')
            except:
                traceback.print_exc()
                status = -1
            print ('RTI_ADD_PHONE::messageToAddPhoneNumber->end')           
            return status

    #Fill in the method name of the class
    def checkIfEmployerExists(self, a, b):
            print ('RTI_ADD_PHONE::checkIfEmployerExists->start')
            status = 1
            try:
                #code comes here to communicate with interface
                #self.meInterfaceConnection.insertItem(0)
                print ('Function with one arguement -> ' + a + ' -> ' + b)
            except:
                traceback.print_exc()
                status = -1
            print ('RTI_ADD_PHONE::checkIfEmployerExists->end')
            return status

    #Fill in the method name of the class
    def deletePhoneNumber(self, a, b):
            print ('RTI_ADD_PHONE::deletePhoneNumber->start')
            status = 0
            try:
                #code comes here to communicate with interfance
                #self.meInterfaceConnection.insertItem(0)
                print ('Function with 2 arguments -> ' + a + ' -> ' + b)
            except:
                traceback.print_exc()
                status = -1
            print ('RTI_ADD_PHONE::deletePhoneNumber->end')
            return status

##########################################################
#Created for manual testing of the methods on the class  #
##########################################################
def main():

    loClassNameExample = RTI_ADD_PHONE()
    #Test the function manually
    loClassNameExample.messageToAddPhoneNumber('a145ba3a-8283-11ef-916c-2cf05d44ba6c','f7d5e376-7c42-11ef-a4ad-2cf05d44ba6c')
    
    #loClassNameExample.methodNameFromClassOneArguments(1)
    #loClassNameExample.methodNameFromClassTwoArguments(1,2)




if __name__ == "__main__":
    main()
