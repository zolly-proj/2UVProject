import sys, traceback
import datetime

sys.path.append('../')
#sys.path.append('C:/Users/woute/AppData/Local/Programs/Python/Python310/Lib/site-packages/')
import IcePy
import Ice
import UVServerAppServer



#####################################
#Fill in the classname of the object#
#####################################
class libCreateOperation:
    #####################################
    #Constructor class (stay's this way)#
    #####################################
    def __init__(self):
            print ('libCreateOperation::__init__->start')
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
            print ('libCreateOperation::__del__->start')
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
    #Fill in the method name of the class
    def openOperation(self, name, objects, argB):
            print ('libCreateOperation::openOperation->start')
            status = 0
            try:

                #create arguments string                                            
                loObjects = None
                for loObject in objects:
                    if loObjects == None:
                        loObjects = str(loObject)
                    else:
                        loObjects = loObjects + '$;$' + str(loObject)
                    
                loDateTime = datetime.datetime.now()                                                          
                loValues = [name, loObjects,loDateTime.strftime("%Y-%m-%d %H:%M:%S.%f")]                                            
                loReturnMessage = self.meInterfaceConnection.insertIntoTableDatabase( 'OPERATION',['name','objects','datum_start'],loValues,['varchar(255)','uuid[]','timestamp'])
                print (loReturnMessage)
                status = 1
            except:
                traceback.print_exc()
                status = -1
            print ('libCreateOperation::openOperation->end')           
            return status

    #Fill in the method name of the class
    def closeOperation(self, name, objects, argB):
            print ('libCreateOperation::closeOperation->start')
            status = 0
            try:
            
                #create arguments string                                            
                loObjects = None
                for loObject in objects:
                    if loObjects == None:
                        loObjects = str(loObject)
                    else:
                        loObjects = loObjects + '$;$' + str(loObject)
                
                loProperties = ['OBJECTS','PERFORMED_WORKSTEP','DATUM_STOP']
                loValues = [loObjects,'','']
                loTypeValues = ['uuid[]','uuid','timestamp']
                loLogExp = ["=","=","="]
                loReturn = self.meInterfaceConnection.getFromTableDatbaseByProperty( 'OPERATION','0','0',loProperties,loValues,loTypeValues,loLogExp)
                
                loPass = loReturn[0]
                loIds = loReturn[1]
                loReturnMessage = loReturn[2]
                print (loReturn)
                
                for loId in loIds:
                    loDateTime = datetime.datetime.now()
                    loTimestamp = loDateTime.strftime("%Y-%m-%d %H:%M:%S.%f")
                    loProperties = ['DATUM_STOP']
                    loValues = [loTimestamp]
                    loPropTypes = ['timestamp']
                    loReturnUpdate = self.meInterfaceConnection.updateIntoTableDatabase('OPERATION',loId,loProperties,loValues,loPropTypes)
                    print (loReturnUpdate)
                status = 1
            except:
                traceback.print_exc()
                status = -1
            print ('libCreateOperation::closeOperation->end')           
            return status

##########################################################
#Created for manual testing of the methods on the class  #
##########################################################
def main():

    loLibCreateOperation = libCreateOperation()
    #Test the function manually
        
    argEmplCust = ['a145ba3a-8283-11ef-916c-2cf05d44ba6c','f7d5e376-7c42-11ef-a4ad-2cf05d44ba6c']
  
    loLibCreateOperation.openOperation('ThisOperation', argEmplCust, '')
    loLibCreateOperation.closeOperation('ThisOperation', argEmplCust, '')
    #loLibCreateOperation.openOperation(argEmplCust)
    
if __name__ == "__main__":
    main()
