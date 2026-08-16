import os
import os.path
import glob
import re
import shutil
import sys

#######################################################
# To modify
################################################################
env = Environment (tools=['default','qt',], TARGET_ARCH='x86_64')
env.Replace (QT_AUTOSCAN=0)
env.Replace (QT_LIB='/usr/lib/x86_64-linux-gnu')
env.Replace (QT_MOCHSUFFIX='.cpp')
env.Replace (QT_UICIMPLSUFFIX='.cpp')         
env["QTDIR"] = "/usr/lib/x86_64-linux-gnu/qt5"       
env['InstallPath'] = '/home/zolly/Desktop/2UVProject/Unix'
env['ICE'] = "/usr/bin"
env.Append (CPPPATH='/usr/include/x86_64-linux-gnu/qt5')
env.Append (CPPPATH='/usr/include/python3.12/')
env.Append (CPPPATH='/usr/include/postgresql')
env.Append (LIBPATH='/usr/lib/x86_64-linux-gnu')
#####################################################################


env.Append (CPPPATH= env['InstallPath'] + '/ServerTest/MethodsDll/qrcode')
env.Append (CPPPATH= env['InstallPath'] + '/ServerTest/MethodsDll/qrcode/qrcodegen')
env.Append (CPPPATH= env['InstallPath'] + '/ServerTest/MethodsDll')
env.Append (CPPPATH= env['InstallPath'] + '/ServerTest/ObjectsDll')
env.Append (CPPPATH= env['InstallPath'] + '/ServerTest')
env.Append (CPPPATH= env['InstallPath'] + '/Server')
env.Append (CPPPATH= env['InstallPath'] + '/AppStartup')
env.Append (CPPPATH= env['InstallPath'] + '/CycleBuilder')
env.Append (CPPPATH= env['InstallPath'] + '/ObjectViewer')
env.Append (CPPPATH= env['InstallPath'] + '/DatabaseEditor')
env.Append (CPPPATH= env['InstallPath'] + '/GraphClient')
env.Append (CPPPATH= env['InstallPath'] + '/SocketBox')
env.Append (CPPPATH= env['InstallPath'] + '/LogServer')


env['BINDIR_ServerTest'] = env['InstallPath'] + "/ServerTest/BINDIR"
env['BINDIR_Server'] = env['InstallPath'] + "/Server/BINDIR"
env['BINDIR_AppStartup'] = env['InstallPath'] + "/AppStartup/BINDIR"
env['BINDIR_CycleBuilder'] = env['InstallPath'] + "/CycleBuilder/BINDIR"
env['BINDIR_ObjectViewer'] = env['InstallPath'] + "/ObjectViewer/BINDIR"
env['BINDIR_DatabaseEditor'] = env['InstallPath'] + "/DatabaseEditor/BINDIR"
env['BINDIR_GraphClient'] = env['InstallPath'] + "/GraphClient/BINDIR"
env['BINDIR_SocketBox'] = env['InstallPath'] + "/SocketBox/BINDIR"
env['BINDIR_LogServer'] = env['InstallPath'] + "/LogServer/BINDIR"

def Qt4UiTargets (target, source, env):
  base, extension = os.path.splitext ((str(source[0])))
  targetlist = []
  targetlist.append( base + '.h')
  return (targetlist, source)

def Qt4UiBuilderFunction (target, source, env):
  uiheader = str(target[0])
  uifile = str(source[0])
  command = env["QTDIR"]+"/bin/uic -o %s %s" % (uiheader, uifile)
  print (command)
  os.system (command)

Qt4UiBuilder = Builder (action=Qt4UiBuilderFunction, emitter=Qt4UiTargets)
env.Append (BUILDERS={'QT4UI' : Qt4UiBuilder})

def IceBuilderPython(env):
    ICEFiles = [[],[],[],[],[],[]]
    ICEFiles[0] = glob.glob ('CycleBuilder/*.ice')
    ICEFiles[1] = glob.glob ('GraphClient/*.ice')
    ICEFiles[2] = glob.glob ('LogServer/*.ice')
    ICEFiles[3] = glob.glob ('ObjectViewer/*.ice')
    ICEFiles[4] = glob.glob ('Server/*.ice')
    ICEFiles[5] = glob.glob ('ServerTest/*.ice')
    for ICEFilesTemp in ICEFiles:
        for ICEFile in ICEFilesTemp:
            #ICEFile = os.path.basename (ICEFile)
            print (ICEFile)
            x = ICEFile.split('/')
            os.system (env["ICE"] + "/slice2cpp --output-dir " + env['InstallPath'] + "/" + x[0] +  " " + env['InstallPath'] + "/" + x[0] + "/" + x[1])

if env['PLATFORM'] == 'win32':
    env.Append (CCFLAGS='-W3 -MD -O2 -EHsc -GR -DQT_THREAD_SUPPORT')
else:
    env.Append (CCFLAGS='-Wall -fPIC -DQT_THREAD_SUPPORT -D_REENTRANT')

Export ('env')

#Python ICE functions
IceBuilderPython(env)

################
#FILE TO COPY #
###############
shutil.copyfile('./AppStartup/AppStarter.xml', env["BINDIR_AppStartup"] + '/AppStarter.xml')

shutil.copyfile('./CycleBuilder/config.client.Server', env["BINDIR_CycleBuilder"] + '/config.client.Server')
shutil.copyfile('./CycleBuilder/config.client.LogServer', env["BINDIR_CycleBuilder"] + '/config.client.LogServer')

shutil.copyfile('./DatabaseEditor/DatabaseConfig.xml', env["BINDIR_DatabaseEditor"] + '/DatabaseConfig.xml')

shutil.copyfile('./GraphClient/config.client.Server', env["BINDIR_GraphClient"] + '/config.client.Server')
shutil.copyfile('./GraphClient/config.client.LogServer', env["BINDIR_GraphClient"] + '/config.client.LogServer')
shutil.copyfile('./GraphClient/2UVGraphView.xml', env["BINDIR_GraphClient"] + '/2UVGraphView.xml')

shutil.copyfile('./LogServer/clLogginServer.xml', env["BINDIR_LogServer"] + '/clLogginServer.xml')
shutil.copyfile('./LogServer/config.client', env["BINDIR_LogServer"] + '/config.client')
shutil.copyfile('./LogServer/config.server', env["BINDIR_LogServer"] + '/config.server')

shutil.copyfile('./ObjectViewer/config.client.Server', env["BINDIR_ObjectViewer"] + '/config.client.Server')
shutil.copyfile('./ObjectViewer/config.client.LogServer', env["BINDIR_ObjectViewer"] + '/config.client.LogServer')
shutil.copyfile('./ObjectViewer/libMap.xml', env["BINDIR_ObjectViewer"] + '/CONFIG/libMap.xml')
shutil.copyfile('./ObjectViewer/libMapGeneral.xml', env["BINDIR_ObjectViewer"] + '/CONFIG/libMapGeneral.xml')
shutil.copyfile('./ObjectViewer/enums.xml', env["BINDIR_ObjectViewer"] + '/CONFIG/enums.xml')
shutil.copyfile('./ObjectViewer/classConfigurationView_01.xml', env["BINDIR_ObjectViewer"] + '/CONFIG/classConfigurationView_01.xml')
shutil.copyfile('./ObjectViewer/machineView_01.xml', env["BINDIR_ObjectViewer"] + '/CONFIG/machineView_01.xml')

shutil.copyfile('./Server/ServerConfiguration.xml', env["BINDIR_Server"] + '/ServerConfiguration.xml')
shutil.copyfile('./Server/DatabaseConfig.xml', env["BINDIR_Server"] + '/DatabaseConfig.xml')
shutil.copyfile('./Server/config.client.LogServer', env["BINDIR_Server"] + '/config.client.LogServer')
shutil.copyfile('./Server/config.client', env["BINDIR_Server"] + '/config.client')
shutil.copyfile('./Server/config.server', env["BINDIR_Server"] + '/config.server')

shutil.copyfile('./ServerTest/config.client.Server', env["BINDIR_ServerTest"] + '/config.client.Server')
shutil.copyfile('./ServerTest/config.client.LogServer', env["BINDIR_ServerTest"] + '/config.client.LogServer')
shutil.copyfile('./ServerTest/libMap.xml', env["BINDIR_ServerTest"] + '/CONFIG/libMap.xml')
shutil.copyfile('./ServerTest/libMapGeneral.xml', env["BINDIR_ServerTest"] + '/CONFIG/libMapGeneral.xml')
shutil.copyfile('./ServerTest/enums.xml', env["BINDIR_ServerTest"] + '/CONFIG/enums.xml')
shutil.copyfile('./ServerTest/DatabaseConfig.xml', env["BINDIR_ServerTest"] + '/DatabaseConfig.xml')

shutil.copyfile('./SocketBox/SocketBox.xml', env["BINDIR_SocketBox"] + '/SocketBox.xml')

#########################
#    Scripts to call    #
#########################
SConscript(env['InstallPath'] + '/AppStartup/SConscript')
SConscript(env['InstallPath'] + '/ServerTest/SConscript')
SConscript(env['InstallPath'] + '/ServerTest/MethodsDll/SConscript')
SConscript(env['InstallPath'] + '/ServerTest/ObjectsDll/SConscript')
SConscript(env['InstallPath'] + '/Server/SConscript')
SConscript(env['InstallPath'] + '/CycleBuilder/SConscript')
SConscript(env['InstallPath'] + '/ObjectViewer/SConscript')
SConscript(env['InstallPath'] + '/DatabaseEditor/SConscript')
SConscript(env['InstallPath'] + '/GraphClient/SConscript')
SConscript(env['InstallPath'] + '/SocketBox/SConscript')
SConscript(env['InstallPath'] + '/LogServer/SConscript')
