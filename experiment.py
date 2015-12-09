from os import system
import sys
import time

max_length = 128 / 4
print max_length

ctr_start = 0
#~ num_iterations = 32
num_iterations = 2

finalString = ""

for i in range (ctr_start, ctr_start+num_iterations):
	hexres = hex(i)
	hexres = hexres[2:]
	num_hex_chars = len(hexres)
	hexres = '0'*(2*max_length - num_hex_chars) +hexres
	
	plaintextFile = "temp/plaintext256_"+str(i)
	outputFile = "temp/output256_"+str(i)
	
	f = open(plaintextFile, 'w')
	f.write(hexres)
	f.close()
	
	num_hex_chars = len(hexres)
	print hexres+":"+ str(num_hex_chars)
	
	command = "./feistel safe128.txt random128.txt "+plaintextFile+" "+outputFile
	print command
	
	system (command)
	#~ time.sleep(2)

	f = open(outputFile, 'r')
	output = f.read()
	f.close()
	
	print "Output = ["+output+"]"
	finalString = finalString + output

print "Final String", finalString
print "Final length", len(finalString)
print "Expected length", 2*max_length*num_iterations

f = open('Experimentresult.txt', 'w')
f.write(finalString)
f.close()
	
