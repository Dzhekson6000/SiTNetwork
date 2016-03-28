#!/usr/bin/env python3
import platform
import os
import shutil
import urllib.request
import subprocess
import shlex

def downloadingAndInstallWindows():
#create dir tmp
	try:
		os.makedirs('tmp_install')
	except OSError:
		pass
#download
	print ("download cmake")
	urllib.request.urlretrieve("https://cmake.org/files/v3.5/cmake-3.5.1-win32-x86.msi", "tmp_install/cmake.msi")
	print ("download openssl")
	urllib.request.urlretrieve("http://slproweb.com/download/Win32OpenSSL-1_0_2g.exe", "tmp_install/openssl.exe")
#instail
	print ("install cmake")
	subprocess.call('msiexec /i %s' % ("tmp_install\cmake.msi"), shell=True)
	print ("install openssl")
	subprocess.Popen([os.path.abspath(os.path.dirname(__file__)) + '\\tmp_install\\openssl.exe']).wait()
#delete dir tmp
	path = os.path.join(os.path.abspath(os.path.dirname(__file__)), 'tmp_install')
	shutil.rmtree(path)

def downloadingAndInstallLinux():
	p = os.popen('sudo apt-get install openssl cmake',"r")
	while 1:
		line = p.readline()
		if not line: break
		print (line)

if platform.system() == "Windows":
	print ("platform Windows")
	downloadingAndInstallWindows()
elif platform.system() == "Linux":
	print ("platform Linux")
	downloadingAndInstallLinux()
input("successfully")