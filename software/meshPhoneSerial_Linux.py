#!/usr/bin/python
#================================================================#
# MESHPHONE SERIAL CONNECTION INTERFACE                          #
# by Le Dang Dung                                                #
#                                                                #
# This program provides serial communication with our Meshphone, #
# working with prototype I, firmware version 1.3.6               #
#                                                                #
#================================================================#

import serial,time,sys,platform,time,datetime,copy

# ++++++++++++++++++++++++++++++++++++++++++++ meshPhoneSerial Def +++++++++++++++++++++++++++++
class meshPhoneSerial:
    def __init__(self):
    # class entities and initializations      
        self.Output = 'output.txt'        
        # serial properties
        self.serialCon = None
        self.solution = None
        self.serialBaudrate = 9600
        self.data = ''
        self.backupRowCount = 15
        
# connect to serial port
    def serialConnect(self,port):
    # open serial port
        try:
            self.serialCon = serial.Serial(port,self.serialBaudrate)            
            print '...serial port open'
            return 0
        except serial.SerialException:
            print 'error:', sys.exc_info()[1]
            return -1
            
# disconnect serial port
    def serialDisconnect(self):
        try:
            if self.serialCon <> None:
                self.serialCon.close()
        except serial.SerialException:
            print 'error:', sys.exc_info()[1]
        #if self.serialCon.isOpen():
        #   self.serialCon.close()
        finally:
            print '...serial port closed'
            
# send instructions via serial port
    def serialSend(self,cmd):
        try:
            self.serialCon.flushInput()
            self.serialCon.flushOutput()
            parsedCmd = cmd+'\n'
            self.serialCon.write(parsedCmd) #target needs '\n' as cmd ternimating char       
            response = ''
            timeout1 = 0
            # handle response: simple ACK, should be same as the sending cmd
            # timeout for response is provided
            while response == '' and timeout1 < 100:
                while self.serialCon.inWaiting():
                    response += self.serialCon.read()
                    time.sleep(0.01)
                time.sleep(0.05)
                timeout1 = timeout1 + 1
            #print response
            #if response <> parsedCmd:
            #    print 'error while sending, please check phone status'
            #    print "res =", response
            #    return None
            return response
        except serial.SerialException:
            print 'error:', sys.exc_info()[1]
            return None
                   
        
# check for serial port and wireless module on local machine      
    def checkSerialPort(self,cmd):
        print '...checking serial ports'
        portNum = 0
        tempSer = serial.Serial()
        tempSer.baudrate = self.serialBaudrate
        # wild guess on the number of serial ports
        while portNum < 32:
            try:
                if platform.system() == 'Linux':
                    tempSer.port = '/dev/ttyUSB'+ str(portNum)
                elif platform.system() == 'windows':
                    tempSer.port = portNum
                tempSer.open()
            except serial.SerialException:
                print '.',#tempSer.port                
            else:             
                print '\n...contact device on port', tempSer.port
                # this is a bit loose... sometimes command has been send could get lost
                # revisit later
                parsedCmd = cmd+'\n'
                tempSer.flushInput()
                tempSer.flushOutput()
                tempSer.write(parsedCmd)    # test status
                time.sleep(1)                
                response1 = ''
                timeout2 = 0;
                while response1 == '':                    
                    while tempSer.inWaiting():
                        response1 += tempSer.read()                        
                    time.sleep(0.05)    
                    timeout2 = timeout2 + 1
                    if timeout2 == 100:
                        break
                #print response1
                # desired response would be...
                #if response1.find('OK') <> -1:
                if response1 == cmd:
                    print '...found MeshPhone on port', portNum                                        
                    self.serialCon = copy.deepcopy(tempSer)
                    #self.serialCon.port = portNum
                    return 0
            portNum = portNum + 1    
        print '\n...no wireless module found'
        return -1
    
    def bind(self):
        try:
            self.serialCon.open()
            rowCount = 0
            while True:
                #response = ''      
                try:                         
                    #if self.serialCon.inWaiting() > 0:
                    # get serial data
                    incommingbyte = self.serialCon.read()
                    if incommingbyte == '\r': # skip this
                        continue
                    # insert timestamp
                    if incommingbyte == '\n':
                        time = datetime.datetime.now()
                        self.data = self.data + ' timestamp ' + time.isoformat()
                        rowCount = rowCount + 1
                    sys.stdout.write(incommingbyte)    
                    self.data = self.data + incommingbyte
                     
                        
                    # back up data
                    if rowCount >= self.backupRowCount:
                        print '...backup'
                        self.exportCSV('backup.csv')
                        rowCount = 0;
                        #print response.replace('\r','\n').rstrip()
                except:
                    print sys.exc_info()
                    break
        except serial.SerialException:
            print 'error:', sys.exc_info()    
        except KeyboardInterrupt:
            print '...binding terminated!'            
        finally:
            #self.serialCon.flushInput()
            #self.serialCon.flushOutput()
            #self.serialCon.close()
            print 'serial port closed'
            #raw_input()
            
    def exportCSV(self,filename):
        #print self.data        
        try:
            f = open(filename, 'w')
            f.write(self.data.replace(' ',','))
            f.close()
            return 1
        except:
            print '...export file error:', sys.exc_info()[1]
            return -1
        #finally:
            
# destructor
    def __del__(self):
        class_name = self.__class__.__name__
        print '...object', class_name, 'destroyed'
        
# recycle bin
#if platform.system() == 'windows':
#if phone.checkSerialPort('t') <> -1:
#ser = serial.Serial('/dev/ttyUSB0',9600)
# ++++++++++++++++++++++++++++++++++++++++++++ end meshPhoneSerial +++++++++++++++++++++++++++++

# ++++++++++++++++++++++++++++++++++++++++++++ Main ++++++++++++++++++++++++++++++++++++++++++++
# class init
phone = meshPhoneSerial()		
# connect and bind to phone
if phone.checkSerialPort("test") <> -1: 
    print '...phone connected!'
    while True:
        print '>',
        c = raw_input()
        if (c == "bind"):
            phone.bind()
        elif(c == "quit"):
            break;
        else:
            res = phone.serialSend(c)            
            print "command response:"
            print res
            
# ending procedures
    phone.serialDisconnect()
    print '...exporting data'
    while True:
        print '...file name: ',
        fo = raw_input()
        if phone.exportCSV(fo+'.csv') == 1:
            break
del phone      
# ++++++++++++++++++++++++++++++++++++++++++++ end Main ++++++++++++++++++++++++++++++++++++++++++++



