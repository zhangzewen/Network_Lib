#! c:\python27\python
#Usage:
#Training: NB.py 1 TrainingDataFile ModelFile
#Testing: NB.py 0 TestDataFile ModelFile OutFile

import sys
import os
import math
import pdb


DefaultFreq = 0.1
TrainingDataFile = "newdata.train"
ModelFile = "newdata.model"
TestDataFile = "newdata.test"
TestOutFile = "newdata.out"
ClassFeaDic = {}
ClassFreq = {}
WordDic = {}
ClassFeaProb = {}
ClassDefaultProb = {}
ClassProb = {}

def Dedup(items):
	tempDic = {}
	for item in items:
		if item not in tempDic:
			tempDic[item] = True
	return tempDic.keys()

def LoadData():
  i =0
  infile = file(TrainingDataFile, 'r')
  sline = infile.readline().strip()
  while len(sline) > 0:
    pos = sline.find("#")
    if pos > 0:
      sline = sline[:pos].strip()
    words = sline.split(' ')
    if len(words) < 1:
      print "Format error!"
      break
    classid = int(words[0])
    if classid not in ClassFeaDic:
      ClassFeaDic[classid] = {}
      ClassFeaProb[classid] = {}
      ClassFreq[classid]  = 0
    ClassFreq[classid] += 1
    words = words[1:]
		#remove duplicate words, binary distribution
		#words = Dedup(words)
    for word in words:
      if len(word) < 1:
        continue
      wid = int(word)
      if wid not in WordDic:
        WordDic[wid] = 1
      if wid not in ClassFeaDic[classid]:
        ClassFeaDic[classid][wid] = 1
      else:
        ClassFeaDic[classid][wid] += 1
    i += 1
    sline = infile.readline().strip()
  infile.close()
  pdb.set_trace()
  print i, "instances loaded!"
  print len(ClassFreq), "classes!", len(WordDic), "words!"


def ComputeModel():
    #to do
    # step one ClassProb
    sum = 0
    for v in ClassFreq.values():
      sum += v
    for classid in ClassFreq.keys():
      ClassProb[classid] = ClassFreq[classid] / (float)sum
    # step two
    


def SaveModel():
	outfile = file(ModelFile, 'w')
	for classid in ClassFreq.keys():
		outfile.write(str(classid))
		outfile.write(' ')
		outfile.write(str(ClassProb[classid]))
		outfile.write(' ')
		outfile.write(str(ClassDefaultProb[classid]))
		outfile.write(' ')
	outfile.write('\n')
	for classid in ClassFeaDic.keys():
		for wid in ClassFeaDic[classid].keys():
			outfile.write(str(wid)+' '+str(ClassFeaProb[classid][wid]))
			outfile.write(' ')
		outfile.write('\n')
	outfile.close()


def LoadModel():
	global WordDic
	WordDic = {}
	global ClassFeaProb
	ClassFeaProb = {}
	global ClassDefaultProb
	ClassDefaultProb = {}
	global ClassProb
	ClassProb = {}
	infile = file(ModelFile, 'r')
	sline = infile.readline().strip()
	items = sline.split(' ')
	if len(items) < 6:
		print "Model format error!"
		return
	i = 0
	while i < len(items):
		classid = int(items[i])
		ClassFeaProb[classid] = {}
		i += 1
		if i >= len(items):
			print "Model format error!" 
			return
		ClassProb[classid] = float(items[i])
		i += 1
		if i >= len(items):
			print "Model format error!" 
			return
		ClassDefaultProb[classid] = float(items[i])
		i += 1
	for classid in ClassProb.keys():
		sline = infile.readline().strip()
		items = sline.split(' ')
		i = 0
		while i < len(items):
			wid  = int(items[i])
			if wid not in WordDic:
				WordDic[wid] = 1
			i += 1
			if i >= len(items):
				print "Model format error!" 
				return
			ClassFeaProb[classid][wid] = float(items[i])
			i += 1
	infile.close()
	print len(ClassProb), "classes!", len(WordDic), "words!"

def Predict():
	global WordDic
	global ClassFeaProb
	global ClassDefaultProb
	global ClassProb

	TrueLabelList = []
	PredLabelList = []
	i =0
	infile = file(TestDataFile, 'r')
	outfile = file(TestOutFile, 'w')
	sline = infile.readline().strip()
	scoreDic = {}
	iline = 0
	#to do

def Evaluate(TrueList, PredList):
	#to do
  pass

def CalPreRec(TrueList,PredList,classid):
	#to do
  pass

#main framework

if len(sys.argv) < 4:
	print "Usage incorrect!"
elif sys.argv[1] == '1':
	print "start training:"
	TrainingDataFile = sys.argv[2]
	ModelFile = sys.argv[3]
	LoadData()
	ComputeModel()
	SaveModel()
elif sys.argv[1] == '0':
	print "start testing:"
	TestDataFile = sys.argv[2]
	ModelFile = sys.argv[3]
	TestOutFile = sys.argv[4]
	LoadModel()
	TList,PList = Predict()
	i = 0
	outfile = file(TestOutFile, 'w')
	while i < len(TList):
		outfile.write(str(TList[i]))
		outfile.write(' ')
		outfile.write(str(PList[i]))
		outfile.write('\n')
		i += 1 
	outfile.close()
	Evaluate(TList,PList)
	for classid in ClassProb.keys():
		pre,rec = CalPreRec(TList, PList,classid)
		print "Precision and recall for Class",classid,":",pre,rec
else:
	print "Usage incorrect!"
