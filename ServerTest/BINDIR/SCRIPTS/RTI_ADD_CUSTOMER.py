#password='mnsbuldog'
#import rpdb2; rpdb2.start_embedded_debugger(password)
import sys, traceback
import Ice
import UVServerAppServer




#####################################
#Fill in the classname of the object#
#####################################
class RTI_ADD_CUSTOMER:
    #####################################
    #Constructor class (stay's this way)#
    #####################################
    def __init__(self):
            print 'RTI_ADD_CUSTOMER::__init__->start'
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
            print 'RTI_ADD_CUSTOMER::__del__->start'
            #Close the interface communication
            if self.meIceCommunicator:
                #Clean up
                try:
                    #self.meIceCommunicator.destroy()
                except:
                    traceback.print_exc()
    #####################################
    # METHODS                           #
    #####################################
    #Fill in the method name of the class
    def methodNameFromClassWithoutArguments(self):
            print 'RTI_ADD_CUSTOMER::methodNameFromClassWithoutArguments->start'
            status = 0
            try:
                #code comes here to communicate with interfance
                self.meInterfaceConnection.insertItem(0)
            except:
                traceback.print_exc()
                status = -1
            print 'RTI_ADD_CUSTOMER::methodNameFromClassWithoutArguments->end'
            return status

    #Fill in the method name of the class
    def methodNameFromClassOneArguments(self, a):
            print 'RTI_ADD_CUSTOMER::methodNameFromClassOneArguments->start'
            status = 0
            try:
                #code comes here to communicate with interface

                self.meInterfaceConnection.insertItem(0)
            except:
                traceback.print_exc()
                status = -1
            print 'RTI_ADD_CUSTOMER::methodNameFromClassOneArguments->end'
            return status

    #Fill in the method name of the class
    def methodNameFromClassTwoArguments(self, a, b):
            print 'RTI_ADD_CUSTOMER::methodNameFromClassTwoArguments->start'
            status = 0
            try:
                #code comes here to communicate with interfance
                self.meInterfaceConnection.insertItem(0)
            except:
                traceback.print_exc()
                status = -1
            print 'RTI_ADD_CUSTOMER::methodNameFromClassTwoArguments->end'
            return status

##########################################################
#Created for manual testing of the methods on the class  #
##########################################################
def main():

    loClassNameExample = RTI_ADD_CUSTOMER()
    #Test the function manually
    #loClassNameExample.methodNameFromClassWithoutArguments()
    #loClassNameExample.methodNameFromClassOneArguments(1)
    #loClassNameExample.methodNameFromClassTwoArguments(1,2)




if __name__ == "__main__":
    main()
