import sys, traceback
import os




#password='mnsbuldog'
#import rpdb2; rpdb2.start_embedded_debugger(password)

#####################################
#Fill in the classname of the object#
#####################################
class clDatabaseFunctions:
    #####################################
    #Constructor class (stay's this way)#
    #####################################
    def __init__(self):
            print 'clDatabaseFunctions::__init__->start'

    ####################################
    #Destructor class (stay's this way)#
    ####################################
    def __del__(self):
            print 'clDatabaseFunctions::__del__->start'

    #####################################
    # METHODS                           #
    #####################################
    def createDatabaseBackup(self, paFileName, paDatabaseName, paUserName):		
            print 'clDatabaseFunctions::createDatabaseBackup->start'
            status = 0
            try:
                
		loCommand = 'pg_dump --username=' + paUserName + ' ' + paDatabaseName + ' > "' + paFileName + '"'
		print loCommand
		print 'end'
                os.system(loCommand)
            except:
                traceback.print_exc()
                status = -1
            print 'clDatabaseFunctions::createDatabaseBackup->end'
            return status

##########################################################
#Created for manual testing of the methods on the class  #
##########################################################
def main():
    print 'main()'
    loDatabaseFunctions = clDatabaseFunctions()
    loDatabaseFunctions.createDatabaseBackup('wouter','DB_TESTPROJECT','postgres')
    
    
if __name__ == "__main__":
    main()
