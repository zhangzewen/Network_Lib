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


def createVocal(alldoc):
	fcount=0
	hs_w={}
	hs_word={}
	hs_file={}
	hs_file_i={}
	for dir in alldoc:
		
		fcount=fcount+1
		file=open("data/"+dir,'r')
		for str in [line.split() for line in file]:
				
			for i in str:
				
				if (not hs_file_i.has_key(dir+i)):#该文档内的某一词出现过，该词则不++了
					hs_word[i]=hs_word.get(i,0)+1 			
				hs_file_i[dir+i]=1									
				
		hs_file[dir]=1
		
	count=0
	fw=open('type.txt','w')#存储下纳入词库的，方便查看到底是哪些词
	dr=open('drop.txt','w') #扔掉的，也就是在多个文章出现的词。
	vocabSet=[]
	for key in hs_word:
			#print sorted(hs_word.items(),key=lambda d:d[1])
			#o=key+str(hs_word[key])
			f1=key
			f2=bytes(hs_word[key])
			out=f1+f2
			if hs_word[key] <50:
				#print key,hs_word[key]
			
				fw.write(out);fw.write('\n')
				count=count+1
				vocabSet.append(f1)
			else:
				#print key,hs_word[key]
				dr.write(out+'\n')
				
	fw.close()
	dr.close()
	return vocabSet
	print "hs_word count:",len(hs_word),"After:",count


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
