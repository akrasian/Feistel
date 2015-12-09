import os
import sys
import time

experimentResultFile = "FinalResult.txt"
max_length = 128 / 4
print max_length

ctr_start = 0
num_iterations = 32
#~ num_iterations = 2

finalString = ""

for i in range (ctr_start, ctr_start+num_iterations):
	hexres = hex(i)
	hexres = hexres[2:]
	num_hex_chars = len(hexres)
	hexres = '0'*(2*max_length - num_hex_chars) +hexres
	
	plaintextFile = "temp/plaintext"+str(i)+".txt"
	outputFile = "temp/output"+str(i)+".txt"
	
	f = open(plaintextFile, 'w')
	f.write(hexres)
	f.close()
	
	num_hex_chars = len(hexres)
	print hexres+":"+ str(num_hex_chars)
	
	command = "./feistel safe128.txt random128.txt "+plaintextFile+" "+outputFile
	print command
	
	os.system (command)
	print "Generating output in ",outputFile
	while not os.path.isfile(outputFile):
		time.sleep(1)
	
	f = open(outputFile, 'r')
	output = f.read()
	f.close()
	
	print "Output = ["+output+"]\n\n\n\n\n\n"
	finalString = finalString + output

print "Final String", finalString
print "Final length", len(finalString)
print "Expected length", 2*max_length*num_iterations

f = open(experimentResultFile, 'w')
f.write(finalString)
f.close()
	
