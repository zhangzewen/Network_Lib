#! c:\python27\
#Combine all labeled documents into a single training data file and a single test data file

import sys
import os
import random
import pdb

WordList = []
WordIDDic = {}
TrainingPercent = 0.8

inpath = "./data/data"
trainOutFile = file("newdata.train", "w")
testOutFile = file("newdata.test", "w")

def ConvertData():
	i = 0
	tag = 0	
	for filename in os.listdir(inpath):
		if filename.find("business") != -1:
			tag = 1
		elif filename.find("auto") != -1:
			tag = 2
		elif filename.find("sport") != -1:
			tag = 3
		i += 1
		rd = random.random()
		outfile = testOutFile
		if rd < TrainingPercent:
			outfile = trainOutFile

		if i % 100 == 0:
			print i,"files processed!\r",

		infile = file(inpath+'/'+filename, 'r')
		outfile.write(str(tag)+" ")
		content = infile.read().strip()
		content = content.decode("utf-8")
		words = content.replace('\n', ' ').split(' ')
		for word in words:
			if len(word.strip()) < 1:
				continue
			if word not in WordIDDic:
				WordList.append(word)
				WordIDDic[word] = len(WordList)
			outfile.write(str(WordIDDic[word])+" ")
		outfile.write("#"+filename+"\n")
		infile.close()

	print i, "files loaded!"
	print len(WordList), "unique words found!"

#pdb.set_trace()
ConvertData()
trainOutFile.close()
testOutFile.close()
